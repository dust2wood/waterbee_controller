/**
 * sensor_manager.c - Sensor manager implementation
 *
 * Keeps max 4 sensors in registry (~24 bytes RAM each).
 * Uses sensor_autodetect_run() at boot to register only detected sensors.
 */
#include "sensor_manager.h"
#include "sensor_autodetect.h"
#ifdef SENSOR_PH_EC
#include "sensor_ph.h"
#include "sensor_ec.h"
#endif
#include "sensor_adc.h"

#define SENSOR_MAX   4

typedef struct {
    const sensor_driver_t *drv;
    water_field_t field;
    uint8_t enabled;
} sensor_entry_t;

water_data_t water_data;

/* External sensor display slots used by ReDisplay_ch2 */
uint8_t           g_ext_sensor_count = 0;
smgr_ext_sensor_t g_ext_sensors[4];

static sensor_entry_t s_sensors[SENSOR_MAX];
static uint8_t s_count;
static uint8_t s_poll_idx;

void sensor_manager_init(void)
{
    uint8_t i;

    water_data.ph = water_data.ec = water_data.cl =
        water_data.ntu = water_data.temperature = SENSOR_INVALID_FLOAT;

    for (i = 0; i < SENSOR_MAX; i++) {
        s_sensors[i].drv = NULL;
        s_sensors[i].enabled = 0;
    }
    s_count = 0;
    s_poll_idx = 0;

#ifdef SENSOR_PH_EC
    if (sensor_autodetect_ph)
        sensor_manager_register(&sensor_ph_driver, WATER_FIELD_PH);
    if (sensor_autodetect_ec)
        sensor_manager_register(&sensor_ec_driver, WATER_FIELD_EC);
#endif
    if (sensor_autodetect_adc)
        sensor_manager_register(&sensor_adc_driver, WATER_FIELD_NTU);
}

int8_t sensor_manager_register(const sensor_driver_t *drv, water_field_t field)
{
    if (drv == NULL || s_count >= SENSOR_MAX) return -1;

    if (drv->init != NULL)
        drv->init();

    s_sensors[s_count].drv = drv;
    s_sensors[s_count].field = field;
    s_sensors[s_count].enabled = 1;
    s_count++;
    return 0;
}

int sensor_manager_count(void)
{
    return (int)s_count;
}

/* Return the water_field_t for display slot (0=primary, 1=secondary).
 * In SENSOR_PH_EC+CH2 mode slot0=pH, slot1=EC by registration order. */
water_field_t sensor_manager_get_display_field(uint8_t slot)
{
    if (slot < s_count && s_sensors[slot].enabled)
        return s_sensors[slot].field;
    /* defaults */
    return (slot == 0) ? WATER_FIELD_PH : WATER_FIELD_EC;
}

void sensor_manager_update(void)
{
    uint8_t i;
    float v;
    sensor_entry_t *e;

    if (s_count == 0) return;

    e = &s_sensors[s_poll_idx];
    s_poll_idx = (s_poll_idx + 1) % s_count;

    if (!e->enabled || e->drv == NULL || e->drv->read == NULL) return;

    v = e->drv->read();
    if (v == SENSOR_INVALID_FLOAT) return;

    switch (e->field) {
        case WATER_FIELD_PH:          water_data.ph = v; break;
        case WATER_FIELD_EC:          water_data.ec = v; break;
        case WATER_FIELD_CL:          water_data.cl = v; break;
        case WATER_FIELD_NTU:         water_data.ntu = v; break;
        case WATER_FIELD_TEMPERATURE: water_data.temperature = v; break;
    }
}
