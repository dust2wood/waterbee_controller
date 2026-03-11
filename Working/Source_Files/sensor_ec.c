/**
 * sensor_ec.c - EC sensor driver (RS485 Modbus)
 *
 * Reads from currentData.S2PPM (filled by Modbus485Handler).
 * S2PPM is stored as: mS/cm * 1000 for low range, * 10 for high range.
 */
#include "sensor_manager.h"
#include "Main.h"


static void sensor_ec_init(void)
{
    /* No hardware init - Modbus485Handler manages RS485 polling */
}

static float sensor_ec_read(void)
{
    /* S2PPM: EC in 0.001 mS/cm (e.g. 1.500 -> 1500) or 0.1 (e.g. 20.0 -> 2000) */
    if (currentData.S2PPM > 200000) return SENSOR_INVALID_FLOAT;
    if (currentData.S2PPM >= 20000)
        return (float)currentData.S2PPM / 10.0f;
    return (float)currentData.S2PPM / 1000.0f;
}

static uint8_t sensor_ec_status(void)
{
    return (Sensor_State4 == SENSOR_OK) ? 0 : 1;
}

const sensor_driver_t sensor_ec_driver = {
    .init   = sensor_ec_init,
    .read   = sensor_ec_read,
    .status = sensor_ec_status,
};
