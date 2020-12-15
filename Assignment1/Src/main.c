/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "rng.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
//#include "ws2812b.h"
#include "stdio.h"
#include "string.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
extern DMA_HandleTypeDef hdma_adc1;
int j;
uint32_t millisec;
int point=0;
int pressed=0;

int color_r, color_g, color_b,r1,g1,b1;

uint32_t countTimePress;
int score=0;
int checkTimeOut=0;

int r=80,g=40,b=70;
uint8_t red,green,blue;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void colorBlindGame(void);
void colorBlindLevel0(void);
void colorBlindLevel1(void);
void colorBlindLevel2(void);

uint8_t randomNumber(uint8_t lower, uint8_t upper);
void randomColor(void);
void setRandomColor(uint8_t index);
void colorsMatching() ;
void checkSW();
void closeLED();
void setCloseLED(uint8_t index);
void displayTime (uint32_t num );
void delay(uint32_t time);

uint16_t Mode1setAllColors();
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  /* USER CODE END 1 */
  

  /* Enable I-Cache---------------------------------------------------------*/
  SCB_EnableICache();

  /* Enable D-Cache---------------------------------------------------------*/
  SCB_EnableDCache();

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_USART3_UART_Init();
  MX_I2C1_Init();
  MX_RNG_Init();
  MX_SPI5_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
	led_init();
	HAL_TIM_Base_Start(&htim2);
	HAL_ADC_Start_IT(&hadc1);
	HAL_TIM_Base_Start_IT(&htim3);
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
			
		Mode1setAllColors();
		
		/*colorBlindGame();
			if(pressed==0)
			{
				break;
			}*/
		//colorsMatching();
		/*if(checkTimeOut==1)
			{
				break;
			}*/
		//displayTime(millisec);
		}
	
	char tmp[500];
	sprintf(tmp, "\n\n\rD E A D\n\rYour score : %d\n\rHigh score : XX", point);
	while(__HAL_UART_GET_FLAG(&huart3, UART_FLAG_TC)==RESET){}
	HAL_UART_Transmit(&huart3, (uint8_t *) tmp, strlen(tmp), 100);
		
	led_set_color_all(0, 0, 0);
		
	led_display();
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure LSE Drive Capability 
  */
  HAL_PWR_EnableBkUpAccess();
  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 216;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Activate the Over-Drive mode 
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART3|RCC_PERIPHCLK_I2C1
                              |RCC_PERIPHCLK_CLK48;
  PeriphClkInitStruct.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
  PeriphClkInitStruct.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  PeriphClkInitStruct.Clk48ClockSelection = RCC_CLK48SOURCE_PLL;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void colorBlindLevel0(void) {
	uint16_t i;
	color_r = (HAL_RNG_GetRandomNumber(&hrng))%256;
	color_g = (HAL_RNG_GetRandomNumber(&hrng))%256;
	color_b = (HAL_RNG_GetRandomNumber(&hrng))%256;
	
	led_set_color(5,color_r, color_g, color_b);
	led_set_color(6,color_r, color_g, color_b);
	led_set_color(9,color_r, color_g, color_b);
	led_set_color(10,color_r, color_g, color_b);
		//if (flag_stop) break;
	
	j = (HAL_RNG_GetRandomNumber(&hrng))%4;
	if(j==0) j=5;
	else if(j==1) j=6;
	else if(j==2) j=9;
	else if(j==3) j=10;
	
	r1 = (HAL_RNG_GetRandomNumber(&hrng))%256;
	g1 = (HAL_RNG_GetRandomNumber(&hrng))%256;
	b1 = (HAL_RNG_GetRandomNumber(&hrng))%256;
	
	while((r1==color_r) && (g1==color_g) && (b1==color_b))
	{
		r1 = (HAL_RNG_GetRandomNumber(&hrng))%256;
		g1 = (HAL_RNG_GetRandomNumber(&hrng))%256;
		b1 = (HAL_RNG_GetRandomNumber(&hrng))%256;
	}
	led_set_color(j, r1, g1, b1);
	led_display();
}


void colorBlindLevel1(void) {
	uint16_t i;
	color_r = (HAL_RNG_GetRandomNumber(&hrng))%256;
	color_g = (HAL_RNG_GetRandomNumber(&hrng))%256;
	color_b = (HAL_RNG_GetRandomNumber(&hrng))%256;
	
	led_set_color_all(color_r, color_g, color_b);
		//if (flag_stop) break;
	
	j = (HAL_RNG_GetRandomNumber(&hrng))%16;
	
	r1 = (HAL_RNG_GetRandomNumber(&hrng))%256;
	g1 = (HAL_RNG_GetRandomNumber(&hrng))%256;
	b1 = (HAL_RNG_GetRandomNumber(&hrng))%256;
	
	while((r1==color_r) && (g1==color_g) && (b1==color_b))
	{
		r1 = (HAL_RNG_GetRandomNumber(&hrng))%256;
		g1 = (HAL_RNG_GetRandomNumber(&hrng))%256;
		b1 = (HAL_RNG_GetRandomNumber(&hrng))%256;
	}
	led_set_color(j, r1, g1, b1);
	led_display();
}

void colorBlindLevel2(void)
{
	uint16_t i;
	color_r = (HAL_RNG_GetRandomNumber(&hrng))%256;
	color_g = (HAL_RNG_GetRandomNumber(&hrng))%256;
	color_b = (HAL_RNG_GetRandomNumber(&hrng))%256;
	
	led_set_color_all(color_r, color_g, color_b);
		//wsled.led_set_color(i, 1, 23, 12);
		//if (flag_stop) break;
	
	int rand=(HAL_RNG_GetRandomNumber(&hrng))%2;
	if(rand==0)
	{
		r1=(color_r+40);
		if(r1>255) r1=255;
	}
	else
	{
		r1=(color_r-40);
		if(r1<0) r1=0;
	}
	rand=(HAL_RNG_GetRandomNumber(&hrng))%2;
	if(rand==0)
	{
		g1=(color_g+40);
		if(g1>255) g1=255;
	}
	else
	{
		g1=(color_g-40);
		if(g1<0) g1=0;
	}
	rand=(HAL_RNG_GetRandomNumber(&hrng))%2;
	if(rand==0)
	{
		b1=(color_b+40);
		if(b1>255) b1=255;
	}
	else
	{
		b1=(color_b-40);
		if(b1<0) b1=0;
	}
	
	j = (HAL_RNG_GetRandomNumber(&hrng))%16;
	
	led_set_color(j, r1, g1, b1);
	//led_set_color(i, 91, 113, 102);
	led_display();
}

void colorBlindGame(void)
{
	/*for(uint8_t i=0;i<10;i++)
	{
		randomColor();
	}*/
	
	pressed=0;
	if(point<=4) colorBlindLevel0();
	else if(point<8) colorBlindLevel1();
	else colorBlindLevel2();
	
	char temp[100];
	sprintf(temp, "\n\n\rr = %d, g = %d, b = %d\n\rr1 = %d, g1 = %d, b1 = %d\n\rj = %d", color_r,color_g,color_b,r1,g1,b1,j);
	while(__HAL_UART_GET_FLAG(&huart3, UART_FLAG_TC)==RESET){}
	HAL_UART_Transmit(&huart3, (uint8_t *) temp, strlen(temp), 100);
	
	millisec=0;
	while(millisec<10000)
	{
		if(j==0)
		{
			if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3)==1)
			{
				point++;
				pressed=1;
				/*char tmp[100];
				sprintf(tmp, "\n\rpoint = %d", point);
				while(__HAL_UART_GET_FLAG(&huart3, UART_FLAG_TC)==RESET){}
				HAL_UART_Transmit(&huart3, (uint8_t *) tmp, strlen(tmp), 100);
				HAL_Delay(100);*/
				
				break;
			}
			else if(
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_5)==1 ||
			HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_7)==1 ||
			HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_15)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_12)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_13)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_15)==1 ||
			HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_6)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_11)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_10)==1 ||
			HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_15)==1 ||
			HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_14)==1 ||
			HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_0)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)==1 ||
			HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==1 ||
			HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_2)==1)
			{
				millisec+=2000;
				HAL_Delay(300);
			}
		}
		else if(j==1)
		{
			if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_5)==1)
			{
				point++;
				pressed=1;
				/*char tmp[100];
				sprintf(tmp, "\n\rpoint = %d", point);
				while(__HAL_UART_GET_FLAG(&huart3, UART_FLAG_TC)==RESET){}
				HAL_UART_Transmit(&huart3, (uint8_t *) tmp, strlen(tmp), 100);
				HAL_Delay(200);*/
				
				break;
			}
			else if(
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3)==1 ||
			HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_7)==1 ||
			HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_15)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_12)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_13)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_15)==1 ||
			HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_6)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_11)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_10)==1 ||
			HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_15)==1 ||
			HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_14)==1 ||
			HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_0)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)==1 ||
			HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==1 ||
			HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_2)==1)
			{
				millisec+=2000;
				HAL_Delay(300);
			}
		}
		else if(j==2)
		{
			if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_7)==1)
			{
				point++;
				pressed=1;
				/*char tmp[100];
				sprintf(tmp, "\n\rpoint = %d", point);
				while(__HAL_UART_GET_FLAG(&huart3, UART_FLAG_TC)==RESET){}
				HAL_UART_Transmit(&huart3, (uint8_t *) tmp, strlen(tmp), 100);
				HAL_Delay(200);*/
				
				break;
			}
			else if(
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_5)==1 ||
			HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_15)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_12)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_13)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_15)==1 ||
			HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_6)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_11)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_10)==1 ||
			HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_15)==1 ||
			HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_14)==1 ||
			HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_0)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)==1 ||
			HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==1 ||
			HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_2)==1)
			{
				millisec+=2000;
				HAL_Delay(300);
			}
		}
		else if(j==3)
		{
			if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_15)==1)
			{
				point++;
				pressed=1;
				/*char tmp[100];
				sprintf(tmp, "\n\rpoint = %d", point);
				while(__HAL_UART_GET_FLAG(&huart3, UART_FLAG_TC)==RESET){}
				HAL_UART_Transmit(&huart3, (uint8_t *) tmp, strlen(tmp), 100);
				HAL_Delay(200);*/
				
				break;
			}
			else if(
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_5)==1 ||
			HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_7)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_12)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_13)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_15)==1 ||
			HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_6)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_11)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_10)==1 ||
			HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_15)==1 ||
			HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_14)==1 ||
			HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_0)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)==1 ||
			HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==1 ||
			HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_2)==1)
			{
				millisec+=2000;
				HAL_Delay(300);
			}
		}
		else if(j==4)
		{
			if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_12)==1)
			{
				point++;
				pressed=1;
			
				/*char tmp[100];
				sprintf(tmp, "\n\rpoint = %d", point);
				while(__HAL_UART_GET_FLAG(&huart3, UART_FLAG_TC)==RESET){}
				HAL_UART_Transmit(&huart3, (uint8_t *) tmp, strlen(tmp), 100);
				HAL_Delay(200);*/
				
				break;
			}
			else if(
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_5)==1 ||
			HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_7)==1 ||
			HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_15)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_12)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_13)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_15)==1 ||
			HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_6)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_11)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_10)==1 ||
			HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_15)==1 ||
			HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_14)==1 ||
			HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_0)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)==1 ||
			HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==1 ||
			HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_2)==1)
			{
				millisec+=2000;
				HAL_Delay(300);
			}
		}
		else if(j==5)
		{
			if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_13)==1)
			{
				point++;
				pressed=1;
			
				/*char tmp[100];
				sprintf(tmp, "\n\rpoint = %d", point);
				while(__HAL_UART_GET_FLAG(&huart3, UART_FLAG_TC)==RESET){}
				HAL_UART_Transmit(&huart3, (uint8_t *) tmp, strlen(tmp), 100);
				HAL_Delay(200);*/
				
				break;
			}
			else if(
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_5)==1 ||
			HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_7)==1 ||
			HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_15)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_12)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_15)==1 ||
			HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_6)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_11)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_10)==1 ||
			HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_15)==1 ||
			HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_14)==1 ||
			HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_0)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)==1 ||
			HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==1 ||
			HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_2)==1)
			{
				millisec+=2000;
				HAL_Delay(300);
			}
		}
		else if(j==6)
		{
			if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_15)==1)
			{
				point++;
				pressed=1;
			
				/*char tmp[100];
				sprintf(tmp, "\n\rpoint = %d", point);
				while(__HAL_UART_GET_FLAG(&huart3, UART_FLAG_TC)==RESET){}
				HAL_UART_Transmit(&huart3, (uint8_t *) tmp, strlen(tmp), 100);
				HAL_Delay(200);*/
				
				break;
			}
			else if(
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_5)==1 ||
			HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_7)==1 ||
			HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_15)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_12)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_13)==1 ||
			HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_6)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_11)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_10)==1 ||
			HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_15)==1 ||
			HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_14)==1 ||
			HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_0)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)==1 ||
			HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==1 ||
			HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_2)==1)
			{
				millisec+=2000;
				HAL_Delay(300);
			}
		}
		else if(j==7)
		{
			if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_6)==1)
			{
				point++;
				pressed=1;
			
				/*char tmp[100];
				sprintf(tmp, "\n\rpoint = %d", point);
				while(__HAL_UART_GET_FLAG(&huart3, UART_FLAG_TC)==RESET){}
				HAL_UART_Transmit(&huart3, (uint8_t *) tmp, strlen(tmp), 100);
				HAL_Delay(200);*/
				
				break;
			}
			else if(
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_5)==1 ||
			HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_7)==1 ||
			HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_15)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_12)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_13)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_15)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_11)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_10)==1 ||
			HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_15)==1 ||
			HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_14)==1 ||
			HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_0)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)==1 ||
			HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==1 ||
			HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_2)==1)
			{
				millisec+=2000;
				HAL_Delay(300);
			}
		}
		else if(j==8)
		{
			if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_11)==1)
			{
				point++;
				pressed=1;
				/*char tmp[100];
				sprintf(tmp, "\n\rpoint = %d", point);
				while(__HAL_UART_GET_FLAG(&huart3, UART_FLAG_TC)==RESET){}
				HAL_UART_Transmit(&huart3, (uint8_t *) tmp, strlen(tmp), 100);
				HAL_Delay(200);*/
				
				break;
			}
			else if(
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_5)==1 ||
			HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_7)==1 ||
			HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_15)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_12)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_13)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_15)==1 ||
			HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_6)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_10)==1 ||
			HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_15)==1 ||
			HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_14)==1 ||
			HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_0)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)==1 ||
			HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==1 ||
			HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_2)==1)
			{
				millisec+=2000;
				HAL_Delay(300);
			}
		}
		else if(j==9)
		{
			if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_10)==1)
			{
				point++;
				pressed=1;
				/*char tmp[100];
				sprintf(tmp, "\n\rpoint = %d", point);
				while(__HAL_UART_GET_FLAG(&huart3, UART_FLAG_TC)==RESET){}
				HAL_UART_Transmit(&huart3, (uint8_t *) tmp, strlen(tmp), 100);
				HAL_Delay(200);*/
				
				break;
			}
			else if(
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_5)==1 ||
			HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_7)==1 ||
			HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_15)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_12)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_13)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_15)==1 ||
			HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_6)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_11)==1 ||
			HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_15)==1 ||
			HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_14)==1 ||
			HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_0)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)==1 ||
			HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==1 ||
			HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_2)==1)
			{
				millisec+=2000;
				HAL_Delay(300);
			}
		}
		else if(j==10)
		{
			if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_15)==1)
			{
				point++;
				pressed=1;
				/*char tmp[100];
				sprintf(tmp, "\n\rpoint = %d", point);
				while(__HAL_UART_GET_FLAG(&huart3, UART_FLAG_TC)==RESET){}
				HAL_UART_Transmit(&huart3, (uint8_t *) tmp, strlen(tmp), 100);
				HAL_Delay(200);*/
				
				break;
			}
			else if(
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_5)==1 ||
			HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_7)==1 ||
			HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_15)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_12)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_13)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_15)==1 ||
			HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_6)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_11)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_10)==1 ||
			HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_14)==1 ||
			HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_0)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)==1 ||
			HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==1 ||
			HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_2)==1)
			{
				millisec+=2000;
				HAL_Delay(300);
			}
		}
		else if(j==11)
		{
			if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_14)==1)
			{
				point++;
				pressed=1;
				/*char tmp[100];
				sprintf(tmp, "\n\rpoint = %d", point);
				while(__HAL_UART_GET_FLAG(&huart3, UART_FLAG_TC)==RESET){}
				HAL_UART_Transmit(&huart3, (uint8_t *) tmp, strlen(tmp), 100);
				HAL_Delay(200);*/
				
				break;
			}
			else if(
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_5)==1 ||
			HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_7)==1 ||
			HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_15)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_12)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_13)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_15)==1 ||
			HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_6)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_11)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_10)==1 ||
			HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_15)==1 ||
			HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_0)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)==1 ||
			HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==1 ||
			HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_2)==1)
			{
				millisec+=2000;
				HAL_Delay(300);
			}
		}
		else if(j==12)
		{
			if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_0)==1)
			{
				point++;
				pressed=1;
				/*char tmp[100];
				sprintf(tmp, "\n\rpoint = %d", point);
				while(__HAL_UART_GET_FLAG(&huart3, UART_FLAG_TC)==RESET){}
				HAL_UART_Transmit(&huart3, (uint8_t *) tmp, strlen(tmp), 100);
				HAL_Delay(200);*/
				
				break;
			}
			else if(
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_5)==1 ||
			HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_7)==1 ||
			HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_15)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_12)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_13)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_15)==1 ||
			HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_6)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_11)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_10)==1 ||
			HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_15)==1 ||
			HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_14)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)==1 ||
			HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==1 ||
			HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_2)==1)
			{
				millisec+=2000;
				HAL_Delay(300);
			}
		}
		else if(j==13)
		{
			if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)==1)
			{
				point++;
				pressed=1;
				/*char tmp[100];
				sprintf(tmp, "\n\rpoint = %d", point);
				while(__HAL_UART_GET_FLAG(&huart3, UART_FLAG_TC)==RESET){}
				HAL_UART_Transmit(&huart3, (uint8_t *) tmp, strlen(tmp), 100);
				HAL_Delay(200);*/
				
				break;
			}
			else if(
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_5)==1 ||
			HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_7)==1 ||
			HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_15)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_12)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_13)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_15)==1 ||
			HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_6)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_11)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_10)==1 ||
			HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_15)==1 ||
			HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_14)==1 ||
			HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_0)==1 ||
			HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==1 ||
			HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_2)==1)
			{
				millisec+=2000;
				HAL_Delay(300);
			}
		}
		else if(j==14)
		{
			if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==1)
			{
				point++;
				pressed=1;
				/*char tmp[100];
				sprintf(tmp, "\n\rpoint = %d", point);
				while(__HAL_UART_GET_FLAG(&huart3, UART_FLAG_TC)==RESET){}
				HAL_UART_Transmit(&huart3, (uint8_t *) tmp, strlen(tmp), 100);
				HAL_Delay(200);*/
				
				break;
			}
			else if(
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_5)==1 ||
			HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_7)==1 ||
			HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_15)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_12)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_13)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_15)==1 ||
			HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_6)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_11)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_10)==1 ||
			HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_15)==1 ||
			HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_14)==1 ||
			HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_0)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)==1 ||
			HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_2)==1)
			{
				millisec+=2000;
				HAL_Delay(300);
			}
		}
		else if(j==15)
		{
			if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_2)==1)
			{
				point++;
				pressed=1;
				/*char tmp[100];
				sprintf(tmp, "\n\rpoint = %d", point);
				while(__HAL_UART_GET_FLAG(&huart3, UART_FLAG_TC)==RESET){}
				HAL_UART_Transmit(&huart3, (uint8_t *) tmp, strlen(tmp), 100);
				HAL_Delay(200);*/
				
				break;
			}
			else if(
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_5)==1 ||
			HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_7)==1 ||
			HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_15)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_12)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_13)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_15)==1 ||
			HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_6)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_11)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_10)==1 ||
			HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_15)==1 ||
			HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_14)==1 ||
			HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_0)==1 ||
			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)==1 ||
			HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==1)
			{
				millisec+=2000;
				HAL_Delay(300);
			}
		}
	}
}

void delay(uint32_t time)
{
	millisec=0;
			while(1)
			{
				if(millisec>time)
				{
					break;
				}
			}
}
void displayTime (uint32_t num )
{
	  char temp[50];
	  sprintf(temp,"\r%d",(num/1000)%60);
	//  sprintf(temp,"\r%d : %d",num/60000,(num/1000)%60);
		while (__HAL_UART_GET_FLAG(&huart3,UART_FLAG_TC)==RESET){}
		HAL_UART_Transmit(&huart3, (uint8_t*)temp, strlen(temp),1000);
	//  HAL_Delay(400);
		
}
void initWSLED(void) 
{
	led_init();
	led_set_color_all(0x0F, 0x0F, 0x0F);  
	led_display();
}
uint8_t randomNumber(uint8_t lower, uint8_t upper) 
{
	return (HAL_RNG_GetRandomNumber(&hrng)% ((upper - lower + 1)) + lower);
}
void randomColor(void) 
{
	uint16_t i;
	uint8_t color_r, color_g, color_b;
	for (i=0;i < 16; i++) {
		color_r = randomNumber(0, 255);
		color_g = randomNumber(0, 255);
		color_b = randomNumber(0, 255);
		led_set_color(i, color_r, color_g, color_b);
	}
	led_display();
	HAL_Delay(100);
}
void setRandomColor(uint8_t index) 
{
	uint8_t color_r, color_g, color_b;
		color_r = randomNumber(0, 255);
		color_g = randomNumber(0, 255);
		color_b = randomNumber(0, 255);
		led_set_color(index, color_r, color_g, color_b);
	  led_display();
	  HAL_Delay(500);
}
void closeLED(void)
{
	led_set_color_all(0, 0, 0);
	led_display();
}
void setCloseLED(uint8_t index)
{
	led_set_color(index, 0,0,0);
	led_display();
}

	
void colorsMatching()
{ 
	HAL_UART_Transmit(&huart3, (unsigned char*)"\n\rStart!",10, 100);
	char tempPrint[50];
	
	uint8_t tempRGB[16][3],tempWrite[16][3];
	int sw[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	
	int rann=0;
  uint8_t tempRandom[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	
	int pair[8]={0,0,0,0,0,0,0,0};
	uint8_t p1[2]={0,0},p2[2]={0,0},p3[2]={0,0},p4[2]={0,0},p5[2]={0,0},p6[2]={0,0},p7[2]={0,0},p8[2]={0,0};
	
	int checkStart =0;
	int checkClose=0;
	int checkPress=0;

	//set color
	for(uint8_t i=0;i<16;i++)
		{
			for(uint8_t j=0;i<2;i++){tempRGB[i][0]=255;tempRGB[i][1]=0;tempRGB[i][2]=0;}       //red 255 0 0
			for(uint8_t j=2;i<4;i++){tempRGB[i][0]=0;tempRGB[i][1]=255;tempRGB[i][2]=0;}       //green 0 255 0
			for(uint8_t j=4;i<6;i++){tempRGB[i][0]=0;tempRGB[i][1]=0;tempRGB[i][2]=255;}       //blue 0 0 255
		  for(uint8_t j=6;i<8;i++){tempRGB[i][0]=255;tempRGB[i][1]=255;tempRGB[i][2]=0;}     //yellow 255 255 0
			for(uint8_t j=8;i<10;i++){tempRGB[i][0]=255;tempRGB[i][1]=0;tempRGB[i][2]=255;}    //pink 255 0 255
			for(uint8_t j=10;i<12;i++){tempRGB[i][0]=255;tempRGB[i][1]=50;tempRGB[i][2]=0;}   //orenge 255 128 0
			for(uint8_t j=12;i<14;i++){tempRGB[i][0]=0;tempRGB[i][1]=255;tempRGB[i][2]=255;} //sky blue 153 255 255
			for(uint8_t j=14;i<16;i++){tempRGB[i][0]=255;tempRGB[i][1]=255;tempRGB[i][2]=255;} //white 255 255 255
		}	
	//wait...
	for(uint8_t i=0;i<10;i++)
	{
		randomColor();
	}
	
	//start...
  while(checkStart==0)
	{
		if(checkClose==1)
		{
			checkStart=1;
			break;
		}
		//random 0-15
		for (uint8_t i=0;i<16;i++)
			{
					tempRandom[i]=0;
			}
			for (uint8_t i=0;i<16;i++)
			{
				do{
	        rann = randomNumber(0,15);
	        if(tempRandom[rann]==0)
	         {
	            tempRandom[rann]=i;
	            break;
	         }
					}while(1);
			}
		// get value for random index
		 for (uint8_t i=0;i<16;i++)
			{
				tempWrite[i][0]=tempRGB[tempRandom[i]][0];
				tempWrite[i][1]=tempRGB[tempRandom[i]][1];
				tempWrite[i][2]=tempRGB[tempRandom[i]][2];
			}
		// check pair
		for(uint8_t i=0;i<16;i++)
				{
					if(tempRandom[i]==0){p1[0]=i;}
					else if (tempRandom[i]==1){p1[1]=i;}
					else if (tempRandom[i]==2){p2[0]=i;}
					else if (tempRandom[i]==3){p2[1]=i;}
					else if (tempRandom[i]==4){p3[0]=i;}
					else if (tempRandom[i]==5){p3[1]=i;}
					else if (tempRandom[i]==6){p4[0]=i;}
					else if (tempRandom[i]==7){p4[1]=i;}
					else if (tempRandom[i]==8){p5[0]=i;}
					else if (tempRandom[i]==9){p5[1]=i;}
					else if (tempRandom[i]==10){p6[0]=i;}
					else if (tempRandom[i]==11){p6[1]=i;}
					else if (tempRandom[i]==12){p7[0]=i;}
					else if (tempRandom[i]==13){p7[1]=i;}
					else if (tempRandom[i]==14){p8[0]=i;}
				  else if (tempRandom[i]==15){p8[1]=i;}
				}
		//Remember...
		for(uint8_t i=0;i<16;i++)
				{
					led_set_color(i,tempWrite[i][0],tempWrite[i][1],tempWrite[i][2]);
					led_display();
				}
		while(checkClose==0)
		{
			delay(3500);
			
			closeLED();	//LED close --> you play
			millisec=0;
			while(1)
			{
				displayTime(millisec);
				if(millisec>40000&&score==0) // 1 round 40 s
				{
					led_set_color_all(255,0,0);
					led_display();
					HAL_UART_Transmit(&huart3, (uint8_t*)"\r\n->TIME OUT",12,1000);
					HAL_Delay(3000);
					score=0;
					checkClose =1; // if user press "play again" else LED read
					checkStart=1;
					checkTimeOut=1;
					break;
				}
				else if(millisec>35000&&score==1) // 2 round 35 s
				{
					led_set_color_all(255,0,0);
					led_display();
					HAL_UART_Transmit(&huart3, (uint8_t*)"\r\n->TIME OUT",12,1000);
					HAL_Delay(3000);
					score=0;
					checkClose =1;// if user press "play again" else LED read
					break;
				}
				else if(millisec>30000&&score==2) // 3 round 30 s
				{
					led_set_color_all(255,0,0);
					led_display();
					HAL_UART_Transmit(&huart3, (uint8_t*)"\r\n->TIME OUT",12,1000);
					HAL_Delay(3000);
					score=0;
					checkClose =1;// if user press "play again" else LED read
					break;
				}
				else if(millisec>25000&&score>2) // 4 round 15 s
				{
					led_set_color_all(255,0,0);
					led_display();
					HAL_UART_Transmit(&huart3, (uint8_t*)"\r\n->TIME OUT",12,1000);
					HAL_Delay(3000);
					score=0;
					checkClose =1;// if user press "play again" else LED read
					break;
				}
			
				//  HAL_UART_Transmit(&huart3, (uint8_t*)"\n->play", 7,1000);
				    if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3)==1)//0
						{
							sw[0]=1;
							led_set_color(0,tempWrite[0][0],tempWrite[0][1],tempWrite[0][2]);
						}
						else if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5)==1)	//1
						{
							sw[1]=1;
							led_set_color(1,tempWrite[1][0],tempWrite[1][1],tempWrite[1][2]);
						}
						else if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_7)==1)//2
						{
							sw[2]=1;
							led_set_color(2,tempWrite[2][0],tempWrite[2][1],tempWrite[2][2]);
						}
						else if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15)==1)//3
						{
							sw[3]=1;
							led_set_color(3,tempWrite[3][0],tempWrite[3][1],tempWrite[3][2]);
						}
						else if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12)==1)//4
						{
							sw[4]=1;
							led_set_color(4,tempWrite[4][0],tempWrite[4][1],tempWrite[4][2]);
						}
						else if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13)==1)//5
						{
							sw[5]=1;
							led_set_color(5,tempWrite[5][0],tempWrite[5][1],tempWrite[5][2]);
						}
						else if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15)==1)//6
						{
							sw[6]=1;
							led_set_color(6,tempWrite[6][0],tempWrite[6][1],tempWrite[6][2]);
						}
						else if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6)==1)//7
						{
							sw[7]=1;
							led_set_color(7,tempWrite[7][0],tempWrite[7][1],tempWrite[7][2]);
						}
				 	  else if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11)==1)//8
						{
							sw[8]=1;
							led_set_color(8,tempWrite[8][0],tempWrite[8][1],tempWrite[8][2]);
						}
						else if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10)==1)	//9
						{
							sw[9]=1;
							led_set_color(9,tempWrite[9][0],tempWrite[9][1],tempWrite[9][2]);
						}
						else if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_15)==1)//10
						{
							sw[10]=1;
							led_set_color(10,tempWrite[10][0],tempWrite[10][1],tempWrite[10][2]);
						}
						else if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_14)==1)//11
						{
							sw[11]=1;
							led_set_color(11,tempWrite[11][0],tempWrite[11][1],tempWrite[11][2]);
						}
						else if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_0)==1)//12
						{
							sw[12]=1;
							led_set_color(12,tempWrite[12][0],tempWrite[12][1],tempWrite[12][2]);
						}
						else if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0)==1)//13
						{
							sw[13]=1;
							led_set_color(13,tempWrite[13][0],tempWrite[13][1],tempWrite[13][2]);
						}
						else if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0)==1)//14
						{
							sw[14]=1;
							led_set_color(14,tempWrite[14][0],tempWrite[14][1],tempWrite[14][2]);
						}
						else if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_2)==1)//15
						{
							sw[15]=1;
							led_set_color(15,tempWrite[15][0],tempWrite[15][1],tempWrite[15][2]);
						}
					
						
				  countTimePress=0;
				  while(sw[0]==1||sw[1]==1||sw[2]==1||sw[3]==1||sw[4]==1||sw[5]==1||sw[6]==1||sw[7]==1||
						sw[8]==1||sw[9]==1||sw[10]==1||sw[11]==1||sw[12]==1||sw[13]==1||sw[14]==1||sw[15]==1)
					{
					//	HAL_UART_Transmit(&huart3, (uint8_t*)"\n->Time...", 10,1000);
						if(countTimePress>300)
						{
								closeLED();
					      for(uint8_t i=0;i<16;i++)
							{
								sw[i]=0;
							}
							break;
						}
						 if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3)==1)//0
						{
							sw[0]=1;
							led_set_color(0,tempWrite[0][0],tempWrite[0][1],tempWrite[0][2]);
						}
						else if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5)==1)	//1
						{
							sw[1]=1;
							led_set_color(1,tempWrite[1][0],tempWrite[1][1],tempWrite[1][2]);
						}
						else if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_7)==1)//2
						{
							sw[2]=1;
							led_set_color(2,tempWrite[2][0],tempWrite[2][1],tempWrite[2][2]);
						}
						else if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15)==1)//3
						{
							sw[3]=1;
							led_set_color(3,tempWrite[3][0],tempWrite[3][1],tempWrite[3][2]);
						}
						else if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12)==1)//4
						{
							sw[4]=1;
							led_set_color(4,tempWrite[4][0],tempWrite[4][1],tempWrite[4][2]);
						}
						else if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13)==1)//5
						{
							sw[5]=1;
							led_set_color(5,tempWrite[5][0],tempWrite[5][1],tempWrite[5][2]);
						}
						else if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15)==1)//6
						{
							sw[6]=1;
							led_set_color(6,tempWrite[6][0],tempWrite[6][1],tempWrite[6][2]);
						}
						else if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6)==1)//7
						{
							sw[7]=1;
							led_set_color(7,tempWrite[7][0],tempWrite[7][1],tempWrite[7][2]);
						}
				 	  else if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11)==1)//8
						{
							sw[8]=1;
							led_set_color(8,tempWrite[8][0],tempWrite[8][1],tempWrite[8][2]);
						}
						else if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10)==1)	//9
						{
							sw[9]=1;
							led_set_color(9,tempWrite[9][0],tempWrite[9][1],tempWrite[9][2]);
						}
						else if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_15)==1)//10
						{
							sw[10]=1;
							led_set_color(10,tempWrite[10][0],tempWrite[10][1],tempWrite[10][2]);
						}
						else if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_14)==1)//11
						{
							sw[11]=1;
							led_set_color(11,tempWrite[11][0],tempWrite[11][1],tempWrite[11][2]);
						}
						else if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_0)==1)//12
						{
							sw[12]=1;
							led_set_color(12,tempWrite[12][0],tempWrite[12][1],tempWrite[12][2]);
						}
						else if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0)==1)//13
						{
							sw[13]=1;
							led_set_color(13,tempWrite[13][0],tempWrite[13][1],tempWrite[13][2]);
						}
						else if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0)==1)//14
						{
							sw[14]=1;
							led_set_color(14,tempWrite[14][0],tempWrite[14][1],tempWrite[14][2]);
						}
						else if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_2)==1)//15
						{
							sw[15]=1;
							led_set_color(15,tempWrite[15][0],tempWrite[15][1],tempWrite[15][2]);
						}
						led_display();
					// check press true
						if(sw[p1[0]]==1&&sw[p1[1]]){pair[0]=1;break;}
						if(sw[p2[0]]==1&&sw[p2[1]]){pair[1]=1;break;}
						if(sw[p3[0]]==1&&sw[p3[1]]){pair[2]=1;break;}
						if(sw[p4[0]]==1&&sw[p4[1]]){pair[3]=1;break;}
						if(sw[p5[0]]==1&&sw[p5[1]]){pair[4]=1;break;}
						if(sw[p6[0]]==1&&sw[p6[1]]){pair[5]=1;break;}
						if(sw[p7[0]]==1&&sw[p7[1]]){pair[6]=1;break;}
						if(sw[p8[0]]==1&&sw[p8[1]]){pair[7]=1;break;}
						
	         // sprintf(tempPrint,"\r%d %d %d %d %d %d %d %d ",sw[0],sw[1],sw[2],sw[3],sw[4],sw[5],sw[6],sw[7]);
	   	     // HAL_UART_Transmit(&huart3, (uint8_t*)tempPrint, strlen(tempPrint),1000);
						
					}//while press :time 1 s
				  
					 
	        /*  sprintf(tempPrint,"\r%d %d %d %d %d %d %d %d ",pair[0],pair[1],pair[2],pair[3],pair[4],pair[5],pair[6],pair[7]);
	   	      HAL_UART_Transmit(&huart3, (uint8_t*)tempPrint, strlen(tempPrint),1000);
					  HAL_UART_Transmit(&huart3, (uint8_t*)"\n", 2,1000);*/
					  //displayTime(millisec);
					//reset switch
						for(uint8_t i=0;i<16;i++)
						{
							sw[i]=0;
						}
						
					// 1 round
		    	if(pair[0]==1&&pair[1]==1&&pair[2]==1&&pair[3]==1&&pair[4]==1&&pair[5]==1&&pair[6]==1&&pair[7]==1)
					{
						score++;
						int rgb = sprintf(tempPrint, "\r\nscore: %d ",score);//0
						HAL_UART_Transmit(&huart3, (unsigned char*) tempPrint, rgb, 100);
						pair[0]=0;pair[1]=0;pair[2]=0;pair[3]=0;
						HAL_Delay(100);
						checkClose =1;// if user press "play again" else LED read
						break;
					}
					//set LED : When --> True pair
					if(pair[0]==1||pair[1]==1||pair[2]==1||pair[3]==1||pair[4]==1||pair[5]==1||pair[6]==1||pair[7]==1)
					{
						if (pair[0]==1)
						{
								led_set_color(p1[0],tempWrite[p1[0]][0],tempWrite[p1[0]][1],tempWrite[p1[0]][2]);
						   	led_set_color(p1[1],tempWrite[p1[1]][0],tempWrite[p1[1]][1],tempWrite[p1[1]][2]);
						  	led_display();
						}
						if (pair[1]==1)
						{
								led_set_color(p2[0],tempWrite[p2[0]][0],tempWrite[p2[0]][1],tempWrite[p2[0]][2]);
						   	led_set_color(p2[1],tempWrite[p2[1]][0],tempWrite[p2[1]][1],tempWrite[p2[1]][2]);
						  	led_display();
						}
						if (pair[2]==1)
						{
								led_set_color(p3[0],tempWrite[p3[0]][0],tempWrite[p3[0]][1],tempWrite[p3[0]][2]);
						   	led_set_color(p3[1],tempWrite[p3[1]][0],tempWrite[p3[1]][1],tempWrite[p3[1]][2]);
						  	led_display();
						}
						if (pair[3]==1)
						{
								led_set_color(p4[0],tempWrite[p4[0]][0],tempWrite[p4[0]][1],tempWrite[p4[0]][2]);
						   	led_set_color(p4[1],tempWrite[p4[1]][0],tempWrite[p4[1]][1],tempWrite[p4[1]][2]);
						  	led_display();
						}
						if (pair[4]==1)
						{
								led_set_color(p5[0],tempWrite[p5[0]][0],tempWrite[p5[0]][1],tempWrite[p5[0]][2]);
						   	led_set_color(p5[1],tempWrite[p5[1]][0],tempWrite[p5[1]][1],tempWrite[p5[1]][2]);
						  	led_display();
						}
						if (pair[5]==1)
						{
								led_set_color(p6[0],tempWrite[p6[0]][0],tempWrite[p6[0]][1],tempWrite[p6[0]][2]);
						   	led_set_color(p6[1],tempWrite[p6[1]][0],tempWrite[p6[1]][1],tempWrite[p6[1]][2]);
						  	led_display();
						}
						if (pair[6]==1)
						{
								led_set_color(p7[0],tempWrite[p7[0]][0],tempWrite[p7[0]][1],tempWrite[p7[0]][2]);
						   	led_set_color(p7[1],tempWrite[p7[1]][0],tempWrite[p7[1]][1],tempWrite[p7[1]][2]);
						  	led_display();
						}
						if (pair[7]==1)
						{
								led_set_color(p8[0],tempWrite[p8[0]][0],tempWrite[p8[0]][1],tempWrite[p8[0]][2]);
						   	led_set_color(p8[1],tempWrite[p8[1]][0],tempWrite[p8[1]][1],tempWrite[p8[1]][2]);
						  	led_display();
						}
					}	
					
			}//while checkPress
		}//while checkClose
	}//while checkStart

		
}

uint16_t Mode1setAllColors(){

    red   = 0*255/100;
    green = 0*255/100;
    blue  = 90*255/100;
	
		led_set_color_all(red,green,blue);
		led_display();
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
