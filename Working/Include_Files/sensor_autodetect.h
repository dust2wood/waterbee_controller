/**
 * sensor_autodetect.h - 실시간 Plug & Play
 */
#ifndef SENSOR_AUTODETECT_H
#define SENSOR_AUTODETECT_H

#include <stdint.h>

extern uint8_t sensor_autodetect_ph;
extern uint8_t sensor_autodetect_ec;
extern uint8_t sensor_autodetect_adc;

void sensor_autodetect_run(void);

/* 실시간 PnP: ID 1 센서를 new_id로 변경. is_ph=1: pH(0x2002), 0: EC(0x0020) */
uint8_t pnp_write_slave_id(uint8_t slave, uint8_t new_id, uint8_t is_ph);

#endif /* SENSOR_AUTODETECT_H */
