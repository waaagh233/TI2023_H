#include "pid.h"
#include "stdio.h"
#include "usart.h"
void PID_Init(PID_TypeDef *pid, float kp, float ki, float kd, float maxSumError)
{
  pid->Kp = kp;
  pid->Ki = ki;
  pid->Kd = kd;
  pid->lastError = 0.0f;
  pid->sumError = 0.0f;
  pid->maxSumError = maxSumError;
}

float PID_Calc(PID_TypeDef *pid, float setPoint, float currentPoint)
{
  float error = setPoint - currentPoint;
  float pTerm = pid->Kp * error;
//  float iTerm = pid->Ki * pid->sumError;
  float dTerm = pid->Kd * (error - pid->lastError);
  float output = pTerm  + dTerm;

  // 限制误差累计值
  if (pid->sumError + error > pid->maxSumError) {
    pid->sumError = pid->maxSumError;
  } else if (pid->sumError + error < -pid->maxSumError) {
    pid->sumError = -pid->maxSumError;
  } else {
    pid->sumError += error;
  }

  pid->lastError = error;
//    printf("%.3f\r\n",output);
  return output;
}

