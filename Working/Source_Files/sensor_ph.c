/**
 * sensor_ph.c - pH sensor driver (RS485 Modbus)
 *
 * Reads from currentData.S1PPM (filled by Modbus485Handler).
 * S1PPM is stored as fixed-point: 7.25 pH = 725.
 */
#include "sensor_manager.h"
#include "Main.h"


static void sensor_ph_init(void)
{
    /* No hardware init - Modbus485Handler manages RS485 polling */
}

static float sensor_ph_read(void)
{
    /* S1PPM: pH * 100 (e.g. 7.25 -> 725) */
    if (currentData.S1PPM > 1400) return SENSOR_INVALID_FLOAT;
    return (float)currentData.S1PPM / 100.0f;
}

static uint8_t sensor_ph_status(void)
{
    return (Sensor_State3 == SENSOR_OK) ? 0 : 1;
}

const sensor_driver_t sensor_ph_driver = {
    .init   = sensor_ph_init,
    .read   = sensor_ph_read,
    .status = sensor_ph_status,
};
