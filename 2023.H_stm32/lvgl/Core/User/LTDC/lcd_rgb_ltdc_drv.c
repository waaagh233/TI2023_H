#include "font.h"
#include "lcd_rgb_ltdc_drv.h"
#include "ltdc.h"
#include "gpio.h"

LTDC_HandleTypeDef  LTDC_Handler;	    //LTDC句柄
DMA2D_HandleTypeDef DMA2D_Handler; 	    //DMA2D句柄

//根据不同的颜色格式,定义帧缓存数组
#if LCD_PIXFORMAT==LCD_PIXFORMAT_ARGB8888||LCD_PIXFORMAT==LCD_PIXFORMAT_RGB888
uint32_t n ltdc_lcd_framebuf[1280][800] __attribute__((section(".malloc")));	//定义最大屏分辨率时,LCD所需的帧缓存数组大小
#else
uint16_t ltdc_lcd_framebuf[1280][800] __attribute__((section(".malloc")));	//定义最大屏分辨率时,LCD所需的帧缓存数组大小
#endif

uint32_t *ltdc_framebuf[2];					//LTDC LCD帧缓存数组指针,必须指向对应大小的内存区域
_ltdc_dev lcdltdc;						//管理LCD LTDC的重要参数

//打开LCD开关
//lcd_switch:1 打开,0，关闭
void LTDC_Switch(uint8_t sw)
{
    if(sw==1) __HAL_LTDC_ENABLE(&LTDC_Handler);
    else if(sw==0)__HAL_LTDC_DISABLE(&LTDC_Handler);
}

//开关指定层
//layerx:层号,0,第一层; 1,第二层
//sw:1 打开;0关闭
void LTDC_Layer_Switch(uint8_t layerx,uint8_t sw)
{
    if(sw==1) __HAL_LTDC_LAYER_ENABLE(&LTDC_Handler,layerx);
    else if(sw==0) __HAL_LTDC_LAYER_DISABLE(&LTDC_Handler,layerx);
            __HAL_LTDC_RELOAD_CONFIG(&LTDC_Handler);
}

//选择层
//layerx:层号;0,第一层;1,第二层;
void LTDC_Select_Layer(uint8_t layerx)
{
    lcdltdc.activelayer=layerx;
}

//设置LCD显示方向
//dir:0,竖屏；1,横屏
void LTDC_Display_Dir(uint8_t dir)
{
    lcdltdc.dir=dir; 	//显示方向
    if(dir==0)			//竖屏
    {
        lcdltdc.width=lcdltdc.pheight;
        lcdltdc.height=lcdltdc.pwidth;
    }else if(dir==1)	//横屏
    {
        lcdltdc.width=lcdltdc.pwidth;
        lcdltdc.height=lcdltdc.pheight;
    }
}

//画点函数
//x,y:坐标
//color:颜色值
void LTDC_Draw_Point(uint16_t x,uint16_t y,uint32_t color)
{
#if LCD_PIXFORMAT==LCD_PIXFORMAT_ARGB8888||LCD_PIXFORMAT==LCD_PIXFORMAT_RGB888
    if(lcdltdc.dir)	//横屏
    {
        *(uint32_t*)((uint32_t)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pwidth*y+x))=color;
    }else 			//竖屏
    {
        *(uint32_t*)((uint32_t)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pwidth*(lcdltdc.pheight-x-1)+y))=color;
    }
#else
    if(lcdltdc.dir)	//横屏
	{
        *(uint16_t *)((uint32_t)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pwidth*y+x))=color;
	}else 			//竖屏
	{
        *(uint16_t *)((uint32_t)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pwidth*(lcdltdc.pheight-x-1)+y))=color;
	}
#endif
}

//读点函数
//返回值:颜色值
uint32_t LTDC_Read_Point(uint16_t x,uint16_t y)
{
#if LCD_PIXFORMAT==LCD_PIXFORMAT_ARGB8888||LCD_PIXFORMAT==LCD_PIXFORMAT_RGB888
    if(lcdltdc.dir)	//横屏
    {
        return *(uint32_t*)((uint32_t)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pwidth*y+x));
    }else 			//竖屏
    {
        return *(uint32_t*)((uint32_t)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pwidth*(lcdltdc.pheight-x-1)+y));
    }
#else
    if(lcdltdc.dir)	//横屏
	{
		return *(uint16_t *)((uint32_t)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pwidth*y+x));
	}else 			//竖屏
	{
		return *(uint16_t *)((uint32_t)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pwidth*(lcdltdc.pheight-x-1)+y));
	}
#endif
}
//在指定区域内填充指定颜色块,DMA2D填充	
//此函数仅支持uint16_t,RGB565格式的颜色数组填充.
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)  
//注意:sx,ex,不能大于lcddev.width-1;sy,ey,不能大于lcddev.height-1!!!
//color:要填充的颜色数组首地址
void LTDC_Color_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t *color)
{
    uint32_t psx,psy,pex,pey;	//以LCD面板为基准的坐标系,不随横竖屏变化而变化
    uint32_t timeout=0;
    uint16_t offline;
    uint32_t addr;
    //坐标系转换
    if(lcdltdc.dir)	//横屏
    {
        psx=sx;psy=sy;
        pex=ex;pey=ey;
    }else			//竖屏
    {
        psx=sy;psy=lcdltdc.pheight-ex-1;
        pex=ey;pey=lcdltdc.pheight-sx-1;
    }
    offline=lcdltdc.pwidth-(pex-psx+1);
    addr=((uint32_t)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pwidth*psy+psx));
    RCC->AHB1ENR|=1<<23;			//使能DM2D时钟
    DMA2D->CR=0<<16;				//存储器到存储器模式
    DMA2D->FGPFCCR=LCD_PIXFORMAT;	//设置颜色格式
    DMA2D->FGOR=0;					//前景层行偏移为0
    DMA2D->OOR=offline;				//设置行偏移 
    DMA2D->CR&=~(1<<0);				//先停止DMA2D
    DMA2D->FGMAR=(uint32_t)color;		//源地址
    DMA2D->OMAR=addr;				//输出存储器地址
    DMA2D->NLR=(pey-psy+1)|((pex-psx+1)<<16);	//设定行数寄存器 
    DMA2D->CR|=1<<0;				//启动DMA2D
    while((DMA2D->ISR&(1<<1))==0)	//等待传输完成
    {
        timeout++;
        if(timeout>0X1FFFFF)break;	//超时退出
    }
    DMA2D->IFCR|=1<<1;				//清除传输完成标志  	
}

//LTDC填充矩形,DMA2D填充
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)
//注意:sx,ex,不能大于lcddev.width-1;sy,ey,不能大于lcddev.height-1!!!
//color:要填充的颜色
void LTDC_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint32_t color)
{
    uint32_t psx,psy,pex,pey;	//以LCD面板为基准的坐标系,不随横竖屏变化而变化
    uint32_t timeout=0;
    uint16_t offline;
    uint32_t addr;
    //坐标系转换
    if(lcdltdc.dir)	//横屏
    {
        psx=sx;psy=sy;
        pex=ex;pey=ey;
    }else			//竖屏
    {
        psx=sy;psy=lcdltdc.pheight-ex-1;
        pex=ey;pey=lcdltdc.pheight-sx-1;
    }
    offline=lcdltdc.pwidth-(pex-psx+1);
    addr=((uint32_t)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pwidth*psy+psx));
    RCC->AHB1ENR|=1<<23;			//使能DM2D时钟
    DMA2D->CR=3<<16;				//寄存器到存储器模式
    DMA2D->OPFCCR=LCD_PIXFORMAT;	//设置颜色格式
    DMA2D->OOR=offline;				//设置行偏移
    DMA2D->CR&=~(1<<0);				//先停止DMA2D
    DMA2D->OMAR=addr;				//输出存储器地址
    DMA2D->NLR=(pey-psy+1)|((pex-psx+1)<<16);	//设定行数寄存器
    DMA2D->OCOLR=color;				//设定输出颜色寄存器
    DMA2D->CR|=1<<0;				//启动DMA2D
    while((DMA2D->ISR&(1<<1))==0)	//等待传输完成
    {
        timeout++;
        if(timeout>0X1FFFFF)break;	//超时退出
    }
    DMA2D->IFCR|=1<<1;				//清除传输完成标志
}

//LCD清屏
//color:颜色值
void LTDC_Clear(uint32_t color)
{
    LTDC_Fill(0,0,lcdltdc.width-1,lcdltdc.height-1,color);
}

//LTDC时钟(Fdclk)设置函数
//PLL3_VCO Input=HSE_VALUE/PLL3M
//PLL3_VCO Output=PLL3_VCO Input * PLL3N
//PLLLCDCLK = PLL3_VCO Output/PLL3R
//假如HSE_VALUE=25MHz，PLL3M=5，PLL3N=160,PLL3R=88
//LTDCLK=PLLLCDCLK=25/5*160/88=9MHz
//
//返回值:0,成功;1,失败。
uint8_t LTDC_Clk_Set(uint32_t pll3m,uint32_t pll3n,uint32_t pll3r)
{
    RCC_PeriphCLKInitTypeDef PeriphClkIniture;
    PeriphClkIniture.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
    PeriphClkIniture.PLL3.PLL3M = pll3m;
    PeriphClkIniture.PLL3.PLL3N = pll3n;
    PeriphClkIniture.PLL3.PLL3P = 2;
    PeriphClkIniture.PLL3.PLL3Q = 2;
    PeriphClkIniture.PLL3.PLL3R = pll3r;
    if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkIniture)==HAL_OK) //配置像素时钟，这里配置为时钟为18.75MHZ
    {
        return 0;   //成功
    }
    else return 1;  //失败
}
//
//读取面板参数,新增了判断悬空全一的排查
//PG6=R7(M0);PI2=G7(M1);PI7=B7(M2);
//M2:M1:M0
//0 :0 :0	//4.3寸480*272 RGB屏,ID=0X4342
//0 :0 :1	//7寸800*480 RGB屏,ID=0X7084
//0 :1 :0	//7寸1024*600 RGB屏,ID=0X7016
//0 :1 :1	//7寸1280*800 RGB屏,ID=0X7018
//1 :0 :0	//4.3寸800*480 RGB屏,ID=0X4384
//1 :0 :1   //10.1寸1280*800,RGB屏,ID=0X1018
//1 :1 :1   //VGA显示器
//返回值:LCD ID:0,非法;其他值,ID;
uint16_t LTDC_PanelID_Read(void)
{
    uint8_t idx=0;
    idx=(uint8_t)HAL_GPIO_ReadPin(GPIOG,GPIO_PIN_6); //读取M0
    idx|=(uint8_t)HAL_GPIO_ReadPin(GPIOI,GPIO_PIN_2)<<1;//读取M1
    idx|=(uint8_t)HAL_GPIO_ReadPin(GPIOI,GPIO_PIN_7)<<2;//读取M2
    if(idx==0) return 0X4342;	    //4.3寸屏,480*272分辨率
    else if(idx==1)return 0X7084;	//7寸屏,800*480分辨率
    else if(idx==2)return 0X7016;	//7寸屏,1024*600分辨率
    else if(idx==3)return 0X7018;	//7寸屏,1280*800分辨率
    else if(idx==4)return 0X4384;	//4.3寸屏,800*480分辨率
    else if(idx==5)return 0X1018;	//10.1寸屏,1280*800分辨率
    else return 0;
}


//LCD初始化函数
void LTDC_Init(void)
{
    uint16_t lcdid=0;
//    lcdid=0X4384;
    lcdid=0X7016;
    if(lcdid==0X4342)
    {
        lcdltdc.pwidth=480;			    //面板宽度,单位:像素
        lcdltdc.pheight=272;		    //面板高度,单位:像素
        lcdltdc.hsw=1;				    //水平同步宽度
        lcdltdc.vsw=1;				    //垂直同步宽度
        lcdltdc.hbp=40;				    //水平后廊
        lcdltdc.vbp=8;				    //垂直后廊
        lcdltdc.hfp=5;				    //水平前廊
        lcdltdc.vfp=8;				    //垂直前廊
        LTDC_Clk_Set(5,160,88);			//设置像素时钟 9Mhz
        //其他参数待定.
    }else if(lcdid==0X7084)
    {
        lcdltdc.pwidth=800;			    //面板宽度,单位:像素
        lcdltdc.pheight=480;		    //面板高度,单位:像素
        lcdltdc.hsw=1;				    //水平同步宽度
        lcdltdc.vsw=1;				    //垂直同步宽度
        lcdltdc.hbp=46;				    //水平后廊
        lcdltdc.vbp=23;				    //垂直后廊
        lcdltdc.hfp=210;			    //水平前廊
        lcdltdc.vfp=22;				    //垂直前廊
        LTDC_Clk_Set(5,160,28);			//设置像素时钟 33M(如果开双显,需要降低DCLK到:18.75Mhz  pll3r=43,才会比较好)
    }else if(lcdid==0X7016)
    {
        lcdltdc.pwidth=1024;			//面板宽度,单位:像素
        lcdltdc.pheight=600;			//面板高度,单位:像素
        lcdltdc.hsw=20;				    //水平同步宽度
        lcdltdc.vsw=3;				    //垂直同步宽度
        lcdltdc.hbp=140;			    //水平后廊
        lcdltdc.vbp=20;				    //垂直后廊
        lcdltdc.hfp=160;			    //水平前廊
        lcdltdc.vfp=12;				    //垂直前廊
        LTDC_Clk_Set(5,160,20);			//设置像素时钟  40Mhz
        //其他参数待定.
    }else if(lcdid==0X7018)
    {
        lcdltdc.pwidth=1280;			//面板宽度,单位:像素
        lcdltdc.pheight=800;			//面板高度,单位:像素
        //其他参数待定.
    }else if(lcdid==0X4384)
    {
        lcdltdc.pwidth=800;			    //面板宽度,单位:像素
        lcdltdc.pheight=480;		    //面板高度,单位:像素
        lcdltdc.hbp=88;				    //水平后廊
        lcdltdc.hfp=40;			    	//水平前廊
        lcdltdc.hsw=48;				    //水平同步宽度
        lcdltdc.vbp=32;				    //垂直后廊
        lcdltdc.vfp=13;				    //垂直前廊
        lcdltdc.vsw=3;				    //垂直同步宽度
        LTDC_Clk_Set(5,160,24);			//设置像素时钟 33M
        //其他参数待定.
    }else if(lcdid==0X1018)				//10.1寸1280*800 RGB屏
    {
        lcdltdc.pwidth=1280;			//面板宽度,单位:像素
        lcdltdc.pheight=800;			//面板高度,单位:像素
        lcdltdc.hbp=140;				//水平后廊
        lcdltdc.hfp=10;			    	//水平前廊
        lcdltdc.hsw=10;					//水平同步宽度
        lcdltdc.vbp=10;					//垂直后廊
        lcdltdc.vfp=10;					//垂直前廊
        lcdltdc.vsw=3;					//垂直同步宽度
        LTDC_Clk_Set(5,160,16);			//设置像素时钟 50MHz
    }	else if(lcdid==0XA001)              //接VGA显示器
    {
        lcdltdc.pwidth=1366;		    //面板宽 度,单位:像素
        lcdltdc.pheight=768;		    //面板高度,单位:像素
        lcdltdc.hsw=143;			    //水平同步宽度
        lcdltdc.hfp=70;			        //水平前廊
        lcdltdc.hbp=213;			    //水平后廊
        lcdltdc.vsw=3;				    //垂直同步宽度
        lcdltdc.vbp=24;				    //垂直后廊
        lcdltdc.vfp=3;				    //垂直前廊
        LTDC_Clk_Set(5,160,28);//设置像素时钟 28.5Mhz

    }else if(lcdid==0XA002)            //接VGA显示器
    {
        lcdltdc.pwidth=1280;		    //面板宽度,单位:像素
        lcdltdc.pheight=800;		    //面板高度,单位:像素
        lcdltdc.hsw=32;			        //水平同步宽度
        lcdltdc.hfp=48;			        //水平前廊
        lcdltdc.hbp=80;			        //水平后廊
        lcdltdc.vsw=6;				    //垂直同步宽度
        lcdltdc.vbp=14;				    //垂直后廊
        lcdltdc.vfp=3;				    //垂直前廊
        LTDC_Clk_Set(5,160,22);//设置像素时钟 35.5Mhz
    }else if(lcdid==0XA003)	        //接VGA显示器
    {
        lcdltdc.pwidth=1280;		    //面板宽度,单位:像素
        lcdltdc.pheight=768;		    //面板高度,单位:像素
        lcdltdc.hsw=32;				    //水平同步宽度
        lcdltdc.hbp=80;			        //水平后廊
        lcdltdc.hfp=48;			        //水平前廊
        lcdltdc.vsw=7;				    //垂直同步宽度
        lcdltdc.vbp=12;				    //垂直后廊
        lcdltdc.vfp=3;				    //垂直前廊
        LTDC_Clk_Set(5,160,23);//设置像素时钟  34.25Mhz
    }else if(lcdid==0XA004)            //接VGA显示器
    {
        lcdltdc.pwidth=1024;		    //面板宽度,单位:像素
        lcdltdc.pheight=768;		    //面板高度,单位:像素
        lcdltdc.hsw=136;			    //水平同步宽度
        lcdltdc.hfp=24;			        //水平前廊
        lcdltdc.hbp=160;			    //水平后廊
        lcdltdc.vsw=6;				    //垂直同步宽度
        lcdltdc.vbp=29;				    //垂直后廊
        lcdltdc.vfp=3;				    //垂直前廊
        LTDC_Clk_Set(5,160,18);//设置像素时钟 43.25Mhz
    }else if(lcdid==0XA005)            //接VGA显示器
    {
        lcdltdc.pwidth=848;		        //面板宽度,单位:像素
        lcdltdc.pheight=480;		    //面板高度,单位:像素
        lcdltdc.hsw=112;		        //水平同步宽度
        lcdltdc.hbp=112;			    //水平后廊
        lcdltdc.hfp=16;			        //水平前廊
        lcdltdc.vsw=8;				    //垂直同步宽度
        lcdltdc.vbp=23;				    //垂直后廊
        lcdltdc.vfp=6;				    //垂直前廊
        LTDC_Clk_Set(5,160,47);//设置像素时钟 17Mhz
    }else if(lcdid==0XA006)            //接VGA显示器
    {
        lcdltdc.pwidth=800;		        //面板宽度,单位:像素
        lcdltdc.pheight=600;		    //面板高度,单位:像素
        lcdltdc.hsw=128;			    //水平同步宽度
        lcdltdc.hbp=88;			        //水平后廊
        lcdltdc.hfp=40;			        //水平前廊
        lcdltdc.vsw=4;				    //垂直同步宽度
        lcdltdc.vbp=23;				    //垂直后廊
        lcdltdc.vfp=1;				    //垂直前廊
        LTDC_Clk_Set(5,160,40);//设置像素时钟 20mz
    }else if(lcdid==0XA007)            //接VGA显示器
    {
        lcdltdc.pwidth=640;		        //面板宽度,单位:像素
        lcdltdc.pheight=480;		    //面板高度,单位:像素
        lcdltdc.hsw=96;				    //水平同步宽度
        lcdltdc.hfp=8;			        //水平前廊
        lcdltdc.hbp=56;			        //水平后廊
        lcdltdc.vsw=2;				    //垂直同步宽度
        lcdltdc.vbp=41;				    //垂直后廊
        lcdltdc.vfp=2;				    //垂直前廊
        LTDC_Clk_Set(5,160,62);//设置像素时钟 12.75Mhz
    }
    lcdltdc.width=lcdltdc.pwidth;
    lcdltdc.height=lcdltdc.pheight;
    lcdltdc.id=lcdid;
    lcdltdc.dir=1;
#if LCD_PIXFORMAT==LCD_PIXFORMAT_ARGB8888||LCD_PIXFORMAT==LCD_PIXFORMAT_RGB888
    ltdc_framebuf[0]=(uint32_t*)&ltdc_lcd_framebuf;
	lcdltdc.pixsize=4;				//每个像素占4个字节
#else
    lcdltdc.pixsize=2;				//每个像素占2个字节
    ltdc_framebuf[0]=(uint32_t *)&ltdc_lcd_framebuf;
#endif

}

void LTDC_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint32_t color)
{
    uint16_t t;
    int xerr=0,yerr=0,delta_x,delta_y,distance;
    int incx,incy,uRow,uCol;
    delta_x=x2-x1; //计算坐标增量
    delta_y=y2-y1;
    uRow=x1;
    uCol=y1;
    if(delta_x>0)incx=1; //设置单步方向
    else if(delta_x==0)incx=0;//垂直线
    else {incx=-1;delta_x=-delta_x;}
    if(delta_y>0)incy=1;
    else if(delta_y==0)incy=0;//水平线
    else{incy=-1;delta_y=-delta_y;}
    if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴
    else distance=delta_y;
    for(t=0;t<=distance+1;t++ )//画线输出
    {
        LTDC_Draw_Point(uRow,uCol,color);//画点
        xerr+=delta_x ;
        yerr+=delta_y ;
        if(xerr>distance)
        {
            xerr-=distance;
            uRow+=incx;
        }
        if(yerr>distance)
        {
            yerr-=distance;
            uCol+=incy;
        }
    }
}
//画矩形
//(x1,y1),(x2,y2):矩形的对角坐标
void LTDC_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint32_t color)
{
    LTDC_DrawLine(x1,y1,x2,y1,color);
    LTDC_DrawLine(x1,y1,x1,y2,color);
    LTDC_DrawLine(x1,y2,x2,y2,color);
    LTDC_DrawLine(x2,y1,x2,y2,color);
}
//在指定位置画一个指定大小的圆
//(x,y):中心点
//r    :半径
void LTDC_Draw_Circle(uint16_t x0,uint16_t y0,uint16_t r,uint32_t color)
{
    int a,b;
    int di;
    a=0;b=r;
    di=3-(r<<1);             //判断下个点位置的标志
    while(a<=b)
    {
        LTDC_Draw_Point(x0+a,y0-b,color);             //5
        LTDC_Draw_Point(x0+b,y0-a,color);             //0
        LTDC_Draw_Point(x0+b,y0+a,color);             //4
        LTDC_Draw_Point(x0+a,y0+b,color);             //6
        LTDC_Draw_Point(x0-a,y0+b,color);             //1
        LTDC_Draw_Point(x0-b,y0+a,color);
        LTDC_Draw_Point(x0-a,y0-b,color);             //2
        LTDC_Draw_Point(x0-b,y0-a,color);             //7
        a++;
        //使用Bresenham算法画圆
        if(di<0)di +=4*a+6;
        else
        {
            di+=10+4*(a-b);
            b--;
        }
    }
}

//在指定位置显示一个字符
//x,y:起始坐标
//num:要显示的字符:" "--->"~"
//size:字体大小 12/16/24/32
//mode:叠加方式(1)还是非叠加方式(0)
void LTDC_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint8_t size,uint8_t mode,uint32_t color)
{
    uint8_t temp,t1,t;
    uint16_t y0=y;
    uint8_t csize=(size/8+((size%8)?1:0))*(size/2);		//得到字体一个字符对应点阵集所占的字节数
    num=num-' ';//得到偏移后的值（ASCII字库是从空格开始取模，所以-' '就是对应字符的字库）
    for(t=0;t<csize;t++)
    {
        if(size==12)temp=asc2_1206[num][t]; 	 	//调用1206字体
        else if(size==16)temp=asc2_1608[num][t];	//调用1608字体
        else if(size==24)temp=asc2_2412[num][t];	//调用2412字体
        else if(size==32)temp=asc2_3216[num][t];	//调用3216字体
        else return;								//没有的字库
        for(t1=0;t1<8;t1++)
        {
            if(temp&0x80)LTDC_Draw_Point(x,y,color);
            else if(mode==0)LTDC_Draw_Point(x,y,color);
            temp<<=1;
            y++;
            if(y>=lcdltdc.height)return;		//超区域了
            if((y-y0)==size)
            {
                y=y0;
                x++;
                if(x>=lcdltdc.width)return;	//超区域了
                break;
            }
        }
    }
}
//m^n函数
//返回值:m^n次方.
uint32_t LTDC_Pow(uint8_t m,uint8_t n)
{
    uint32_t result=1;
    while(n--)result*=m;
    return result;
}
//显示数字,高位为0,则不显示
//x,y :起点坐标
//len :数字的位数
//size:字体大小
//color:颜色
//num:数值(0~4294967295);
void LTDC_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size,uint32_t color)
{
    uint8_t t,temp;
    uint8_t enshow=0;
    for(t=0;t<len;t++)
    {
        temp=(num/LTDC_Pow(10,len-t-1))%10;
        if(enshow==0&&t<(len-1))
        {
            if(temp==0)
            {
                LTDC_ShowChar(x+(size/2)*t,y,' ',size,1,color);
                continue;
            }else enshow=1;

        }
        LTDC_ShowChar(x+(size/2)*t,y,temp+'0',size,1,color);
    }
}
//显示数字,高位为0,还是显示
//x,y:起点坐标
//num:数值(0~999999999);
//len:长度(即要显示的位数)
//size:字体大小
//mode:
//[7]:0,不填充;1,填充0.
//[6:1]:保留
//[0]:0,非叠加显示;1,叠加显示.
void LTDC_ShowxNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size,uint8_t mode,uint32_t color)
{
    uint8_t t,temp;
    uint8_t enshow=0;
    for(t=0;t<len;t++)
    {
        temp=(num/LTDC_Pow(10,len-t-1))%10;
        if(enshow==0&&t<(len-1))
        {
            if(temp==0)
            {
                if(mode&0X80)LTDC_ShowChar(x+(size/2)*t,y,'0',size,mode&0X01,color);
                else LTDC_ShowChar(x+(size/2)*t,y,' ',size,mode&0X01,color);
                continue;
            }else enshow=1;

        }
        LTDC_ShowChar(x+(size/2)*t,y,temp+'0',size,mode&0X01,color);
    }
}
//显示字符串
//x,y:起点坐标
//width,height:区域大小
//size:字体大小
//*p:字符串起始地址
void LTDC_ShowString(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t size,uint8_t *p,uint32_t color)
{
    uint8_t x0=x;
    width+=x;
    height+=y;
    while((*p<='~')&&(*p>=' '))//判断是不是非法字符!
    {
        if(x>=width){x=x0;y+=size;}
        if(y>=height)break;//退出
        LTDC_ShowChar(x,y,*p,size,1,color);
        x+=size/2;
        p++;
    }
}

//显示一个指定大小的汉字
//x,y :汉字的坐标
//index:汉字在数组中的位置
//size:字体大小
//pointcolor:画笔颜色
//backcolor：背景颜色
//mode:0,正常显示,1,叠加显示
void LTDC_Show_Chinese(uint16_t x,uint16_t y,uint16_t index,uint8_t size,uint16_t pointcolor,uint16_t backcolor,uint8_t mode)
{
    uint8_t temp,t,t1;
    uint16_t y0=y;
    uint8_t csize=(size/8+((size%8)?1:0))*(size);	//一个汉字数据字节数，直接存就可比如size=24一个汉字是｛72数据｝，16是32
    for(t=0;t<csize;t++)
    {
        temp=Hz[index][t];			//得到点阵数据
        for(t1=0;t1<8;t1++)
        {
            if(temp&0x80)LTDC_Draw_Point(x,y,pointcolor);
            else if(mode==0)
                LTDC_Draw_Point(x,y,backcolor);
            temp<<=1;
            y++;
            if((y-y0)==size)
            {
                y=y0;
                x++;
                break;
            }
        }
    }
}

