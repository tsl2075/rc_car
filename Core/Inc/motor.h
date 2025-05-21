
#ifndef INC_MOTOR_H_
#define INC_MOTOR_H_

#include <math.h>
#include "main.h"
#include "tim.h"

#define IN1_PIN 	GPIO_PIN_1   // PB1
#define IN2_PIN 	GPIO_PIN_15  // PB15
#define IN3_PIN		GPIO_PIN_14  // PB14
#define IN4_PIN 	GPIO_PIN_13  // PB13

#define IN1_PORT 	GPIOB
#define IN2_PORT 	GPIOB
#define IN3_PORT 	GPIOB
#define IN4_PORT 	GPIOB

extern char current_direction;  // 방향 상태를 외부에서 접근 가능하게

#define IN1_HIGH() HAL_GPIO_WritePin(IN1_PORT, IN1_PIN, GPIO_PIN_SET)
#define IN1_LOW()  HAL_GPIO_WritePin(IN1_PORT, IN1_PIN, GPIO_PIN_RESET)

#define IN2_HIGH() HAL_GPIO_WritePin(IN2_PORT, IN2_PIN, GPIO_PIN_SET)
#define IN2_LOW()  HAL_GPIO_WritePin(IN2_PORT, IN2_PIN, GPIO_PIN_RESET)

#define IN3_HIGH() HAL_GPIO_WritePin(IN3_PORT, IN3_PIN, GPIO_PIN_SET)
#define IN3_LOW()  HAL_GPIO_WritePin(IN3_PORT, IN3_PIN, GPIO_PIN_RESET)

#define IN4_HIGH() HAL_GPIO_WritePin(IN4_PORT, IN4_PIN, GPIO_PIN_SET)
#define IN4_LOW()  HAL_GPIO_WritePin(IN4_PORT, IN4_PIN, GPIO_PIN_RESET)

void Motor_Init();
void SetMotorSpeed(uint16_t duty);


void Set_Direction(char dir);
void Set_Speed(int speed_percent);


#endif /* INC_MOTOR_H_ */
