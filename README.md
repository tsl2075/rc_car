## 프로젝트 개요
장애물을 회피하면서 주행하도록 만든 RC카 프로젝트입니다.


## 개발 개요
- 프로젝트 : RC카
- 개발기간 : 2025.05.15 ~ 05.21
- 개발 언어 : C


## 목표
- 초음파센서와 블루투스UART 통신의 DMA를 활용하여 실시간 값을 이용해서 
  다양한 회피 알고리즘 구현
- Free RTOS 기반의 실시간 제어 구조를 통해 센서,모터 등을 TASK 단위로 분리하여
  효울적으로 시스템을 운용하며 RTOS의 개념실습

## 주요 기술
- Free RTOS

## Board
- STM32F411RE

## 세부설명
HC-SR04 초음파 모듈을 사용하여 전방에 있는 장애물을 회피하면서 주행하도록 만들었고, 
후방에는 LCD를 연동시켜 방향지시표시, 블루투스모드의 모드를 표시했습니다.
 
## 알고리즘 동작
##### 1. 초음파 센서 데이터 
  - 전방, 좌측, 우측 거리 값 수집
##### 2. 거리 데이터 분석 
  - 센서 측정값이 사전에 설계한 임계값 보다 작을 때 장애물로 인식
ex) 임계값을 9cm로 설정 시 , 9cm 이내 물체는 장애물로 판단해서 회피동작
  - 전방이 28cm이하일 때 좌우 센서 간의 차이를 기반으로 방향 결정
ex) 차이가 4cm이하일 경우 더 좁은 쪽으로 회피 해서 직각 코스에서 대각선으로 이동시 탈출
그 외 좌우 거리 비교하여 넓은 쪽으로 회피

<img width="239" height="310" alt="Image" src="https://github.com/user-attachments/assets/f8529b95-534c-4d95-a262-d12308d266f8" />


### Youtube 링크
https://www.youtube.com/watch?v=M6ztx0a8rrA


### &nbsp;&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;동작영상   &nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;   &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;    | 

![제목 없는 동영상 - Clipchamp로 제작 (2)](https://github.com/user-attachments/assets/8b5268bf-f44d-4a94-9d2a-56a41e47fb00) ![제목 없는 동영상 - Clipchamp로 제작 (3)](https://github.com/user-attachments/assets/c1e7d3a2-ba81-48b7-acfb-f3bf966ee5b0)

