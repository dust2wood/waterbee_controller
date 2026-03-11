/**
 * sensor_autodetect.c - Boot-time RS485 + ADC sensor detection
 */
#include "sensor_autodetect.h"
#include "Communication.h"
#include "Interrupt.h"
#include "Main.h"
#include "Adc_Read.h"
#include "stm32f10x_usart.h"

uint8_t sensor_autodetect_ph  = 0;
uint8_t sensor_autodetect_ec  = 0;
uint8_t sensor_autodetect_adc = 0;

#define RS485_SCAN_ADDR_MIN   1
#define RS485_SCAN_ADDR_MAX   32
#define RS485_PROBE_TIMEOUT_10MS  12
#define ADC_VALID_MIN         100
#define ADC_VALID_MAX         4000

typedef enum {
    RS485_PROBE_NONE = 0,
    RS485_PROBE_PH,
    RS485_PROBE_EC
} rs485_probe_type_t;

/* Blocking send Modbus 0x03 to addr, then classify the returned sensor frame. */
static rs485_probe_type_t probe_modbus_addr(uint8_t addr)
{
    uint16_t crc;
    uint16_t i;
    uint16_t timeout;
    uint16_t r0;
    uint16_t r1;
    uint16_t stable_count = 0;
    uint16_t last_rx_size = 0;

    tx3Buffer[0] = addr;
    tx3Buffer[1] = 0x03;
    tx3Buffer[2] = 0;
    tx3Buffer[3] = 0;
    tx3Buffer[4] = 0;
    tx3Buffer[5] = 4;
    crc = CRC16Modbus(tx3Buffer, 6);
    tx3Buffer[6] = crc & 0xFF;
    tx3Buffer[7] = (crc >> 8) & 0xFF;

    rx3Size = 0;
    RS485_DRIVE_HIGH;
    Delay_10msec(2);

    for (i = 0; i < 8; i++) {
        USART_SendData(USART3, tx3Buffer[i] & 0xFF);
        while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET) { }
    }
    RS485_DRIVE_LOW;
    Delay_10msec(1);

    timeout = 0;
    while (timeout < RS485_PROBE_TIMEOUT_10MS) {
        Delay_10msec(1);
        if (rx3Size >= 9) {
            if (rx3Size == last_rx_size) {
                ++stable_count;
                if (stable_count >= 2)
                    break;
            } else {
                last_rx_size = rx3Size;
                stable_count = 0;
            }
        }
        ++timeout;
    }

    if (rx3Buffer[0] != addr || rx3Buffer[1] != 0x03) return RS485_PROBE_NONE;

    if (rx3Size == 13) {
        crc = CRC16Modbus(rx3Buffer, 11);
        if (crc != ((rx3Buffer[12] << 8) | rx3Buffer[11])) return RS485_PROBE_NONE;

        r0 = (uint16_t)((rx3Buffer[3] << 8) | rx3Buffer[4]);
        r1 = (uint16_t)((rx3Buffer[5] << 8) | rx3Buffer[6]);
        return (r0 <= 14 && r1 < 100) ? RS485_PROBE_PH : RS485_PROBE_EC;
    }

    if (rx3Size == 9) {
        crc = CRC16Modbus(rx3Buffer, 7);
        if (crc != ((rx3Buffer[8] << 8) | rx3Buffer[7])) return RS485_PROBE_NONE;

        r0 = (uint16_t)((rx3Buffer[3] << 8) | rx3Buffer[4]);
        r1 = (uint16_t)((rx3Buffer[5] << 8) | rx3Buffer[6]);
        return (r0 <= 14 && r1 < 100) ? RS485_PROBE_PH : RS485_PROBE_EC;
    }

    return RS485_PROBE_NONE;
}

/* Check if ADC has valid 4-20mA range (not open/short) */
static uint8_t probe_adc(void)
{
    uint16_t v;
    uint8_t i;
    uint32_t sum = 0;

    for (i = 0; i < 5; i++) {
        v = Read_Channel_ADC1();
        sum += v;
        Delay_10msec(1);
    }
    v = (uint16_t)(sum / 5);
    return (v >= ADC_VALID_MIN && v <= ADC_VALID_MAX) ? 1 : 0;
}

void sensor_autodetect_run(void)
{
    /* Autodetect 스캔 비활성화. pH/EC는 고정 매핑(addr 2, 4) 사용. */
    uint8_t addr;

    sensor_autodetect_ph = 0;
    sensor_autodetect_ec = 0;
    sensor_autodetect_adc = probe_adc();

    for (addr = RS485_SCAN_ADDR_MIN; addr <= RS485_SCAN_ADDR_MAX; ++addr) {
        rs485_probe_type_t probe_type = probe_modbus_addr(addr);

        if ((probe_type == RS485_PROBE_PH) && (sensor_autodetect_ph == 0)) {
            configData.modbusConfig.modbusSensor1Addr = addr;
            sensor_autodetect_ph = 1;
        } else if ((probe_type == RS485_PROBE_EC) && (sensor_autodetect_ec == 0)) {
            configData.modbusConfig.modbusSensor2Addr = addr;
            sensor_autodetect_ec = 1;
        }

        if ((sensor_autodetect_ph != 0) && (sensor_autodetect_ec != 0))
            break;
    }
}
