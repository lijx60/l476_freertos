/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */     
#include "stdio.h"
#include "gpio.h"
  extern KEY_STA key_status;
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
/* USER CODE BEGIN Variables */
  uint8_t  interrupt_flag = 0;
	// uint8_t  raising_flag = 0;
/* USER CODE END Variables */
osThreadId usart_Task1Handle;
osThreadId key_Task2Handle;
osThreadId led_Task3Handle;
osThreadId confirmTask4Handle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
   
/* USER CODE END FunctionPrototypes */

void StartTask1(void const * argument);
void StartTask2(void const * argument);
void StartTask3(void const * argument);
void StartTask4(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];
  
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}                   
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
       
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of usart_Task1 */
  osThreadDef(usart_Task1, StartTask1, osPriorityNormal, 0, 128);
  usart_Task1Handle = osThreadCreate(osThread(usart_Task1), NULL);

  /* definition and creation of key_Task2 */
  osThreadDef(key_Task2, StartTask2, osPriorityIdle, 0, 128);
  key_Task2Handle = osThreadCreate(osThread(key_Task2), NULL);

  /* definition and creation of led_Task3 */
  osThreadDef(led_Task3, StartTask3, osPriorityIdle, 0, 128);
  led_Task3Handle = osThreadCreate(osThread(led_Task3), NULL);

  /* definition and creation of confirmTask4 */
  osThreadDef(confirmTask4, StartTask4, osPriorityIdle, 0, 128);
  confirmTask4Handle = osThreadCreate(osThread(confirmTask4), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartTask1 */
/**
  * @brief  Function implementing the usart_Task1 thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_StartTask1 */
void StartTask1(void const * argument)
{
  /* USER CODE BEGIN StartTask1 */
  /* Infinite loop */
  for(;;)
  {
		printf("task1 is running \r\n");
    osDelay(1000);
  }
  /* USER CODE END StartTask1 */
}

/* USER CODE BEGIN Header_StartTask2 */
/**
* @brief Function implementing the key_Task2 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask2 */
void StartTask2(void const * argument)
{
  /* USER CODE BEGIN StartTask2 */
  /* Infinite loop */
  for(;;)
  {
    
		if( key_status == KEY_ON )
			{
				osThreadSuspend (led_Task3Handle);
				osThreadSuspend (usart_Task1Handle );
		
			}
			else {
				osThreadResume(led_Task3Handle);
				osThreadResume(usart_Task1Handle );
		  }
		osDelay(800);
		
  }
  /* USER CODE END StartTask2 */
}

/* USER CODE BEGIN Header_StartTask3 */
/**
* @brief Function implementing the led_Task3 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask3 */
void StartTask3(void const * argument)
{
  /* USER CODE BEGIN StartTask3 */
  /* Infinite loop */
  for(;;)
  {
		HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
    osDelay(1000);
  }
  /* USER CODE END StartTask3 */
}

/* USER CODE BEGIN Header_StartTask4 */
/**
* @brief Function implementing the confirmTask4 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask4 */
void StartTask4(void const * argument)
{
  /* USER CODE BEGIN StartTask4 */
  /* Infinite loop */
  for(;;)
  {
		if(interrupt_flag == 1)
		{
		 if( (HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin)) == RESET )
			 {  
				 osDelay(30);
	    if( (HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin)) == RESET )
			  {
				   key_status = KEY_ON;
				}
			}
      else if ((HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin)) == SET )			 
			{
				  osDelay(30);
				if( (HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin)) == SET )
			  {
				   key_status = KEY_OFF;
				}
			}
         // key_status = KEY_UNKNOW;
			 
			}
		 
      osDelay(400);
  }
  /* USER CODE END StartTask4 */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
