#ifndef __PID_H
#define __PID_H

typedef struct
{
  float Kp;        // ��������
  float Ki;        // ���ֳ���
  float Kd;        // ΢�ֳ���
  float lastError; // ��һ�����
  float sumError;  // ����ۼ�
  float maxSumError;  // ����ۼ�����
} PID_TypeDef;

void PID_Init(PID_TypeDef *pid, float kp, float ki, float kd, float maxSumError);
float PID_Calc(PID_TypeDef *pid, float setPoint, float currentPoint);

#endif
