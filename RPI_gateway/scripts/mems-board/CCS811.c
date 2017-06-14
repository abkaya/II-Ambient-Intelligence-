/*
 * CCS811.c
 *
 *  Created on: 17 Mar 2017
 *      Author: Fifth
 */

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "CCS811.h"
#include <bcm2835.h>

#define CCS811_ADDR 0x5B
#define CSS811_STATUS 0x00
#define CSS811_MEAS_MODE 0x01
#define CSS811_ALG_RESULT_DATA 0x02
#define CSS811_RAW_DATA 0x03
#define CSS811_ENV_DATA 0x05
#define CSS811_NTC 0x06
#define CSS811_THRESHOLDS 0x10
#define CSS811_BASELINE 0x11
#define CSS811_HW_ID 0x20
#define CSS811_HW_VERSION 0x21
#define CSS811_FW_BOOT_VERSION 0x23
#define CSS811_FW_APP_VERSION 0x24
#define CSS811_ERROR_ID 0xE0
#define CSS811_APP_START 0xF4
#define CSS811_SW_RESET 0xFF
#define WRITE_REG_INVALID 0
#define READ_REG_INVALID 1
#define MEASMODE_INVALID 2
#define MAX_RESISTANCE 3
#define HEATER_FAULT 4
#define HEATER_SUPPLY 5
#define NO_ERROR 5

uint8_t getError_CCS811()
{
	char error[1];
	READ_REGISTER_CCS811(error,CSS811_ERROR_ID,1);
	printf("Error: ");
	printf("%x \n", error[0]);
	if (error[0] & 1 << 5)
		return HEATER_SUPPLY;
	else if (error[0] & 1 << 4)
		return HEATER_FAULT;
	else if (error[0] & 1 << 3)
		return MAX_RESISTANCE;
	else if (error[0] & 1 << 2)
		return MEASMODE_INVALID;
	else if (error[0] & 1 << 1)
		return READ_REG_INVALID;
	else if (error[0] & 1 << 0)
		return WRITE_REG_INVALID;
	else
		return NO_ERROR;
}

void WRITE_REGISTER_CCS811(char pData[],uint8_t length)
{
	bcm2835_i2c_setSlaveAddress(CCS811_ADDR);
	bcm2835_i2c_write(pData, length);
	return;
}
void READ_REGISTER_CCS811(char buf[],char reg,uint8_t length)
{
	char regaddr = reg;
	bcm2835_i2c_setSlaveAddress(CCS811_ADDR);
	bcm2835_i2c_write(&regaddr, length);
	bcm2835_i2c_read(buf, length);
	return;
}

void getData_CCS811(uint32_t *data)
{
	char buf[4];
	READ_REGISTER_CCS811(buf,CSS811_ALG_RESULT_DATA,4);
	uint32_t requiredData= buf[0]<<24 | buf[1]<<16|buf[2]<<8|buf[3];
	*data=requiredData;
	return;
}

void parseResult_CCS811(uint32_t *data,uint16_t *CO2,uint16_t *tVOC)
{
	  *CO2 = *data>>16;
	  *tVOC = *data;
}

void setMode_CCS811(uint8_t mode)
{
  if (mode > 4) mode = 4; //Error correction
  char setting[1];
  READ_REGISTER_CCS811(setting,CSS811_MEAS_MODE,1);
  setting[0] &= ~(0b00000111 << 4); //Clear DRIVE_MODE bits
  setting[0] |= (mode << 4); //Mask in mode
  char writeData[2]={CSS811_MEAS_MODE, setting[0]};
  WRITE_REGISTER_CCS811(writeData,2);
}

uint8_t configure_CCS811(uint8_t mode)
{
	uint8_t state=0;
	char hwID[1];
	READ_REGISTER_CCS811(hwID,CSS811_HW_ID,1);

	if (hwID[0] == 0x81)
	{
		char ready[1];
		READ_REGISTER_CCS811(ready,CSS811_STATUS,1);
		if(ready[0] & 1 << 4)
		{
			char data[1];
			data[0]=CSS811_APP_START;
			WRITE_REGISTER_CCS811(data,1);
			setMode_CCS811(mode);
			state=1;
		}
		else
		{
			state=0;
			getError_CCS811();
		}
	}
	else
	{
		state=0;
		getError_CCS811();
	}
	return state;
}


uint8_t dataAvailable_CCS811()
{
	char value[1];
	READ_REGISTER_CCS811(value,CSS811_STATUS,1);
	printf("value: ");
	printf("%x\n", value[0]);
	getError_CCS811();
	if(value[0] & 1 << 3)
		return (1);
	else
		return 0;
}
uint8_t checkForError_CCS811()
{
	char value[1];
	READ_REGISTER_CCS811(value,CSS811_STATUS,1);
	return (value[0] & 1 << 0);
}
uint16_t getBaseline_CCS811()
{
	char baselineElements[2];
	uint16_t  baseline;
	READ_REGISTER_CCS811(baselineElements,CSS811_BASELINE,2);
	baseline= baselineElements[0]<<8|baselineElements[1];
	return baseline;
}

uint8_t appValid_CCS811()
{
	char value[1];
	READ_REGISTER_CCS811(value,CSS811_STATUS,1);
	return (value[0] & 1 << 4);
}

void enableInterrupts_CCS811()
{
	char setting[1];
	READ_REGISTER_CCS811(setting,CSS811_MEAS_MODE,1);
	setting[0] |= (1 << 3); //Set INTERRUPT bit
	char writeData[2]={CSS811_MEAS_MODE, setting[0]};
	WRITE_REGISTER_CCS811(writeData,2);
}

void disableInterrupts_CCS811()
{
	char setting[1];
	READ_REGISTER_CCS811(setting,CSS811_MEAS_MODE,1);
	setting[0] &= ~(1 << 3); //Clear INTERRUPT bit
	char writeData[2]={CSS811_MEAS_MODE, setting[0]};
	WRITE_REGISTER_CCS811(writeData,2);
}