

#include "motor.h"

char current_direction = 'S';  // 현재 방향을 나타냄

void Motor_Init()
{
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1); // 꼭 먼저 호출
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0); // 초기 듀티 0
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 0);
}


void Set_Direction(char dir)
{
  current_direction = dir; // 현재 방향 저장

    switch (dir)
    {
	case 'Z':
          // 전진
          HAL_GPIO_WritePin(IN1_PORT, IN1_PIN, GPIO_PIN_SET);
          HAL_GPIO_WritePin(IN2_PORT, IN2_PIN, GPIO_PIN_RESET);
          HAL_GPIO_WritePin(IN3_PORT, IN3_PIN, GPIO_PIN_SET);
          HAL_GPIO_WritePin(IN4_PORT, IN4_PIN, GPIO_PIN_RESET);

          break;
        case 'F':
            // 전진
            HAL_GPIO_WritePin(IN1_PORT, IN1_PIN, GPIO_PIN_SET);
            HAL_GPIO_WritePin(IN2_PORT, IN2_PIN, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(IN3_PORT, IN3_PIN, GPIO_PIN_SET);
            HAL_GPIO_WritePin(IN4_PORT, IN4_PIN, GPIO_PIN_RESET);

            break;
        case 'B':
            // 후진
            HAL_GPIO_WritePin(IN1_PORT, IN1_PIN, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(IN2_PORT, IN2_PIN, GPIO_PIN_SET);
            HAL_GPIO_WritePin(IN3_PORT, IN3_PIN, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(IN4_PORT, IN4_PIN, GPIO_PIN_SET);

            break;
        case 'S':
        default:
            // 정지
            HAL_GPIO_WritePin(IN1_PORT, IN1_PIN, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(IN2_PORT, IN2_PIN, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(IN3_PORT, IN3_PIN, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(IN4_PORT, IN4_PIN, GPIO_PIN_RESET);

            break;
        case 'L':
            // 제자리 좌회전
            HAL_GPIO_WritePin(IN1_PORT, IN1_PIN, GPIO_PIN_SET);
            HAL_GPIO_WritePin(IN2_PORT, IN2_PIN, GPIO_PIN_RESET);   // 왼쪽 후진
            HAL_GPIO_WritePin(IN3_PORT, IN3_PIN, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(IN4_PORT, IN4_PIN, GPIO_PIN_SET); // 오른쪽 전진
            break;


        case 'R':
	    // 제자리 우회전
            HAL_GPIO_WritePin(IN1_PORT, IN1_PIN, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(IN2_PORT, IN2_PIN, GPIO_PIN_SET);
            HAL_GPIO_WritePin(IN3_PORT, IN3_PIN, GPIO_PIN_SET);
            HAL_GPIO_WritePin(IN4_PORT, IN4_PIN, GPIO_PIN_RESET);
            break;
    }
}


void Set_Speed(int speed_percent)
{
    if (speed_percent < 0) speed_percent = 0;
    if (speed_percent > 100) speed_percent = 100;

    uint16_t duty = (4999 * speed_percent) / 100;  // PWM Period = 4999
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, duty);
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, duty);
}




