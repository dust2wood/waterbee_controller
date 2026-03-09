/**
 * sensor_manager.c - Display candidate aggregator (unified firmware)
 *
 * Collects active sensors for UI display (max 2 channels).
 * - PH, EC: RS485 고정 매핑 (addr 2, 4)
 * - CL, NTU: RS232 push ($RES1, $RES2)
 */
#include "sensor_manager.h"
#include "Main.h"
#include "sensor_ph.h"
#include "sensor_ec.h"

#define SENSOR_MAX   4

typedef struct {
    const sensor_driver_t *drv;
    water_field_t field;
    uint8_t enabled;
} sensor_entry_t;

water_data_t water_data;

static sensor_entry_t s_sensors[SENSOR_MAX];
static uint8_t s_count;
static uint8_t s_poll_idx;

/* RS232 fixed: CL, NTU - read from currentData (filled by $RES1/$RES2) */
static void sensor_rs232_cl_init(void) { }
static float sensor_rs232_cl_read(void) {
    if (currentData.S1PPM >= 9999) return SENSOR_INVALID_FLOAT;
    return (float)currentData.S1PPM / 100.0f;
}
static uint8_t sensor_rs232_cl_status(void) { return 0; }
static void sensor_rs232_ntu_init(void) { }
static float sensor_rs232_ntu_read(void) {
    if (currentData.S2PPM >= 99999) return SENSOR_INVALID_FLOAT;
    return (float)currentData.S2PPM / 1000.0f;
}
static uint8_t sensor_rs232_ntu_status(void) { return 0; }
static const sensor_driver_t sensor_rs232_cl_driver = {
    .init = sensor_rs232_cl_init, .read = sensor_rs232_cl_read, .status = sensor_rs232_cl_status,
};
static const sensor_driver_t sensor_rs232_ntu_driver = {
    .init = sensor_rs232_ntu_init, .read = sensor_rs232_ntu_read, .status = sensor_rs232_ntu_status,
};

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

    /* 1. RS485 고정: pH(addr2), EC(addr4) - autodetect 없이 항상 등록 */
    sensor_manager_register(&sensor_ph_driver, WATER_FIELD_PH);
    sensor_manager_register(&sensor_ec_driver, WATER_FIELD_EC);

    /* 2. RS232: CL($RES1), NTU($RES2) */
    sensor_manager_register(&sensor_rs232_cl_driver, WATER_FIELD_CL);
    sensor_manager_register(&sensor_rs232_ntu_driver, WATER_FIELD_NTU);
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

water_field_t sensor_manager_get_display_field(int slot_index)
{
    if (slot_index < 0 || slot_index >= s_count) return WATER_FIELD_TEMPERATURE;
    return s_sensors[slot_index].field;
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
