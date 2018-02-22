/*
 * i2c_setup.c
 *
 *  Created on: Feb 16, 2018
 *      Author: Eric Fryters
 */

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_i2c.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/i2c.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"

#include "i2c_setup.h"


void init_i2c0(void)
{
    //Enable I2C0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);

    //Reset module
    SysCtlPeripheralReset(SYSCTL_PERIPH_I2C0);

    //enable the GPIO pins that will use I2C0--which is PortB
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

    //Configure pins for I2C0 on PB2 and PB3
    GPIOPinConfigure(GPIO_PB2_I2C0SCL);
    GPIOPinConfigure(GPIO_PB3_I2C0SDA);

    //Select i2c function on these pins
    GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2);
    GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_3);


    //Enable and start i2c0 module
    //use system clock for the module
    I2CMasterInitExpClk(I2C0_BASE, SysCtlClockGet(), false);

    //clear i2c fifo
    HWREG(I2C0_BASE + I2C_O_FIFOCTL) = 80008000;
}

void i2c_send_2nibble(uint8_t slave_addr, uint8_t args, ...)
{
    //sets the address for the i2c module to send to bus
    I2CMasterSlaveAddrSet(I2C0_BASE, slave_addr, false);

    //stores list of variable num of args
    va_list vargs;

    //set up va_list to open the last argument, then vargs will know where to look
    va_start(vargs, args);

    //put data to be sent into FIFO
    I2CMasterDataPut(I2C0_BASE, va_arg(vargs, uint32_t));



}

uint32_t i2c_read(uint8_t slave_addr, uint8_t read_addr)
{
    //sets the address for the i2c module to send to bus
    I2CMasterSlaveAddrSet(I2C0_BASE, slave_addr, false);

    //select and specify the address to read
    I2CMasterDataPut(I2C0_BASE, read_addr);

    //Send the read control byte and address to device
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);

    //Wait for MCU to finish sending
    while(I2CMasterBusy(I2C0_BASE));

    //Going to read from device
    I2CMasterSlaveAddrSet(I2C0_BASE, slave_addr, true);

    //
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);

    //Wait for MCU to finish sending
    while(I2CMasterBusy(I2C0_BASE));

    //return the data received
    return I2CMasterDataGet(I2C0_BASE);
}

void i2c_send(uint8_t slave_addr, uint8_t args, ...)
{
    //sets the address for the i2c module to send to bus
    I2CMasterSlaveAddrSet(I2C0_BASE, slave_addr, false);

    //stores list of variable num of args
    va_list vargs;

    //set up va_list to open the last argument, then vargs will know where to look
    va_start(vargs, args);

    //put data to be sent into FIFO
    I2CMasterDataPut(I2C0_BASE, va_arg(vargs, uint32_t));

    if (args == 1)
    {
        // init send SINGLE data from mcu
        I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_SEND);

        //wait until mcu is done
        while (I2CMasterBusy(I2C0_BASE));

        //close the arg list
        va_end(vargs);
    }
    else
    {
        //init send of multiple data
        I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);

        //wait until mcu is done
        while (I2CMasterBusy(I2C0_BASE));
        uint8_t i;
        for (i = 1; i < (args - 1); i++)
        {
            //put next data into i2c fifo
            I2CMasterDataPut(I2C0_BASE, va_arg(vargs, uint32_t));
            //and send that fifo data
            I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_CONT);
            //wait until mcu is done
            while (I2CMasterBusy(I2C0_BASE));
        }

        //put last data into fifo
        I2CMasterDataPut(I2C0_BASE, va_arg(vargs, uint32_t));
        //send that last bit of data, which is a separate command from before
        I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
        //wait until mcu is done
        while (I2CMasterBusy(I2C0_BASE));

        //close the arg list
        va_end(vargs);
    }

}
