
//430的HBLCD的并口测试程序
//修改时间：2011-12-29
//修改人：杨贤
//P4为DB0-DB7数据口，P5.0为RST，P1.7为REQ，P1.6为BUSY
#define	 LCD_GLOBALS
#include "..\include.h"








void Delay_us(uint time)
{
  while (time--)
    _NOP();
}

void Delay_ms(uint time) 
{
  while (time--)
    Delay_us(860);
} 

void  Reset_to_lcm(void)
{
	RST0;                    //RESET PIN TO LCM
	Delay_ms(2);//延时1ms
	RST1;	 
}

void LCD_Init(void)
{
    Delay_ms(800);                 //等待LCM内部初始化 延时600ms
    P5DIR |= 0x01;//RST设置为输出
    RST1;
    P5DIR |= 0x20;//REQ置为输出
    REQ1;
    P5DIR&=0xBF;//P1.6 SET IN BUSY
    //P1IE|=0x40;//P1.6 中断允许
    //P1IES=0x00;//P1.6 上升沿中断触发
    P4DIR |= 0xff;//PORT4设置为输出
    Reset_to_lcm();                 //对LCM复位
    Delay_ms(800);                //复位后还得等待LCM内部初始化
}

void WriteByteToLcm(uchar ch)
{			
        while (LCDBUSY==(1<<6));   //等待LCM空闲(BUSY为低)
	P4OUT = ch;  				//向总线送数 
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
	REQ0;				//REQ=0,向LCD发请求命令        
        Delay_us(100);              //等待LCM相应请求(BUSY为高)   
	REQ1;				//复原请求信号
}

void Screen_Clear(void)
{
    WriteByteToLcm(SCREEN_CLEAR);
    Delay_ms(20);//最大10ms
}

//发送一次E5命令，打开背光，在发送一次E5命令，关闭背光
void Back_Toggle(void)
{
    WriteByteToLcm(BACK_TOGGLE);
    Delay_ms(2);//最大1ms
} 

void Soft_Reset(void)
{
    WriteByteToLcm(SOFT_RESET);
    Delay_ms(3);//最大1.2ms
} 

/************************************************************* 
函数名称：Area_Data
函数功能：任意指定区域块显示数据/全亮/反显
输入参数：Fiber-数据清零/全亮/反显=0/1/2/3, CursorY-区域块左上角
          行坐标(0-63)，CursorX-区域块左上角坐标(0-127)
          Ls-区域块水平方向上的点数除以8所得的商，Ly-区域块水平
          方向上的点数除以8所得的余数(0-7),High-区域块垂直方向的
          行数
*************************************************************/
void Area_Data(unsigned char Fiber, unsigned char CursorY, unsigned char CursorX, 
                          unsigned char Ls, unsigned char Ly, unsigned char High)
{
    WriteByteToLcm(AREA_DATA);
    WriteByteToLcm(Fiber);
    WriteByteToLcm(CursorY);
    WriteByteToLcm(CursorX); 
    WriteByteToLcm(Ls);
    WriteByteToLcm(Ly);
    WriteByteToLcm(High);                 // 显示字符 
    Delay_ms(3);//最大1.2ms
}

/************************************************************* 
函数名称：Str_8x16
函数功能：在指定位置显示16X16中文和8X16ASCII字符串
输入参数：Y-行数(0-3),X-列数(0-15), *S-显示内容
*************************************************************/
void Str_8x16(unsigned char Y, unsigned char X,  unsigned char *s)
{
    WriteByteToLcm(STR_8X16);
    WriteByteToLcm(Y);
    WriteByteToLcm(X);
    while (*s)                          
    { 
        WriteByteToLcm(*s);                 // 显示字符 
        s++; 
    }
    WriteByteToLcm(0X00);                   //结束符 
    Delay_ms(4);//最大1ms
}






