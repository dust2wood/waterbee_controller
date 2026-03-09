# SENSOR_PH_EC 분기 정리 (통합 후)

## 적용 완료
- Sensor_Manager.c: SENSOR_PH_EC 제거, 4종 통합 등록
- sensor_autodetect.c: RS485 스캔 항상 수행
- Communication.c: init_tx3Buffer, Modbus485Handler #ifdef 제거
- Interrupt.c: $RES1,2,3,4 모두 처리 (CL, NTU, pH, EC)
- ReDisplay_ch1/ch2: water_data 기반 표시, RedrawBottomArea 동적화
- main.c: init_tx3Buffer 항상 호출

## 남은 분기 (Main.h SENSOR_PH_EC 유지 시)
- Communication.c: SensorComHandler, S1PPm_Data_offset (#ifndef SENSOR_PH_EC)
- main.c, Display.c, ReDisplay_ch2.c: 교정/알람/트렌드 등
- ※ 핵심 통로(sensor_manager, 표시)는 통합 완료

---
# SENSOR_PH_EC 분기 정리 (원본)

## 1. 분기 위치 (파일별)

### Sensor_Manager.c
| 라인 | 분기 | 내용 |
|------|------|------|
| 11-15 | #ifdef SENSOR_PH_EC | sensor_ph.h, sensor_ec.h include |
| 31-51 | #ifndef SENSOR_PH_EC | RS232 CL/NTU 드라이버 정의 |
| 67-73 | #ifdef SENSOR_PH_EC | PH, EC autodetect 기반 등록 |
| 75-79 | #ifndef SENSOR_PH_EC | CL, NTU RS232 고정 등록 |

### sensor_autodetect.c
| 라인 | 분기 | 내용 |
|------|------|------|
| 86-94 | #ifdef SENSOR_PH_EC | RS485 스캔 (pH addr 2, EC addr 4) |

### Communication.c
| 라인 | 분기 | 내용 |
|------|------|------|
| 107-170 | #ifndef / #else | SensorComHandler: 통신 타임아웃 시 S2PPM 초기값 (15 vs 0) |
| 307-312 | #ifndef / #else | S1PPm_Data_offset: S2PPM 하한 (15 vs 0) |
| 496-548 | #ifdef SENSOR_PH_EC | init_tx3Buffer(), RS485 폴링 시작 |
| 563-570 | #ifdef SENSOR_PH_EC | Modbus485Handler case 0: init_tx3Buffer 호출 |
| 579-... | #ifdef SENSOR_PH_EC | Modbus485Handler: RS485 수신 파싱 (pH/EC) |

### main.c
| 라인 | 분기 | 내용 |
|------|------|------|
| 251-254 | #ifdef SENSOR_PH_EC | init_tx3Buffer() 호출 |
| 453-464 | #ifndef / #else | Device_Selector_Mode별 처리 |
| 632-721 | #ifndef / #else | 알람 제로 체크 (S1PPM==0, S2PPM) |
| 912-929 | #ifdef SENSOR_PH_EC | PH 관련 상태 처리 |
| 1044-1082 | #ifndef / #else | STATE_CALIB_ZERO 분기 |
| 1373-1408 | #ifndef / #else | 조정 메뉴 |
| 1444-1495 | #ifndef / #else | 트렌드 선택 |
| 3332, ... | #ifndef | 기타 분기 |

### ReDisplay_ch2.c
| 라인 | 분기 | 내용 |
|------|------|------|
| 119-128 | #ifndef / #else | RedrawTitle: display_CL1/2 vs display_O2/elec |
| 172-271 | #ifndef / #else | RedrawViewArea STATE_ALARM: CL+TU vs PH+EC |
| 294-342 | #ifndef / #else | 트렌드 등 small unit |
| 367-390 | #ifndef / #else | 기타 뷰 |
| 406-424 | #ifndef / #else | 표시 값 포맷 |
| 627 | #ifndef | 트렌드 관련 |
| 1394-1400 | #ifndef / #else | display_set3_buff |
| 1557-1576 | #ifndef / #else | RedrawBottomArea_CH2: set_cl/tu vs set_ph/ec |

### Display.c
| 라인 | 분기 | 내용 |
|------|------|------|
| 44-95 | #ifndef / #else | DrawTrandDiamond 등 |
| 142-184 | #ifndef / #else | Alarm 뷰 |
| 252-304 | #ifndef / #else | Config 뷰 |
| 318-324 | #ifdef | display_zero_W_XY |
| 1092-1334 | #ifndef / #else | 트렌드 관련 |
| 1511-1689 | #ifndef / #else | Config 출력 등 |

### Interrupt.c
| 라인 | 분기 | 내용 |
|------|------|------|
| 560-567 | #ifndef / #else | $RES1,2→CL,NTU vs $RES3,4→pH,EC (comm_S1PPM, comm_S2NTU) |
| 581-593 | #ifndef / #else | mV 데이터 채널 |
| 612-631 | #ifndef / #else | 온도 데이터 채널 |

---

## 2. 데이터 충돌 분석

| 변수 | PH/EC 모드 소스 | CL/NTU 모드 소스 |
|------|-----------------|------------------|
| S1PPM | RS485 addr 2 (pH) | RS232 $RES1 (CL) → comm_S1PPM |
| S2PPM | RS485 addr 4 (EC) | RS232 $RES2 (NTU) → comm_S2NTU |

통합 시: RS232 $RES1=CL, $RES2=NTU, $RES3=pH, $RES4=EC. RS485는 pH→S1PPM, EC→S2PPM.
- PH와 CL 모두 S1PPM 사용 → 동시 활성 시 충돌
- 해결: sensor_manager에 등록된 센서별로 독립 데이터 소스 사용 필요. 현재는 표시 시 water_field_t에 따라 S1PPM/S2PPM/water_data.ntu 선택.

---

## 3. 최소 수정 통합 계획

### Phase A: sensor_manager 통합 (핵심)
- SENSOR_PH_EC 분기 제거
- CL, NTU RS232 드라이버 항상 정의
- PH, EC autodetect 시 등록 + CL, NTU 항상 등록
- 등록 순서: PH, EC (autodetect), CL, NTU → 최대 4개, 표시는 앞 2개

### Phase B: sensor_autodetect
- #ifdef SENSOR_PH_EC 제거 → RS485 스캔 항상 수행

### Phase C: Communication
- init_tx3Buffer, Modbus485Handler: #ifdef 제거, autodetect 결과에 따라 동작 (이미 내부에서 체크)
- SensorComHandler, S1PPm_Data_offset: 통합 시 CL/NTU/PH/EC 공존을 위해 값 덮어쓰기 정책 결정 필요

### Phase D: Interrupt.c RS232
- $RES1,2,3,4 모두 처리 (CL, NTU, pH, EC)
- comm_S1PPM, comm_S2NTU만으로는 4종 분리 불가 → comm_cl, comm_ntu, comm_ph, comm_ec 또는 기존 변수로 채널 분리

### Phase E: UI (ReDisplay_ch2, Display, main)
- Device_Selector_Mode 대신 sensor_manager_count(), get_display_field() 사용
- SENSOR_PH_EC 분기 제거, 동적 표시

---

## 4. 데이터 소스 매핑 (통합)

| water_field_t | 데이터 소스 | 비고 |
|---------------|-------------|------|
| PH | currentData.S1PPM (RS485) 또는 comm_S1PPM ($RES3) | RS485 우선? |
| EC | currentData.S2PPM (RS485) 또는 comm_S2NTU ($RES4) | |
| CL | comm_S1PPM ($RES1) → S1PPM | S1PPm_Data_offset 호출 시 |
| NTU | comm_S2NTU ($RES2) → S2PPM | |

S1PPm_Data_offset는 comm_S1PPM, comm_S2NTU → S1PPM, S2PPM 변환. 현재 PH/EC/CL/NTU 모두 이 경로 또는 RS485 직접写入 사용. 통합 시:
- RS485 수신: Modbus485Handler가 S1PPM(pH), S2PPM(EC) 직접 갱신
- RS232 수신: $RES1→comm_S1PPM(CL), $RES2→comm_S2NTU(NTU), $RES3→comm_S1PPM(pH), $RES4→comm_S2NTU(EC)
- S1PPm_Data_offset가 comm→S1PPM,S2PPM 변환 시, PH와 CL이 comm_S1PPM을 공유. $RES1과 $RES3가 같은 변수 사용.

결론: 변수 분리 필요. comm_cl, comm_ntu 추가 또는 comm_S1PPM을 "채널1", comm_S2NTU를 "채널2"로 두고 RS232는 $RES1,2만 받도록 단순화. ($RES3,4는 RS485가 있을 때 사용 안 함)

최소 수정: RS232는 $RES1(CL), $RES2(NTU)만. RS485는 pH, EC. 4종이 동시에 두 포트에 연결되지 않는다고 가정.
