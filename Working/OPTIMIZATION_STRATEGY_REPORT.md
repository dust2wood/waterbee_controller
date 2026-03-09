# 전략 A/B/C 종합 분석 보고서

STM32F103VE (512KB Flash) Waterbee Controller 프로젝트

---

## 전략 A: Dead Code 제거

### 1. 링커 리포트(UNUSED) 기준 미사용 함수

ID_Solution.htm 맵 기반으로 **UNUSED**로 표시된 함수들:

| 파일 | 함수 | 예상 절감(bytes) |
|------|------|------------------|
| display.o | DrawRelay2Alarm | 28 |
| display.o | DrawRelayAlarm | 2 |
| display.o | DrawAdjustFactoryReset | 2 |
| display.o | **DrawCalibAuto** | **1048** |
| display.o | **DrawCalibManual** | **1004** |
| display.o | **DrawCalibZero** | **1184** |
| display.o | **DrawCalibBuff** | **1158** |
| display.o | DrawBottomCurrent | 30 |
| main.o | ReadSenosr2ADC | 2 |
| main.o | **State_CalibZero** | **892** |
| main.o | **State_CalibManual** | **1012** |
| main.o | ClearstrBuffer | 18 |
| nandinterface.o | NAND_ReadSNumber, NAND_ReadMNumberW/Y, NAND_ReadLNumber | ~64 |
| nand.o | FSMC_NAND_ReadID, WriteSmallPage, EraseBlock 등 | ~600+ |
| tft.o | TFT_WR_CMD, TFT_WR_Data_8, TFT_DrawImageSmall 등 | ~800+ |
| sdio_sd.o | SD_Erase, SD_ProcessIRQSrc 등 | ~1500+ |
| display_icon.o | NEMERIC_* 변형, DrawLargeNumber, Draw_Back_Oval* 등 | ~1500+ |
| icon_ch2_ch4.o | display_4ch_*, Draw_4CH_* (4CH 미사용 시) | ~800+ |

**주의:** UNUSED 함수는 `#ifdef` 등 조건부 컴파일로 특정 빌드에서만 사용될 수 있음.  
삭제 전 호출 관계를 확인할 것.

### 2. #ifdef 블록 정리 대상

| 파일 | 블록 | 비고 |
|------|------|------|
| Main.h | SENSOR_PH_EC, CH2, CH4, BUZ_ENABLE 등 | 기능 선택용, 유지 권장 |
| ReDisplay_ch2.c | #ifdef SENSOR_PH_EC / #else | PH+EC vs CL+TU 분기, 유지 |
| main.c | #ifndef SENSOR_PH_EC (온도 캘리) | 현재 SENSOR_PH_EC 정의됨 |
| interrupt.c | #ifdef BUZ_ENABLE | 부저 미사용 시 분기 |
| Tft.c, Button.c, Communication.c | 다수 #ifdef | 하드웨어/통신 옵션 |

**권장:** 사용하지 않는 옵션(예: CH4, BUZ_ENABLE)은 `#define` 제거로 해당 블록만 제거.  
전체 삭제는 빌드 옵션 변경 시 복구 어려우므로 신중히.

### 3. 중복/레거시 의심 코드

- `ReDisplay.c` vs `ReDisplay_ch2.c`: uvprojx에서 ReDisplay.c 빌드 제외 완료.  
  ReDisplay.c 파일 자체 삭제는 향후 4CH 전용 빌드에 필요할 수 있어 유지 가능.
- `sensor_manager_init` vs `SensorManager_Init`: main.c에 둘 다 호출됨.  
  레거시 통합 여부 검토 필요.

---

## 전략 B: 메모리(플래시) 가계부

### 맵 기반 Top 5 용량 추정 (함수 크기 합산)

| 순위 | 오브젝트 | 대표 함수/크기 | 예상 영역 |
|------|----------|----------------|-----------|
| 1 | **main.o** | RedrawViewArea 5760B, State_CalibLog 2936B | UI/상태머신 |
| 2 | **interrupt.o** | USART1_IRQHandler 2378B, TIM3 502B | 통신/타이머 |
| 3 | **display.o** | DrawCalibLog 1190B, DrawCalibSpan 1416B | 캘리/그래프 |
| 4 | **LwIP (tcp, ip 등)** | 네트워크 스택 | 이더넷 사용 시 |
| 5 | **nanddisplay.o / display_icon.o** | NAND 읽기/숫자 렌더 | UI 리소스 |

### 절감 제안

1. **main.o (RedrawViewArea 등)**  
   - 화면별 `RedrawViewArea` 분리(예: `RedrawMainArea`, `RedrawConfigArea`)  
   - 현재 상태에 해당하는 핸들러만 링크되도록 분리 → 미사용 화면 코드 제거

2. **printf / sprintf**  
   - `-specs=nano.specs` 사용 여부 확인  
   - 불필요한 printf 제거, 고정 문자열은 `const`로 두어 ROM 배치

3. **LwIP**  
   - 이더넷 미사용 시 LwIP 소스 제외  
   - 사용 시 필요한 프로토콜만 활성화(TCP/UDP 등)

4. **NAND/이미지**  
   - NAND 폰트/아이콘 블록 번호 재배치로 중복 제거  
   - 자주 쓰는 리소스만 로딩, 나머지는 온디맨드

5. **컴파일 옵션**  
   - `-O2` 또는 `-Os` 적용으로 코드 크기 최소화  
   - 링크 시 `--gc-sections` 로 미참조 섹션 제거

---

## 전략 C: 똑같은 일 두 번 하지 말기 (Logic 최적화)

### 현재 구조

```
main loop
  → sensor_manager_update()     // 매 루프 1센서 폴링
  → Modbus232/485/Ethernet      // 통신으로 currentData 갱신
  → (여러 곳에서) RedrawValue() / RedrawMainValue() 호출
```

**RedrawValue/RedrawMainValue 호출 위치:**
- Communication.c: ComCouter==0, 수신 처리 후
- main.c: 버튼/상태 전환 시 다수

### 문제점

- `RedrawMainValue()` → `Display_Update()` → `ReDisplay_ch1/2/4()`  
  값이 바뀌지 않아도 매번 전부 다시 그림
- NAND에서 폰트/아이콘을 반복 로드  
  → CPU·NAND 부하 증가, 버벅임 가능

### 제안: 값 변경 시에만 갱신

1. **이전 값 저장**

```c
// display_update.c 또는 전용 모듈
static uint32_t s_prev_S1PPM, s_prev_S2PPM;
static int16_t s_prev_temp, s_prev_temp1;

int Display_NeedsUpdate(void) {
    if (currentData.S1PPM != s_prev_S1PPM) return 1;
    if (currentData.S2PPM != s_prev_S2PPM) return 1;
    if (currentData.temperature != s_prev_temp) return 1;
    if (currentData.temperature1 != s_prev_temp1) return 1;
    return 0;
}

void Display_UpdateIfChanged(void) {
    if (!Display_NeedsUpdate()) return;
    s_prev_S1PPM = currentData.S1PPM;
    s_prev_S2PPM = currentData.S2PPM;
    s_prev_temp = currentData.temperature;
    s_prev_temp1 = currentData.temperature1;
    Display_Update();
}
```

2. **호출부 변경**

```c
// RedrawMainValue() 내부 또는 호출 직전
void RedrawMainValue(void) {
    Display_UpdateIfChanged();  // 기존: Display_Update()
}
```

3. **타이머 기반 스로틀**

- 10ms/100ms 주기마다 `Display_UpdateIfChanged()` 호출  
  → 짧은 주기로 확인하고, 변경 시에만 화면 갱신

### 기대 효과

- 값 유지 시: NAND 읽기·TFT 그리기 생략  
- CPU 사용 감소, 화면 깜빡임·버벅임 완화

---

## 실행 우선순위 제안

| 순위 | 전략 | 위험도 | 예상 효과 |
|------|------|--------|-----------|
| 1 | **C: 변경 시에만 갱신** | 낮음 | CPU·NAND 부하 감소, 반응성 개선 |
| 2 | **A: UNUSED display/icon 함수** | 중간 | Flash ~2~3KB 절감 (검증 필요) |
| 3 | **B: 컴파일·링크 옵션** | 낮음 | 전체 5~15% 절감 가능 |
| 4 | **B: LwIP/이더넷 조건부** | 중간 | 이더넷 미사용 시 수십 KB 절감 |
| 5 | **A: #ifdef 정리** | 높음 | 옵션 복귀 시 빌드 깨짐 위험 |

---

*분석 기준: Working/Out/ID_Solution.htm (2025-06-09 빌드), 소스 구조 검토*
