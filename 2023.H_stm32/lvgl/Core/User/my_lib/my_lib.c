/*
 *
 *
 *
 *
 *
 */
#include <stdio.h>
#include "main.h"
#include "my_lib.h"
#include "win.h"
/**
 * 取模函数
 **/
double mod(double a,double b)
{
    double ans;
    ans =a - b * (int)(a / b);
    return ans;
}
/**
 * apfft_inputbuf : 输入信号指针
 * mode :待求谐波序号
 * fs : 输入信号的采样频率
 * fre :信号频率指针
 * phase:谐波相位指针
 * A:谐波幅值指针
 **/
void apfft(float * apfft_inputbuf,uint16_t mode,double fs,double* fre,double *phase,double *A)
{
    double win[FFT_LENGTH],win1[FFT_LENGTH*2-1]={0},win2[FFT_LENGTH],y1[FFT_LENGTH*2-1],y2[FFT_LENGTH],phase2,phase3,phase4,phase5,A1,A2,A3,A4,A5,f;
    float y1a[FFT_LENGTH*2],y2a[FFT_LENGTH*2],apfft_outputbuf1[FFT_LENGTH],apfft_outputbuf2[FFT_LENGTH],phase_outputbuf1[FFT_LENGTH],phase_outputbuf2[FFT_LENGTH],amp_outputbuf[FFT_LENGTH],Fmax=0;
    uint16_t r1,r2,r3,r4,r5,fc1,fc2,fc3,fc4,fc5;
    uint32_t max_pos;
    for(int i = 0;i < FFT_LENGTH*2-1;i++) {
        y1[i] = apfft_inputbuf[i] * blackman_conv[i];
//        printf("%lf,%d\r\n",y1[i],i);
    }
    win2[0] = blackman[0] * 1.0 / blackman_sum;
    y2a[0] = apfft_inputbuf[FFT_LENGTH - 1] * win2[0];
    y2a[1] = 0;
    for(int i = 1;i < FFT_LENGTH;i++)
    {
        win2[i] = blackman[i] * 1.0 / blackman_sum;
        y2a[i * 2] = apfft_inputbuf[FFT_LENGTH + i - 1] * win2[i];
        y2a[i * 2 + 1] = 0;
        y1a[i * 2] = y1[i - 1] + y1[i+FFT_LENGTH-1];
        y1a[i * 2 + 1] = 0;
    }
    y1a[0] = y1[1023];y1a[1] = 0;
    arm_cfft_f32(&arm_cfft_sR_f32_len1024, y1a, 0, 1);
    arm_cmplx_mag_f32(y1a, apfft_outputbuf1, FFT_LENGTH);
    arm_max_f32(apfft_outputbuf1, 1024, &Fmax, &max_pos);
    arm_cfft_f32(&arm_cfft_sR_f32_len1024, y2a, 0, 1);
    arm_cmplx_mag_f32(y2a, apfft_outputbuf2, FFT_LENGTH);
    for (int i = 0; i < FFT_LENGTH; i++)
    {
        if(y1a[i*2+1] >= 0 && y1a[i*2] >= 0)
            phase_outputbuf1[i] = atan(y1a[i*2+1] / y1a[i*2]) / PI * 180;
        else if(y1a[i*2+1] >= 0 && y1a[i*2] <= 0)
            phase_outputbuf1[i] = atan(y1a[i*2+1] / y1a[i*2]) / PI * 180 + 180;
        else if(y1a[i*2+1] <= 0 && y1a[i*2] <= 0)
            phase_outputbuf1[i] = atan(y1a[i*2+1] / y1a[i*2]) / PI * 180 + 180;
        else if(y1a[i*2+1] <= 0 && y1a[i*2] >= 0)
            phase_outputbuf1[i] = atan(y1a[i*2+1] / y1a[i*2]) / PI * 180 + 360;
    }
    for (int i = 0; i < FFT_LENGTH; i++)
    {
        if(y2a[i*2+1] >= 0 && y2a[i*2] >= 0)
            phase_outputbuf2[i] = atan(y2a[i*2+1] / y2a[i*2]) / PI * 180;
        else if(y2a[i*2+1] >= 0 && y2a[i*2] <= 0)
            phase_outputbuf2[i] = atan(y2a[i*2+1] / y2a[i*2]) / PI * 180 + 180;
        else if(y2a[i*2+1] <= 0 && y2a[i*2] <= 0)
            phase_outputbuf2[i] = atan(y2a[i*2+1] / y2a[i*2]) / PI * 180 + 180;
        else if(y2a[i*2+1] <= 0 && y2a[i*2] >= 0)
            phase_outputbuf2[i] = atan(y2a[i*2+1] / y2a[i*2]) / PI * 180 + 360;
    }
    if(max_pos > 512) max_pos = 1024 - max_pos;
    if(phase_outputbuf2[max_pos]-phase_outputbuf1[max_pos] <= 180 && phase_outputbuf2[max_pos]-phase_outputbuf1[max_pos] >= -180)
        f=(mod((mod(phase_outputbuf2[max_pos]-phase_outputbuf1[max_pos],360))/180,1) + max_pos) * fs / FFT_LENGTH;
    else
        if(phase_outputbuf2[max_pos] > phase_outputbuf1[max_pos])
            f=(mod((mod(phase_outputbuf2[max_pos]-360-phase_outputbuf1[max_pos],360))/180,1) + max_pos) * fs / FFT_LENGTH;
        else
            f=(mod((mod(phase_outputbuf2[max_pos]+360-phase_outputbuf1[max_pos],360))/180,1) + max_pos) * fs / FFT_LENGTH;
//    printf("f=%lf,max_pos=%d,p2=%lf,p1=%lf\r\n",f,max_pos,phase_outputbuf2[max_pos],phase_outputbuf1[max_pos]);
    *fre = f;
    fc1 = f,fc2 = 2*fc1,fc3 = 3*fc1,fc4= 4*fc1,fc5 = 5*fc1;
    r1=round(fc1 * 1.0 / fs * FFT_LENGTH);
    r2=round(fc2 * 1.0 / fs * FFT_LENGTH);
    r3=round(fc3 * 1.0 / fs * FFT_LENGTH);
    r4=round(fc4 * 1.0 / fs * FFT_LENGTH);
    r5=round(fc5 * 1.0 / fs * FFT_LENGTH);
    phase2 = mod(phase_outputbuf1[r2] - mod((phase_outputbuf1[r1] / 360 * fs / fc1) / fs * fc2 *360,360) + 360 - 90,360);
//    printf("phase2=%lf\r\n",phase2);
    phase3 = mod(phase_outputbuf1[r3] - mod((phase_outputbuf1[r1] / 360 * fs / fc1) / fs * fc3 *360,360) + 360 - 180,360);
//    printf("phase3=%lf\r\n",phase3);
    phase4 = mod(phase_outputbuf1[r4] - mod((phase_outputbuf1[r1] / 360 * fs / fc1) / fs * fc4 *360,360) + 360 - 270,360);
//    printf("phase4=%lf\r\n",phase4);
    phase5 = mod(phase_outputbuf1[r5] - mod((phase_outputbuf1[r1] / 360 * fs / fc1) / fs * fc5 *360,360) + 360,360);
//    printf("phase5=%lf\r\n",phase5);
    A1 = apfft_outputbuf2[r1] * apfft_outputbuf2[r1] / apfft_outputbuf1[r1] * 2;
//    printf("A1=%lf\r\n",A1);
    A2 = apfft_outputbuf2[r2] * apfft_outputbuf2[r2] / apfft_outputbuf1[r2] * 2;
//    printf("A2=%lf\r\n",A2);
    A3 = apfft_outputbuf2[r3] * apfft_outputbuf2[r3] / apfft_outputbuf1[r3] * 2;
//    printf("A3=%lf\r\n",A3);
    A4 = apfft_outputbuf2[r4] * apfft_outputbuf2[r4] / apfft_outputbuf1[r4] * 2;
//    printf("A4=%lf\r\n",A4);
    A5 = apfft_outputbuf2[r5] * apfft_outputbuf2[r5] / apfft_outputbuf1[r5] * 2;
//    printf("A5=%lf\r\n",A5);
//    for(int i = 0;i < 1024; i++) {
//        amp_outputbuf[i] = apfft_outputbuf2[i] * apfft_outputbuf1[i] / apfft_outputbuf1[i] * 2;
//        printf("%lf,%d\r\n",amp_outputbuf[i],i);
//    }
    if(mode == 1) {*phase = phase_outputbuf1[r1];*A = A1;}
    else if(mode == 2) {*phase = phase2;*A = A2;}
    else if(mode == 3) {*phase = phase3;*A = A3;}
    else if(mode == 4) {*phase = phase4;*A = A4;}
    else if(mode == 5) {*phase = phase5;*A = A5;}
}

void apfft_phase(float * apfft_inputbuf,uint16_t mode,double fs,double fre,double *phase)
{
    double y1[FFT_LENGTH*2-1],phase2,phase3,phase4,phase5;
    float y1a[FFT_LENGTH*2],apfft_outputbuf1[FFT_LENGTH],phase_outputbuf1[FFT_LENGTH];
    uint32_t r1,r2,r3,r4,r5,fc1,fc2,fc3,fc4,fc5;
    for(int i = 0;i < FFT_LENGTH*2-1;i++) {
        y1[i] = apfft_inputbuf[i] * blackman_conv[i];
    }
    for(int i = 1;i < FFT_LENGTH;i++)
    {
        y1a[i * 2] = y1[i - 1] + y1[i+FFT_LENGTH-1];
        y1a[i * 2 + 1] = 0;
    }
    y1a[0] = y1[1023];y1a[1] = 0;
    arm_cfft_f32(&arm_cfft_sR_f32_len1024, y1a, 0, 1);
    arm_cmplx_mag_f32(y1a, apfft_outputbuf1, FFT_LENGTH);
    for (int i = 0; i < FFT_LENGTH; i++)
    {
        if(y1a[i*2+1] >= 0 && y1a[i*2] >= 0)
            phase_outputbuf1[i] = atan(y1a[i*2+1] / y1a[i*2]) / PI * 180;
        else if(y1a[i*2+1] >= 0 && y1a[i*2] <= 0)
            phase_outputbuf1[i] = atan(y1a[i*2+1] / y1a[i*2]) / PI * 180 + 180;
        else if(y1a[i*2+1] <= 0 && y1a[i*2] <= 0)
            phase_outputbuf1[i] = atan(y1a[i*2+1] / y1a[i*2]) / PI * 180 + 180;
        else if(y1a[i*2+1] <= 0 && y1a[i*2] >= 0)
            phase_outputbuf1[i] = atan(y1a[i*2+1] / y1a[i*2]) / PI * 180 + 360;
    }
    fc1 = fre,fc2 = 2*fc1,fc3 = 3*fc1,fc4= 4*fc1,fc5 = 5*fc1;
    r1=round(fc1 * 1.0 / fs * FFT_LENGTH);
    r2=round(fc2 * 1.0 / fs * FFT_LENGTH);
    r3=round(fc3 * 1.0 / fs * FFT_LENGTH);
    r4=round(fc4 * 1.0 / fs * FFT_LENGTH);
    r5=round(fc5 * 1.0 / fs * FFT_LENGTH);
    phase2 = mod(phase_outputbuf1[r2] - mod((phase_outputbuf1[r1] / 360 * fs / fc1) / fs * fc2 *360,360) + 360 - 90,360);
//    printf("phase2=%lf\r\n",phase2);
    phase3 = mod(phase_outputbuf1[r3] - mod((phase_outputbuf1[r1] / 360 * fs / fc1) / fs * fc3 *360,360) + 360 - 180,360);
//    printf("phase3=%lf\r\n",phase3);
    phase4 = mod(phase_outputbuf1[r4] - mod((phase_outputbuf1[r1] / 360 * fs / fc1) / fs * fc4 *360,360) + 360 - 270,360);
//    printf("phase4=%lf\r\n",phase4);
    phase5 = mod(phase_outputbuf1[r5] - mod((phase_outputbuf1[r1] / 360 * fs / fc1) / fs * fc5 *360,360) + 360,360);
//    printf("phase5=%lf\r\n",phase5);
    if(mode == 1) {*phase = phase_outputbuf1[r1];}
    else if(mode == 2) {*phase = phase2;}
    else if(mode == 3) {*phase = phase3;}
    else if(mode == 4) {*phase = phase4;}
    else if(mode == 5) {*phase = phase5;}
}
