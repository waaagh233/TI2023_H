/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "dma2d.h"
#include "ltdc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "fmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "touch.h"
#include "lcd_rgb_ltdc_drv.h"
#include "lvgl.h"
#include "lv_port_indev.h"
#include "lv_port_disp.h"
#include "retarget.h"
#include "gui_guider.h"
#include "arm_math.h"
#include "arm_const_structs.h"
#include "ad9959.h"
#include "my_lib.h"
#include "PID.h"
//#include "ad9910.h"
#define Vref1 3.598
#define Vref2 3.599
#define FFT_LENGTH 1024
#define sin 1;
#define tri 2;
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
lv_ui guider_ui;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
uint32_t data_out[6]={1073741824,1073741824,0,0,8589935,1717987},spi_data_in[1024],spi_data_out[1024]={0};
uint16_t spi_flag = 0,start_flag1 = 0,start_flag2 = 0,start_flag3 = 0,set_phase = 0,amp_flag = 0;
uint32_t data_in[2047];
uint32_t Fs=640000;
float data_in1[2047],data_in2[2047];
//double Phase=0,Phase1=0,Phase2=0,Amp1,Amp2;
uint16_t flag=1,mode=1;
//extern uint16_t flag0;
//float u[5]={0};
float fft_inputbuf[FFT_LENGTH * 2];
float fft_outputbuf[FFT_LENGTH];
PID_TypeDef mypid1;
PID_TypeDef mypid2;
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

void ADC_Start(void)
{
    flag = 1;
    HAL_ADCEx_MultiModeStart_DMA(&hadc1,(uint32_t*)data_in,2047);
    while(flag);
    for(int i = 0;i < 2047;i++)
    {
        data_in1[i] = (((data_in[i] >> 16) * 2.0 / 65535.0-1.0) * Vref1 - 0.138);
        data_in2[i] = (((data_in[i] & 0xffff) * 2.0 / 65535.0-1.0) * Vref2 - 0.14);
//          if(Vmax1 < data_in1[i]) Vmax1 = data_in1[i];
//          if(Vmin1 > data_in1[i]) Vmin1 = data_in1[i];
//        printf("%lf,%lf,%d\r\n",data_in1[i],data_in2[i],i);
    }
}

Get_Amp(double* amp,uint16_t * loc,uint16_t* num)
{
    uint16_t Num = 1;
    for(int i = 1;i < FFT_LENGTH/2;i++)
        if(fft_outputbuf[i] > 0.1) {amp[0] = fft_outputbuf[i];loc[0] = i; ;break; }
    for(int i = loc[0]+1;i < FFT_LENGTH/2;i++) {
        if (fft_outputbuf[i] > 0.01) {
            amp[Num] = fft_outputbuf[i];
            loc[Num] = i;
//            printf("amp=%lf,%d,%d\r\n",amp[Num],loc[Num],i);
            Num++;
        }
    }
    *num = Num - 1;
}

Judge_wave(double* amp,uint16_t* num,uint16_t* wave1,uint16_t* wave2)
{
    double max=0;
    if(*num == 1) {*wave1 = sin;*wave2 = sin;}
    else if(*num > 1)
    {
        if(amp[0] > 0.46) {*wave1 = sin;*wave2 = tri;}
        else if(amp[0] < 0.46)
        {
            for(int i = 1;i <= *num;i++)
                if(amp[i] > max) max = amp[i];
            if(max > 0.43) {*wave1 = tri;*wave2 = sin;}
            else {*wave1 = tri;*wave2 = tri;}
        }
    }
}

Get_fre(double* amp,uint16_t * loc,uint16_t* num,uint16_t* wave1,uint16_t* wave2,double* Fre1,double* Fre2,double * phase1,double * phase2)
{
    double max=0;
    uint16_t max1,max2;
    uint16_t Location;
    for(int i = 1;i <= *num;i++)
    {
        if(amp[i] > max) {max = amp[i];Location=i;}
    }
    *Fre2 = *Fre1 * loc[Location] * 1.0 / loc[0];
    max1 = *Fre1 / 625;
    if(fft_inputbuf[max1*2+1] >= 0 && fft_inputbuf[max1*2] >= 0)
        *phase1 = atan(fft_inputbuf[max1*2+1] / fft_inputbuf[max1*2]) / PI * 180;
    else if(fft_inputbuf[max1*2+1] >= 0 && fft_inputbuf[max1*2] <= 0)
        *phase1 = atan(fft_inputbuf[max1*2+1] / fft_inputbuf[max1*2]) / PI * 180 + 180;
    else if(fft_inputbuf[max1*2+1] <= 0 && fft_inputbuf[max1*2] <= 0)
        *phase1 = atan(fft_inputbuf[max1*2+1] / fft_inputbuf[max1*2]) / PI * 180 + 180;
    else if(fft_inputbuf[max1*2+1] <= 0 && fft_inputbuf[max1*2] >= 0)
        *phase1 = atan(fft_inputbuf[max1*2+1] / fft_inputbuf[max1*2]) / PI * 180 + 360;
    max2 = *Fre2 / 625;
    if(fft_inputbuf[max2*2+1] >= 0 && fft_inputbuf[max2*2] >= 0)
        *phase2 = atan(fft_inputbuf[max2*2+1] / fft_inputbuf[max2*2]) / PI * 180;
    else if(fft_inputbuf[max2*2+1] >= 0 && fft_inputbuf[max2*2] <= 0)
        *phase2 = atan(fft_inputbuf[max2*2+1] / fft_inputbuf[max2*2]) / PI * 180 + 180;
    else if(fft_inputbuf[max2*2+1] <= 0 && fft_inputbuf[max2*2] <= 0)
        *phase2 = atan(fft_inputbuf[max2*2+1] / fft_inputbuf[max2*2]) / PI * 180 + 180;
    else if(fft_inputbuf[max2*2+1] <= 0 && fft_inputbuf[max2*2] >= 0)
        *phase2 = atan(fft_inputbuf[max2*2+1] / fft_inputbuf[max2*2]) / PI * 180 + 360;
//    printf("Location=%d\r\n",Location);
}

void FFT1(double * fre)
{
    uint16_t i,max1,max2,mode = 0;
    for(i=0;i<FFT_LENGTH;i++)
    {
        fft_inputbuf[i * 2]=data_in1[i];
        fft_inputbuf[i * 2 + 1] = 0;
//        printf("%.2lf\r\n",fft_inputbuf[i*2]);
    }
    arm_cfft_f32(&arm_cfft_sR_f32_len1024, fft_inputbuf, 0, 1);
    arm_cmplx_mag_f32(fft_inputbuf, fft_outputbuf, FFT_LENGTH);
    fft_outputbuf[0] /= FFT_LENGTH;
    mode=0;
    for (i = 1; i < FFT_LENGTH/2; i++)
    {
        fft_outputbuf[i] /= FFT_LENGTH/2;
        if(fft_outputbuf[i] < 0.006) fft_outputbuf[i] = 0;
//        printf("%lf,%d\r\n",fft_outputbuf[i],i);
    }
    for (i = 1; i < FFT_LENGTH/2; i++)
    {
        if(fft_outputbuf[i] > 0.1 && !mode)
        {
            max1 = i;mode=1;break;
        }
    }
    mode = 0;
    for (i = max1+1; i < FFT_LENGTH/2; i++)
    {
        if(fft_outputbuf[i] > 0.1 && !mode)
        {
            max2 = i;mode=1;break;
        }
    }
    *fre = 625*max1;
}

void FFT2(double* fre1,double* fre2)
{
    uint16_t i,max1,max2,mode = 0;
    double max_1,max_2;
    for(i=0;i<FFT_LENGTH;i++)
    {
        fft_inputbuf[i * 2]=spi_data_in[i];
        fft_inputbuf[i * 2 + 1] = 0;
//        printf("%.2lf\r\n",fft_inputbuf[i*2]);
    }
    arm_cfft_f32(&arm_cfft_sR_f32_len1024, fft_inputbuf, 0, 1);
    arm_cmplx_mag_f32(fft_inputbuf, fft_outputbuf, FFT_LENGTH);
    fft_outputbuf[0] /= FFT_LENGTH;
    mode=0;
    for (i = 1; i < FFT_LENGTH/2; i++)
    {
        fft_outputbuf[i] /= FFT_LENGTH/2;
        if(fft_outputbuf[i] < 0.006) fft_outputbuf[i] = 0;
//        printf("%lf,%d\r\n",fft_outputbuf[i],i);
    }
    for (i = 1; i < FFT_LENGTH/2; i++)
    {
        if(fft_outputbuf[i] > 100 && !mode)
        {
            max1 = i;mode=1;break;
        }
    }
    mode = 0;
    for (i = max1+2; i < FFT_LENGTH/2; i++)
    {
        if(fft_outputbuf[i] > 100 && !mode)
        {
            max2 = i;mode=1;break;
        }
    }
    if(fft_outputbuf[max1+1] > 0.001 || fft_outputbuf[max1-1] > 0.001)
    {
        if(fft_outputbuf[max1+1]>=fft_outputbuf[max1-1])
        {max_1=max1+fft_outputbuf[max1+1]*1.0/(fft_outputbuf[max1+1]+fft_outputbuf[max1]);}
        else
        {max_1=max1-fft_outputbuf[max1-1]*1.0/(fft_outputbuf[max1-1]+fft_outputbuf[max1]);}
    }
    if(fft_outputbuf[max2+1] > 0.001 || fft_outputbuf[max2-1] > 0.001)
    {
        if(fft_outputbuf[max2+1]>=fft_outputbuf[max2-1])
        {max_2=max2+fft_outputbuf[max2+1]*1.0/(fft_outputbuf[max2+1]+fft_outputbuf[max2]);}
        else
        {max_2=max2-fft_outputbuf[max2-1]*1.0/(fft_outputbuf[max2-1]+fft_outputbuf[max2]);}
    }
    *fre1 = round(48828.125 * max_1 / 50000) * 50000;
    *fre2 = round(48828.125 * max_2 / 50000) * 50000;
    if(*fre1 < 0) *fre1 = 0;
    if(*fre2 < 0) *fre2 = 0;
//    printf("max1=%d,max_1=%lf,max2=%d,max_2=%lf\r\n",max1,max_1,max2,max_2);
}

void FFT3(double* fre1,double* fre2)
{
    uint16_t i,max1,max2,mode = 0;
    double max_1,max_2;
    for(i=0;i<FFT_LENGTH;i++)
    {
        fft_inputbuf[i * 2]=data_in1[i];
        fft_inputbuf[i * 2 + 1] = 0;
//        printf("%.2lf\r\n",fft_inputbuf[i*2]);
    }
    arm_cfft_f32(&arm_cfft_sR_f32_len1024, fft_inputbuf, 0, 1);
    arm_cmplx_mag_f32(fft_inputbuf, fft_outputbuf, FFT_LENGTH);
    fft_outputbuf[0] /= FFT_LENGTH;
    mode=0;
    for (i = 1; i < FFT_LENGTH/2; i++)
    {
        fft_outputbuf[i] /= FFT_LENGTH/2;
        if(fft_outputbuf[i] < 0.006) fft_outputbuf[i] = 0;
//        printf("%lf,%d\r\n",fft_outputbuf[i],i);
    }
    for (i = 1; i < FFT_LENGTH/2; i++)
    {
        if(fft_outputbuf[i] > 0.2 && !mode)
        {
            max1 = i;mode=1;break;
        }
    }
    mode = 0;
    for (i = max1+4; i < FFT_LENGTH/2; i++)
    {
        if(fft_outputbuf[i] > 0.2 && !mode)
        {
            max2 = i;mode=1;break;
        }
    }
    if(fft_outputbuf[max1+1] > 0.001 || fft_outputbuf[max1-1] > 0.001)
    {
        if(fft_outputbuf[max1+1]>=fft_outputbuf[max1-1])
        {max_1=max1+fft_outputbuf[max1+1]*1.0/(fft_outputbuf[max1+1]+fft_outputbuf[max1]);}
        else
        {max_1=max1-fft_outputbuf[max1-1]*1.0/(fft_outputbuf[max1-1]+fft_outputbuf[max1]);}
    }
    else max_1 = max1;
    if(fft_outputbuf[max2+1] > 0.001 || fft_outputbuf[max2-1] > 0.001)
    {
        if(fft_outputbuf[max2+1]>=fft_outputbuf[max2-1])
        {max_2=max2+fft_outputbuf[max2+1]*1.0/(fft_outputbuf[max2+1]+fft_outputbuf[max2]);}
        else
        {max_2=max2-fft_outputbuf[max2-1]*1.0/(fft_outputbuf[max2-1]+fft_outputbuf[max2]);}
    }
    else max_2 = max2;
    *fre1 = round(246.71 * max_1 / 1000) * 1000;
    *fre2 = round(246.71 * max_2 / 1000) * 1000;
    if(*fre1 < 0) *fre1 = 0;
    if(*fre2 < 0) *fre2 = 0;
//    printf("max1=%d,max_1=%lf,max2=%d,max_2=%lf\r\n",max1,max_1,max2,max_2);
}

void SPI_1(uint16_t* wave1,uint16_t* wave2)
{
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,0);
    spi_flag = 0;
    HAL_SPI_Transmit_DMA(&hspi2, data_out,6);
    while (!spi_flag) {}
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, 1);
    if(*wave1 == 1) HAL_GPIO_WritePin(GPIOH, GPIO_PIN_2,0);
    else if(*wave1 == 2) HAL_GPIO_WritePin(GPIOH, GPIO_PIN_2,1);
    if(*wave2 == 1) HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2,0);
    else if(*wave2 == 2) HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2,1);
}

void SPI_2(void)
{
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,0);
    spi_flag = 0;
    HAL_SPI_TransmitReceive_DMA(&hspi2,spi_data_out,spi_data_in,1024);
    while (!spi_flag) {}
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, 1);
}

//void change_wave(uint16_t* wave1,uint16_t* wave2)
//{
//    if(*wave1 == 1) HAL_GPIO_WritePin(GPIOH,GPIO_PIN_3,1);
//    else if(*wave1 == 2) HAL_GPIO_WritePin(GPIOH,GPIO_PIN_3,0);
//    if(*wave2 == 1) HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,1);
//    else if(*wave2 == 2) HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,0);
//}

void show_data(double fre1,double fre2,uint16_t* wave1,uint16_t* wave2)
{
    double Vpp;
    char FRE1[10]="",FRE2[10]="",WAVE1[10]="",WAVE2[10]="";
    sprintf(FRE1,"%.0lfHz",fre1);
    sprintf(FRE2,"%.0lfHz",fre2);
    if(*wave1 == 1) sprintf(WAVE1,"sin");
    else if(*wave1 == 2) sprintf(WAVE1,"triangle");
    if(*wave2 == 1) sprintf(WAVE2,"sin");
    else if(*wave2 == 2) sprintf(WAVE2,"triangle");
    lv_label_set_text(guider_ui.screen_label_10, FRE1);
    lv_label_set_text(guider_ui.screen_label_11, FRE2);
    lv_label_set_text(guider_ui.screen_label_12, WAVE1);
    lv_label_set_text(guider_ui.screen_label_13, WAVE2);
    return;
}

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void PeriphCommonClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
    static uint16_t LCD_BUFFER[600][1024] __attribute__((section(".malloc")));
    double amp[512]={0},Fre1,Fre2,fre1,fre2,phase1,phase2,phase3,phase4,dphase1,dphase2,dphase;
    uint16_t num,wave1,wave2,loc[512]={0},mode=0;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

/* Configure the peripherals common clocks */
  PeriphCommonClock_Config();

  /* USER CODE BEGIN SysInit */
  RetargetInit(&huart1);
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_FMC_Init();
  MX_DMA2D_Init();
  MX_LTDC_Init();
  MX_TIM6_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_USART1_UART_Init();
  MX_TIM2_Init();
  MX_TIM5_Init();
  MX_TIM7_Init();
  MX_ADC2_Init();
  MX_SPI2_Init();
  /* USER CODE BEGIN 2 */
  LTDC_Init();
  tp_dev.init();
  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_SET);
  HAL_TIM_Base_Start_IT(&htim2);
  TIM6->ARR = 14;
  HAL_TIM_Base_Start(&htim6);
  HAL_TIM_Base_Start(&htim7);
  HAL_ADCEx_Calibration_Start(&hadc1, ADC_CALIB_OFFSET_LINEARITY, ADC_SINGLE_ENDED);
  lv_init();
  lv_port_disp_init();
  lv_port_indev_init();
  setup_ui(&guider_ui);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
      lv_task_handler();
      if(start_flag1) {
          HAL_TIM_Base_Stop(&htim6);
          TIM6->ARR = 14;
          HAL_TIM_Base_Start(&htim6);
          if(amp_flag) HAL_GPIO_WritePin(GPIOH,GPIO_PIN_5,1);
          else HAL_GPIO_WritePin(GPIOH,GPIO_PIN_5,0);
          HAL_GPIO_WritePin(GPIOH,GPIO_PIN_4,0);
          HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,0);
          ADC_Start();
          FFT1(&fre1);
          Get_Amp(amp, loc, &num);
          Judge_wave(amp, &num, &wave1, &wave2);
          Get_fre(amp, loc, &num, &wave1, &wave2, &fre1, &fre2, &phase1, &phase2);
          dphase = 0;
          if(wave1 == 1 && wave2 == 1 && mod(fre2,fre1) == 0)
          {
              mode = fre2 / fre1;
              apfft_phase(data_in1,mode,640000,fre1,&dphase);
              dphase = mod((dphase + 360), 360) >= 180 ? mod((dphase + 360), 360) - 360 : mod(
                      (dphase + 360), 360);
//              printf("dphase=%lf\r\n",dphase);
          }
          data_out[2] = ((360 - set_phase + dphase - 1 * (fre2/fre1/2)) * 11930464.71) / (fre2 / fre1);
//          printf("set_phase=%d,dphase=%lf,data_out=%lf\r\n",set_phase,dphase,data_out[2]);
          data_out[4] = 85.899 * fre1;
          data_out[5] = 85.899 * fre2;
          SPI_1(&wave1,&wave2);
//          printf("fre1=%lf,fre2=%lf,wave1=%d,wave2=%d\r\n",fre1,fre2,wave1,wave2);
          show_data(fre1, fre2, &wave1, &wave2);
      }
      else if(start_flag2)
      {
          HAL_GPIO_WritePin(GPIOH,GPIO_PIN_5,0);
          HAL_GPIO_WritePin(GPIOH,GPIO_PIN_4,1);
          HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,0);
          HAL_GPIO_WritePin(GPIOH, GPIO_PIN_2,0);
          HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2,0);
          if(HAL_GPIO_ReadPin(GPIOH,GPIO_PIN_3)) {
              SPI_2();
//              printf("123\r\n");
//              for(int i = 0;i < 1024;i++)
//                  printf("%d,%d\r\n",spi_data_in[i],i);
          }
          FFT2(&fre1,&fre2);
          spi_data_out[0] = fre1 * 85.899;
          spi_data_out[1] = fre2 * 85.899;
//          printf("fre1=%lf,fre2=%lf\r\n",fre1,fre2);
      }
      else if(start_flag3)
      {
          HAL_TIM_Base_Stop(&htim6);
          TIM6->ARR = 37;
          HAL_TIM_Base_Start(&htim6);
          HAL_GPIO_WritePin(GPIOH,GPIO_PIN_5,0);
          HAL_GPIO_WritePin(GPIOH,GPIO_PIN_4,0);
          HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,0);
          ADC_Start();
          FFT3(&fre1,&fre2);
          data_out[4] = 85.899 * fre1;
          data_out[5] = 85.899 * fre2;
          wave1 = 1;
          wave2 = 1;
          SPI_1(&wave1,&wave2);
//          printf("fre1=%lf,fre2=%lf\r\n",fre1,fre2);
      }
      else {
          HAL_GPIO_WritePin(GPIOH,GPIO_PIN_5,0);
          HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,1);
          HAL_GPIO_WritePin(GPIOH,GPIO_PIN_4,0);
      }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);
  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}
  /** Macro to configure the PLL clock source
  */
  __HAL_RCC_PLL_PLLSOURCE_CONFIG(RCC_PLLSOURCE_HSE);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 5;
  RCC_OscInitStruct.PLL.PLLN = 192;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief Peripherals Common Clock Configuration
  * @retval None
  */
void PeriphCommonClock_Config(void)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Initializes the peripherals clock
  */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADC|RCC_PERIPHCLK_SPI2;
  PeriphClkInitStruct.PLL2.PLL2M = 2;
  PeriphClkInitStruct.PLL2.PLL2N = 12;
  PeriphClkInitStruct.PLL2.PLL2P = 3;
  PeriphClkInitStruct.PLL2.PLL2Q = 2;
  PeriphClkInitStruct.PLL2.PLL2R = 2;
  PeriphClkInitStruct.PLL2.PLL2RGE = RCC_PLL2VCIRANGE_3;
  PeriphClkInitStruct.PLL2.PLL2VCOSEL = RCC_PLL2VCOMEDIUM;
  PeriphClkInitStruct.PLL2.PLL2FRACN = 0;
  PeriphClkInitStruct.Spi123ClockSelection = RCC_SPI123CLKSOURCE_PLL2;
  PeriphClkInitStruct.AdcClockSelection = RCC_ADCCLKSOURCE_PLL2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
