#include <stdbool.h>
//#include <plib.h>
#include <peripheral/i2c.h>

#include "../../../../common/commons.h"
#include "../../../../common/error/error.h"
#include "../i2cSlaveSetup.h"

#define I2C_FREQUENCY	   100000L

#define BRG_VAL 	((FOSC / 2 / I2C_FREQUENCY)-2)

bool initialized = false;

void i2cSlaveInitialize(unsigned char writeAddress) {
    // Avoid more than one initialization
    if (initialized) {
        writeError(I2C_SLAVE_ALREADY_INITIALIZED);
        return;
    }
	initialized = true;
    // Enable the I2C module with clock stretching enabled
	OpenI2C1(I2C_ON | I2C_7BIT_ADD | I2C_STR_EN, BRG_VAL);

    // 7-bit I2C slave address must be initialised here.
    // we shift because i2c address is shift to the right
    // to manage read and write address
    I2C1ADD = writeAddress >> 1;
    I2C1MSK = 0;

    // Interruption on I2C Slave
    // -> Priority of I2C Slave interruption
    mI2C1SetIntPriority(I2C_INT_PRI_3 | I2C_INT_SLAVE);
    // -> Enable Interruption Flag => See the same code in interruption
    mI2C1SClearIntFlag();

    // Enable I2C
    EnableIntSI2C1;
}