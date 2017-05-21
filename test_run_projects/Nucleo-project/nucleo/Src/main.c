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

/* USER CODE BEGIN Includes */
/* Private function prototypes -----------------------------------------------*/
#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

#define RXBUFFERSIZE                      10
#define DASH7_ARRAYLENGTH                 3
#define DASH7_DATALENGTH                  DASH7_ARRAYLENGTH * 2      
#define ALP_LENGTH                        12 + DASH7_DATALENGTH  
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c2;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

PCD_HandleTypeDef hpcd_USB_FS;

uint8_t aRxBuffer[RXBUFFERSIZE];

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Error_Handler(char *error);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_I2C1_Init(void);
static void MX_I2C2_Init(void);
static void MX_USB_PCD_Init(void);
static void EXTI15_10_IRQHandler_Config(void);
static void DASH7_Write();
static void DASH7_Read();
static void dataToHex(int data,int dataHex[]);
static void ALP_Generator(int data[]);
//unsigned int int_to_int(unsigned int k);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_USART1_UART_Init();
  MX_I2C1_Init();
  MX_I2C2_Init();
  MX_USB_PCD_Init();

  /* USER CODE BEGIN 2 */
   EXTI15_10_IRQHandler_Config();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    DASH7_Read();
    
  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
  RCC_OscInitStruct.PLL.PLLDIV = RCC_PLL_DIV3;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler("RCC OSCCONFIG ERROR");
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler("RCC CLOCK CONFIG ERROR");
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* I2C1 init function */
static void MX_I2C1_Init(void)
{

  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler("I2C1 INIT ERROR");
  }

}

/* I2C2 init function */
static void MX_I2C2_Init(void)
{

  hi2c2.Instance = I2C2;
  hi2c2.Init.ClockSpeed = 100000;
  hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler("I2C2 INIT ERROR");
  }

}

/* USART1 init function */
static void MX_USART1_UART_Init(void)
{

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler("UART1 INIT ERROR");
  }

}

/* USART2 init function */
static void MX_USART2_UART_Init(void)
{

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler("UART2 INIT ERROR");
  }

}

/* USB init function */
static void MX_USB_PCD_Init(void)
{

  hpcd_USB_FS.Instance = USB;
  hpcd_USB_FS.Init.dev_endpoints = 8;
  hpcd_USB_FS.Init.speed = PCD_SPEED_FULL;
  hpcd_USB_FS.Init.ep0_mps = DEP0CTL_MPS_8;
  hpcd_USB_FS.Init.phy_itface = PCD_PHY_EMBEDDED;
  hpcd_USB_FS.Init.low_power_enable = DISABLE;
  hpcd_USB_FS.Init.battery_charging_enable = DISABLE;
  if (HAL_PCD_Init(&hpcd_USB_FS) != HAL_OK)
  {
    Error_Handler("USB PCD INIT ERROR");
  }

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
PUTCHAR_PROTOTYPE
{
 /* Place your implementation of fputc here */
 /* e.g. write a character to the USART3 and Loop until the end of transmission */
 HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 0xFFFF);  return ch;
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(char *error)
{
  printf("** ERROR ** \n\r");
  printf("[ERROR]: %s \n\r",error);
}

static void EXTI15_10_IRQHandler_Config(void)
{
  /* Enable and set EXTI line 15_10 Interrupt to the lowest priority */
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

/**
  * @brief EXTI line detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == B1_Pin)
  {
   //printf("** SEND DASH7 DATA ** \n\r");
    

  //DASH7_Write();
  int data[] = {460,20,300};
  ALP_Generator(data);
    
    
    HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
  }
}

static void DASH7_Write(){  
    uint8_t message[] = {0x41 ,0x54 ,0x24 ,0x44 ,0xc0 ,0x00 ,0x0e ,0xb4 ,0xaf ,0x32 ,0xd7 ,0x01 ,0x00 ,0x10 ,0x01 ,0x20 ,0x01 ,0x00 ,0x02 ,0x00 ,0x01};  
    //printf("[DASH7_DATA] = %s \n\n\r",message);
    //HAL_UART_Transmit(&huart1, (uint8_t*)message, sizeof(message),HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart2, (uint8_t*)message, sizeof(message),HAL_MAX_DELAY);
}



static void DASH7_Read(){
  char readBuf[1];
  HAL_UART_Receive(&huart1, (uint8_t*)readBuf, 1, HAL_MAX_DELAY);
  int opt = atoi(readBuf);
  if (opt != 0) {
      printf("** RECEIVED DASH7 DATA ** \n\r");
      printf("[DASH7_DATA] = %s \n\n\r",readBuf);
      HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
      HAL_Delay(100);
      HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
  }
}

void getBin(int num, char *str)
{
  *(str+15) = '\0';
  int mask = 0x8000 << 1;
  while(mask >>= 1)
    *str++ = !!(mask & num) + '0';
}


static void dataToHex(int data,int dataHex[2]){
  char binstr[16];
  getBin(data, binstr);
  int len = strlen(binstr);
  
  printf("[DATA] = %d \n\r",data);
  printf("[BIN] = %s \n\r",binstr);
  
  int len1 = len/2;
  int len2 = len - len1; // Compensate for possible odd length
  char *s1 = (uint8_t*)malloc(len1+1); // one for the null terminator
  memcpy(s1, binstr, len1);
  s1[len1] = '\0';
  char *s2 = (uint8_t*)malloc(len2+1); // one for the null terminator
  memcpy(s2, binstr+len1, len2);
  s2[len2] = '\0';
  
  printf("[BIN1] = %s \n\r",s1);
  printf("[BIN2] = %s \n\n\r",s2);
  
  dataHex[0] = strtol(s1, NULL, 2);
  dataHex[1] = strtol(s2, NULL, 2);
  
  free(s1);
  free(s2);

}

static void ALP_Generator(int data[]){
      
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
    
    uint8_t ALPCommand[7 + ALP_LENGTH] = {  
      0x41, 0x54, 0x24, 0x44, 0xc0, 0x00, ALP_LENGTH, // SERIAL
      0x34, 0x01, //TAG
      0x32, 0xd7, 0x01, 0x00, 0x10, 0x01, //FORWARD
      0x20, 0x01, 0x00, //CommnandLine
      DASH7_DATALENGTH //Data
    };

   int datacounter = 0;
   int arraycounter = 19;
   int dataHex[2];
 
   while(datacounter < DASH7_ARRAYLENGTH){

    dataToHex(data[datacounter],dataHex);

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
 
  HAL_UART_Transmit(&huart2, (uint8_t*)ALPCommand, sizeof(ALPCommand),HAL_MAX_DELAY);
   
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
