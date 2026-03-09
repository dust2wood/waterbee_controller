/**
 * sensor_manager.h - Plug-and-play sensor manager
 *
 * Registers sensors, polls them, updates water_data_t.
 */
#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include "sensor_driver.h"

/*---------------------------------------------------------------------------
 * Max registered sensors (RAM footprint: ~16 bytes each for registry)
 *---------------------------------------------------------------------------*/
#define SENSOR_MAX_REGISTERED      8

/*---------------------------------------------------------------------------
 * Shared water quality data (single source of truth)
 *
 * Values are float. Use SENSOR_INVALID_VALUE when no valid reading.
 *---------------------------------------------------------------------------*/
typedef struct {
    float ph;
    float ec;
    float cl;           /* residual chlorine, mg/L */
    float ntu;          /* turbidity */
    float temperature;
} water_data_t;

/*---------------------------------------------------------------------------
 * Data field ID (for mapping driver -> water_data_t field)
 *---------------------------------------------------------------------------*/
typedef enum {
    WATER_FIELD_PH          = 0,
    WATER_FIELD_EC          = 1,
    WATER_FIELD_CL          = 2,
    WATER_FIELD_NTU         = 3,
    WATER_FIELD_TEMPERATURE = 4,
    WATER_FIELD_COUNT
} water_field_id_t;

/*---------------------------------------------------------------------------
 * Registration entry: driver + which water_data_t field it feeds
 *---------------------------------------------------------------------------*/
typedef struct {
    const sensor_driver_t *driver;
    water_field_id_t       field_id;
    uint8_t                enabled;
} sensor_registry_entry_t;

/*---------------------------------------------------------------------------
 * Public API
 *---------------------------------------------------------------------------*/

/** Global water data (read by UI, alarm, logging) */
extern water_data_t g_water_data;

/**
 * Initialize sensor manager. Call once after system init.
 * Clears g_water_data and resets registry.
 */
void SensorManager_Init(void);

/**
 * Register a sensor. Call before main loop.
 * @return 0=OK, -1=registry full
 */
int8_t SensorManager_Register(const sensor_driver_t *drv, water_field_id_t field);

/**
 * Poll all registered sensors (non-blocking).
 * Call from main loop every ~10–50 ms.
 */
void SensorManager_Poll(void);

/**
 * Sync g_water_data into legacy currentData (DataSet).
 * Call after SensorManager_Poll if using existing UI.
 * Converts float -> fixed-point (S1PPM, S2PPM, etc.)
 */
void SensorManager_SyncToCurrentData(void);

#endif /* SENSOR_MANAGER_H */
