
#include "delay.h"


void delay_us(uint16_t us)
{
  __HAL_TIM_SET_COUNTER(&htim11, 0); //타이머를 0으로 초기화 한다는 뜻이고
  //50000만까지 올라가다가 초과되면 빠져 나옴
  while((__HAL_TIM_GET_COUNTER(&htim11)) < us);
} //호출했더니 들어가서 값을 갖고옴 us에 100을줬으면 set되고
//참이라서 while로 들어가고 딜레이 함수에 걸림


