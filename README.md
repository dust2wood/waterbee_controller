# Waterbee Controller

STM32F103VE 기반 수질 측정 컨트롤러 펌웨어 프로젝트입니다.  
잔류염소(Cl), 탁도(NTU), pH, EC(전기전도도), 온도 등의 수질 파라미터를 실시간으로 측정·표시·기록합니다.

---

## 하드웨어 스펙

| 항목 | 내용 |
|------|------|
| MCU | STM32F103VE (ARM Cortex-M3, 72MHz) |
| Flash | 512KB (0x08000000) |
| SRAM | 64KB (0x20000000) |
| 디스플레이 | TFT LCD (FSMC 인터페이스) |
| NAND Flash | FSMC 인터페이스 (트렌드 데이터 저장) |
| EEPROM | SPI2 (PB12~PB15, 설정값 저장) |
| SD Card | SDIO 인터페이스 (데이터 로깅) |
| 통신 | RS-232 / RS-485 / Ethernet (ENC28J60) |
| 릴레이 출력 | 3채널 (세정, 경보, 릴레이 출력) |
| 아날로그 출력 | 4~20mA (DAC7512, SPI) |
| 아날로그 입력 | ADC (센서 전압/전류 측정) |
| 터치 | 저항막 방식 터치 스크린 |
| RTC | 내장 RTC (배터리 백업) |

---

## 폴더 구조

```
waterbee_controller/
├── Working/                        # 메인 개발 프로젝트 (Keil MDK)
│   ├── Source_Files/               # 프로젝트 핵심 소스 파일 (.c)
│   ├── Include_Files/              # 프로젝트 헤더 파일 (.h)
│   ├── CMSIS/                      # ARM Cortex-M3 코어 드라이버
│   ├── StdPeriph_Driver/           # STM32F10x 표준 주변장치 라이브러리
│   ├── Drivers/                    # STM32F1xx HAL 드라이버
│   ├── FATFS_V0.08A/               # FAT 파일시스템 라이브러리
│   ├── LwIP/                       # 경량 TCP/IP 스택
│   ├── source_ethernet/            # ENC28J60 이더넷 드라이버 및 Modbus TCP
│   ├── Inc_ethernet/               # 이더넷 관련 헤더
│   ├── image/                      # UI 배경 이미지 헤더
│   ├── Out/                        # 빌드 출력물
│   ├── ID_Solution.uvprojx         # Keil MDK 프로젝트 파일 (μVision5)
│   └── startup_stm32f10x_hd.s      # 시작 어셈블리 (HD 시리즈)
│
└── FlashDownload/                  # 플래시 다운로드용 별도 프로젝트
    ├── Source_Files/               # 플래시 다운로드 소스
    ├── Include_Files/              # 플래시 다운로드 헤더
    ├── IMAGE*/                     # LCD 표시용 이미지 데이터 (배열 형태)
    └── ID_Solution.uvproj          # Keil MDK 프로젝트 파일 (μVision4)
```

---

## 소스 파일 역할 (Working/Source_Files/)

| 파일 | 역할 |
|------|------|
| `main.c` | 메인 루프, 상태 머신, 전역 변수 정의, 릴레이/경보/세정 처리 |
| `Initialize.c` | 시스템 초기화 (RCC, GPIO, TIM, ADC, FSMC, SPI, UART, RTC 등) |
| `Interrupt.c` | 타이머/UART 인터럽트 핸들러 (10ms 주기 틱, 시리얼 수신) |
| `Communication.c` | 센서 RS-232/RS-485 통신, Modbus RTU 슬레이브, 교정 명령 처리 |
| `Display.c` | 메뉴/설정/교정/트렌드 화면 렌더링 |
| `ReDisplay.c` | 메인 측정 화면 (4채널) 재렌더링 |
| `ReDisplay_ch2.c` | 메인 측정 화면 (2채널) 재렌더링 |
| `NANDDisplay.c` | NAND Flash에서 트렌드 데이터 읽어서 그래프 출력 |
| `NandInterface.c` | NAND Flash 읽기/쓰기 인터페이스 |
| `NAND.c` | NAND Flash 저수준 드라이버 |
| `Tft.c` | TFT LCD 저수준 드라이버 (픽셀, 선, 문자, 이미지 출력) |
| `Touch2.c` | 터치 스크린 좌표 읽기 및 터치 감지 |
| `Button.c` | 터치 기반 버튼 이벤트 처리 |
| `CurrentSorceTrans.c` | 4~20mA 전류 출력 변환 (DAC7512 SPI 제어) |
| `DAC7512Interface.c` | DAC7512 SPI 인터페이스 드라이버 |
| `Adc_Read.c` | ADC 채널 읽기 (센서 전압/전류 측정) |
| `Flash.c` | 내장 Flash 읽기/쓰기 (설정값 영구 저장) |
| `eeprom.c` | SPI EEPROM 읽기/쓰기 |
| `RTC_Time.c` | RTC 시각 설정/읽기, 날짜/시간 문자열 변환 |
| `serial.c` | UART1/2/3 시리얼 통신 드라이버 |
| `Watchdog.c` | IWDG 워치독 타이머 설정 및 리셋 |
| `MicroSd.c` | SD 카드 파일 쓰기 (CSV 형식 데이터 로깅) |
| `sdio_sd.c` | SDIO 기반 SD 카드 저수준 드라이버 |
| `display_icon.c` | 공통 아이콘 및 UI 요소 렌더링 |
| `icon_ch2_ch4.c` | 2채널/4채널 선택 아이콘 렌더링 |

---

## 주요 기능

### 1. 수질 측정
- **잔류염소 (Cl)**: 2채널 또는 4채널 측정 (`#define CH2` / `#define CH4`)
- **탁도 (NTU)**: 광학 탁도 센서 측정
- **pH**: 버퍼 교정(pH 4.0 / 7.0) 지원
- **EC (전기전도도)**: μS/cm 측정
- **온도**: 4채널 온도 보정 지원
- 측정값 필터링 (이동평균), 기울기/오프셋 보정

### 2. 디스플레이 UI (상태 머신)
- **메인 화면**: 실시간 측정값, 센서 상태, 시간, 릴레이 상태
- **메뉴 → 설정**: 출력, 통신, 릴레이, 시간, 조정
- **메뉴 → 교정**: 제로 교정, 수동 교정, 온도 교정, 교정 로그
- **메뉴 → 트렌드**: NAND Flash 기반 트렌드 그래프
- **메뉴 → 경보 설정**: 상/하한 경보 임계값
- **메뉴 → 진단**: 센서 상태 진단

### 3. 통신
- **RS-232**: 센서 데이터 수신 (UART1)
- **RS-485**: Modbus RTU 슬레이브 (UART2, PC4 방향 제어)
- **Ethernet**: ENC28J60 + LwIP + Modbus TCP (옵션)
- 통신 타입 선택: 4~20mA / RS-232 / RS-485

### 4. 데이터 저장
- **내장 Flash** (`0x08070000`): 설정값(ConfigSet) 영구 저장
- **SPI EEPROM**: 보조 설정 저장
- **NAND Flash**: 트렌드 데이터 저장 (분당 또는 초당 기록)
- **SD Card (SDIO)**: CSV 형식 데이터 로깅

### 5. 릴레이 및 출력
- **릴레이 1**: 자동 세정 주기 제어
- **릴레이 2**: 경보 출력
- **릴레이 3**: 릴레이 출력 (수동/자동)
- **4~20mA 출력**: DAC7512로 측정값 아날로그 출력

---

## 빌드 방법

### 필수 도구
- **Keil MDK-ARM** (μVision5 권장)
- STM32F1xx DFP 패키지 (Keil.STM32F1xx_DFP.2.3.0)

### 빌드 절차
1. Keil MDK 실행
2. `Working/ID_Solution.uvprojx` 열기
3. `Project → Build Target` (F7) 실행
4. 빌드 산출물: `Working/Out/ID_Solution.axf`

### 플래시 다운로드
- `FlashDownload/ID_Solution.uvproj` 사용 (μVision4 형식)
- J-Link 또는 ST-Link 연결 후 `Flash → Download` 실행

---

## 조건부 컴파일 옵션 (Include_Files/Main.h)

| 매크로 | 설명 |
|--------|------|
| `#define CH2` | 2채널 측정 모드 활성화 |
| `#define CH4` | 4채널 측정 모드 활성화 |
| `#define NEW_BOARD` | 신버전 보드 핀맵 사용 (릴레이/외부입력 GPIO 변경) |
| `#define SENSOR_PH_EC` | pH / EC 센서 기능 활성화 |
| `#define SAVE_1_MINUTE` | 1분 주기 데이터 저장 (미정의시 3분) |
| `#define SAVE_SECOND` | 초단위 데이터 저장 (60개/분) |
| `#define BUZ_ENABLE` | 부저 출력 활성화 |

---

## 센서 상태 코드

| 코드 | 의미 |
|------|------|
| `SENSOR_OK (0)` | 정상 |
| `SENSOR_ERROR1 (1)` | 센서 범위 초과 |
| `SENSOR_ERROR2 (2)` | 램프 이상 |
| `SENSOR_ERROR3 (3)` | 통신 불가 / 센서 연결 실패 |
