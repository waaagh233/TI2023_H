#ifndef LVGL_MY_LIB_H
#define LVGL_MY_LIB_H


#include "arm_math.h"
#include "arm_const_structs.h"
#define FFT_LENGTH 1024

double mod(double a,double b);
void apfft(float * apfft_inputbuf,uint16_t mode,double fs,double* fre,double *phase,double *A);
void apfft_phase(float * apfft_inputbuf,uint16_t mode,double fs,double fre,double *phase);

#endif //LVGL_MY_LIB_H
