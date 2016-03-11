#ifdef	KEY_GLOBALS
#define 	KEY_EXT
#else
#define		KEY_EXT		extern
#endif

#define ENTER_KEY		0x0d
#define ESC_KEY			0x01
#define UP_KEY			0x81
#define DOWN_KEY		0x80
#define LEFT_KEY		0x82
#define RIGHT_KEY		0x83
#define EMPTY 				0
#define NOTEMPTY 			2
#define FULL 				1
#define NOTFULL				3


KEY_EXT         unchar KeyPadState;
KEY_EXT		unchar GetKey1(void);
KEY_EXT		void Init_TimerB(void );
KEY_EXT         void KeyScan(void);
KEY_EXT         void ClrKeyBuf(void);
