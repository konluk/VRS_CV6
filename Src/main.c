/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
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
#include "usart.h"
#include "gpio.h"


void SystemClock_Config(void);

void process_serial_data(uint8_t ch);

char pole[6];

int main(void)
{
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  SystemClock_Config();

  MX_GPIO_Init();
  MX_USART2_UART_Init();

  USART2_RegisterCallback(process_serial_data);

   while (1)
  {
	  if((LL_GPIO_ReadInputPort(GPIOB) & (1 << 3)) >> 3){
		  LL_USART_TransmitData8(USART2, '1');
	  }else{
		  LL_USART_TransmitData8(USART2, '0');
	  }
	  LL_mDelay(1000);
  }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);

  if(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_0)
  {
  Error_Handler();  
  }
  LL_RCC_HSI_Enable();

   /* Wait till HSI is ready */
  while(LL_RCC_HSI_IsReady() != 1)
  {
    
  }
  LL_RCC_HSI_SetCalibTrimming(16);
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI)
  {
  
  }
  LL_Init1msTick(8000000);
  LL_SYSTICK_SetClkSource(LL_SYSTICK_CLKSOURCE_HCLK);
  LL_SetSystemCoreClock(8000000);
}


void process_serial_data(uint8_t ch)
{
	static uint8_t poc = 0;

	pole[poc] = ch;

	if(poc == 0 && pole[0] != 'l'){memset(pole, 0, 6); poc=0; return;}
	if(poc == 1 && pole[1] != 'e'){memset(pole, 0, 6); poc=0; return;}
	if(poc == 2 && pole[2] != 'd'){memset(pole, 0, 6); poc=0; return;}
	if(poc == 3 && pole[3] != 'O'){memset(pole, 0, 6); poc=0; return;}

	if(poc == 4 && pole[4] == 'N'){memset(pole, 0, 6); poc=0; LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_3); return;}
	if(poc == 4 && pole[4] != 'F'){memset(pole, 0, 6); poc=0; return;}
	if(poc == 5 && pole[5] == 'F'){memset(pole, 0, 6); poc=0; LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_3); return;}

	if(poc == 5){memset(pole, 0, 6); poc=0; return;}

	poc++;
	return;
}


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
void assert_failed(char *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
