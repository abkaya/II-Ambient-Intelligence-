/**
 ******************************************************************************
 * File Name          : main.c
 * Description        : Main program body
 ******************************************************************************
 *
 * COPYRIGHT(c) 2017 STMicroelectronics
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of STMicroelectronics nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32l1xx_hal.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

uint8_t MAG_Initialized = 0;

uint8_t aRxBuffer[RXBUFFERSIZE];

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Error_Handler(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART1_UART_Init(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */
//SMALL SIZE PACKAGE
#define DASH7_ARRAYLENGTH                       4
#define DASH7_DATALENGTH                        DASH7_ARRAYLENGTH * 2
#define ALP_LENGTH                              12 + DASH7_DATALENGTH

//LARGE SIZE PACKAGE
#define DASH7_ARRAYLENGTH_LARGE                 8
#define DASH7_DATALENGTH_LARGE                  DASH7_ARRAYLENGTH_LARGE * 2
#define ALP_LENGTH_LARGE                        12 + DASH7_DATALENGTH_LARGE

#define MAG_ADDR 0x0E //7-bit unshifted default I2C Address
#define NELEMS(x)  (sizeof(x) / sizeof((x)[0])) // macro to get length of array

void I2C_WRITE_REGISTER(uint32_t adr, uint8_t reg, uint8_t value) {
	uint8_t pData[] = { reg, value };
	HAL_I2C_Master_Transmit(&hi2c1, adr << 1, pData, 2, HAL_MAX_DELAY);
}

void I2C_WRITE_REGISTER_NO_VAL(uint32_t adr, uint8_t reg) {
	HAL_I2C_Master_Transmit(&hi2c1, adr << 1, &reg, 1, HAL_MAX_DELAY);
}

void I2C_READ_REGISTER(uint32_t adr, uint8_t* buf, uint8_t reg,
		uint8_t burstSize) {
	//uint8_t buf[burstSize];
	//reallocate for changeing burst, thus buffer size in bytes to read from the i2c channel
	// Make the array bigger
	uint8_t* reallocated_buf = realloc(buf, burstSize * sizeof(uint8_t));
	buf = reallocated_buf;

	//uint8_t (*out)[burstSize] = &buf;
	//int status =...
	HAL_I2C_Mem_Read(&hi2c1, adr << 1, reg, I2C_MEMADD_SIZE_8BIT, buf,
			burstSize, HAL_MAX_DELAY);
}

/*Hardware ID (0x20) Register Handling
 The hardware ID register, sometimes referred to as the ‘Who Am I’ register, can be read during CCS811
 initialization to ensure that the device is indeed a CCS811. When this mailbox is the target of a read its proxy
 register will return the value 0x81. To read this register the host software would perform the following:*/
int MAG_Init(uint8_t* buf) {
	char str[200];
	uint8_t* reallocated_buf = realloc(buf, 1* sizeof(uint8_t));
	buf = reallocated_buf;
	I2C_READ_REGISTER(MAG_ADDR, buf, 0x07, 1);

	sprintf(str, "HW ID : %d\r\n", buf[0]);
	HAL_UART_Transmit(&huart2, (uint8_t*) str, strlen(str), HAL_MAX_DELAY);

	//WHO_AM_I(2) R 0x07 0x08 0xC4 Device ID Number
	if (buf[0] == 0xC4) {
		MAG_Initialized = 1;
		return 1;
	} else
		return 0;
}

/* Checks for the HW ID,  */
void configureMAG(uint8_t* buf) {
	//Verify the hardware ID
	//Hardware ID should be 0x81
	if (MAG_Init(buf) == 1) {
		HAL_UART_Transmit(&huart2, (uint8_t*) "MAG Found \r\n",
				strlen("MAG Found \r\n"), HAL_MAX_DELAY);

		//set auto resets
		I2C_WRITE_REGISTER(MAG_ADDR, 0x11, 0x80);
		//active mode
		I2C_WRITE_REGISTER(MAG_ADDR, 0x10, 0x01);

		HAL_UART_Transmit(&huart2, (uint8_t*) "Init successful \r\n",
				strlen("Init successful \r\n"), HAL_MAX_DELAY);

	} else
		HAL_UART_Transmit(&huart2,
				(uint8_t*) "MAG not found. Please check wiring. \r\n",
				strlen("MAG not found. Please check wiring. \r\n"),
				HAL_MAX_DELAY);

}

void getBin(int num, char *str) {
	*(str + 15) = '\0';
	int mask = 0x8000 << 1;
	while (mask >>= 1)
		*str++ = !!(mask & num) + '0';

}

static void dataToHex(int data, int dataHex[2]) {
	char binstr[16];
	getBin(data, binstr);
	int len = strlen(binstr);

	/*
	 printf("[DATA] = %d \n\r", data);
	 printf("[BIN] = %s \n\r", binstr);
	 */

	int len1 = len / 2;
	int len2 = len - len1; // Compensate for possible odd length
	char *s1 = (uint8_t*) malloc(len1 + 1); // one for the null terminator
	memcpy(s1, binstr, len1);
	s1[len1] = '\0';
	char *s2 = (uint8_t*) malloc(len2 + 1); // one for the null terminator
	memcpy(s2, binstr + len1, len2);
	s2[len2] = '\0';

	/*
	 printf("[BIN1] = %s \n\r", s1);
	 printf("[BIN2] = %s \n\n\r", s2);
	 */

	dataHex[0] = strtol(s1, NULL, 2);
	dataHex[1] = strtol(s2, NULL, 2);

	free(s1);
	free(s2);
}

void D7SendData(int data[], int length) {

	/*
	 -----------------------------FULL COMMAND WITH DATA [0,1] -------------------
	 41 54 24 44 c0 00 0e 34 af 32 d7 01 00 10 01 20 01 00 02 00 01
	 0x41 0x54 0x24 0x44 0xc0 0x00 0x0e 0x34 0xaf 0x32 0xd7 0x01 0x00 0x10 0x01 0x20 0x01 0x00 0x02 0x00 0x01
	 $41 $54 $24 $44 $c0 $00 $0e $34 $af $32 $d7 $01 $00 $10 $01 $20 $01 $00 $02 $00 $01

	 ---------------------SERIALIZE------------------------
	 41 54 24 44 c0 00 0e

	 STATIC SER       LENGTH
	 [41 54 24 44 c0 00]  [0e]

	 ++++++++++++++++ LENGTH ++++++++++++++++
	 Length of bytes of the ALP COMMAND

	 0e --> 14 --> Length

	 ---------------------ALP COMMAND----------------------
	 34 af 32 d7 01 00 10 01 20 01 00 02 00 01

	 TAG         FORWARD         RETURN FILE DATA
	 [34 af] [32 d7 01 00 10 01] [20 01 00 02 00 01]

	 ++++++++++++++++ TAG ++++++++++++++++

	 ALP COMMAND:
	 34 af

	 34 --> 52   --> Request Tag
	 af --> 175  --> Tag Value (random)

	 PARSED: Command with tag 175 (executing)

	 ++++++++++++++++ FORWARD ++++++++++++++++

	 ALP COMMAND:
	 32 d7 01 00 10 01

	 32 --> 50    --> Forward
	 d7 --> 215   --> Interface Name (D7ASP)
	 01 --> 01    --> ResponseMode.RESP_MODE_ANY
	 00 --> 00    --> mant
	 10 --> 16    --> ??
	 01 --> 01    --> access_class

	 PARSED:
	 actions:
	 action: Forward: interface-id=InterfaceType.D7ASP, configuration={'addressee': {'nls_method': <NlsMethod.NONE: 0>, 'access_class': 1, '__CLASS__': 'Addressee', 'id_type': <IdType.NOID: 1>, 'id': None}, 'qos': {'record': False, 'resp_mod': <ResponseMode.RESP_MODE_ALL: 1>, '__CLASS__': 'QoS', 'retry_mod': <RetryMode.RETRY_MODE_NO: 0>, 'stop_on_err': False}, '__CLASS__': 'Configuration', 'dorm_to': {'mant': 0, '__CLASS__': 'CT', 'exp': 0}}
	 ++++++++++++++ RETURN FILE DATA ++++++++++++++++

	 ALP COMMAND:
	 20 01 00 02 00 01

	 20 --> 32   --> Return File Data
	 01 --> 01   --> File ID
	 00 --> 00   --> Offset
	 02 --> 02   --> Length data bytes

	 <DATA> [0,1] LENGTH:2
	 00 --> 00
	 00 --> 01

	 PARSED:
	 actions:
	 action: ReturnFileData: file-id=1, size=1, offset=0, length=2, data=[0, 1]
	 */
	//printf("Length: %d \r\n", length);
	if (length <= 4) {
		uint8_t ALPCommand[7 + ALP_LENGTH] = { 0x41, 0x54, 0x24, 0x44, 0xc0,
				0x00, ALP_LENGTH, // SERIAL
				0x34, 0x01, //TAG
				0x32, 0xd7, 0x01, 0x00, 0x10, 0x01, //FORWARD
				0x20, 0x01, 0x00, //CommnandLine
				DASH7_DATALENGTH //Data
				};

		int datacounter = 0;
		int arraycounter = 19;
		int dataHex[2];

		while (datacounter < length) {
			dataToHex(data[datacounter], dataHex);
			ALPCommand[arraycounter] = dataHex[0];
			//printf("[SPOT0] = %d \n\n\r",arraycounter);
			//printf("[DATA%d:HEX0] = %X \n\r",datacounter,dataHex[0]);
			arraycounter++;

			ALPCommand[arraycounter] = dataHex[1];
			//printf("[SPOT1] = %d \n\n\r",arraycounter);
			//printf("[DATA%d:HEX1] = %X \n\n\r",datacounter,dataHex[1]);
			arraycounter++;
			datacounter++;

			dataHex[0] = 0;
			dataHex[1] = 0;

		}

		//printf("ALP: %s \r\n", ALPCommand);
		HAL_UART_Transmit(&huart1, (uint8_t*) ALPCommand, sizeof(ALPCommand),
		HAL_MAX_DELAY);
	}

	else if (length <= 8) {
		uint8_t ALPCommand[7 + ALP_LENGTH_LARGE] = { 0x41, 0x54, 0x24, 0x44,
				0xc0, 0x00, ALP_LENGTH_LARGE, // SERIAL
				0x34, 0x01, //TAG
				0x32, 0xd7, 0x01, 0x00, 0x10, 0x01, //FORWARD
				0x20, 0x01, 0x00, //CommnandLine
				DASH7_DATALENGTH_LARGE //Data
				};

		int datacounter = 0;
		int arraycounter = 19;
		int dataHex[2];

		while (datacounter < length) {
			dataToHex(data[datacounter], dataHex);
			ALPCommand[arraycounter] = dataHex[0];
			//printf("[SPOT0] = %d \n\n\r",arraycounter);
			//printf("[DATA%d:HEX0] = %X \n\r",datacounter,dataHex[0]);
			arraycounter++;

			ALPCommand[arraycounter] = dataHex[1];
			//printf("[SPOT1] = %d \n\n\r",arraycounter);
			//printf("[DATA%d:HEX1] = %X \n\n\r",datacounter,dataHex[1]);
			arraycounter++;
			datacounter++;

			dataHex[0] = 0;
			dataHex[1] = 0;
		}
		//printf("ALP: %s \r\n", ALPCommand);
		HAL_UART_Transmit(&huart1, (uint8_t*) ALPCommand, sizeof(ALPCommand),
		HAL_MAX_DELAY);

	} else {
		//printf("Data array too long. \r\n");
	}

}

void Dash7Send(void) {
	uint8_t message[] = { 0x41, 0x54, 0x24, 0x44, 0xc0, 0x00, 0x0e, 0xb4, 0xaf,
			0x32, 0xd7, 0x01, 0x00, 0x10, 0x01, 0x20, 0x01, 0x00, 0x02, 0x00,
			0x01 };
	HAL_UART_Transmit(&huart1, (uint8_t*) message, sizeof(message),
	HAL_MAX_DELAY);
}

int16_t x = 0;
int16_t y = 0;
int16_t z = 0;
int main(void) {

	uint8_t* buf = malloc(8 * sizeof(uint8_t));
	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* Configure the system clock */
	SystemClock_Config();

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_I2C1_Init();
	MX_USART2_UART_Init();
	MX_USART1_UART_Init();

	/* Configure the air quality measurement sensor */
	configureMAG(buf);

	//Read at least once after configuration, so the sensor triggers a "new value interrupt"
	I2C_READ_REGISTER(MAG_ADDR, buf, 0x01, 6);

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		uint8_t* buf = malloc(8 * sizeof(uint8_t));
		/* Read the result data, returning 6 bytes : 2 MSBytes for x,y,z, 2 LSBytes for x,y,z*/
//I2C_READ_REGISTER(MAG_ADDR, buf, 0x01, 6);
		I2C_READ_REGISTER(MAG_ADDR, buf, 0x01, 6);
		char str[200];

		x = (buf[0] << 8) | (buf[1]);
		y = (buf[2] << 8) | (buf[3]);
		z = (buf[4] << 8) | (buf[5]);
		int Data[] = { 0xc7c7, x, y, z };
		D7SendData(Data, NELEMS(Data));

		sprintf(str, "sync:%d %d, x:%d y:%d z:%d | ,%d,%d,%d,%d,%d,%d\r\n",
				0xc7, 0xc7, x, y, z, buf[0], buf[1], buf[2], buf[3], buf[4],
				buf[5]);
		HAL_UART_Transmit(&huart2, (uint8_t*) str, strlen(str), HAL_MAX_DELAY);
		HAL_Delay(2000);
	}

}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if (GPIO_Pin == GPIO_PIN_8) {

	}
}

/** System Clock Configuration
 */
void SystemClock_Config(void) {

	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_ClkInitTypeDef RCC_ClkInitStruct;

	/**Configure the main internal regulator output voltage
	 */
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	/**Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
	RCC_OscInitStruct.MSIState = RCC_MSI_ON;
	RCC_OscInitStruct.MSICalibrationValue = 0;
	RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_5;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}

	/**Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) {
		Error_Handler();
	}

	/**Configure the Systick interrupt time
	 */
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);

	/**Configure the Systick
	 */
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

	/* SysTick_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* I2C1 init function */
static void MX_I2C1_Init(void) {

	hi2c1.Instance = I2C1;
	hi2c1.Init.ClockSpeed = 100000;
	hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
	hi2c1.Init.OwnAddress1 = 0;
	hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c1.Init.OwnAddress2 = 0;
	hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(&hi2c1) != HAL_OK) {
		Error_Handler();
	}

}

/* USART1 init function */
static void MX_USART1_UART_Init(void) {

	huart1.Instance = USART1;
	huart1.Init.BaudRate = 115200;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_RTS_CTS;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart1) != HAL_OK) {
		Error_Handler();
	}

}

/* USART2 init function */
static void MX_USART2_UART_Init(void) {

	huart2.Instance = USART2;
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_RTS_CTS;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart2) != HAL_OK) {
		Error_Handler();
	}

}

/** Configure pins as 
 * Analog
 * Input
 * Output
 * EVENT_OUT
 * EXTI
 */
static void MX_GPIO_Init(void) {

	GPIO_InitTypeDef GPIO_InitStruct;

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOA_CLK_ENABLE()
	;
	__HAL_RCC_GPIOC_CLK_ENABLE()
	;
	__HAL_RCC_GPIOB_CLK_ENABLE()
	;

	/*Configure GPIO pin : PC8 */
	GPIO_InitStruct.Pin = GPIO_PIN_8;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/* EXTI interrupt init*/
	HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @param  None
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler */
	/* User can add his own implementation to report the HAL error return state */
	while (1) {
	}
	/* USER CODE END Error_Handler */
}

#ifdef USE_FULL_ASSERT

/**
 * @brief Reports the name of the source file and the source line number
 * where the assert_param error has occurred.
 * @param file: pointer to the source file name
 * @param line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t* file, uint32_t line)
{
	/* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
	 ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	/* USER CODE END 6 */

}

#endif

/**
 * @}
 */

/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
