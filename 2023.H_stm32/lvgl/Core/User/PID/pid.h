#ifndef __PID_H
#define __PID_H

typedef struct
{
  float Kp;        // 比例常数
  float Ki;        // 积分常数
  float Kd;        // 微分常数
  float lastError; // 上一次误差
  float sumError;  // 误差累计
  float maxSumError;  // 误差累计上限
} PID_TypeDef;

void PID_Init(PID_TypeDef *pid, float kp, float ki, float kd, float maxSumError);
float PID_Calc(PID_TypeDef *pid, float setPoint, float currentPoint);

#endif
