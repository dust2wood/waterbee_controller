/**
 * sensor_autodetect.c - 실시간 Plug & Play (백그라운드 스캔 + ID 자동 재할당)
 * EC ID1->4 (0x0020), pH ID1->2 (0x2002)
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
#define RS485_SCAN_ADDR_MAX   10
#define RS485_PROBE_TIMEOUT_MS   200
#define ADC_VALID_MIN         100
#define ADC_VALID_MAX         4000
#define PNP_EC106_ADDR_REG    0x0020   /* EC106 장치주소 */
#define PNP_PH105_ADDR_REG    0x2002   /* PH105 장치주소 */

/* Modbus FC06 Write Single Register. Returns 1 if OK. */
static uint8_t modbus_write_single_reg(uint8_t slave, uint16_t reg_addr, uint16_t value)
{
    uint16_t crc;
    uint16_t i;
    uint16_t timeout;

    tx3Buffer[0] = slave;
    tx3Buffer[1] = 0x06;
    tx3Buffer[2] = (reg_addr >> 8) & 0xFF;
    tx3Buffer[3] = reg_addr & 0xFF;
    tx3Buffer[4] = (value >> 8) & 0xFF;
    tx3Buffer[5] = value & 0xFF;
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
    while (rx3Size < 8 && timeout < RS485_PROBE_TIMEOUT_MS) {
        Delay_1msec(1);
        timeout++;
    }

    if (rx3Size != 8) return 0;
    if (rx3Buffer[0] != slave || rx3Buffer[1] != 0x06) return 0;
    crc = CRC16Modbus(rx3Buffer, 6);
    if (crc != ((rx3Buffer[7] << 8) | rx3Buffer[6])) return 0;
    return 1;
}

/* 실시간 PnP: ID 1인 센서를 2(pH) 또는 4(EC)로 즉시 변경. is_ph=1: pH(0x2002), 0: EC(0x0020) */
uint8_t pnp_write_slave_id(uint8_t slave, uint8_t new_id, uint8_t is_ph)
{
    uint16_t reg = is_ph ? PNP_PH105_ADDR_REG : PNP_EC106_ADDR_REG;
    return modbus_write_single_reg(slave, reg, new_id);
}

/* 4레지스터 읽기. 응답 13바이트. return 1=pH, 2=EC, 0=실패. 바이트 기반 판별 */
static uint8_t modbus_read_and_detect_type(uint8_t addr)
{
    uint16_t crc;
    uint16_t i;
    uint16_t timeout;

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
    while (rx3Size < 13 && timeout < RS485_PROBE_TIMEOUT_MS) {
        Delay_1msec(1);
        timeout++;
    }

    if (rx3Size != 13 || rx3Buffer[0] != addr || rx3Buffer[1] != 0x03 || rx3Buffer[2] != 8)
        return 0;
    crc = CRC16Modbus(rx3Buffer, 11);
    if (crc != ((rx3Buffer[12] << 8) | rx3Buffer[11])) return 0;

    uint16_t r0 = (rx3Buffer[3] << 8) | rx3Buffer[4];
    uint16_t r1 = (rx3Buffer[5] << 8) | rx3Buffer[6];
    if (r0 <= 14 && r1 < 100) return 1;   /* PH105 */
    return 2;   /* EC106 */
}

/* Blocking send Modbus 0x03 to addr (1 reg from 0), wait for 7-byte response. Returns 1 if OK. */
static uint8_t probe_modbus_addr(uint8_t addr)
{
    uint16_t crc;
    uint16_t i;
    uint16_t timeout;

    tx3Buffer[0] = addr;
    tx3Buffer[1] = 0x03;
    tx3Buffer[2] = 0;
    tx3Buffer[3] = 0;
    tx3Buffer[4] = 0;
    tx3Buffer[5] = 1;
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
    while (rx3Size < 7 && timeout < RS485_PROBE_TIMEOUT_MS) {
        Delay_1msec(1);
        timeout++;
    }

    if (rx3Size != 7) return 0;
    if (rx3Buffer[0] != addr || rx3Buffer[1] != 0x03) return 0;
    crc = CRC16Modbus(rx3Buffer, 5);
    if (crc != ((rx3Buffer[6] << 8) | rx3Buffer[5])) return 0;
    return 1;
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
        Delay_1msec(10);
    }
    v = (uint16_t)(sum / 5);
    return (v >= ADC_VALID_MIN && v <= ADC_VALID_MAX) ? 1 : 0;
}

/* 부팅 시 최소 초기화만. 블로킹 스캔 없음. 실시간 PnP가 메인 루프에서 처리 */
void sensor_autodetect_run(void)
{
    sensor_autodetect_ph  = 1;  /* pH/EC 항상 등록, 백그라운드 스캔으로 발견 */
    sensor_autodetect_ec  = 1;
    sensor_autodetect_adc = probe_adc();
}
