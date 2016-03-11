
//430��HBLCD�Ĳ��ڲ��Գ���
//�޸�ʱ�䣺2011-12-29
//�޸��ˣ�����
//P4ΪDB0-DB7���ݿڣ�P5.0ΪRST��P1.7ΪREQ��P1.6ΪBUSY
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
	Delay_ms(2);//��ʱ1ms
	RST1;	 
}

void LCD_Init(void)
{
    Delay_ms(800);                 //�ȴ�LCM�ڲ���ʼ�� ��ʱ600ms
    P5DIR |= 0x01;//RST����Ϊ���
    RST1;
    P5DIR |= 0x20;//REQ��Ϊ���
    REQ1;
    P5DIR&=0xBF;//P1.6 SET IN BUSY
    //P1IE|=0x40;//P1.6 �ж�����
    //P1IES=0x00;//P1.6 �������жϴ���
    P4DIR |= 0xff;//PORT4����Ϊ���
    Reset_to_lcm();                 //��LCM��λ
    Delay_ms(800);                //��λ�󻹵õȴ�LCM�ڲ���ʼ��
}

void WriteByteToLcm(uchar ch)
{			
        while (LCDBUSY==(1<<6));   //�ȴ�LCM����(BUSYΪ��)
	P4OUT = ch;  				//���������� 
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
	REQ0;				//REQ=0,��LCD����������        
        Delay_us(100);              //�ȴ�LCM��Ӧ����(BUSYΪ��)   
	REQ1;				//��ԭ�����ź�
}

void Screen_Clear(void)
{
    WriteByteToLcm(SCREEN_CLEAR);
    Delay_ms(20);//���10ms
}

//����һ��E5����򿪱��⣬�ڷ���һ��E5����رձ���
void Back_Toggle(void)
{
    WriteByteToLcm(BACK_TOGGLE);
    Delay_ms(2);//���1ms
} 

void Soft_Reset(void)
{
    WriteByteToLcm(SOFT_RESET);
    Delay_ms(3);//���1.2ms
} 

/************************************************************* 
�������ƣ�Area_Data
�������ܣ�����ָ���������ʾ����/ȫ��/����
���������Fiber-��������/ȫ��/����=0/1/2/3, CursorY-��������Ͻ�
          ������(0-63)��CursorX-��������Ͻ�����(0-127)
          Ls-�����ˮƽ�����ϵĵ�������8���õ��̣�Ly-�����ˮƽ
          �����ϵĵ�������8���õ�����(0-7),High-����鴹ֱ�����
          ����
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
    WriteByteToLcm(High);                 // ��ʾ�ַ� 
    Delay_ms(3);//���1.2ms
}

/************************************************************* 
�������ƣ�Str_8x16
�������ܣ���ָ��λ����ʾ16X16���ĺ�8X16ASCII�ַ���
���������Y-����(0-3),X-����(0-15), *S-��ʾ����
*************************************************************/
void Str_8x16(unsigned char Y, unsigned char X,  unsigned char *s)
{
    WriteByteToLcm(STR_8X16);
    WriteByteToLcm(Y);
    WriteByteToLcm(X);
    while (*s)                          
    { 
        WriteByteToLcm(*s);                 // ��ʾ�ַ� 
        s++; 
    }
    WriteByteToLcm(0X00);                   //������ 
    Delay_ms(4);//���1ms
}






