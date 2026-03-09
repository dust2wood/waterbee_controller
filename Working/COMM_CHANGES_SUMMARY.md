# 통신 구조 변경 요약

## 변경 일자
2025-03-05

## 1. RS232: CL/NTU push 수신 유지
- **USART1**: $RES1=잔류염소, $RES2=탁도
- **Polling 없음**: 기존 push 수신 구조 유지
- **코드**: `Interrupt.c` USART1_IRQHandler (변경 없음)

## 2. RS485: pH/EC 고정 매핑 polling
- **슬롯1**: pH (Modbus slave addr 2)
- **슬롯2**: EC (Modbus slave addr 4)
- **슬롯3/4**: unused
- **교대 폴링**: Modbus485Handler에서 pH 응답 처리 후 EC 요청, EC 응답 처리 후 pH 요청 (round-robin)

### 실제 사용 slave address
| Slave Addr | 센서 | Register |
|------------|------|----------|
| 2 | pH | 0, length 1 (0x03) |
| 4 | EC | 0, length 1 (0x03) |

### Modbus 요청 형식
- Function: 0x03 (Read Holding Registers)
- Start addr: 0
- Register count: 1
- 프레임: [addr][0x03][0][0][0][1][CRC_L][CRC_H] (8 bytes)

## 3. Autodetect 제거/우회
| 항목 | 변경 |
|------|------|
| `sensor_autodetect_run()` | RS485 스캔 제거, sensor_autodetect_ph=1/ec=1 고정 설정 |
| `init_tx3Buffer()` | autodetect 분기 제거, 항상 addr 2로 시작 |
| `Sensor_Manager.c` | pH/EC autodetect 플래그 의존 제거, 항상 등록 |
| `Communication.c` | sensor_autodetect.h include 제거 |

## 4. 수정 파일 목록
- `Working/Source_Files/sensor_autodetect.c`
- `Working/Source_Files/Communication.c`
- `Working/Source_Files/Sensor_Manager.c`
