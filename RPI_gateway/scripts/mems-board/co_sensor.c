#include <bcm2835.h>

#define STATUS_REG 0x00
#define MEAS_MODE_REG 0x01
#define ALG_RESULT_DATA 0x02
#define ENV_DATA 0x05
#define NTC_REG 0x06
#define THRESHOLDS 0x10
#define BASELINE 0x11
#define HW_ID_REG 0x20
#define ERROR_ID_REG 0xE0
#define APP_START_REG 0xF4
#define SW_RESET 0xFF
#define CCS_811_ADDRESS 0x5A
#define GPIO_WAKE 0x5
#define DRIVE_MODE_IDLE 0x0
#define DRIVE_MODE_1SEC 0x10
#define DRIVE_MODE_10SEC 0x20
#define DRIVE_MODE_60SEC 0x30
#define INTERRUPT_DRIVEN 0x8
#define THRESHOLDS_ENABLED 0x4

int i2c_buff[8];
bool wake_gpio_enabled = true;

//adress = CCS_811_ADDRESS, register = Mailbox ID, length = # of bytes
void i2c_write(int address, int reg, int *tx_data_ptr, int length)
{
	char buf[length] = { reg, value };
	bcm2835_i2c_setSlaveAddress(address);
	bcm2835_i2c_write(buf, 2);
	
}

//adress = CCS_811_ADDRESS, register = Mailbox ID, length = # of bytes
uint8_t i2c_read(int address, int reg, int length)
{
	char buf[length];
	char regaddr = reg;
	bcm2835_i2c_setSlaveAddress(address);
	bcm2835_i2c_read_register_rs(&regaddr, buf, length);
	return buf[0];
}

//gpio_id = GPIO pin number to be written high/low, level = 1/0
void gpio_write(int gpio_id, int level)
{
	
}

int main(void)
{
	
	
	return 0;
}