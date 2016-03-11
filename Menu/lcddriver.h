#ifdef		LCD_GLOBALS
#define		LCD_EXT
#else
#define		LCD_EXT		extern
#endif

#define	  uchar		unsigned char
#define   uint		unsigned int
#define   _nop_()       asm("nop")
#define   ushort        unsigned short
#define     SCREEN_CLEAR            0XF4        //整屏清屏命令   
#define     SOFT_RESET              0XEF        //复位命令
#define     STR_8X16                0XE9        //固定处: 显示16X16中文和8X16ASCII混合字符串命令 
#define     AREA_DATA               0XC7        //任意指定: 区域块显示数据 清零/全亮/反显
#define     BACK_TOGGLE             0XE5        //背光开关命令

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

