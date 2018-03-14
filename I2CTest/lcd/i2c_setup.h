/*
 * i2c_setup.h
 *
 *  Created on: Feb 16, 2018
 *      Author: Eric
 */

#ifndef I2C_SETUP_H_
#define I2C_SETUP_H_


//Prototypes
void init_i2c0(void);
void i2c_send(uint8_t slave_addr, uint8_t args, ...); // '...' is an actual parameter
uint32_t i2c_read(uint8_t slave_addr, uint8_t read_addr);

#endif /* I2C_SETUP_H_ */
