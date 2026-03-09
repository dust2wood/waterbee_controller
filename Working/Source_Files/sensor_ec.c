/**
 * sensor_ec.c - EC sensor driver (RS485 Modbus)
 *
 * Reads from currentData.S2PPM (filled by Modbus485Handler).
 * S2PPM is stored as: mS/cm * 1000 for low range, * 10 for high range.
 */
#include "sensor_manager.h"
#include "Main.h"

#define SENSOR_OK    0
#define SENSOR_ERROR 3

static void sensor_ec_init(void)
{
    /* No hardware init - Modbus485Handler manages RS485 polling */
}

static float sensor_ec_read(void)
{
    /* RS485 exclusive: water_data.ec set by Modbus485Handler */
    if (water_data.ec < 0.0f) return SENSOR_INVALID_FLOAT;
    return water_data.ec;
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
