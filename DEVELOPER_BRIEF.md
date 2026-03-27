# Waterbee Controller — 개발자 브리핑 문서
> 최종 업데이트: 2026-03-28
> 목적: 신규 개발자 온보딩 및 레퍼런스

---

## 1. 제품 개요

Waterbee는 수처리 시설용 수질 측정 컨트롤러 및 센서를 개발하는 회사입니다.

**측정 항목:** 잔류염소(CL), 탁도(NTU), pH, EC(전기전도도), 온도

**현재 구조:**
```
[MAIN 보드] STM32F103VE — LCD/UI/데이터 저장 담당
[SUB 보드]  AVR128DA64  — 센서 통신/아날로그 출력 담당 (→ ESP32로 교체 진행 중)
```

---

## 2. SUB 보드 V21 변경사항 (크몽 의뢰 진행 중)

### 변경 배경
- 기존 AVR128DA64: UART 부족, 아날로그 노이즈, RS485 버스 충돌 문제
- ESP32로 교체하여 모든 문제 해결

### 부품 변경

| 구분 | 기존 | 변경 |
|------|------|------|
| MCU | AVR128DA64 | ESP32-WROOM-32E (LCSC: C701341) |
| UART 확장 | 없음 | SC16IS752IPW (LCSC: C2644862) |
| ADC | 내장 ADC | ADS1115 외부 16-bit ADC (LCSC: C37593) |
| 아날로그 절연 | 없음 (외부 Autonics CN-6100-C1 임시 사용) | ADUM3190 또는 HCNR200 내장 |
| RS485 보호 | 없음 | TVS 다이오드 |
| RS485 종단저항 | 고정 | 120Ω 점퍼 선택형 |

### ⚠️ 핵심 주의사항

**SC16IS752는 반드시 SPI 모드로 배선**
- I2C 모드: Modbus RTU 타이밍 위반 위험 (레이턴시 이슈)
- SPI 4MHz 이상: 안정적인 Modbus RTU 타이밍 확보
- IRQ 핀 ESP32에 연결 필수

**ADS1115 외부 ADC 필수**
- ESP32 내장 ADC 노이즈: 21.8mV → 수질 센서 정밀도에 치명적
- ADS1115 노이즈: 2mV → 10배 차이
- 내장 ADC 절대 사용 금지

**AGND / DGND 완전 분리**
- 4-20mA 출력 절연 회로로 디지털 노이즈 차단

### 견적 (크몽)
- 회로도 수정: 45만원 (5일)
- 아트웍 수정: 60만원 (6일)
- 합계: 105만원
- 납품: KiCad 소스 + Gerber + BOM (LCSC번호 포함)
- 발주처: JLCPCB PCBA

---

## 3. 통신 구조

```
[컨트롤러 SUB 보드 ESP32]
  │
  ├── UART0 ─── 디버그/프로그래밍 (USB)
  │
  ├── SC16IS752 (SPI) ─── UART-A ─── RS485 #1 ─── pH 센서 (addr 0x02, 9600bps)
  │                    └── UART-B ─── RS485 #2 ─── EC 센서  (addr 0x04, 9600bps)
  │
  ├── UART1 ─── RS232 ─── 잔류염소(CL) + 탁도(NTU) 센서  ← Push 방식, 충돌 없음
  │
  ├── UART2 ─── RS485 ─── Modbus RTU Slave (호스트 SCADA 응답)
  │
  ├── SPI ──── W5500 또는 ENC28J60 (Ethernet)
  │
  ├── I2C ──── ADS1115 (외부 16-bit ADC)
  │
  └── GPIO ─── 릴레이 3채널, 4-20mA 출력 2채널
```

### RS232 센서 통신 방식
- **Push 방식**: 센서가 자동으로 데이터를 밀어넣음, 컨트롤러는 수신만
- 프레임 포맷: `$RES1,DDDDD,MMMMM,STTTT\r\n` (24 bytes)
- 충돌 없는 이유: 반이중 버스가 아닌 전이중 개별 선로

### RS485 Modbus RTU
- pH/EC: 각각 독립 채널 (버스 충돌 해결)
- 9600bps, Modbus RTU Master (폴링)
- GC 영향 최소화: `gc.collect()` 폴링 사이클 간 명시 호출

---

## 4. 펌웨어 개발 계획

**언어:** MicroPython (ESP32)

**공통 라이브러리 구조:**
```
lib/
  modbus_rtu.py     # Modbus RTU Master/Slave 공통
  rs485_driver.py   # DE/RE 핀 제어, 타이밍
  sensor_base.py    # 캘리브레이션, 에러처리, NVM 저장
  watchdog.py       # 하드웨어 워치독
  ads1115.py        # 외부 ADC 드라이버

main.py             # 메인 루프
config.py           # 센서 주소, 캘리브레이션 설정
```

**MicroPython GC 관리 전략:**
```python
import gc
gc.collect()  # Modbus 폴링 사이클 사이에 명시 호출
gc.disable()  # 송수신 구간에서 일시 비활성화
```

---

## 5. 제품 로드맵

```
1순위 (진행 중): SUB 보드 V21 ESP32 교체 — 크몽 의뢰
2순위:           무방류 경보기 탁도계 ESP32 보드 (중국 디텍터 스펙 수령 후)
3순위:           무방류 경보기 잔류염소계 ESP32 보드
4순위:           pH/EC 보드 ESP32 재설계 (기회될 때)
```

### 무방류 경보기 센서 스펙

| 항목 | 탁도계 | 잔류염소계 |
|------|--------|----------|
| 측정 방식 | ISO 7027, IR 850~880nm, 90도 산란 | 비시약형 전류법 (Amperometric) |
| 측정 범위 | 0~100 NTU | 0~5 mg/L |
| MCU | ESP32 + MicroPython | ESP32 + MicroPython |
| ADC | ADS1115 | ADS1115 |
| 통신 | RS485 Modbus RTU | RS485 Modbus RTU |
| 중국 협력사 | 램프/디텍터 모듈 공급 (위챗 연결됨) | - |

---

## 6. 경쟁사 레퍼런스

| 회사 | 제품 | 가격 |
|------|------|------|
| Hach | SC200 + CL17sc + TU5 | $5,000~8,000 (700~1100만원) |
| Yokogawa | FLXA402T + TB820D + FC800D | 동급 |
| YSI | EXO 시리즈 | 동급 |
| **Waterbee** | **ESP32 통합 보드** | **10~20만원대 목표** |

**차별화 포인트:**
- 가격 1/50
- 한국어 UI
- 한국 수도법 맞춤 경보 로직
- MicroPython 현장 펌웨어 수정 용이
- JLCPCB PCBA로 빠른 양산

---

## 7. 참고 파일

| 파일 | 위치 | 내용 |
|------|------|------|
| 현 SUB 회로도 | `크몽_의뢰/SUB7-2.pdf` | AVR 기반 현재 보드 |
| Gerber | `크몽_의뢰/WB_SUB7-2-gbr.zip` | PCB 제조 파일 |
| ESP32 설계스펙 | `크몽_의뢰/WaterBee_SubBoard_ESP32_설계스펙.docx` | 핀 배정 상세 |
| STM32 펌웨어 | `firmware/Working/` | 현 C 코드 (참고용) |
| AI 브리핑 | `firmware/GEMINI_BRIEF.md` | 펌웨어 구조 상세 설명 |

> 파일 경로 기준: Google Drive `dust2wood@gmail.com / 내 드라이브 / 6. Waterbee_controller/`
