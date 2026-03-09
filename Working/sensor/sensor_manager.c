/**
 * sensor_manager.c - Plug-and-play sensor manager
 *
 * Round-robin polls registered sensors, updates g_water_data.
 * Optional sync to legacy currentData for backward compatibility.
 */
#include "sensor_manager.h"
#include "Main.h"
#include <string.h>

water_data_t g_water_data;

static sensor_registry_entry_t s_registry[SENSOR_MAX_REGISTERED];
static uint8_t s_registered_count = 0;
static uint8_t s_poll_idx = 0;

void SensorManager_Init(void)
{
    uint8_t i;

    memset(&g_water_data, 0, sizeof(g_water_data));
    g_water_data.ph = g_water_data.ec = g_water_data.cl = g_water_data.ntu =
        g_water_data.temperature = SENSOR_INVALID_VALUE;

    for (i = 0; i < SENSOR_MAX_REGISTERED; i++) {
        s_registry[i].driver  = NULL;
        s_registry[i].enabled = 0;
    }
    s_registered_count = 0;
    s_poll_idx = 0;
}

int8_t SensorManager_Register(const sensor_driver_t *drv, water_field_id_t field)
{
    if (drv == NULL || s_registered_count >= SENSOR_MAX_REGISTERED)
        return -1;

    if (drv->init != NULL)
        drv->init();

    s_registry[s_registered_count].driver   = drv;
    s_registry[s_registered_count].field_id = field;
    s_registry[s_registered_count].enabled  = 1;
    s_registered_count++;
    return 0;
}

void SensorManager_Poll(void)
{
    uint8_t i;
    float val;
    sensor_registry_entry_t *e;

    if (s_registered_count == 0) return;

    e = &s_registry[s_poll_idx];
    s_poll_idx = (s_poll_idx + 1) % s_registered_count;

    if (!e->enabled || e->driver == NULL) return;
    if (e->driver->read == NULL) return;

    val = e->driver->read();

    if (val != SENSOR_INVALID_VALUE) {
        switch (e->field_id) {
            case WATER_FIELD_PH:          g_water_data.ph = val; break;
            case WATER_FIELD_EC:          g_water_data.ec = val; break;
            case WATER_FIELD_CL:          g_water_data.cl = val; break;
            case WATER_FIELD_NTU:         g_water_data.ntu = val; break;
            case WATER_FIELD_TEMPERATURE:  g_water_data.temperature = val; break;
            default: break;
        }
    }
}

void SensorManager_SyncToCurrentData(void)
{
#ifdef SENSOR_PH_EC
    if (g_water_data.ph != SENSOR_INVALID_VALUE)
        currentData.S1PPM = (uint32_t)(g_water_data.ph * 100.0f);
    if (g_water_data.ec != SENSOR_INVALID_VALUE)
        currentData.S2PPM = (uint32_t)(g_water_data.ec * 10.0f);
#else
    if (g_water_data.cl != SENSOR_INVALID_VALUE)
        currentData.S1PPM = (uint32_t)(g_water_data.cl * 100.0f);
    if (g_water_data.ntu != SENSOR_INVALID_VALUE)
        currentData.S2PPM = (uint32_t)(g_water_data.ntu * 1000.0f);
#endif
    if (g_water_data.temperature != SENSOR_INVALID_VALUE)
        currentData.temperature = (int16_t)(g_water_data.temperature * 10.0f);
}
