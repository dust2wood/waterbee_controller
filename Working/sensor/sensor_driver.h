/**
 * sensor_driver.h - Generic sensor driver interface for plug-and-play sensors
 *
 * Supports: ADC, RS232, RS485 Modbus
 * Target: STM32F103VE (64KB RAM, 512KB Flash)
 */
#ifndef SENSOR_DRIVER_H
#define SENSOR_DRIVER_H

#include <stdint.h>

/*---------------------------------------------------------------------------
 * Sensor types (for routing / debug)
 *---------------------------------------------------------------------------*/
typedef enum {
    SENSOR_TYPE_ADC     = 0,
    SENSOR_TYPE_RS232   = 1,
    SENSOR_TYPE_RS485   = 2,
    SENSOR_TYPE_COUNT
} sensor_type_t;

/*---------------------------------------------------------------------------
 * Sensor status codes
 *---------------------------------------------------------------------------*/
#define SENSOR_STATUS_OK           0
#define SENSOR_STATUS_PENDING      0xFF
#define SENSOR_STATUS_TIMEOUT      3
#define SENSOR_STATUS_CRC_ERR      4
#define SENSOR_STATUS_BUS_BUSY     5
#define SENSOR_STATUS_ERROR        1

/*---------------------------------------------------------------------------
 * Invalid value sentinel (no valid reading yet)
 *---------------------------------------------------------------------------*/
#define SENSOR_INVALID_VALUE       (-999.0f)

/*---------------------------------------------------------------------------
 * Generic sensor driver interface
 *
 * - init: one-time setup (ADC config, UART, etc.). Called by sensor_manager.
 * - read: non-blocking poll, returns latest value or SENSOR_INVALID_VALUE
 * - status: SENSOR_STATUS_* (OK, TIMEOUT, etc.)
 *
 * Drivers implement these; sensor_manager calls them in round-robin.
 *---------------------------------------------------------------------------*/
typedef struct {
    void    (*init)(void);
    float   (*read)(void);
    uint8_t (*status)(void);
} sensor_driver_t;

#endif /* SENSOR_DRIVER_H */
