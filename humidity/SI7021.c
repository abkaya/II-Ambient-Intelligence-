#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <bcm2835.h>

#define SI7021_ADDR                      0x40
#define SI7021_MEASRH_HOLD_CMD           0xE5
#define SI7021_MEASRH_NOHOLD_CMD         0xF5
#define SI7021_MEASTEMP_HOLD_CMD         0xE3
#define SI7021_MEASTEMP_NOHOLD_CMD       0xF3
#define SI7021_READPREVTEMP_CMD          0xE0
#define SI7021_RESET_CMD                 0xFE
#define SI7021_WRITERHT_REG_CMD          0xE6
#define SI7021_READRHT_REG_CMD           0xE7
#define SI7021_WRITEHEATER_REG_CMD       0x51
#define SI7021_READHEATER_REG_CMD        0x11
#define SI7021_ID1_CMD                   0xFA0F
#define SI7021_ID2_CMD                   0xFCC9
#define SI7021_FIRMVERS_CMD              0x84B8

float readHumidity(void) 
{
    char buf[1];
    buf[0] = SI7021_MEASRH_NOHOLD_CMD;
    bcm2835_i2c_setSlaveAddress(SI7021_ADDR);   
    bcm2835_i2c_write(buf, 1);
    
    bcm2835_delay(30);

    uint16_t rxbuf[1];	
    char regadr = SI7021_ADDR;
	bcm2835_i2c_setSlaveAddress(SI7021_ADDR);
	bcm2835_i2c_read_register_rs(&regadr, rxbuf, 1);

   float humidity = rxbuf[0];;
  humidity *= 125;
  humidity /= 65536;
  humidity -= 6;

  return humidity;
}
