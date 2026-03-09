# 통신 구조 조사 보고서

## 1. 물리 포트 매핑

| Serial Port | USART | 용도 | 통신 방식 |
|-------------|-------|------|-----------|
| SERIAL_PORT1 | USART1 | RS232 센서 입력 | 수신 전용 (센서가 push) |
| SERIAL_PORT2 | USART2 | Modbus RTU Slave | 호스트 SCADA가 우리 레지스터 읽기/쓰기 |
| SERIAL_PORT3 | USART3 | RS485 센서 | Modbus RTU Master (우리가 polling) |

- **RS232 (CL/NTU)**: USART1 하나만 사용. $RES1, $RES2 프레임이 같은 버스로 들어옴.
- **RS485 (pH/EC)**: USART3 + RS485 트랜시버(드라이버). 우리가 addr 2, 4로 폴링.

---

## 2. RS232 잔류염소/탁도 처리

### 2.1 Polling 여부
- **Polling 없음.** 센서가 주기적으로 `$RESn,DDDDD,...` 형식으로 **push 전송**.
- 수신: `Interrupt.c` → `USART1_IRQHandler()` → `rx1Buffer` 적재.

### 2.2 코드 위치
| 파일 | 내용 |
|------|------|
| `Interrupt.c` 358~610행 | USART1 RX → rx1Buffer 수집 |
| `Interrupt.c` 538~605행 | `$RES1`~`$RES4` 파싱 및 매핑 |

### 2.3 프레임 형식
```
$RESn,DDDDD[,MMMMM][,STTTT]\n
  n=1~4 (슬롯번호)
  DDDDD = 측정값 5자리 (예: 00123 = 1.23)
  MMMMM = mV (선택)
  STTTT = 온도 (선택, S=부호)
```

### 2.4 현재 데이터 매핑 (CH2, Interrupt.c 560~565행)
```c
if (sensor_no==1) currentData.comm_S1PPM  = sensor;  /* CL */
else if (sensor_no==2) currentData.comm_S2NTU = sensor;  /* NTU */
else if (sensor_no==3) currentData.comm_S1PPM  = sensor;  /* pH */
else if (sensor_no==4) currentData.comm_S2NTU = sensor;  /* EC */
```
→ $RES1=CL, $RES2=NTU (RS232). $RES3=pH, $RES4=EC는 원래 RS485 데이터용으로 쓰이지만, **실제 pH/EC는 RS485에서 옴**.

### 2.5 RS232 관련 Polling
- **Modbus232Handler** (Communication.c 349~450행): USART2용. **호스트(SCADA)의 Modbus 질의 응답**이지, CL/NTU 센서 폴링 아님.
- USART1(RS232)에는 **폴링 코드 없음** – 완전 수신(push) 모델.

---

## 3. RS485 pH/EC Polling

### 3.1 Polling 코드 위치
| 파일 | 함수 | 역할 |
|------|------|------|
| `Communication.c` | `init_tx3Buffer()` | 첫 Modbus 0x03 요청 구성 (addr 2 or 4) |
| `Communication.c` | `Modbus485Handler()` | rx3 수신 처리, 다음 요청 예약 |
| `main.c` 251행 | `init_tx3Buffer()` 호출 | 부팅 시 RS485 폴링 시작 |
| `main.c` 397행 | `Modbus485Handler()` 호출 | 10ms 주기 호출 |

### 3.2 init_tx3Buffer() – autodetect 의존 (464~476행)
```c
if (sensor_autodetect_ph && sensor_autodetect_ec) {
    tx3Buffer[0] = (SENSOR_1_MODE) ? 2 : 4;  // pH or EC by Device_Selector_Mode
} else if (sensor_autodetect_ph) {
    tx3Buffer[0] = 2;
} else if (sensor_autodetect_ec) {
    tx3Buffer[0] = 4;
} else {
    return;  /* No RS485 sensors detected, skip */
}
```
→ **autodetect 결과 없으면 RS485 폴링 중단.**

### 3.3 Modbus485Handler – addr별 처리
- **addr 2**: pH + 온도(ph_temp 토글). currentData.S1PPM, water_data.ph
- **addr 4**: EC + 온도. currentData.S2PPM, water_data.ec

### 3.4 RS485 수신/송신
- USART3 사용.
- `RS485_DRIVE_HIGH` → TX, `RS485_DRIVE_LOW` → RX.
- tx3Buffer → Modbus 0x03 (레지스터 0, 1워드).

---

## 4. 슬롯/포트별 매핑 테이블

### 4.1 현재 구조

| 구분 | 물리 포트 | 프로토콜 | 슬롯1 | 슬롯2 | 비고 |
|------|-----------|----------|-------|-------|------|
| RS232 | USART1 | $RESn ASCII | $RES1 → CL | $RES2 → NTU | 수신만, 폴링 없음 |
| RS485 | USART3 | Modbus RTU | addr 2 → pH | addr 4 → EC | autodetect 후 폴링 |

### 4.2 목표 구조 (사장님 요청)

| 구분 | 슬롯1 | 슬롯2 | 슬롯3/4 |
|------|-------|-------|---------|
| RS485 | pH (addr 2) 고정 | EC (addr 4) 고정 | unused |
| RS232 | 포트1: CL 또는 NTU (설정) | 포트2: CL 또는 NTU (설정) | - |

**참고**: RS232는 USART1 하나만 사용. “포트1/포트2”는 논리 슬롯($RES1, $RES2)으로 해석.

---

## 5. Autodetect 코드 – 제거/우회 대상

### 5.1 sensor_autodetect.c
| 함수 | 동작 | 조치 |
|------|------|------|
| `probe_modbus_addr(addr)` | RS485 addr 1~10 스캔 | **제거 또는 미호출** |
| `probe_adc()` | ADC 4-20mA 유효 검사 | 사용 안 하면 제거 |
| `sensor_autodetect_run()` | 부팅 시 전체 스캔 | **pH/EC 고정 매핑 시 호출 생략 또는 no-op** |

### 5.2 autodetect 결과 사용처
| 파일 | 사용 | 조치 |
|------|------|------|
| `Communication.c` `init_tx3Buffer()` | sensor_autodetect_ph, sensor_autodetect_ec | **고정 addr 2, 4로 대체** |
| `Sensor_Manager.c` `sensor_manager_init()` | PH/EC 등록 조건 | **고정 등록 또는 설정 기반** |

### 5.3 main.c
- 233행: `sensor_autodetect_run()` 호출 → **미호출 또는 no-op**.

---

## 6. 수정 포인트 요약

### RS485 (pH/EC) 고정 매핑
1. **init_tx3Buffer()**: autodetect 조건 제거. 항상 pH(2), EC(4) 순환 폴링.
2. **Modbus485Handler**: 이미 addr 2, 4만 처리. 변경 최소화.
3. **sensor_autodetect_run()**: RS485 스캔 제거 또는 pH/EC 고정 플래그만 설정.

### RS232 (CL/NTU)
1. **Interrupt.c**: $RES1→슬롯1, $RES2→슬롯2 유지.
2. **설정 기반 매핑**: 포트1=CL/NTU, 포트2=CL/NTU 선택을 config에서 읽어 comm_S1PPM/comm_S2NTU 매핑.
3. **폴링**: 기존처럼 없음(push 수신).

### Autodetect 최소화
1. `sensor_autodetect_run()`: RS485 scan 제거, pH=1/EC=1 고정 또는 config 기반.
2. `Sensor_Manager.c`: autodetect 의존 제거, pH/EC 항상 또는 설정 기반 등록.
3. `init_tx3Buffer()`: autodetect 분기 제거.

---

## 7. 데이터 흐름 다이어그램

```
[RS232 USART1]                    [RS485 USART3]
     │                                  │
     │ $RES1,ddddd                      │ Modbus 0x03 → addr 2
     │ $RES2,ddddd                      │ Modbus 0x03 → addr 4
     ▼                                  ▼
USART1_IRQHandler                 Modbus485Handler
     │                                  │
     ├─ comm_S1PPM (슬롯1)              ├─ addr 2 → S1PPM, water_data.ph
     └─ comm_S2NTU (슬롯2)              └─ addr 4 → S2PPM, water_data.ec
     │                                  │
     ▼                                  ▼
S1PPm_Data_offset_function    (이미 currentData에 기록)
     │
     ├─ currentData.S1PPM
     └─ currentData.S2PPM
```
