#ifndef _LCD_H
#define _LCD_H
#include "ltdc.h"
#include "gpio.h"

//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000
#define BLUE         	 0x001F
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色
//GUI颜色

#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色

#define LIGHTGREEN     	 0X841F //浅绿色
//#define LIGHTGRAY        0XEF5B //浅灰色(PANNEL)
#define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)

//LCD LTDC重要参数集
typedef struct
{
    uint16_t id;
    uint32_t pwidth;			//LCD面板的宽度,固定参数,不随显示方向改变,如果为0,说明没有任何RGB屏接入
    uint32_t pheight;		//LCD面板的高度,固定参数,不随显示方向改变
    uint16_t hsw;			//水平同步宽度
    uint16_t vsw;			//垂直同步宽度
    uint16_t hbp;			//水平后廊
    uint16_t vbp;			//垂直后廊
    uint16_t hfp;			//水平前廊
    uint16_t vfp;			//垂直前廊
    uint8_t activelayer;		//当前层编号:0/1
    uint8_t dir;				//0,竖屏;1,横屏;
    uint16_t width;			//LCD宽度
    uint16_t height;			//LCD高度
    uint32_t pixsize;		//每个像素所占字节数
}_ltdc_dev;



extern _ltdc_dev lcdltdc;		            //管理LCD LTDC参数
extern LTDC_HandleTypeDef LTDC_Handler;	    //LTDC句柄
extern DMA2D_HandleTypeDef DMA2D_Handler;   //DMA2D句柄
extern uint32_t *ltdc_framebuf[2];					//LTDC LCD帧缓存数组指针,必须指向对应大小的内存区域

#define LCD_PIXEL_FORMAT_ARGB8888       0X00
#define LCD_PIXEL_FORMAT_RGB888         0X01
#define LCD_PIXEL_FORMAT_RGB565         0X02
#define LCD_PIXEL_FORMAT_ARGB1555       0X03
#define LCD_PIXEL_FORMAT_ARGB4444       0X04
#define LCD_PIXEL_FORMAT_L8             0X05
#define LCD_PIXEL_FORMAT_AL44           0X06
#define LCD_PIXEL_FORMAT_AL88           0X07

///////////////////////////////////////////////////////////////////////
//用户修改配置部分:

//定义颜色像素格式,一般用RGB565
#define LCD_PIXFORMAT				LCD_PIXEL_FORMAT_RGB565
//定义默认背景层颜色
#define LTDC_BACKLAYERCOLOR			0X00000000
//LCD帧缓冲区首地址,这里定义在SDRAM里面.
#define LCD_FRAME_BUF_ADDR			0XC0000000

void LTDC_Switch(uint8_t sw);					//LTDC开关
void LTDC_Layer_Switch(uint8_t layerx,uint8_t sw);	//层开关
void LTDC_Select_Layer(uint8_t layerx);			//层选择
void LTDC_Display_Dir(uint8_t dir);				//显示方向控制
void LTDC_Draw_Point(uint16_t x,uint16_t y,uint32_t color);//画点函数
uint32_t LTDC_Read_Point(uint16_t x,uint16_t y);			//读点函数
void LTDC_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint32_t color);			//矩形单色填充函数
void LTDC_Color_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t *color);	//矩形彩色填充函数
void LTDC_Clear(uint32_t color);					//清屏函数
uint8_t LTDC_Clk_Set(uint32_t pll3m,uint32_t pll3n,uint32_t pll3r);//LTDC时钟配置
//u8 LTDC_Clk_Set(u32 pllsain,u32 pllsair,u32 pllsaidivr);//LTDC时钟配置
void LTDC_Layer_Window_Config(uint8_t layerx,uint16_t sx,uint16_t sy,uint16_t width,uint16_t height);//LTDC层窗口设置
void LTDC_Layer_Parameter_Config(uint8_t layerx,uint32_t bufaddr,uint8_t pixformat,uint8_t alpha,uint8_t alpha0,uint8_t bfac1,uint8_t bfac2,uint32_t bkcolor);//LTDC基本参数设置
uint16_t LTDC_PanelID_Read(void);				//LCD ID读取函数
void LTDC_Init(void);						//LTDC初始化函数
void LTDC_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint32_t color);
void LTDC_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint32_t color);
void LTDC_Draw_Circle(uint16_t x0,uint16_t y0,uint16_t r,uint32_t color);
void LTDC_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint8_t size,uint8_t mode,uint32_t color);
void LTDC_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size,uint32_t color);
void LTDC_ShowString(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t size,uint8_t *p,uint32_t color);
void LTDC_Show_Chinese(uint16_t x,uint16_t y,uint16_t index,uint8_t size,uint16_t pointcolor,uint16_t backcolor,uint8_t mode);
#endif
