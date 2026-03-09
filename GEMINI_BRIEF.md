# Waterbee Controller — Codebase Brief for AI Assistant

## 1. Project Overview

- **Target MCU**: STM32F103VE (ARM Cortex-M3, 72MHz, 512KB Flash, 64KB SRAM)
- **Purpose**: Water quality monitoring controller
- **Sensors measured**: Residual Chlorine (Cl), Turbidity (NTU), pH, EC (conductivity), Temperature
- **Build tool**: Keil MDK-ARM (μVision5), project file: `Working/ID_Solution.uvprojx`

---

## 2. Hardware Interfaces

| Interface | Purpose |
|-----------|---------|
| UART1 (RS-232) | Proprietary sensor protocol (`$RESV,...` ASCII frames) |
| UART3 (RS-485) | Modbus RTU slave/master — external sensors (pH, EC) + host SCADA |
| UART2 (RS-485) | Modbus RTU slave to host PC/PLC |
| SPI2 | EEPROM (config storage) + DAC7512 (4-20mA output) |
| FSMC | TFT LCD display + NAND Flash (trend data) |
| SDIO | SD card (CSV data logging) |
| ADC | 4-20mA current input from sensors |
| GPIO | 3x relay outputs, buzzer, touch screen |
| Ethernet | ENC28J60 + LwIP + Modbus TCP (optional, compile-time) |

---

## 3. Software Architecture

### Main Loop (main.c)
```
Initialize() → Load config (Flash/EEPROM) → sensor_manager_init()
Loop:
  sensor_manager_update()   ← poll external pH/EC via RS485 Modbus
  Communication()           ← poll chlorine/turbidity sensor via RS232
  Modbus485Handler()        ← respond to host SCADA queries
  Display_Update()          ← refresh TFT based on UI mode
  Relay/Alarm logic
  SD card logging
  Watchdog feed
```

### UI State Machine (Display.c, ReDisplay_ch1/ch2/ch4.c)
- `g_ui_mode` (type `ui_mode_t`): `UI_MODE_1CH` / `UI_MODE_2CH` / `UI_MODE_4CH`
- Mode is determined at boot by `GetUiMode()` based on `sensor_manager_count()`
- **1CH**: single sensor centered on screen
- **2CH**: Cl+NTU (or PH+EC) side by side
- **4CH**: 2×2 grid, unused channels show `--`

### Sensor Manager (Sensor_Manager.c/h)
- Generic driver table: `g_ext_sensors[]` array of `sensor_entry_t`
- Each entry has: `sensor_driver_t` (init/read/status callbacks) + RS485 Modbus address
- Current drivers: `sensor_ph_driver`, `sensor_ec_driver`
- `sensor_manager_update()` cycles through all entries, calls Modbus RTU polls

### Sensor Driver Interface (Sensor_Manager.h)
```c
typedef struct {
    void    (*init)(void);
    float   (*read)(void);       // returns SENSOR_INVALID_FLOAT on error
    uint8_t (*status)(void);     // 0=OK, 1=error
} sensor_driver_t;
```

---

## 4. Key Data Structures (Main.h)

### DataSet (currentData)
```c
typedef struct {
    uint32_t S1PPM;        // Chlorine (×100) or pH (×100)
    uint32_t S2PPM;        // Turbidity raw or EC
    uint32_t S3PH;         // pH (4CH mode)
    uint32_t S4EC;         // EC (4CH mode)
    uint32_t comm_S1PPM;   // raw comm value before filtering
    uint32_t comm_S2NTU;
    int16_t  temperature;  // ×100, e.g. 2500 = 25.00°C
    int16_t  temperature1, temperature3, temperature4;
    uint32_t S1mVolt, S2mVolt, S3mVolt, S4mVolt;  // mV readings
    uint32_t Device_Selector_Mode;  // bitmask: SENSOR_1_MODE, SENSOR_2_MODE ...
} DataSet;
```

### ConfigSet (configData) — stored in internal Flash @ 0x08070000
```c
typedef struct {
    CommConfig       commConfig;       // baud, protocol, Modbus address
    RelayConfig      relayConfig;      // relay thresholds, timing
    AlarmConfig      alarmConfig;      // upper/lower alarm limits
    CalibrationConfig calibrationConfig; // span/offset for each channel
    DisplayConfig    displayConfig;
} ConfigSet;
```

---

## 5. Communication Protocols

### RS-232 Sensor Protocol (UART1 @ 115200)
ASCII frame format:
- **Measurement**: `$RESV,DDDDD,MMMMM,STTTT\r\n` (24 bytes)
  - D=data, M=mV, S=sign, T=temp×100
- **Multi-sensor**: `$RES1,DDDDD,...` to `$RES4,...`
- **Calibration response**: `$RESD,DDDDD\r\n` (12 bytes)

### RS-485 Modbus RTU (UART3 @ 9600)
- Standard Modbus RTU frames
- CRC16 validation
- `probe_modbus_addr(addr)` used at boot for auto-detection
- Direction control: `RS485_DRIVE_HIGH` (TX) / `RS485_DRIVE_LOW` (RX)

### RS-485 Modbus RTU Slave (UART2 — host SCADA)
- Handled in `Communication.c` → `Modbus485Handler()`
- Exposes holding registers: measurement values, config, relay status

### Modbus TCP (optional, ENC28J60)
- Enabled at compile time (LwIP + source_ethernet/)
- Mirrors RTU register map

---

## 6. Conditional Compile Flags (Main.h)

| Macro | Effect |
|-------|--------|
| `#define CH2` | 2-channel mode (Cl + NTU, or PH + EC) |
| `#define CH4` | 4-channel mode (Cl + NTU + PH + EC) |
| `#define NEW_BOARD` | New PCB pin mapping (relay/input GPIOs differ) |
| `#define SENSOR_PH_EC` | Enable pH/EC sensor paths |
| `#define SAVE_1_MINUTE` | Trend save interval: 1 min (default: 3 min) |
| `#define SAVE_SECOND` | Trend save interval: 1 sec |
| `#define BUZ_ENABLE` | Enable buzzer |

---

## 7. Sensor State Codes

| Code | Value | Meaning |
|------|-------|---------|
| `SENSOR_OK` | 0 | Normal |
| `SENSOR_ERROR1` | 1 | Out of range |
| `SENSOR_ERROR2` | 2 | Lamp fault (NTU sensor) |
| `SENSOR_ERROR3` | 3 | No communication / disconnected |

Timeout logic: `SensorN_OK_TIME` counter decrements per second in TIM3 ISR. Hits 0 → `SENSOR_ERROR3`.

---

## 8. Bug Fixes Applied (2026-03-08, 11 items)

### P0 — Build Errors
1. `main.c`: `Sensor_Manager_Init()` → `sensor_manager_init()` (undefined call)
2. `main.c`: `Update_All_External_Sensors()` → `sensor_manager_update()` (undefined call)

### P1 — Runtime Misbehavior
3. `main.c`: `char led` uninitialized → `led = 0`
4. `main.c`: `delay_temp` → `volatile uint16_t delay_temp` (prevent compiler optimization killing delay)
5. `Communication.c`: Missing error recovery in `SENSOR_PH_EC` mode → added `ComCouter` timeout-based error handling
6. `main.c`: Added `#include "ui_mode.h"` + `GetUiMode()` call to initialize `g_ui_mode`

### P2 — Logic Errors
7. `Communication.c`: `uint32_t S2PPM < 0` always false → cast to `(int32_t)`
8. `Communication.c`: EC clamp upper limit `20000` → `200000` (allows `sensor_ec_read()` high-range branch to be reached)
9. `sensor_ph.c` / `sensor_ec.c`: Local `#define SENSOR_OK/SENSOR_ERROR` conflicting with `Main.h` → removed local defines

### Design Fixes
10. `Main.h`: Declaration `Trend_Data_move()` → `Trend_Data_calc()` (match actual function name)
11. `ui_mode.c/h`: Added `g_ui_mode` global, `GetUiMode()` now sets it

---

## 9. Extension Points (Where to Add Features)

### Adding a New Third-Party Sensor (RS485 Modbus)
1. Create `sensor_xxx.c` implementing `sensor_driver_t` (init/read/status)
2. Add entry to `g_ext_sensors[]` in `Sensor_Manager.c` with its Modbus address
3. `sensor_manager_update()` will auto-poll it
4. Add auto-detection in `sensor_autodetect.c` → `probe_modbus_addr(addr)`
5. Map value to a `currentData` field and display in `ReDisplay_ch4.c`

### Adding a New Third-Party Sensor (4-20mA Analog)
1. Use `Adc_Read.c` → `Read_Channel_ADC1()` / `Read_Channel_ADC2()`
2. Scale raw ADC (0–4095) to engineering units using span/offset from `configData.calibrationConfig`
3. Assign to a `currentData` field

### Adding English Language Support
- All Korean strings are currently hardcoded in:
  - `Display.c` — menus, settings, calibration screens
  - `ReDisplay.c`, `ReDisplay_ch2.c`, `ReDisplay_ch4.c` — main measurement screen labels
  - `display_icon.c` — icons with embedded Korean text
- Approach: Add `#define LANG_EN` flag + string table or `#ifdef LANG_EN` branches per label
- TFT text function: `TFT_Printf(x, y, color, bg, font, "text")` in `Tft.c`

### Modbus Register Map (for SCADA integration)
- Slave address configured in `configData.commConfig`
- Registers exposed by `Modbus485Handler()` in `Communication.c`
- To add new registers: extend the register read/write cases in that function

---

## 10. File Map (Working/Source_Files/)

| File | Role |
|------|------|
| `main.c` | Main loop, state machine, relay/alarm logic |
| `Initialize.c` | System init (GPIO, timers, UART, SPI, ADC, FSMC) |
| `Interrupt.c` | TIM2/TIM3 ISR (10ms tick, watchdog), UART1/2/3 ISR |
| `Communication.c` | RS232 sensor poll, Modbus RTU slave (UART2) |
| `Sensor_Manager.c/h` | Generic RS485 Modbus sensor driver table |
| `sensor_ph.c/h` | pH sensor driver |
| `sensor_ec.c/h` | EC sensor driver |
| `sensor_autodetect.c/h` | Boot-time RS485 + ADC sensor detection |
| `sensor_adc.c/h` | ADC sensor (4-20mA) read |
| `ui_mode.c/h` | UI mode selection (1CH/2CH/4CH) |
| `display_update.c/h` | Top-level display dispatcher |
| `Display.c` | Menu/settings/calibration screens |
| `ReDisplay_ch1/ch2/ch4.c` | Main measurement screen rendering |
| `NANDDisplay.c` | Trend graph from NAND Flash |
| `NandInterface.c` / `NAND.c` | NAND Flash R/W |
| `Tft.c` | TFT LCD low-level driver |
| `Touch2.c` | Touch screen coordinate reading |
| `Button.c` | Touch button event handling |
| `CurrentSorceTrans.c` | 4-20mA output conversion |
| `DAC7512Interface.c` | DAC7512 SPI driver |
| `Adc_Read.c` | ADC channel read |
| `Flash.c` | Internal Flash R/W (config storage) |
| `eeprom.c` | SPI EEPROM R/W |
| `RTC_Time.c` | RTC set/read |
| `serial.c` | UART1/2/3 driver |
| `Watchdog.c` | IWDG watchdog |
| `MicroSd.c` | SD card CSV logging |
| `sdio_sd.c` | SDIO low-level driver |
