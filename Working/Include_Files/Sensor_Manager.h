/**
 * Sensor_Manager.h - Generic sensor manager for plug-and-play sensors
 *
 * Target: STM32F103 (64KB RAM) - minimal footprint
 */
#ifndef SENSOR_MANAGER_MODULE_H
#define SENSOR_MANAGER_MODULE_H

#include <stdint.h>

/*---------------------------------------------------------------------------
 * Generic sensor driver interface
 *---------------------------------------------------------------------------*/
typedef struct {
    void    (*init)(void);
    float   (*read)(void);
    uint8_t (*status)(void);
} sensor_driver_t;

/*---------------------------------------------------------------------------
 * Central water quality data
 *---------------------------------------------------------------------------*/
typedef struct {
    float ph;
    float ec;
    float cl;           /* residual chlorine, mg/L */
    float ntu;          /* turbidity */
    float temperature;
} water_data_t;

#define SENSOR_INVALID_FLOAT   (-999.0f)

/*---------------------------------------------------------------------------
 * Public API
 *---------------------------------------------------------------------------*/
extern water_data_t water_data;

void sensor_manager_init(void);
void sensor_manager_update(void);
int  sensor_manager_count(void);

typedef enum {
    WATER_FIELD_PH = 0,
    WATER_FIELD_EC,
    WATER_FIELD_CL,
    WATER_FIELD_NTU,
    WATER_FIELD_TEMPERATURE
} water_field_t;

int8_t sensor_manager_register(const sensor_driver_t *drv, water_field_t field);

#endif /* SENSOR_MANAGER_MODULE_H */
