/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include<stdbool.h>
#include "input.h"
#include "kernel.h"
#define PADDING 2
#define STRIDE 1
#define MAX_LINE_LEN 100
#define OUTPUT_HEIGHT I_H-K_H+1
#define OUTPUT_WIDTH I_W-K_W+1
#define OUTPUT_DEPTH 3
#define Erat 10000
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
TIM_HandleTypeDef htim16;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM16_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
//void padded_set(int input[I_C][I_H][I_W], int output[I_C][I_H+2*PADDING][I_W+2*PADDING]){
//
//    int output_height = I_H+2*PADDING;
//    int output_width = I_W+2*PADDING;
//    int input_channels = I_C;
//
//    int array[I_C][I_H+2*PADDING][I_W+2*PADDING];
//
//    for (int i = 0; i < output_height; i++) {
//        for (int j = 0; j < output_width; j++) {
//            for (int c = 0; c < input_channels; c++) {
//                if(i<PADDING || j<PADDING || i>=output_height-PADDING || j>=output_width-PADDING){
//                    output[c][i][j] = 0;
//                }
//                else{
//                    output[c][i][j] = input[c][i-PADDING][j-PADDING];
//                }
//            }
//        }
//    }
//}
//
//void custom_convolution(int input[I_C][I_H][I_W],int kernel[K_C][K_H][K_W],int output[OUTPUT_H][OUTPUT_W]) {
//    // write a convolution function that takes in an input tensor and a kernel tensor
//
//    int k=0,l=0;
//
//    assert(STRIDE > 0);
//    assert(PADDING >= 0);
//
//    // apply padding to the input table if necessary without torch library
//
//    int temp[I_C][I_H+2*PADDING][I_W+2*PADDING];
//    if(PADDING>0){
//        padded_set(input,temp);
//    }
//
//    //set the output tensor to zero
//
//    for(int i=0;i<OUTPUT_H;i++){
//        for(int j=0;j<OUTPUT_W;j++){
//            output[i][j] = 0;
//        }
//    }
//
//    // loop over the output pixels
//    for (int i = 0; i < OUTPUT_H; i++) {
//        for (int j = 0; j < OUTPUT_W; j++) {
//            // loop over the input channels
//            for (int c = 0; c < I_C; c++) {
//                //loop over the kernel pixels
//                for (int m = 0; m < K_H; m++) {
//                    for (int n = 0; n < K_W; n++) {
//                        k = i*STRIDE + m;
//                        l = j*STRIDE + n;
//                        if(PADDING>0){
//                            output[i][j] += temp[c][k][l] * kernel[c][m][n];
//                        }
//                        else{
//                            output[i][j] += input[c][k][l] * kernel[c][m][n];
//                        }
//                    }
//                }
//            }
//
//        }
//    }
//}


void erat(){
	int n = Erat;
	bool prime[Erat];
	for (int i = 0; i <= n; i++) {
        prime[i] = false;
    }
	for (int i = 0; i <= n; i++) {
        if (prime[i] == true) {
        	uart_buf_len = sprintf(uart_buf, "ERROR\r\n");
    	HAL_UART_Transmit(&huart2, (uint8_t *)uart_buf, uart_buf_len, 100);
        }
    }
	for (int i = 0; i <= n; i++) {
        prime[i] = true;
    }
	for (int p = 2; p * p <= n; p++) {
		if (prime[p] == true) {
			for (int i = p * p; i <= n; i += p)
				prime[i] = false;
	        }
	}

    for (int p = 2; p <= n; p++) {
        if (prime[p]){
        	uart_buf_len = sprintf(uart_buf, "%d \r\n", p);
        	HAL_UART_Transmit(&huart2, (uint8_t *)uart_buf, uart_buf_len, 100);

    }}


}

int convolut(){
	int i, j, k, m=0, n=0, l;
	int sum;
	for (i = 0; i < OUTPUT_DEPTH; i++) {
		for (j = 0; j < OUTPUT_HEIGHT; j++) {
			for (k = 0; k < OUTPUT_WIDTH; k++) {
				sum = 0;
				for (l = 0; l < K_C; l++) {
					for (m = 0; m < K_H; m++) {
						for (n = 0; n < K_W; n++) {
							sum += input[l][i+m][j+n] * kernel[l][m][n];
						}
					}
				}
				output[i][j][k] = sum;
			}
		}
	}
	//Code for showing output to help the debbuging

//			 for (int depth = 0; depth < OUTPUT_DEPTH; depth++) {
//			        for (int row = 0; row < OUTPUT_HEIGHT; row++) {
//			            for (int col = 0; col < OUTPUT_WIDTH; col++) {
//			                // Format the output string for each element
//			                uart_buf_len = sprintf(uart_buf, "Matrix[%d][%d][%d]:  \t %d us\r\n", row, col, depth, output[depth][col][row]);
//			                // Transmit the formatted string over UART
//			                HAL_UART_Transmit(&huart2, (uint8_t *)uart_buf, uart_buf_len, 100);
//			                			            }
//			        }
//			    }

	sum+=output[2][1][1];
}


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	char uart_buf[50];
	int uart_buf_len;
	uint16_t timer_val;

  /* USER CODE END 1 */

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
  MX_USART2_UART_Init();
  MX_TIM16_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start(&htim16);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  int flag=0;
  HAL_UART_Transmit(&huart2, (uint8_t *)"EISAI MALAKAS\r\n", strlen("EISAI MALAKAS\r\n"), 100);

  while (1)
  {
	  timer_val = 0;
	  float output[OUTPUT_DEPTH][OUTPUT_HEIGHT][OUTPUT_WIDTH];
	if (flag==0) {
		//we make the scaf in order to stop generating output
		for (int scaf = 0; scaf<10; scaf++) {
			timer_val = __HAL_TIM_GET_COUNTER(&htim16);

			HAL_Delay(50);
			//Choose function one to run the benchmark
			//Sometimes you need to make the functions into macros for erat
		//	erat();
			int sum = convolut();
			timer_val = __HAL_TIM_GET_COUNTER(&htim16)-timer_val;
			uart_buf_len = sprintf(uart_buf, "%u \t us\r\n", timer_val );
			HAL_UART_Transmit(&huart2, (uint8_t *)uart_buf, uart_buf_len, 100);
		}
		flag=1;
	}
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

  }
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM16 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM16_Init(void)
{

  /* USER CODE BEGIN TIM16_Init 0 */

  /* USER CODE END TIM16_Init 0 */

  /* USER CODE BEGIN TIM16_Init 1 */

  /* USER CODE END TIM16_Init 1 */
  htim16.Instance = TIM16;
  htim16.Init.Prescaler = 160-1;
  htim16.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim16.Init.Period = 65536-1;
  htim16.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim16.Init.RepetitionCounter = 0;
  htim16.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim16) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM16_Init 2 */

  /* USER CODE END TIM16_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 38400;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
