# CL/NTU autodetect 경로 및 sensor_manager 등록 흐름 조사

## 1. CL/NTU가 autodetect 경로에 들어가는지 여부

### NTU
- **등록됨**: `sensor_autodetect_adc` → `sensor_manager_register(&sensor_adc_driver, WATER_FIELD_NTU)`
- **경로**: `sensor_autodetect_run()` → `probe_adc()` (ADC 4-20mA 유효 여부) → sensor_autodetect_adc=1
- **데이터 소스**: `sensor_adc_driver.read()` → `Read_Channel_ADC1()` (ADC 직접 읽기)
- **결론**: NTU는 **autodetect 경로에 있음** (ADC probe 기반). 그러나 NTU는 RS232 고정 입력이어야 함.

### CL
- **등록되지 않음**: sensor_manager_init()에 CL 등록 코드 없음
- **autodetect 없음**: sensor_autodetect_cl 등 없음
- **데이터 소스**: RS232 (USART1) $RES1,xxxxx → currentData.comm_S1PPM → S1PPm_Data_offset_function() → currentData.S1PPM
- **결론**: CL은 autodetect 경로에 **없음**. 다만 sensor_manager에도 **등록되지 않아** 표시 후보에 포함되지 않음.

---

## 2. sensor_manager 등록 흐름

```
main() boot
  └─ sensor_autodetect_run()
       ├─ RS485 스캔 (addr 1~10) → sensor_autodetect_ph (addr 2), sensor_autodetect_ec (addr 4)
       └─ ADC probe → sensor_autodetect_adc
  └─ sensor_manager_init()
       ├─ if (sensor_autodetect_ph)  sensor_manager_register(&sensor_ph_driver, WATER_FIELD_PH)
       ├─ if (sensor_autodetect_ec)  sensor_manager_register(&sensor_ec_driver, WATER_FIELD_EC)
       └─ if (sensor_autodetect_adc) sensor_manager_register(&sensor_adc_driver, WATER_FIELD_NTU)  ← NTU
       (CL 등록 없음)
```

---

## 3. 데이터 소스 정리

| 센서 | 현재 등록 | autodetect | 실제 데이터 소스 |
|------|-----------|------------|------------------|
| PH   | sensor_manager | sensor_autodetect_ph (RS485 addr 2) | RS485 Modbus485Handler → currentData.S1PPM |
| EC   | sensor_manager | sensor_autodetect_ec (RS485 addr 4) | RS485 Modbus485Handler → currentData.S2PPM |
| NTU  | sensor_manager | sensor_autodetect_adc (ADC probe) | ADC 4-20mA (sensor_adc_read) → water_data.ntu |
| CL   | 미등록    | 없음       | RS232 $RES1 → comm_S1PPM → S1PPM (currentData.S1PPM) |

---

## 4. RS232 수신 (Interrupt.c)

- **$RES1,xxxxx**: SENSOR_1_MODE일 때 comm_S1PPM (CL)
- **$RES2,xxxxx**: SENSOR_2_MODE일 때 comm_S2NTU (NTU)
- **$RES3,xxxxx**: SENSOR_PH_EC 모드에서 comm_S1PPM (pH)
- **$RES4,xxxxx**: SENSOR_PH_EC 모드에서 comm_S2NTU (EC)

CL/NTU는 RS232 포트 고정, $RES1(채널1), $RES2(채널2)로 수신.

---

## 5. 수정 적용 내용

1. **NTU**: sensor_autodetect_adc 기반 등록 제거 (ADC 경로 사용 안 함)
2. **CL**: RS232 고정 센서로서 sensor_manager에 등록 (#ifndef SENSOR_PH_EC 시)
3. **PH/EC**: RS485 autodetect만 사용 (유지)
4. **sensor_manager**: 표시 후보 수집자
   - PH, EC: autodetect 결과로만 등록
   - CL, NTU: RS232 고정 등록 (autodetect 미사용)
