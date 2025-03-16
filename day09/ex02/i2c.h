#ifndef I2C_H
#define I2C_H

#include <avr/io.h>
#include <util/delay.h>

#define SLA 0x38
#define PWBR 0x48

void i2c_init(void);
void i2c_init_read(void);
void i2c_init_write(void);
void i2c_start(void);
void i2c_stop(void);
void i2c_read(void);
void i2c_read_ack(void);
void i2c_read_nack(void);
void i2c_write(uint8_t data);

#endif
