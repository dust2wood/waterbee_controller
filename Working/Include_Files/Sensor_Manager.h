/*===========================================================================
 * Sensor_Manager.h
 *
 * 범용 RS-485 Modbus RTU 외부 센서 인터페이스
 *
 * 사용법:
 *   1. g_ext_sensors[] 배열에 센서를 추가한다 (Sensor_Manager.c 상단 참조)
 *   2. Initialize() 이후 Sensor_Manager_Init() 한 번 호출
 *   3. 메인 루프에서 SensorComHandler() 직후 Update_All_External_Sensors() 호출
 *
 * 기존 코드 수정 범위:
 *   - main.c  2줄 추가 (주석으로 위치 안내됨)
 *   - 기존 Communication.c / Interrupt.c 는 무수정
 *===========================================================================*/
#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include "Main.h"

/*---------------------------------------------------------------------------
 * 컴파일 타임 상수
 *---------------------------------------------------------------------------*/

/* 관리 가능한 외부 센서 최대 수 */
#define SMGR_MAX_SENSORS        8

/* 한 요청으로 읽을 수 있는 레지스터 최대 수 (FC03 응답 버퍼 한계) */
#define SMGR_MAX_REGS           4

/* 응답 타임아웃 : 10ms 틱 단위 (150 = 1.5초) */
#define SMGR_TIMEOUT_TICKS      150

/* RS-485 DE 설정 후 송신 대기 틱 수 (2틱 × 10ms = 20ms, 기존 코드 동일) */
#define SMGR_TX_GUARD_TICKS     2

/* 버스 점유 대기 최대 틱 (30 × 10ms = 300ms) */
#define SMGR_BUS_WAIT_TICKS     30

/*---------------------------------------------------------------------------
 * 센서 상태 코드  (기존 SENSOR_ERROR3 = 3 과 호환)
 *---------------------------------------------------------------------------*/
#define SMGR_STATUS_PENDING     0xFF    /* 초기값, 아직 데이터 없음 */
#define SMGR_STATUS_OK          0       /* 정상 수신 */
#define SMGR_STATUS_TIMEOUT     3       /* 응답 없음  (SENSOR_ERROR3 동일) */
#define SMGR_STATUS_CRC_ERR     4       /* CRC 오류 */
#define SMGR_STATUS_BUS_BUSY    5       /* 버스 점유 타임아웃 */

/*---------------------------------------------------------------------------
 * Generic_Sensor_t  -  센서 하나를 기술하는 구조체
 *
 * ┌─ 사용자 설정 필드 ──────────────────────────────────────────────────┐
 * │  slave_addr   : Modbus 슬레이브 주소 (0x01~0xF7)                    │
 * │  reg_start    : 읽기 시작 레지스터 주소 (예: 0x0000)                 │
 * │  reg_count    : 읽을 레지스터 수 (1 ~ SMGR_MAX_REGS)                │
 * │  scale        : scaled_value = (float)raw[0] × scale + offset       │
 * │  offset       : 위 공식의 offset (정수)                              │
 * │  poll_interval: 폴링 주기 (초). 0 = 매 사이클                        │
 * │  name         : 디버그·표시용 이름 (null-terminated)                │
 * └────────────────────────────────────────────────────────────────────┘
 * ┌─ 런타임 필드 (건드리지 마세요) ──────────────────────────────────────┐
 * │  raw[]        : 수신된 레지스터 원시값 (big-endian 복원)              │
 * │  scaled_value : scale/offset 적용된 최종 정수 결과                   │
 * │  status       : SMGR_STATUS_*                                        │
 * │  timeout_cnt  : 타임아웃 카운터 (내부 사용)                           │
 * │  poll_counter : 폴링 간격 카운터 (내부 사용)                          │
 * └────────────────────────────────────────────────────────────────────┘
 *---------------------------------------------------------------------------*/
typedef struct {
    /* ── 사용자 설정 필드 ─────────────────────────── */
    uint8_t  slave_addr;                    /* Modbus 슬레이브 주소        */
    uint16_t reg_start;                     /* 시작 레지스터 주소           */
    uint8_t  reg_count;                     /* 읽을 레지스터 수             */
    float    scale;                         /* 스케일 계수                  */
    int32_t  offset;                        /* 스케일 후 덧셈 오프셋         */
    uint8_t  poll_interval;                 /* 폴링 주기 (초, 0=매 사이클)  */
    char     name[12];                      /* 디버그·표시용 이름           */

    /* ── 런타임 필드 (내부 사용) ──────────────────── */
    uint16_t raw[SMGR_MAX_REGS];            /* 레지스터 원시값              */
    int32_t  scaled_value;                  /* 최종 결과값                  */
    uint8_t  status;                        /* SMGR_STATUS_*               */
    uint16_t timeout_cnt;                   /* 응답 타임아웃 카운터         */
    uint8_t  poll_counter;                  /* 폴링 간격 카운터             */
} Generic_Sensor_t;

/*---------------------------------------------------------------------------
 * 공개 변수 (Sensor_Manager.c 에서 정의)
 *---------------------------------------------------------------------------*/
extern Generic_Sensor_t g_ext_sensors[];
extern const uint8_t    g_ext_sensor_count;

/*---------------------------------------------------------------------------
 * 공개 API
 *---------------------------------------------------------------------------*/

/**
 * @brief  센서 매니저 초기화.
 *         Initialize() 완료 직후 한 번 호출한다.
 */
void Sensor_Manager_Init(void);

/**
 * @brief  외부 센서 순차 폴링 (비블로킹 상태 머신).
 *
 *         메인 루프에서 SensorComHandler() 직후에 호출한다.
 *         내부적으로 RS-485 버스 사용 여부를 판단하여
 *         기존 Modbus485Handler() 와 충돌하지 않는다.
 *
 *         // main.c  while(1) 내부 권장 호출 순서
 *         SensorComHandler();
 *         Update_All_External_Sensors();   // <-- 여기
 *         Update_CurrentTrans();
 *         ...
 *         Modbus485Handler();              // 기존 그대로
 */
void Update_All_External_Sensors(void);

#endif /* SENSOR_MANAGER_H */
