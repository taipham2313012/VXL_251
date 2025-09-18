/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2025 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

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
	void display7SEG1(int num) {
		const uint8_t segmentMap[10][7] = {
			{1,1,1,1,1,1,0}, // 0
			{0,1,1,0,0,0,0}, // 1
			{1,1,0,1,1,0,1}, // 2
			{1,1,1,1,0,0,1}, // 3
			{0,1,1,0,0,1,1}, // 4
			{1,0,1,1,0,1,1}, // 5
			{1,0,1,1,1,1,1}, // 6
			{1,1,1,0,0,0,0}, // 7
			{1,1,1,1,1,1,1}, // 8
			{1,1,1,1,0,1,1}  // 9
		};

		if (num < 0 || num > 9) return; // invalid input

		HAL_GPIO_WritePin(LED_1A_GPIO_Port, LED_1A_Pin, segmentMap[num][0] ? GPIO_PIN_RESET : GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED_1B_GPIO_Port, LED_1B_Pin, segmentMap[num][1] ? GPIO_PIN_RESET : GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED_1C_GPIO_Port, LED_1C_Pin, segmentMap[num][2] ? GPIO_PIN_RESET : GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED_1D_GPIO_Port, LED_1D_Pin, segmentMap[num][3] ? GPIO_PIN_RESET : GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED_1E_GPIO_Port, LED_1E_Pin, segmentMap[num][4] ? GPIO_PIN_RESET : GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED_1F_GPIO_Port, LED_1F_Pin, segmentMap[num][5] ? GPIO_PIN_RESET : GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED_1G_GPIO_Port, LED_1G_Pin, segmentMap[num][6] ? GPIO_PIN_RESET : GPIO_PIN_SET);

	}
	void display7SEG2(int num) {
		const uint8_t segmentMap[10][7] = {
			{1,1,1,1,1,1,0}, // 0
			{0,1,1,0,0,0,0}, // 1
			{1,1,0,1,1,0,1}, // 2
			{1,1,1,1,0,0,1}, // 3
			{0,1,1,0,0,1,1}, // 4
			{1,0,1,1,0,1,1}, // 5
			{1,0,1,1,1,1,1}, // 6
			{1,1,1,0,0,0,0}, // 7
			{1,1,1,1,1,1,1}, // 8
			{1,1,1,1,0,1,1}  // 9
		};

		if (num < 0 || num > 9) return; // invalid input

		HAL_GPIO_WritePin(LED_2A_GPIO_Port, LED_2A_Pin, segmentMap[num][0] ? GPIO_PIN_RESET : GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED_2B_GPIO_Port, LED_2B_Pin, segmentMap[num][1] ? GPIO_PIN_RESET : GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED_2C_GPIO_Port, LED_2C_Pin, segmentMap[num][2] ? GPIO_PIN_RESET : GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED_2D_GPIO_Port, LED_2D_Pin, segmentMap[num][3] ? GPIO_PIN_RESET : GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED_2E_GPIO_Port, LED_2E_Pin, segmentMap[num][4] ? GPIO_PIN_RESET : GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED_2F_GPIO_Port, LED_2F_Pin, segmentMap[num][5] ? GPIO_PIN_RESET : GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED_2G_GPIO_Port, LED_2G_Pin, segmentMap[num][6] ? GPIO_PIN_RESET : GPIO_PIN_SET);

	}
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
  /* USER CODE BEGIN 2 */
  int RED_TIME    = 9;
  int GREEN_TIME  = 5;
  int YELLOW_TIME = RED_TIME-GREEN_TIME;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  //RED1 GREEN2 3S
	  HAL_GPIO_WritePin(LED_RED1_GPIO_Port, LED_RED1_Pin, GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(LED_YELLOW1_GPIO_Port, LED_YELLOW1_Pin, GPIO_PIN_SET);
	  HAL_GPIO_WritePin(LED_GREEN1_GPIO_Port, LED_GREEN1_Pin, GPIO_PIN_SET);

	  HAL_GPIO_WritePin(LED_RED2_GPIO_Port, LED_RED2_Pin, GPIO_PIN_SET);
	  HAL_GPIO_WritePin(LED_YELLOW2_GPIO_Port, LED_YELLOW2_Pin, GPIO_PIN_SET);
	  HAL_GPIO_WritePin(LED_GREEN2_GPIO_Port, LED_GREEN2_Pin, GPIO_PIN_RESET);
	  for (int t = GREEN_TIME; t > 0; t--) {
		  display7SEG1(t+YELLOW_TIME);
		  display7SEG2(t);
		  HAL_Delay(1000);
	  }
	  //RED1 YELLOW2 2S
	  HAL_GPIO_WritePin(LED_RED1_GPIO_Port, LED_RED1_Pin, GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(LED_YELLOW1_GPIO_Port, LED_YELLOW1_Pin, GPIO_PIN_SET);
	  HAL_GPIO_WritePin(LED_GREEN1_GPIO_Port, LED_GREEN1_Pin, GPIO_PIN_SET);

	  HAL_GPIO_WritePin(LED_RED2_GPIO_Port, LED_RED2_Pin, GPIO_PIN_SET);
	  HAL_GPIO_WritePin(LED_YELLOW2_GPIO_Port, LED_YELLOW2_Pin, GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(LED_GREEN2_GPIO_Port, LED_GREEN2_Pin, GPIO_PIN_SET);
	  for (int t = YELLOW_TIME; t > 0; t--) {
		  display7SEG1(t);
		  display7SEG2(t);
		  HAL_Delay(1000);
	  }
	  //GREEN1 RED2 3S
	  HAL_GPIO_WritePin(LED_RED1_GPIO_Port, LED_RED1_Pin, GPIO_PIN_SET);
	  HAL_GPIO_WritePin(LED_YELLOW1_GPIO_Port, LED_YELLOW1_Pin, GPIO_PIN_SET);
	  HAL_GPIO_WritePin(LED_GREEN1_GPIO_Port, LED_GREEN1_Pin, GPIO_PIN_RESET);

	  HAL_GPIO_WritePin(LED_RED2_GPIO_Port, LED_RED2_Pin, GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(LED_YELLOW2_GPIO_Port, LED_YELLOW2_Pin, GPIO_PIN_SET);
	  HAL_GPIO_WritePin(LED_GREEN2_GPIO_Port, LED_GREEN2_Pin, GPIO_PIN_SET);
	  for (int t = GREEN_TIME; t > 0; t--) {
		  display7SEG1(t);
		  display7SEG2(t+YELLOW_TIME);
		  HAL_Delay(1000);
	  }
	  // YELLOW1 RED2 2S
	  HAL_GPIO_WritePin(LED_RED1_GPIO_Port, LED_RED1_Pin, GPIO_PIN_SET);
	  HAL_GPIO_WritePin(LED_YELLOW1_GPIO_Port, LED_YELLOW1_Pin, GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(LED_GREEN1_GPIO_Port, LED_GREEN1_Pin, GPIO_PIN_SET);

	  HAL_GPIO_WritePin(LED_RED2_GPIO_Port, LED_RED2_Pin, GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(LED_YELLOW2_GPIO_Port, LED_YELLOW2_Pin, GPIO_PIN_SET);
	  HAL_GPIO_WritePin(LED_GREEN2_GPIO_Port, LED_GREEN2_Pin, GPIO_PIN_SET);
	  for (int t = YELLOW_TIME; t > 0; t--) {
		  display7SEG1(t);
		  display7SEG2(t);
		  HAL_Delay(1000);
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
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LED_1A_Pin|LED_1B_Pin|LED_1C_Pin|LED_1D_Pin
                          |LED_1E_Pin|LED_1F_Pin|LED_1G_Pin|LED_RED1_Pin
                          |LED_YELLOW1_Pin|LED_GREEN1_Pin|LED_RED2_Pin|LED_YELLOW2_Pin
                          |LED_GREEN2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LED_2A_Pin|LED_2B_Pin|LED_2C_Pin|LED_2D_Pin
                          |LED_2E_Pin|LED_2F_Pin|LED_2G_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : LED_1A_Pin LED_1B_Pin LED_1C_Pin LED_1D_Pin
                           LED_1E_Pin LED_1F_Pin LED_1G_Pin LED_RED1_Pin
                           LED_YELLOW1_Pin LED_GREEN1_Pin LED_RED2_Pin LED_YELLOW2_Pin
                           LED_GREEN2_Pin */
  GPIO_InitStruct.Pin = LED_1A_Pin|LED_1B_Pin|LED_1C_Pin|LED_1D_Pin
                          |LED_1E_Pin|LED_1F_Pin|LED_1G_Pin|LED_RED1_Pin
                          |LED_YELLOW1_Pin|LED_GREEN1_Pin|LED_RED2_Pin|LED_YELLOW2_Pin
                          |LED_GREEN2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : LED_2A_Pin LED_2B_Pin LED_2C_Pin LED_2D_Pin
                           LED_2E_Pin LED_2F_Pin LED_2G_Pin */
  GPIO_InitStruct.Pin = LED_2A_Pin|LED_2B_Pin|LED_2C_Pin|LED_2D_Pin
                          |LED_2E_Pin|LED_2F_Pin|LED_2G_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
