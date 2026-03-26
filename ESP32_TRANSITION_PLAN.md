# ESP32 전환 계획서
> 작성일: 2026-03-26
> 현재 보드: STM32F103VE (V20) → 신규 보드: ESP32

---

## 전환 배경

### 현 STM32 보드 문제점

#### 1. 아날로그 노이즈 문제
- 내장 ADC에 디지털 회로 노이즈 유입
- 아날로그/디지털 전원 분리 미흡

#### 2. RS485 멀티센서 충돌 (멀티모달 문제)
- USART3 단일 RS485 버스에 pH(addr 0x02) + EC(addr 0x04) 공존
- 코드 분석으로 확인된 실제 버그:

| 버그 | 위치 | 내용 |
|------|------|------|
| Race condition | `Communication.c` Case 0 (line 517-523) | `init_count > 10` 발동 시 rx3Size 클리어로 응답 유실 |
| 블로킹 딜레이 | `init_tx3Buffer()` (line 495) | 런타임 호출 시 `Delay_10msec(10)` = 100ms 메인루프 동결 |
| DE 타이밍 | `Interrupt.c` USART3_IRQHandler (line 799) | TC 후 즉시 RS485_DRIVE_LOW, stop bit 안정화 없음 |
| 에코 필터 | `Interrupt.c` (line 807) | 첫 바이트 0x00 조건 스킵 — 실 데이터 손실 가능 |

---

## 신규 ESP32 보드 아키텍처

### 하드웨어 구성

```
[ESP32 (ESP-WROOM-32)]
  │
  ├── UART1 ─── RS485 트랜시버 #1 ──┬── pH 센서     (addr 0x02, 9600bps)
  │             (MAX485 × 1)         └── EC 센서     (addr 0x04, 9600bps)
  │
  ├── UART2 ─── RS485 트랜시버 #2 ──┬── 잔류염소(CL) 센서
  │             (MAX485 × 1)         └── 탁도(NTU) 센서
  │
  ├── I2C ───── ADS1115 (16-bit 외부 ADC)
  │             └── 아날로그 센서 입력 (전원 완전 분리)
  │
  ├── SPI ───── W5500 또는 ENC28J60 (Ethernet)
  │
  └── WiFi ──── 내장 (OTA 업데이트, 원격 모니터링 추가 가능)
```

### 해결 매핑

| 문제 | 해결 방법 |
|------|----------|
| 멀티센서 RS485 충돌 | UART1 / UART2 물리 채널 분리 |
| 아날로그 노이즈 | 외부 ADS1115 16-bit ADC (I2C), 아날로그 전원 독립 |
| 디버깅/개발 편의 | Arduino/ESP-IDF 생태계, USB 직결 프로그래밍 |
| 원격 관리 | 내장 WiFi로 OTA, MQTT 등 확장 가능 |

### ⚠️ 주의: ESP32 내장 ADC 사용 금지
ESP32 내장 ADC는 비선형 오차 ±6% 수준으로 STM32보다 열악.
**반드시 외부 ADC(ADS1115 등) 사용할 것.**

---

## 통신 프로토콜 유지 사항

- Modbus RTU 슬레이브 (호스트 SCADA 응답) → 유지
- RS485 Modbus 마스터 (센서 폴링) → 유지, 채널 분리로 개선
- 4-20mA 아날로그 출력 → 유지
- Ethernet (ENC28J60 or W5500) → 유지

---

## 크몽 의뢰 시 필수 스펙

```
[개발 목표]
STM32F103VE 기반 수질 측정 컨트롤러를 ESP32 기반으로 재설계

[측정 항목]
- 잔류염소(CL), 탁도(NTU), pH, EC, 온도

[통신]
- RS485 Modbus RTU 마스터 (센서 폴링, 2채널 독립)
- RS232/RS485 Modbus RTU 슬레이브 (호스트 SCADA 응답)
- Ethernet (W5500 권장)
- 4-20mA 아날로그 출력 2채널

[핵심 요구사항]
1. RS485 버스 2채널 물리 분리 (UART1, UART2 각각 독립 MAX485)
2. 아날로그 입력은 외부 16-bit ADC(ADS1115) 사용 — 내장 ADC 사용 불가
3. 기존 Modbus 레지스터 맵 호환 유지
4. 아날로그/디지털 전원 분리 설계

[참고 보드]
현재 V20 보드 회로도 및 펌웨어: waterbee_controller GitHub 참조
```

---

## 참고 파일 (현 STM32 코드)

- `Working/Source_Files/Communication.c` — Modbus 485/232 핸들러
- `Working/Source_Files/Interrupt.c` — USART IRQ 핸들러
- `Working/Include_Files/Communication.h` — DE/RE 핀 매크로
- `Working/sensor/sensor_manager.c` — 센서 매니저 인터페이스
