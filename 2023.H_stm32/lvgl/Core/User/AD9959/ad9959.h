/*
 * @Author: your name
 * @Date: 2021-10-26 20:06:41
 * @LastEditTime: 2021-11-04 21:07:19
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \MDK-ARMc:\Users\Administrator\Desktop\MyProject\STMCbue\AD9959\Drivers\AD9959\ad9959.h
 */
#ifndef AD9959_AD9959_H
#define AD9959_AD9959_H

#include "main.h"
#include "stdbool.h"
#include "gpio.h"
#include "main.h"

#define AD9959_CS_1 HAL_GPIO_WritePin(AD9959_CS_GPIO_Port,AD9959_CS_Pin,GPIO_PIN_SET)
#define AD9959_SCLK_1 HAL_GPIO_WritePin(AD9959_SCLK_GPIO_Port,AD9959_SCLK_Pin,GPIO_PIN_SET)
#define AD9959_UPDATE_1 HAL_GPIO_WritePin(AD9959_UPDATE_GPIO_Port,AD9959_UPDATE_Pin,GPIO_PIN_SET)
#define AD9959_PS0_1 HAL_GPIO_WritePin(AD9959_PS0_GPIO_Port,AD9959_PS0_Pin,GPIO_PIN_SET)
#define AD9959_PS1_1 HAL_GPIO_WritePin(AD9959_PS1_GPIO_Port,AD9959_PS1_Pin,GPIO_PIN_SET)
#define AD9959_PS2_1 HAL_GPIO_WritePin(AD9959_PS2_GPIO_Port,AD9959_PS2_Pin,GPIO_PIN_SET)
#define AD9959_PS3_1 HAL_GPIO_WritePin(AD9959_PS3_GPIO_Port,AD9959_PS3_Pin,GPIO_PIN_SET)
#define AD9959_SDIO0_1 HAL_GPIO_WritePin(AD9959_SDIO0_GPIO_Port,AD9959_SDIO0_Pin,GPIO_PIN_SET)
#define AD9959_SDIO1_1 HAL_GPIO_WritePin(AD9959_SDIO1_GPIO_Port,AD9959_SDIO1_Pin,GPIO_PIN_SET)
#define AD9959_SDIO2_1 HAL_GPIO_WritePin(AD9959_SDIO2_GPIO_Port,AD9959_SDIO2_Pin,GPIO_PIN_SET)
#define AD9959_SDIO3_1 HAL_GPIO_WritePin(AD9959_SDIO3_GPIO_Port,AD9959_SDIO3_Pin,GPIO_PIN_SET)
#define AD9959_PDC_1 HAL_GPIO_WritePin(AD9959_PDC_GPIO_Port,AD9959_PDC_Pin,GPIO_PIN_SET)
#define AD9959_RESET_1 HAL_GPIO_WritePin(AD9959_RESET_GPIO_Port,AD9959_RESET_Pin,GPIO_PIN_SET)

#define AD9959_CS_0 HAL_GPIO_WritePin(AD9959_CS_GPIO_Port,AD9959_CS_Pin,GPIO_PIN_RESET)
#define AD9959_SCLK_0 HAL_GPIO_WritePin(AD9959_SCLK_GPIO_Port,AD9959_SCLK_Pin,GPIO_PIN_RESET)
#define AD9959_UPDATE_0 HAL_GPIO_WritePin(AD9959_UPDATE_GPIO_Port,AD9959_UPDATE_Pin,GPIO_PIN_RESET)
#define AD9959_PS0_0 HAL_GPIO_WritePin(AD9959_PS0_GPIO_Port,AD9959_PS0_Pin,GPIO_PIN_RESET)
#define AD9959_PS1_0 HAL_GPIO_WritePin(AD9959_PS1_GPIO_Port,AD9959_PS1_Pin,GPIO_PIN_RESET)
#define AD9959_PS2_0 HAL_GPIO_WritePin(AD9959_PS2_GPIO_Port,AD9959_PS2_Pin,GPIO_PIN_RESET)
#define AD9959_PS3_0 HAL_GPIO_WritePin(AD9959_PS3_GPIO_Port,AD9959_PS3_Pin,GPIO_PIN_RESET)
#define AD9959_SDIO0_0 HAL_GPIO_WritePin(AD9959_SDIO0_GPIO_Port,AD9959_SDIO0_Pin,GPIO_PIN_RESET)
#define AD9959_SDIO1_0 HAL_GPIO_WritePin(AD9959_SDIO1_GPIO_Port,AD9959_SDIO1_Pin,GPIO_PIN_RESET)
#define AD9959_SDIO2_0 HAL_GPIO_WritePin(AD9959_SDIO2_GPIO_Port,AD9959_SDIO2_Pin,GPIO_PIN_RESET)
#define AD9959_SDIO3_0 HAL_GPIO_WritePin(AD9959_SDIO3_GPIO_Port,AD9959_SDIO3_Pin,GPIO_PIN_RESET)
#define AD9959_PDC_0 HAL_GPIO_WritePin(AD9959_PDC_GPIO_Port,AD9959_PDC_Pin,GPIO_PIN_RESET)
#define AD9959_RESET_0 HAL_GPIO_WritePin(AD9959_RESET_GPIO_Port,AD9959_RESET_Pin,GPIO_PIN_RESET)
 


typedef enum {
    AD9959_CHANNEL_0 = 0x10U,
    AD9959_CHANNEL_1 = 0x20U,
    AD9959_CHANNEL_2 = 0x40U,
    AD9959_CHANNEL_3 = 0x80U,
} AD9959_CHANNEL;

typedef enum {
    AD9959_REG_CSR       =   0x00U,  //CSR 通道选择寄存器
    AD9959_REG_FR1       =   0x01U,  //FR1 功能寄存器1
    AD9959_REG_FR2       =   0x02U,  //FR2 功能寄存器2
    AD9959_REG_CFR       =   0x03U,  //CFR 通道功能寄存器
    AD9959_REG_CFTW0     =   0x04U,  //CTW0 通道频率转换字寄存器
    AD9959_REG_CPOW0     =   0x05U,  //CPW0 通道相位转换字寄存器
    AD9959_REG_ACR       =   0x06U,  //ACR 幅度控制寄存器
    AD9959_REG_LSRR      =   0x07U,  //LSR 通道线性扫描寄存器
    AD9959_REG_RDW       =   0x08U,  //RDW 通道线性向上扫描寄存器
    AD9959_REG_FDW       =   0x09U,  //FDW 通道线性向下扫描寄存器
} AD9959_REG_ADDR;

//FR1[9:8] 调制电平选择位
#define LEVEL_MOD_2  	0x00//2电平调制 2阶调制
#define LEVEL_MOD_4		0x01//4电平调制	4阶调制
#define LEVEL_MOD_8		0x02//8电平调制	8阶调制
#define LEVEL_MOD_16	0x03//16电平调制	16阶调制

//CFR[23:22]  幅频相位（AFP）选择位
#define	DISABLE_Mod		0x00	//00	调制已禁用
#define	ASK 					0x40	//01	振幅调制，幅移键控
#define	FSK 					0x80	//10	频率调制，频移键控
#define	PSK 					0xc0	//11	相位调制，相移键控

//（CFR[14]）线性扫描启用 sweep enable																				
#define	SWEEP_ENABLE	0x40	//1	启用
#define	SWEEP_DISABLE	0x00	//0	不启用

#define IS_AD9959_REG_ADDR(REG_ADDR)    (((REG_ADDR) == AD9959_REG_CSR  ) || \
                                         ((REG_ADDR) == AD9959_REG_FR1  ) || \
                                         ((REG_ADDR) == AD9959_REG_FR2  ) || \
                                         ((REG_ADDR) == AD9959_REG_CFR  ) || \
                                         ((REG_ADDR) == AD9959_REG_CFTW0) || \
                                         ((REG_ADDR) == AD9959_REG_CPOW0) || \
                                         ((REG_ADDR) == AD9959_REG_ACR  ) || \
                                         ((REG_ADDR) == AD9959_REG_LSRR ) || \
                                         ((REG_ADDR) == AD9959_REG_RDW  ) || \
                                         ((REG_ADDR) == AD9959_REG_FDW  ))

#define IS_AD9959_CHANNEL(CHANNEL)      (((CHANNEL) == AD9959_CHANNEL_0 ) || \
                                         ((CHANNEL) == AD9959_CHANNEL_1 ) || \
                                         ((CHANNEL) == AD9959_CHANNEL_2 ) || \
                                         ((CHANNEL) == AD9959_CHANNEL_3 ))
void sweep_init(void);

void ad9959_init(void);
void ad9959_reset(void);
void ad9959_io_init(void);
void ad9959_io_update(void);
void ad9959_write_data(AD9959_REG_ADDR register_address, uint8_t number_of_registers, const uint8_t *register_data, bool update);
void ad9959_write_phase(AD9959_CHANNEL channel, uint16_t phase);
void ad9959_write_frequency(AD9959_CHANNEL channel, uint32_t frequency);
void ad9959_write_frequency1(AD9959_CHANNEL channel, uint32_t frequency);
void ad9959_write_amplitude(AD9959_CHANNEL channel, uint16_t amplitude);
void AD9959_Modulation_Init(uint8_t Channel,uint8_t Modulation,uint8_t Sweep_en,uint8_t Nlevel);
void AD9959_SetFre_Sweep(uint8_t Channel, uint32_t s_data,uint32_t e_data,uint32_t r_delta,uint32_t f_delta,uint8_t rsrr,uint8_t fsrr,uint16_t Ampli,uint16_t Phase);
#endif //AD9959_AD9959_H
