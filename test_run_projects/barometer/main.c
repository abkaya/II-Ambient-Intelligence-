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

/* USER CODE BEGIN Includes */

#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
uint8_t MPL3115A2_Initialized = 0;
uint8_t MAG3110_Initialized = 0;
/* Private variables ---------------------------------------------------------*/
#define barom 0x60
#define magneto 14
#define accel 0x55
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

/********************
*********************
**I2C R/W FUNCTIONS**
*********************
*********************/
void SENSE_WRITE_REGISTER(uint8_t reg, uint8_t value, uint8_t device)
{
  uint8_t pData[]={reg, value};
  device = device <<1;
  int status=HAL_I2C_Master_Transmit(&hi2c1, device, pData,2, HAL_MAX_DELAY);  
}

uint8_t SENSE_READ_REGISTER(uint8_t reg, uint8_t device)
{
  uint8_t buf;
  device = device <<1;
  int status = HAL_I2C_Mem_Read(&hi2c1, device, reg, I2C_MEMADD_SIZE_8BIT, &buf, 1, HAL_MAX_DELAY);

  return buf;
}

/*************************
**************************
* MAGNETOMETER FUNCTIONS**
**************************
**************************/

int16_t MAG3110_ReadRawData_x() 
{
  char r = 0;
  int16_t a = 0;
  if (MAG3110_Initialized == 1) 
  {
    a = SENSE_READ_REGISTER(1, magneto);
    a = SENSE_READ_REGISTER(2, magneto);
    SENSE_WRITE_REGISTER(17, 128, magneto);
    SENSE_WRITE_REGISTER(16, 2, magneto);
    do 
    {
      r = SENSE_READ_REGISTER(0, magneto);
    } 
    while (!(r & 1));
    a = SENSE_READ_REGISTER(1, magneto);
    a = a << 8;
    a = a + SENSE_READ_REGISTER(2, magneto);
  }	
  return a;
}

int16_t MAG3110_ReadRawData_y() 
{
  char r = 0;
  int16_t a = 0;
  if (MAG3110_Initialized == 1) 
  {
    a = SENSE_READ_REGISTER(3, magneto);
    a = SENSE_READ_REGISTER(4, magneto);
    SENSE_WRITE_REGISTER(17, 128, magneto);
    SENSE_WRITE_REGISTER(16, 2, magneto);
    do 
    {
      r = SENSE_READ_REGISTER(0, magneto);
    } 
    while (!(r & 2));
    a = SENSE_READ_REGISTER(3, magneto);
    a = a << 8;
    a = a + SENSE_READ_REGISTER(4, magneto);
  }	
  return a;
}

int16_t MAG3110_ReadRawData_z() 
{
  char r = 0;
  int16_t a = 0;
  if (MAG3110_Initialized == 1) 
  {
    a = SENSE_READ_REGISTER(5, magneto);
    a = SENSE_READ_REGISTER(6, magneto);
    SENSE_WRITE_REGISTER(17, 128, magneto);
    SENSE_WRITE_REGISTER(16, 2, magneto);
    do 
    {
      r = SENSE_READ_REGISTER(0, magneto);
    } 
    while (!(r & 4));
    a = SENSE_READ_REGISTER(5, magneto);
    a = a << 8;
    a = a + SENSE_READ_REGISTER(6, magneto);
  }	
  return a;
}

int16_t* Read_MagRawData()
{
 static int16_t xyz[3];
  
  xyz[0] = MAG3110_ReadRawData_x();
  xyz[1] = MAG3110_ReadRawData_y();
  xyz[2] = MAG3110_ReadRawData_z();  
  return xyz;
}

void MAG3110_Init() 
{
  uint8_t v = 0;  
  //bcm2835_i2c_setClockDivider(2500);
  SENSE_WRITE_REGISTER(16, 0, magneto);
  v = SENSE_READ_REGISTER(7, magneto);
  if (v == 0xc4) 
  {
   SENSE_WRITE_REGISTER(17, 128, magneto);
    MAG3110_Initialized = 1;
  }  
}

/**********************
***********************
* BAROMETER FUNCTIONS**
***********************
***********************/

void MPL3115A2_Init_Bar()
{
  uint8_t c = SENSE_READ_REGISTER(12, barom);
  if (c == 0xc4) 
  {
    MPL3115A2_Initialized = 1;
    SENSE_WRITE_REGISTER(0x26, 0x38, barom);
    SENSE_WRITE_REGISTER(0x27, 0x00, barom);
    SENSE_WRITE_REGISTER(0x28, 0x11, barom);
    SENSE_WRITE_REGISTER(0x29, 0x00, barom);
    SENSE_WRITE_REGISTER(0x2a, 0x00, barom);
    SENSE_WRITE_REGISTER(0x13, 0x07, barom);
  }
}

void MPL3115A2_Active()
{
  uint8_t v = SENSE_READ_REGISTER(0x26, barom);
  v = v | 0x01;
  SENSE_WRITE_REGISTER(0x26, v, barom);
}

void MPL3115A2_Standby()
{
  uint8_t v = SENSE_READ_REGISTER(0x26, barom);
  v = v & ~0x01;
  SENSE_WRITE_REGISTER(0x26, v, barom);
}

int MPL3115A2_Read_Alt()
{
  int a = 0;
  if (MPL3115A2_Initialized == 1) 
  {
    a = SENSE_READ_REGISTER(0x01, barom);
    a <<= 8;
    int b = SENSE_READ_REGISTER(0x02, barom);
    a = a + b;
    a <<= 8;
    int c = SENSE_READ_REGISTER(0x03, barom);
    a = a + c;
  }
  return a;
}

double getBar() 
{
  int v = MPL3115A2_Read_Alt();
  int alt_m = v >> 6;
  int alt_l = v & 0x30;
  return alt_m + alt_l / 64.0;
}

/************************
*************************
**THERMOMETER FUNCTIONS**
*************************
*************************/

double getTemp() {
  int t = MPL3115A2_Read_Temp();
  int t_m = (t >> 8) & 0xFF;
  int t_l = t & 0xFF;
  if (t_m > 0x7f) t_m = t_m - 256;
  return t_m + t_l / 256.0;
}

int MPL3115A2_Read_Temp() 
{
  int a = 0;
  if (MPL3115A2_Initialized == 1) 
  {
    a = (SENSE_READ_REGISTER(0x04, barom) << 8) + SENSE_READ_REGISTER(0x05, barom);
  }
  return a;
}

/**************************
***************************
**ACCELEROMETER FUNCTIONS**
***************************
***************************/
void EnableAccel()
{
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
  printf("Enable is set:%d\n\r", HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7));
}

void DisableAccel()
{
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
  printf("Enable is reset:%d\n\r", HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7));
}

int16_t* MMA8491_ReadRaw() 
{
  int16_t x = 0;
  int16_t y = 0;
  int16_t z = 0;
  uint8_t status = 0;
  uint8_t bit = 0;
  EnableAccel();
  HAL_Delay(50);
  do 
  {
    status = SENSE_READ_REGISTER(0x00, accel);
    bit = (status >> 3) & 1;
  }while(!bit);
  x = SENSE_READ_REGISTER(0x01, accel) << 8;
  x += SENSE_READ_REGISTER(0x02, accel);
  x >>= 2; 
  y = SENSE_READ_REGISTER(0x03, accel) << 8;
  y += SENSE_READ_REGISTER(0x04, accel);
  y >>= 2;
  z = SENSE_READ_REGISTER(0x05, accel) << 8;
  z += SENSE_READ_REGISTER(0x06, accel);
  z >>= 2;
  DisableAccel();  
  static int16_t xyz[3];
  xyz[0] = x;
  xyz[1] = y; 
  xyz[2] = z;
  return xyz;
}                     
                    

/*
* use printf through uart
*/
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART3 and Loop until the end of transmission */
  HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 0xFFFF);  return ch;
}
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
  MX_I2C1_Init();
  MX_USART2_UART_Init();
  
  /* USER CODE BEGIN 2 */
  MPL3115A2_Init_Bar();
  MPL3115A2_Active();  
  MAG3110_Init();
  
  /* USER CODE END 2 */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
    /* USER CODE BEGIN 3 */

    int16_t* xyz;
//    xyz = Read_MagRawData();
//    printf("Magnetometer is geinitialiseerd: %d\r\n", MAG3110_Initialized);
//    printf("X-magneetwaarde: %d\n\r", *(xyz+0));
//    printf("Y-magneetwaarde: %d\n\r", *(xyz+1));
//    printf("Z-magneetwaarde: %d\n\r", *(xyz+2));
    xyz = MMA8491_ReadRaw();
    printf("X-accel: %d\n\r", *(xyz+0));
    printf("Y-accel: %d\n\r", *(xyz+1));
    printf("Z-accel: %d\n\r\n", *(xyz+2));
    HAL_Delay(500);
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_5;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
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
  GPIO_InitTypeDef GPIO_InitStruct;

  /* Peripheral clock enable */
  __HAL_RCC_I2C1_CLK_ENABLE();

  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 50000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  //HAL_I2C_Init(&hi2c1);       
  GPIO_InitStruct.Pin =GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
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
    Error_Handler();
  }
}

/** Pinout Configuration
*/
static void MX_GPIO_Init(void)
{  
  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
  GPIO_InitTypeDef GPIO_InitStruct;
  
  GPIO_InitStruct.Pin = GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
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
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
