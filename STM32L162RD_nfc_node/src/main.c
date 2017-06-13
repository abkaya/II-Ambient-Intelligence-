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
#include <stdlib.h>
#include <string.h>
#include "PN532.h"

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;
UART_HandleTypeDef huart1;

#define STARTUPNFC                      20
#define STARTUPDASH7                    1400
#define SENDTIMEDASH7					100
#define LOOKFORCARDTIME					5000
#define MAXTRIES						10
//SMALL SIZE PACKAGE
#define DASH7_DATALENGTH                        DASH7_ARRAYLENGTH * 2
#define ALP_LENGTH                              12 + DASH7_DATALENGTH

//LARGE SIZE PACKAGE
#define DASH7_ARRAYLENGTH_LARGE                 8
#define DASH7_DATALENGTH_LARGE                  DASH7_ARRAYLENGTH_LARGE * 2
#define ALP_LENGTH_LARGE                        12 + DASH7_DATALENGTH_LARGE
#define NELEMS(x)  (sizeof(x) / sizeof((x)[0])) // macro to get length of array

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
volatile char flags = 0b00000001;
uint8_t success;
uint8_t uidLength;
uint8_t failCounter = 0;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Error_Handler(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART1_UART_Init(void);

void getBin(int num, char *str);
static void dataToHex(int data, int dataHex[]);
uint8_t * getALP(int data[]);

void SleepMode(void);
void StopMode(void);

int main(void) {
	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* Configure the system clock */
	SystemClock_Config();

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_I2C1_Init();
	MX_USART1_UART_Init();

	/* USER CODE BEGIN 2 */

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);

	while (1) {
		toggleWhite(250);
		while (1) {
			if ((flags & 0b00000001) == 1) {
				//toggleBlue(20);
				HAL_GPIO_WritePin(GPIOA, VNFC_Pin, GPIO_PIN_SET);
				HAL_Delay(STARTUPNFC);
				setI2CInterface_PN532(&hi2c1);

				while (!getFirmwareVersion()) {
					if (failCounter >= MAXTRIES) {
						HAL_GPIO_WritePin(GPIOA, VNFC_Pin, GPIO_PIN_RESET);
						while (1) {
							toggleRed(50);
							HAL_Delay(2000);
						}
					}
					failCounter++;
					HAL_GPIO_WritePin(GPIOA, VNFC_Pin, GPIO_PIN_RESET);
					toggleRed(10);
					HAL_Delay(200);
					HAL_GPIO_WritePin(GPIOA, VNFC_Pin, GPIO_PIN_SET);
					HAL_Delay(STARTUPNFC * 10);
				}

				SAMConfig();
				uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };
				success = readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0],
						&uidLength, LOOKFORCARDTIME);
				int intUID[]={uid[0],uid[1],uid[2],uid[3],uid[4],uid[5],uid[6]};
				HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_7);
				uint8_t * alpCmd = getALP(intUID);
				HAL_UART_Transmit(&huart1, (uint8_t*) alpCmd, sizeof(alpCmd),
				HAL_MAX_DELAY);
				free(alpCmd);
				HAL_Delay(100);
				HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_7);
				HAL_Delay(2000);

				flags = flags & 0b11111110;
			}
			SleepMode();
		}
		/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */

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
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart1) != HAL_OK) {
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
	__HAL_RCC_GPIOB_CLK_ENABLE()
	;

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7,
			GPIO_PIN_RESET);

	/*Configure GPIO pins : PA5 PA6 PA7 */
	GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pin : PA11 */
	GPIO_InitStruct.Pin = GPIO_PIN_11;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pin : PA13 */
	GPIO_InitStruct.Pin = GPIO_PIN_13;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/* EXTI interrupt init*/
	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

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

void SleepMode(void) {
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
	//HAL_Delay(10);
	//GPIO_InitTypeDef GPIO_InitStruct;
	// MX_GPIO_Deinit();
	//HAL_I2C_MspDeInit(&hi2c1);
	HAL_SuspendTick();
	__HAL_RCC_PWR_CLK_ENABLE()
	;
	//HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI); //sleep
	//HAL_PWR_EnterSLEEPMode(PWR_LOWPOWERREGULATOR_ON, PWR_SLEEPENTRY_WFI); //lowPowerSleep
	//HAL_PWR_EnterSTOPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI); //stopMode
	HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_SLEEPENTRY_WFI); //LOWpowerstopMode
	HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1);

	/* Enter STANDBY mode */
	//HAL_PWR_EnterSTANDBYMode();
	HAL_ResumeTick();
	// MX_USART1_UART_Init();*
	//MX_USART2_UART_Init();
	// MX_I2C1_Init();
	// MX_GPIO_Init();
	HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);
	__HAL_GPIO_EXTI_CLEAR_IT(11);
}
void StopMode(void) {
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
	HAL_Delay(10);
	//GPIO_InitTypeDef GPIO_InitStruct;
	// MX_GPIO_Deinit();
	HAL_UART_DeInit(&huart1);
	//HAL_I2C_MspDeInit(&hi2c1);
	HAL_SuspendTick();
	__HAL_RCC_PWR_CLK_ENABLE()
	;
	/* Request to enter SLEEP mode */
	HAL_PWR_EnterSTOPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
	HAL_ResumeTick();
	// MX_USART1_UART_Init();
	MX_USART2_UART_Init();
	// MX_I2C1_Init();
	// MX_GPIO_Init();
	HAL_Delay(10);
	__HAL_GPIO_EXTI_CLEAR_IT(11);
	HAL_Delay(10);
	HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);
	HAL_Delay(10);
	__HAL_GPIO_EXTI_CLEAR_IT(11);
	HAL_Delay(10);

}

void toggleRed(uint8_t time) {
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_6); //RED LED
	HAL_Delay(time);
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_6); //RED LED
}
void toggleGreen(uint8_t time) {
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5); //GREEN LED
	HAL_Delay(time);
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5); //GREEN LED
}
void toggleBlue(uint8_t time) {
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_7); //BlUE LED
	HAL_Delay(time);
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_7); //BlUE LED
}
void toggleWhite(uint8_t time) {
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5); //RED LED
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_6); //GREEN LED
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_7); //BlUE LED
	HAL_Delay(time);
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5); //RED LED
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_6); //GREEN LED
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_7); //BlUE LED
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
	uint8_t len = strlen(binstr);

	uint8_t len1 = len / 2;
	uint8_t len2 = len - len1; // Compensate for possible odd length
	char *s1 = (uint8_t*) malloc(len1 + 1); // one for the null terminator
	memcpy(s1, binstr, len1);
	s1[len1] = '\0';
	char *s2 = (uint8_t*) malloc(len2 + 1); // one for the null terminator
	memcpy(s2, binstr + len1, len2);
	s2[len2] = '\0';

	dataHex[0] = strtol(s1, NULL, 2);
	dataHex[1] = strtol(s2, NULL, 2);

	free(s1);
	free(s2);

}

uint8_t * getALP(int data[]) {

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

	//printf("[ALP_LENGTH] = %d \n\n\r",7 + ALP_LENGTH);
	uint8_t ALPCommand[7 + ALP_LENGTH] = { 0x41, 0x54, 0x24, 0x44, 0xc0, 0x00,
	ALP_LENGTH, // SERIAL
			0x34, 0x01, //TAG
			0x32, 0xd7, 0x01, 0x00, 0x10, 0x01, //FORWARD
			0x20, 0x01, 0x00, //CommnandLine
			DASH7_DATALENGTH //Data
			};

	int datacounter = 0;
	int arraycounter = 19;
	int dataHex[2];

	while (datacounter < DASH7_ARRAYLENGTH) {

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

	HAL_UART_Transmit(&huart1, (uint8_t*) ALPCommand, sizeof(ALPCommand),
	HAL_MAX_DELAY);

	/*
	 //Don't forget to free ALPCommand's allocated memory wherever it is returned
	 uint8_t *ALPCommand = malloc(sizeof(uint8_t) * (7 + ALP_LENGTH));
	 ALPCommand[0] = 0x41;	// SERIAL
	 ALPCommand[1] = 0x54;
	 ALPCommand[2] = 0x24;
	 ALPCommand[3] = 0x44;
	 ALPCommand[4] = 0xC0;
	 ALPCommand[5] = 0x88;
	 ALPCommand[6] = ALP_LENGTH;
	 ALPCommand[7] = 0x34;	//TAG
	 ALPCommand[8] = 0x01;
	 ALPCommand[9] = 0x32;	//FORWARD
	 ALPCommand[10] = 0xd7;
	 ALPCommand[11] = 0x01;
	 ALPCommand[12] = 0x00;
	 ALPCommand[13] = 0x10;
	 ALPCommand[14] = 0x01;
	 ALPCommand[15] = 0x20;	//CommnandLine
	 ALPCommand[16] = 0x01;
	 ALPCommand[17] = 0x00;
	 ALPCommand[18] = DASH7_DATALENGTH;
	 //Data

	 int datacounter = 0;
	 int arraycounter = 19;
	 int dataHex[2];

	 while (datacounter < DASH7_ARRAYLENGTH) {

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
	 */
	return ALPCommand;

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
