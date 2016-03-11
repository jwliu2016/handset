#ifdef		LCD_GLOBALS
#define		LCD_EXT
#else
#define		LCD_EXT		extern
#endif

#define	  uchar		unsigned char
#define   uint		unsigned int
#define   _nop_()       asm("nop")
#define   ushort        unsigned short
#define     SCREEN_CLEAR            0XF4        //������������   
#define     SOFT_RESET              0XEF        //��λ����
#define     STR_8X16                0XE9        //�̶���: ��ʾ16X16���ĺ�8X16ASCII����ַ������� 
#define     AREA_DATA               0XC7        //����ָ��: �������ʾ���� ����/ȫ��/����
#define     BACK_TOGGLE             0XE5        //���⿪������

#define     REQ1  P5OUT|=0x20
#define     REQ0  P5OUT&=0xDF
#define     RST1  P5OUT|=0x01
#define     RST0  P5OUT&=0xFE
#define	    LCDBUSY  (P5IN&(1<<6))
LCD_EXT         void Delay_us(uint time);
LCD_EXT         void Delay_ms(uint time);
LCD_EXT		void  Reset_to_lcm(void);
LCD_EXT		void LCD_Init(void);
LCD_EXT		void WriteByteToLcm(uchar ch);
LCD_EXT		void Screen_Clear(void);
LCD_EXT		void Back_Toggle(void);
LCD_EXT		void Soft_Reset(void);
LCD_EXT		void Area_Data(unsigned char Fiber, unsigned char CursorY, unsigned char CursorX, 
                                unsigned char Ls, unsigned char Ly, unsigned char High);
LCD_EXT		void Str_8x16(unsigned char Y, unsigned char X,  unsigned char *s);

