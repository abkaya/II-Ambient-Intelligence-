#include "MPL3115A2.h"

uint8_t WRITE_REGISTER_MPL3115A2(uint8_t pData[], uint8_t length) {
	uint8_t status = HAL_I2C_Master_Transmit(hi2cLib, MPL3115A2_ADDRESS << 1,
			pData, length, HAL_MAX_DELAY);
	return status;
}
uint8_t READ_REGISTER_MPL3115A2(uint8_t buf[], uint8_t reg, uint8_t length) {
	uint8_t status = HAL_I2C_Mem_Read(hi2cLib, MPL3115A2_ADDRESS << 1, reg,
	I2C_MEMADD_SIZE_8BIT, buf, length, HAL_MAX_DELAY);
	return status;
}

void setModeAltimeter_MPL3115A2() {
	uint8_t tempSetting[1];
	READ_REGISTER_MPL3115A2(tempSetting, CTRL_REG1, 1); //Read current settings
	tempSetting[0] |= (1 << 7); //Set ALT bit
	uint8_t pData[2] = { CTRL_REG1, tempSetting[0] };
	WRITE_REGISTER_MPL3115A2(pData, 2);
}

void setModeActive_MPL3115A2() {
	uint8_t tempSetting[1];
	READ_REGISTER_MPL3115A2(tempSetting, CTRL_REG1, 1); //Read current settings
	tempSetting[0] |= (1 << 0); //Set SBYB bit for Active mode
	uint8_t pData[2] = { CTRL_REG1, tempSetting[0] };
	WRITE_REGISTER_MPL3115A2(pData, 2);
}

void enableDataFlags_MPL3115A2() {
	uint8_t pData[2] = { PT_DATA_CFG, 0x07 };
	WRITE_REGISTER_MPL3115A2(pData, 2);
}

void init_MPL3115A2(I2C_HandleTypeDef *hi2c) {
	hi2cLib = hi2c;
	setModeAltimeter_MPL3115A2();
	setModeActive_MPL3115A2();
	enableDataFlags_MPL3115A2();
	setOversampleRate_MPL3115A2(7);
}

//Clears then sets the OST bit which causes the sensor to immediately take another reading
//Needed to sample faster than 1Hz
void toggleOneShot_MPL3115A2() {
	uint8_t tempSetting[1];
	READ_REGISTER_MPL3115A2(tempSetting, CTRL_REG1, 1); //Read current settings
	tempSetting[0] &= ~(1 << 1); //Clear OST bit
	uint8_t pData[2] = { CTRL_REG1, tempSetting[0] };
	WRITE_REGISTER_MPL3115A2(pData, 2);

	READ_REGISTER_MPL3115A2(tempSetting, CTRL_REG1, 1); //Read current settings
	tempSetting[0] |= (1 << 1); //Set OST bit
	uint8_t newPData[2] = { CTRL_REG1, tempSetting[0] };
	WRITE_REGISTER_MPL3115A2(newPData, 2);

}


/*
 * Call with a rate from 0 to 7. See page 33 for table of ratios.
 * Sets the over sample rate. Datasheet calls for 128 but you can set it
 * from 1 to 128 samples. The higher the oversample rate the greater
 * the time between data samples.
 * 1 1 1 128 512 ms
 */
void setOversampleRate_MPL3115A2(uint8_t sampleRate)
{
  if(sampleRate > 7) sampleRate = 7; //OS cannot be larger than 0b.0111
  sampleRate <<= 3; //Align it for the CTRL_REG1 register

  uint8_t tempSetting[1];
  READ_REGISTER_MPL3115A2(tempSetting, CTRL_REG1, 1); //Read current settings

  tempSetting[0] &= 0b11000111; //Clear out old OS bits
  tempSetting[0] |= sampleRate; //Mask in new OS bits
  uint8_t pData[2] = { CTRL_REG1, tempSetting[0] };
  WRITE_REGISTER_MPL3115A2(pData, 2);
}


double readAltitude_MPL3115A2() {
	//toggleOneShot_MPL3115A2();

	// Wait for PDR bit, indicates we have new pressure data
	uint8_t tempStatus[1];
	/*
	int counter = 0;
	while ((READ_REGISTER_MPL3115A2(tempStatus, STATUS, 1) & (1 << 1)) == 0) {
		if (++counter > 600)
			return (-1); //Error out after max of 512ms for a read
		HAL_Delay(1);
	}
	*/

	uint8_t alt[3];
	READ_REGISTER_MPL3115A2(alt, OUT_P_MSB, 3);

	// The least significant bytes l_altitude and l_temp are 4-bit,
	// fractional values, so you must cast the calulation in (float),
	// shift the value over 4 spots to the right and divide by 16 (since
	// there are 16 values in 4-bits).
	double tempCSB = (alt[2] >> 4) / 16.0;

	double altitude = (double) ((alt[0] << 8) | alt[1]) + tempCSB;
	return (altitude);

}


uint16_t Read_Temp_MPL3115A()
{
	uint16_t t = 0;
	uint8_t t1[1];
	READ_REGISTER_MPL3115A2(t1,0x04,1);
	uint8_t t2[1];
	READ_REGISTER_MPL3115A2(t2,0x05,1);
	t = (t1[0] << 8) | t2[0];
	return t;
}
double parseTemp_MPL3115A()
{
	uint16_t temp = Read_Temp_MPL3115A();
	uint16_t t_m = (temp >> 8) & 0xFF;
	uint16_t t_l = temp & 0xFF;
	if (t_m > 0x7f) t_m = t_m - 256;
	return t_m;// + t_l / 256.0;
}
