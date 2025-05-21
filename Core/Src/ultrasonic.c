
#include "ultrasonic.h"


uint8_t current_sensor = 0;
uint32_t last_trigger_time = 0;


// 센서 배열 정의
UltrasonicSensor_t sensors[NUM_SENSORS] =
{
    {GPIOB, GPIO_PIN_10, &htim1, TIM_CHANNEL_1},
    {GPIOB, GPIO_PIN_9, &htim2, TIM_CHANNEL_1},
    {GPIOB, GPIO_PIN_8, &htim5, TIM_CHANNEL_2}
};

// 거리 가져오기 함수
uint8_t GetDistance(uint8_t index)
{
    return sensors[index].distance;
}

// 트리거 함수
void TriggerSensor(UltrasonicSensor_t* sensor)
{
    HAL_TIM_IC_Stop_IT(sensor->htim, sensor->channel);
    __HAL_TIM_SET_CAPTUREPOLARITY(sensor->htim, sensor->channel, TIM_INPUTCHANNELPOLARITY_RISING);
    HAL_TIM_IC_Start_IT(sensor->htim, sensor->channel);

    HAL_GPIO_WritePin(sensor->trig_port, sensor->trig_pin, GPIO_PIN_RESET);
    delay_us(1);
    HAL_GPIO_WritePin(sensor->trig_port, sensor->trig_pin, GPIO_PIN_SET);
    delay_us(10);
    HAL_GPIO_WritePin(sensor->trig_port, sensor->trig_pin, GPIO_PIN_RESET);


    sensor->capture_flag = 0;
}



// 인터럽트 콜백 함수
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    for (int i = 0; i < NUM_SENSORS; i++)
    {
        UltrasonicSensor_t* sensor = &sensors[i];
        if (htim->Instance == sensor->htim->Instance)
        {
            if (sensor->capture_flag == 0)
            {
                sensor->ic_val1 = HAL_TIM_ReadCapturedValue(sensor->htim, sensor->channel);
                sensor->capture_flag = 1;
                __HAL_TIM_SET_CAPTUREPOLARITY(sensor->htim, sensor->channel, TIM_INPUTCHANNELPOLARITY_FALLING);
            }
            else
            {
                sensor->ic_val2 = HAL_TIM_ReadCapturedValue(sensor->htim, sensor->channel);
                if (sensor->ic_val2 >= sensor->ic_val1)
                    sensor->echo_time = sensor->ic_val2 - sensor->ic_val1;
                else
                    sensor->echo_time = (0xFFFF - sensor->ic_val1) + sensor->ic_val2;

                sensor->distance = sensor->echo_time / 58;
                sensor->capture_flag = 0;

                __HAL_TIM_DISABLE_IT(sensor->htim, sensor->channel);
                //printf("Sensor %d: %d cm\n", i, sensor->distance);
            }
        }
    }
}













