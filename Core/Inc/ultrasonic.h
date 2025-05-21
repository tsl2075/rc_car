
#ifndef INC_ULTRASONIC_H_
#define INC_ULTRASONIC_H_
#define AVG_WINDOW 5


#include "main.h"
#include "tim.h"
#include "delay.h"
#include <stdio.h>
#include "motor.h"


#define NUM_SENSORS 3


// 센서 구조체 정의
typedef struct
{
    GPIO_TypeDef* trig_port;
    uint16_t trig_pin;
    TIM_HandleTypeDef* htim;
    uint32_t channel;
    uint16_t ic_val1;
    uint16_t ic_val2;
    uint8_t capture_flag;
    uint16_t echo_time;
    uint8_t distance;
} UltrasonicSensor_t;


extern UltrasonicSensor_t sensors[NUM_SENSORS];

void TriggerSensor(UltrasonicSensor_t* sensor);
uint8_t GetDistance(uint8_t index);
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);


#endif /* INC_ULTRASONIC_H_ */
