/**
 * Sensor_Manager.h - Display candidate aggregator
 *
 * Collects active sensors for UI display (max 2 channels).
 * - PH, EC: RS485 autodetect
 * - CL, NTU: RS232 fixed (no autodetect)
 */
#ifndef SENSOR_MANAGER_MODULE_H
#define SENSOR_MANAGER_MODULE_H

#include <stdint.h>
#include <stddef.h>

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

typedef enum {
    WATER_FIELD_PH = 0,
    WATER_FIELD_EC,
    WATER_FIELD_CL,
    WATER_FIELD_NTU,
    WATER_FIELD_TEMPERATURE
} water_field_t;

/*---------------------------------------------------------------------------
 * Public API
 *---------------------------------------------------------------------------*/
extern water_data_t water_data;

void sensor_manager_init(void);
void sensor_manager_update(void);
int  sensor_manager_count(void);

/** Get water_field_t for display slot index (0 or 1). Returns WATER_FIELD_TEMPERATURE if invalid. */
water_field_t sensor_manager_get_display_field(int slot_index);

int8_t sensor_manager_register(const sensor_driver_t *drv, water_field_t field);

/*---------------------------------------------------------------------------
 * External sensor display (ReDisplay_ch2 raw display slots)
 *---------------------------------------------------------------------------*/
#define SMGR_STATUS_OK   0
#define SMGR_STATUS_ERR  1

typedef struct {
    uint8_t  status;
    uint16_t raw[2];
} smgr_ext_sensor_t;

extern uint8_t           g_ext_sensor_count;
extern smgr_ext_sensor_t g_ext_sensors[4];

#endif /* SENSOR_MANAGER_MODULE_H */
