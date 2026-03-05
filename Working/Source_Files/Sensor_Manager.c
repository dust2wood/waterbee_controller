/*===========================================================================
 * Sensor_Manager.c
 *
 * 범용 RS-485 Modbus RTU 외부 센서 인터페이스
 *
 * ─────────────────────────────────────────────────────────────────────────
 *  새 중국산 센서 추가 방법  (이 파일만 편집)
 *
 *  g_ext_sensors[] 배열에 한 줄 추가:
 *
 *  {
 *    .slave_addr    = 0x01,   // 센서 Modbus 주소 (DIP 스위치로 확인)
 *    .reg_start     = 0x0000, // 읽기 시작 레지스터 주소
 *    .reg_count     = 1,      // 읽을 레지스터 수 (1~4)
 *    .scale         = 0.01f,  // 결과 = raw × scale + offset
 *    .offset        = 0,
 *    .poll_interval = 1,      // 1초마다 폴링 (0 = 매 사이클)
 *    .name          = "pH_EXT"
 *  },
 *
 *  결과값 접근:
 *    g_ext_sensors[0].scaled_value   // 최종 측정값
 *    g_ext_sensors[0].status         // SMGR_STATUS_OK / SMGR_STATUS_TIMEOUT 등
 * ─────────────────────────────────────────────────────────────────────────
 *
 *  기존 코드와의 관계
 *  ──────────────────
 *  · USART3 (RS-485) 버스를 기존 Modbus485Handler() 와 공유한다.
 *  · com485State == 0 && rx3Size == 0 일 때만 버스를 점유한다.
 *  · Update_All_External_Sensors() 를 Modbus485Handler() 호출 직전에
 *    배치하면 응답 버퍼(rx3Buffer)를 먼저 확인하여 충돌을 방지한다.
 *  · 기존 Communication.c / Interrupt.c 수정 없음.
 *===========================================================================*/

#include "Sensor_Manager.h"
#include "Communication.h"   /* CRC16Modbus, RS485_DRIVE_HIGH/LOW          */
#include "Interrupt.h"        /* rx3Buffer, tx3Buffer, rx3Size, tx3Size ...  */

/* USART3 직접 접근 (serial.c 와 동일 방식) */
#include "stm32f10x_usart.h"

/*===========================================================================
 * ★ 사용자 설정 영역 - 여기에 새 센서를 추가하세요 ★
 *===========================================================================*/
Generic_Sensor_t g_ext_sensors[] = {

    /* ── 예시 1: 중국산 pH 센서 (슬레이브 addr=0x01, 레지스터 0x0000) ─── */
    {
        .slave_addr    = 0x01,
        .reg_start     = 0x0000,
        .reg_count     = 1,
        .scale         = 0.01f,   /* 원시값 700 → 7.00 pH */
        .offset        = 0,
        .poll_interval = 1,       /* 1초마다 */
        .name          = "pH_EXT"
    },

    /* ── 예시 2: 중국산 EC 센서 (슬레이브 addr=0x02, 레지스터 0x0000) ─── */
    {
        .slave_addr    = 0x02,
        .reg_start     = 0x0000,
        .reg_count     = 1,
        .scale         = 1.0f,    /* 원시값 = µS/cm 직접 */
        .offset        = 0,
        .poll_interval = 1,
        .name          = "EC_EXT"
    },

    /* ── 예시 3: 탁도 센서 (슬레이브 addr=0x03, 레지스터 2개 읽기) ─────── */
    /*
    {
        .slave_addr    = 0x03,
        .reg_start     = 0x0000,
        .reg_count     = 2,       // reg[0]=정수부, reg[1]=소수부
        .scale         = 0.01f,
        .offset        = 0,
        .poll_interval = 2,       // 2초마다
        .name          = "NTU_EXT"
    },
    */
};

/* 배열 크기 자동 계산 - 수정 불필요 */
const uint8_t g_ext_sensor_count =
    (uint8_t)(sizeof(g_ext_sensors) / sizeof(Generic_Sensor_t));

/*===========================================================================
 * 내부 상태 머신
 *===========================================================================*/

typedef enum {
    SM_IDLE = 0,        /* 다음 센서 선택 대기                              */
    SM_WAIT_BUS,        /* com485State==0 && rx3Size==0 될 때까지 대기       */
    SM_TX_GUARD,        /* RS485_DRIVE_HIGH 후 DE 설정 안정화 대기 (~20ms)  */
    SM_WAIT_TX_DONE,    /* USART3 ISR이 TX를 완료하고 com485State=3 될 때까지*/
    SM_WAIT_RESPONSE,   /* 응답 타임아웃 감시                               */
    SM_PARSE,           /* 응답 CRC 검증 및 값 추출                          */
    SM_ERROR_RECOVER,   /* 오류 후 버스 복구                                 */
} SM_State_t;

static SM_State_t  sm_state     = SM_IDLE;
static uint8_t     sm_idx       = 0;        /* 현재 폴링 센서 인덱스        */
static uint16_t    sm_tick      = 0;        /* 범용 틱 카운터               */

/* com485State, rs485DriveCount 는 Communication.c 에 정의되어 있음 */
/* Interrupt.h 에 extern char com485State; 로 이미 선언됨            */
extern uint32_t rs485DriveCount;

/*===========================================================================
 * 내부 헬퍼: 다음으로 폴링할 센서 인덱스 찾기
 * poll_interval > 0 이면 poll_counter 가 그 값에 도달했을 때만 폴링
 *===========================================================================*/
static int8_t _find_next_sensor(void)
{
    uint8_t i;
    uint8_t start = sm_idx;

    for (i = 0; i < g_ext_sensor_count; i++) {
        uint8_t idx = (start + i) % g_ext_sensor_count;
        Generic_Sensor_t *s = &g_ext_sensors[idx];

        if (s->poll_interval == 0) {
            return (int8_t)idx;
        }
        if (s->poll_counter == 0) {
            return (int8_t)idx;
        }
    }
    return -1; /* 폴링할 센서 없음 */
}

/*===========================================================================
 * 내부 헬퍼: poll_counter 1초 감소 (TIM3 1초 틱과 연동)
 * 메인 루프가 약 10ms 주기로 돌 때, 100회마다 1초로 계산하는 대신
 * 간단하게 Update_All_External_Sensors() 가 SM_IDLE 에 진입할 때마다
 * 카운터를 줄인다. poll_interval 단위는 "SM_IDLE 진입 횟수" 로 간주.
 * (주기가 약 10~100ms 수준이므로 실용상 문제없음)
 *===========================================================================*/
static void _tick_poll_counters(void)
{
    uint8_t i;
    for (i = 0; i < g_ext_sensor_count; i++) {
        if (g_ext_sensors[i].poll_interval > 0 &&
            g_ext_sensors[i].poll_counter > 0) {
            g_ext_sensors[i].poll_counter--;
        }
    }
}

/*===========================================================================
 * 내부 헬퍼: Modbus RTU FC03 Read Holding Registers 패킷 빌드
 *===========================================================================*/
static void _build_fc03_request(const Generic_Sensor_t *s)
{
    uint16_t crc;

    tx3Buffer[0] = s->slave_addr;
    tx3Buffer[1] = 0x03;                            /* FC 03: Read Holding Regs */
    tx3Buffer[2] = (uint8_t)((s->reg_start >> 8) & 0xFF);
    tx3Buffer[3] = (uint8_t)( s->reg_start       & 0xFF);
    tx3Buffer[4] = 0x00;
    tx3Buffer[5] = s->reg_count;

    crc = CRC16Modbus(tx3Buffer, 6);
    tx3Buffer[6] = (uint8_t)(crc & 0xFF);           /* CRC Low  */
    tx3Buffer[7] = (uint8_t)((crc >> 8) & 0xFF);   /* CRC High */

    tx3Size  = 8;
    tx3Count = 1;
}

/*===========================================================================
 * 내부 헬퍼: FC03 응답 파싱
 *
 * 반환값:
 *   1  - 파싱 완료 (OK 또는 CRC 오류 포함)
 *   0  - 아직 내 응답이 아님 (다른 디바이스 or 미수신)
 *===========================================================================*/
static uint8_t _parse_fc03_response(Generic_Sensor_t *s)
{
    uint8_t  i;
    uint16_t expected_len;
    uint16_t crc_calc, crc_recv;

    /* 최소 길이: addr(1) + fc(1) + byte_cnt(1) + data(reg_count×2) + crc(2) */
    expected_len = (uint16_t)(5 + s->reg_count * 2);

    if (rx3Size < expected_len) {
        return 0; /* 아직 미수신 */
    }

    /* 주소·기능코드 확인 */
    if (rx3Buffer[0] != s->slave_addr) return 0;
    if (rx3Buffer[1] != 0x03)          return 0;
    if (rx3Buffer[2] != s->reg_count * 2) return 0;

    /* CRC 검증 */
    crc_calc = CRC16Modbus(rx3Buffer, (uint16_t)(expected_len - 2));
    crc_recv = (uint16_t)( rx3Buffer[expected_len - 2] )
             | (uint16_t)( rx3Buffer[expected_len - 1] << 8 );

    if (crc_calc != crc_recv) {
        s->status = SMGR_STATUS_CRC_ERR;
        return 1; /* 내 응답이지만 CRC 오류 */
    }

    /* 레지스터 원시값 추출 (big-endian) */
    for (i = 0; i < s->reg_count && i < SMGR_MAX_REGS; i++) {
        s->raw[i] = (uint16_t)((rx3Buffer[3 + i * 2] << 8)
                              |  rx3Buffer[3 + i * 2 + 1]);
    }

    /* 스케일 · 오프셋 적용 (첫 번째 레지스터 기준) */
    s->scaled_value = (int32_t)((float)s->raw[0] * s->scale)
                    + s->offset;
    s->status = SMGR_STATUS_OK;
    return 1;
}

/*===========================================================================
 * 공개 API: Sensor_Manager_Init
 *===========================================================================*/
void Sensor_Manager_Init(void)
{
    uint8_t i;

    sm_state = SM_IDLE;
    sm_idx   = 0;
    sm_tick  = 0;

    for (i = 0; i < g_ext_sensor_count; i++) {
        g_ext_sensors[i].status       = SMGR_STATUS_PENDING;
        g_ext_sensors[i].scaled_value = 0;
        g_ext_sensors[i].timeout_cnt  = 0;
        g_ext_sensors[i].poll_counter = 0; /* 첫 사이클부터 즉시 폴링 */
    }
}

/*===========================================================================
 * 공개 API: Update_All_External_Sensors
 *
 * 상태 머신 흐름도:
 *
 *  SM_IDLE
 *    ↓ 다음 폴링 센서 결정
 *  SM_WAIT_BUS  ← 버스 점유 시 대기 (최대 300ms)
 *    ↓ com485State==0 && rx3Size==0
 *    RS485_DRIVE_HIGH + 패킷 빌드
 *  SM_TX_GUARD  ← DE 안정화 20ms 대기
 *    ↓ 2틱 경과
 *    USART_SendData(USART3, ...) + TXC 인터럽트 활성화
 *  SM_WAIT_TX_DONE  ← USART3 ISR 완료 대기 (ISR에서 com485State=3 설정)
 *    ↓ com485State==3 (TX 완료 + RS485_DRIVE_LOW 이미 실행됨)
 *    com485State = 0
 *  SM_WAIT_RESPONSE  ← 응답 타임아웃 감시 (1.5초)
 *    ↓ rx3Buffer에 내 슬레이브 주소 응답 도착
 *  SM_PARSE  ← CRC 검증 + scaled_value 계산
 *    ↓
 *  SM_IDLE  (다음 센서로)
 *
 *  오류 발생 시:
 *    SM_ERROR_RECOVER → RS485_DRIVE_LOW + 버스 리셋 → SM_IDLE
 *===========================================================================*/
void Update_All_External_Sensors(void)
{
    Generic_Sensor_t *s;

    if (g_ext_sensor_count == 0) return;

    switch (sm_state) {

    /*-----------------------------------------------------------------------
     * SM_IDLE : 다음 센서 선택
     *-----------------------------------------------------------------------*/
    case SM_IDLE: {
        int8_t next;

        _tick_poll_counters();

        next = _find_next_sensor();
        if (next < 0) {
            /* 이번 사이클에서 폴링할 센서 없음 → sm_idx 순환만 */
            sm_idx = (sm_idx + 1) % g_ext_sensor_count;
            break;
        }

        sm_idx  = (uint8_t)next;
        sm_tick = 0;
        sm_state = SM_WAIT_BUS;
        break;
    }

    /*-----------------------------------------------------------------------
     * SM_WAIT_BUS : 기존 통신이 버스를 비울 때까지 대기
     *-----------------------------------------------------------------------*/
    case SM_WAIT_BUS:
        if (com485State == 0 && rx3Size == 0) {
            /* 버스 비어 있음 → 패킷 빌드 + RS485 방향을 TX로 전환 */
            s = &g_ext_sensors[sm_idx];
            _build_fc03_request(s);
            RS485_DRIVE_HIGH;
            sm_tick  = 0;
            sm_state = SM_TX_GUARD;
        } else {
            sm_tick++;
            if (sm_tick > SMGR_BUS_WAIT_TICKS) {
                /* 버스가 너무 오래 점유됨 → 이번 센서 스킵 */
                g_ext_sensors[sm_idx].status = SMGR_STATUS_BUS_BUSY;
                sm_idx = (sm_idx + 1) % g_ext_sensor_count;
                sm_state = SM_IDLE;
            }
        }
        break;

    /*-----------------------------------------------------------------------
     * SM_TX_GUARD : RS485 DE 신호 안정화 대기 (기존 코드와 동일 20ms)
     *-----------------------------------------------------------------------*/
    case SM_TX_GUARD:
        sm_tick++;
        if (sm_tick >= SMGR_TX_GUARD_TICKS) {
            /* TX 시작 : 기존 Modbus485Handler 방식과 동일 */
            com485State = 2;          /* State 2: TX 완료 대기로 직행        */
            rs485DriveCount = 0;

            USART_SendData(USART3, (uint16_t)tx3Buffer[0]);
            USART3->CR1 |= 0x40;     /* TXC 인터럽트 활성화                */

            sm_tick  = 0;
            sm_state = SM_WAIT_TX_DONE;
        }
        break;

    /*-----------------------------------------------------------------------
     * SM_WAIT_TX_DONE : USART3 ISR 이 TX를 완료할 때까지 대기
     *   USART3_IRQHandler 에서 TX 완료 시:
     *     com485State = 3; RS485_DRIVE_LOW; 를 이미 실행함
     *-----------------------------------------------------------------------*/
    case SM_WAIT_TX_DONE:
        sm_tick++;
        if (com485State == 3) {
            /* ISR이 TX 완료 + RS485_DRIVE_LOW 처리 완료                    */
            com485State = 0;   /* Modbus485Handler 의 State3 작업 대신 처리  */
            rx3Size     = 0;   /* TX 중 들어온 에코·잡음 제거                */
            sm_tick     = 0;
            sm_state    = SM_WAIT_RESPONSE;
        } else if (sm_tick > SMGR_BUS_WAIT_TICKS) {
            sm_state = SM_ERROR_RECOVER;
        }
        break;

    /*-----------------------------------------------------------------------
     * SM_WAIT_RESPONSE : 응답 수신 또는 타임아웃 감시
     *-----------------------------------------------------------------------*/
    case SM_WAIT_RESPONSE:
        sm_tick++;

        if (sm_tick > SMGR_TIMEOUT_TICKS) {
            g_ext_sensors[sm_idx].status = SMGR_STATUS_TIMEOUT;
            sm_state = SM_ERROR_RECOVER;
            break;
        }

        /* 슬레이브 주소가 맞는 응답이 오면 파싱 단계로 이동
         * ※ Modbus485Handler 보다 먼저 호출될 때만 이 조건이 유효.
         *   Update_All_External_Sensors() 를 Modbus485Handler() 앞에
         *   배치하는 이유이다. */
        if (rx3Size > 0 &&
            rx3Buffer[0] == g_ext_sensors[sm_idx].slave_addr) {
            sm_state = SM_PARSE;
        }
        break;

    /*-----------------------------------------------------------------------
     * SM_PARSE : CRC 검증 + scaled_value 추출
     *-----------------------------------------------------------------------*/
    case SM_PARSE:
        s = &g_ext_sensors[sm_idx];

        if (_parse_fc03_response(s)) {
            /* 내 응답 처리 완료 → 버퍼 클리어 */
            rx3Size  = 0;

            /* poll_interval 이 있으면 다음 폴링까지 카운터 재설정 */
            if (s->poll_interval > 0) {
                s->poll_counter = s->poll_interval;
            }

            sm_idx = (sm_idx + 1) % g_ext_sensor_count;
            sm_state = SM_IDLE;
        } else {
            /* 아직 데이터가 부족하거나 다른 디바이스의 응답
             * → 응답 대기로 되돌아감 (타임아웃 카운터 유지) */
            sm_state = SM_WAIT_RESPONSE;
        }
        break;

    /*-----------------------------------------------------------------------
     * SM_ERROR_RECOVER : 버스를 안전한 상태로 복구
     *-----------------------------------------------------------------------*/
    case SM_ERROR_RECOVER:
        RS485_DRIVE_LOW;
        com485State = 0;
        rx3Size     = 0;
        sm_idx = (sm_idx + 1) % g_ext_sensor_count;
        sm_state = SM_IDLE;
        break;

    default:
        sm_state = SM_IDLE;
        break;
    }
}
