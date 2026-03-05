/**
 * sensor_autodetect.h - Boot-time sensor auto-detection
 *
 * RS485 Modbus: scans addresses, detects pH (addr 2), EC (addr 4).
 * ADC: probes if 4-20mA input has valid signal.
 */
#ifndef SENSOR_AUTODETECT_H
#define SENSOR_AUTODETECT_H

#include <stdint.h>

/* Detection results (1=found) */
extern uint8_t sensor_autodetect_ph;   /* addr 2 */
extern uint8_t sensor_autodetect_ec;   /* addr 4 */
extern uint8_t sensor_autodetect_adc;  /* ADC channel valid */

/**
 * Run full scan at boot. Call before sensor_manager_init().
 * Uses ~2-3 seconds for RS485 scan.
 */
void sensor_autodetect_run(void);

#endif /* SENSOR_AUTODETECT_H */
