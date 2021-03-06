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
#include "string.h"
#include "stdio.h"
#include "stdlib.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
uint8_t AIRQ_Initialized = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Error_Handler(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART2_UART_Init(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/*
 CCS811 Air Quality Sensor Example Code --(Adjusted to fit our use-case -Abdil)
 By: Nathan Seidle
 SparkFun Electronics
 Date: February 7th, 2017
 License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).

 Read the TVOC and CO2 values from the SparkFun CSS811 breakout board

 A new sensor requires at 48-burn in. Once burned in a sensor requires
 20 minutes of run in before readings are considered good.

 Hardware Connections (Breakoutboard to Arduino):
 3.3V = 3.3V
 GND = GND
 SDA = A4
 SCL = A5
 WAKE = D5 - Optional, can be left unconnected
 */
#define CCS811_ADDR 0x5B //7-bit unshifted default I2C Address

#define WAKE 13 //!Wake on breakout connected to pin 5 on Arduino

//Register addresses
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

void AIRQ_WRITE_REGISTER(uint8_t reg, uint8_t value) {
	uint8_t pData[] = { reg, value };
	HAL_I2C_Master_Transmit(&hi2c1, CCS811_ADDR << 1, pData, 2, HAL_MAX_DELAY);
}

void AIRQ_WRITE_REGISTER_NO_VAL(uint8_t reg) {
	HAL_I2C_Master_Transmit(&hi2c1, CCS811_ADDR << 1, &reg, 1, HAL_MAX_DELAY);
}

void AIRQ_READ_REGISTER(uint8_t* buf, uint8_t reg, uint8_t burstSize) {
	//uint8_t buf[burstSize];
	//reallocate for changeing burst, thus buffer size in bytes to read from the i2c channel
	// Make the array bigger
	uint8_t* reallocated_buf = realloc(buf, burstSize * sizeof(uint8_t));
	buf = reallocated_buf;

	//uint8_t (*out)[burstSize] = &buf;
	//int status =...
	HAL_I2C_Mem_Read(&hi2c1, CCS811_ADDR << 1, reg, I2C_MEMADD_SIZE_8BIT, buf,
			burstSize, HAL_MAX_DELAY);
}

/*Hardware ID (0x20) Register Handling
 The hardware ID register, sometimes referred to as the ‘Who Am I’ register, can be read during CCS811
 initialization to ensure that the device is indeed a CCS811. When this mailbox is the target of a read its proxy
 register will return the value 0x81. To read this register the host software would perform the following:*/
int AIRQ_Init(uint8_t* buf) {
	char str[200];

	AIRQ_READ_REGISTER(buf, 0x20, 1);

	sprintf(str, "HW ID : %d\r\n", buf[0]);
	HAL_UART_Transmit(&huart2, (uint8_t*) str, strlen(str), HAL_MAX_DELAY);

	if (buf[0] == 0x81) {
		AIRQ_Initialized = 1;
		//if HW ID is correct, start the application by writing to the start app register address/mailbox
		AIRQ_WRITE_REGISTER_NO_VAL(CSS811_APP_START);
		return 1;
	} else
		return 0;
}

/* Checks for the HW ID,  */
void configureCCS811(uint8_t* buf) {
	//Verify the hardware ID
	//Hardware ID should be 0x81
	if (AIRQ_Init(buf) == 1) {
		HAL_UART_Transmit(&huart2, (uint8_t*) "CCS811 Found \r\n",
				strlen("CCS811 Found \r\n"), HAL_MAX_DELAY);

	} else
		HAL_UART_Transmit(&huart2,
				(uint8_t*) "CCS811 not found. Please check wiring. \r\n",
				strlen("CCS811 not found. Please check wiring. \r\n"),
				HAL_MAX_DELAY);

	//Adjust the measurement mode, using a mask.
	//DRIVE_MODE : a measurement is performed every second
	//INTERRUPT : The nINT signal is asserted (driven low) when a new sample is ready in
	//ALG_RESULT_DATA. The nINT signal will stop being driven low when
	//ALG_RESULT_DATA is read on the I
	//2C interface.
	//Make sure to pass the buffer pointer and the buffer size, so memory reallocation is done accordingly
	AIRQ_READ_REGISTER(buf, CSS811_MEAS_MODE, 1);
	buf[0] &= 0b10000111;
	buf[0] |= 0b00011000;
	AIRQ_WRITE_REGISTER(CSS811_MEAS_MODE, buf[0]);

	HAL_UART_Transmit(&huart2, (uint8_t*) "Init successful \r\n",
			strlen("Init successful \r\n"), HAL_MAX_DELAY);

}

//initial allocation for buffer
uint16_t CO2;
uint16_t tVOC;

int main(void) {

	uint8_t* buf = malloc(8 * sizeof(uint8_t));
	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MCU Configuration----------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* Configure the system clock */
	SystemClock_Config();

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_I2C1_Init();
	MX_USART2_UART_Init();

	/* USER CODE BEGIN 2 */

	/* USER CODE END 2 */

	/* Configure the air quality measurement sensor */
	configureCCS811(buf);

	/* define a burstsize for the number of bytes to read in the buffer */
	int burstSize = 4;

	//this bit is only to make continuous prints visible in xterm by using alternating dots
	//int i = 0;
	//char *dots[] = { ".", "..", "...", "....", ".....", "......", ".......","........" };

	//Read at least once after configuration, so the sensor triggers a "new value interrupt"
	AIRQ_READ_REGISTER(buf, CSS811_ALG_RESULT_DATA, burstSize);
	char str[200];

	CO2 = (buf[0] << 8) | buf[1];
	tVOC = (buf[2] << 8) | buf[3];

	//sprintf(str, "CO2 : %d \t , \t tVOC : %d %s\r\n", CO2, tVOC,dots[i]);
	sprintf(str, "%d %d\r\n", CO2, tVOC);
	HAL_UART_Transmit(&huart2, (uint8_t*) str, strlen(str), HAL_MAX_DELAY);

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */

	}
	/* USER CODE END 3 */

}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if (GPIO_Pin == GPIO_PIN_8) {
		uint8_t* buf = malloc(8 * sizeof(uint8_t));
		/* Read the algorithm result data, returning 4 bytes : 2 MSBytes for CO2, 2 LSBytes for tVOC */
		AIRQ_READ_REGISTER(buf, CSS811_ALG_RESULT_DATA, 4);
		char str[200];

		CO2 = (buf[0] << 8) | buf[1];
		tVOC = (buf[2] << 8) | buf[3];

		//sprintf(str, "CO2 : %d \t , \t tVOC : %d %s\r\n", CO2, tVOC,dots[i]);
		sprintf(str, "%d %d\r\n", CO2, tVOC);
		HAL_UART_Transmit(&huart2, (uint8_t*) str, strlen(str), HAL_MAX_DELAY);

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
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
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
