/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "ultrasonic.h"
#include "motor.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "i2c.h"
#include "i2c_lcd.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

#define FRONT_SAFE_CM 25  // 앞에 이 거리 이하로 오면 회피
#define SIDE_SAFE_CM  20  // 좌우 회피 판단 기준

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

  extern volatile uint8_t manual_override;
  extern volatile uint32_t last_manual_time;
  volatile char last_turn_dir = 0;


/* USER CODE END Variables */
/* Definitions for UltrasonicTask */
osThreadId_t UltrasonicTaskHandle;
const osThreadAttr_t UltrasonicTask_attributes = {
  .name = "UltrasonicTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for MotorTask */
osThreadId_t MotorTaskHandle;
const osThreadAttr_t MotorTask_attributes = {
  .name = "MotorTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for LcdTask */
osThreadId_t LcdTaskHandle;
const osThreadAttr_t LcdTask_attributes = {
  .name = "LcdTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for ControlTask */
osThreadId_t ControlTaskHandle;
const osThreadAttr_t ControlTask_attributes = {
  .name = "ControlTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartUltrasonicTask(void *argument);
void StartMotorTask(void *argument);
void StartLcdTask(void *argument);
void StartControlTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */


    /* Create the thread(s) */
    osThreadAttr_t ultrasonicTask_attributes = {
      .name = "ultrasonicTask",
      .priority = (osPriority_t) osPriorityNormal,
      .stack_size = 256 * 4
    };
    osThreadNew(StartUltrasonicTask, NULL, &ultrasonicTask_attributes);

    osThreadAttr_t controlTask_attributes = {
      .name = "controlTask",
      .priority = (osPriority_t) osPriorityAboveNormal,
      .stack_size = 128 * 4
    };
    osThreadNew(StartControlTask, NULL, &controlTask_attributes);


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
  /* creation of UltrasonicTask */
  UltrasonicTaskHandle = osThreadNew(StartUltrasonicTask, NULL, &UltrasonicTask_attributes);

  /* creation of MotorTask */
  MotorTaskHandle = osThreadNew(StartMotorTask, NULL, &MotorTask_attributes);

  /* creation of LcdTask */
  LcdTaskHandle = osThreadNew(StartLcdTask, NULL, &LcdTask_attributes);

  /* creation of ControlTask */
  ControlTaskHandle = osThreadNew(StartControlTask, NULL, &ControlTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartUltrasonicTask */
/**
  * @brief  Function implementing the UltrasonicTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartUltrasonicTask */
void StartUltrasonicTask(void *argument)
{
  /* USER CODE BEGIN StartUltrasonicTask */

  uint8_t current_sensor = 0;


  /* Infinite loop */
  for (;;)
  {
      TriggerSensor(&sensors[current_sensor]);
      current_sensor = (current_sensor + 1) % NUM_SENSORS;

      osDelay(40);  // 간섭 방지용 딜레이

      printf("Distances: L:%dcm, F:%dcm, R:%dcm\n",
             GetDistance(0), GetDistance(1), GetDistance(2));
  }
  /* USER CODE END StartUltrasonicTask */
}

/* USER CODE BEGIN Header_StartMotorTask */
/**
* @brief Function implementing the MotorTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartMotorTask */
void StartMotorTask(void *argument)
{
  /* USER CODE BEGIN StartMotorTask */

  extern void Set_Direction(char dir);  // 'F', 'B', 'L', 'R'
  extern void Set_Speed(int speed_percent); // 0~100%
  //extern volatile uint8_t manual_override;
  //i2c_lcd_init();       // LCD 초기화
  /* Infinite loop */
  for(;;)
  {
      //char line1[17];
      //char line2[17];

      //i2c_lcd_init();       // LCD 초기화
      //sprintf(line1, "Mode: %s", manual_override ? "Manual" : "Auto");
      // 수동 제어 중이면 자율주행 정지
      if (manual_override)
	{
	  // 수동 제어 유지 시간 설정 (예: 3000ms)
	  if (HAL_GetTick() - last_manual_time > 5000)
	    {
	      manual_override = 0;
	    }
	  osDelay(40);
	  continue;
	}


      uint8_t left  = GetDistance(0);
      uint8_t front = GetDistance(1);
      uint8_t right = GetDistance(2);

      //  벽에 너무 가까울 때 반대 방향으로 약간 회피
      if (left <= 11)
	{
	  Set_Direction('R');
	  //sprintf(line2, "Drive: Right   ");
	  osDelay(30);
	}
      else if (right <= 11)
	{
	  Set_Direction('L');
	  //sprintf(line2, "Drive: Left    ");
	  osDelay(30);
	}

      if (front < 28)
      {
          // 기존 회피 알고리즘 그대로
          int diff = abs((int)left - (int)right);

          if (diff <= 4)
          {
              if (left < right)
        	{
                  Set_Direction('L');  // 좁은 쪽으로 탈출
                  //sprintf(line2, "Drive: Left    ");
        	}
              else
        	{
                  Set_Direction('R');
                  //sprintf(line2, "Drive: Right   ");
        	}
          }
          else
          {
              if (left < 18 && right < 18)
        	{
                  Set_Direction('B');
        	}
              else if (left > right)
        	{
                  Set_Direction('L');
        	}
              else
        	{
                  Set_Direction('R');
        	}
          }
      }
      else
      {
          // 정상 직진
          Set_Direction('F');
      }
      Set_Speed(84);
      osDelay(30);  // 주기*/



  }



  /* USER CODE END StartMotorTask */
}

/* USER CODE BEGIN Header_StartLcdTask */
/**
* @brief Function implementing the LcdTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartLcdTask */
void StartLcdTask(void *argument)
{
  /* USER CODE BEGIN StartLcdTask */

  extern volatile uint8_t manual_override;
  extern char current_direction;  // 방향 상태를 외부에서 접근 가능하게

  char line1[17];
  char line2[17];

  i2c_lcd_init();  // ❗ 여기서 딱 한 번만 호출


  for(;;)
  {
      sprintf(line1, "Mode: %-10s", manual_override ? "Manual" : "Auto");

       char dir_snapshot = current_direction;

       if (dir_snapshot == 'F' || dir_snapshot == 'Z')
           sprintf(line2, "Drive: Forward ");
       else if (dir_snapshot == 'B')
           sprintf(line2, "Drive: Backward");
       else if (dir_snapshot == 'L')
           sprintf(line2, "Drive: Left    ");
       else if (dir_snapshot == 'R')
           sprintf(line2, "Drive: Right   ");
       else
           sprintf(line2, "Drive: Stop    ");

       move_cursor(0, 0);
       lcd_string(line1);

       move_cursor(1, 0);
       lcd_string(line2);

       osDelay(500);

  }

  /* USER CODE END StartLcdTask */
}

/* USER CODE BEGIN Header_StartControlTask */
/**
* @brief Function implementing the ControlTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartControlTask */
void StartControlTask(void *argument)
{
  /* USER CODE BEGIN StartControlTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartControlTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

