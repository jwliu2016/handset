
#ifdef    HYP_GLOBALS
#define   HYP_EXT
#else
#define   HYP_EXT   extern
#endif



/*****************************************************************************/
#define MAX_SERIAL_BUFFER_SIZE		40
#define MAX_PROMPT_BUFFER_SIZE		30
#define MAX_COMMAND_NUM				10

typedef enum {
	PS_WHITESPACE,
	PS_TOKEN,
	PS_STRING,
	PS_ESCAPE
}PARSESTATE;

typedef struct {
	 unchar *CommandName;
	void (*CommandFunc)(unchar argc,  unchar **argv);
	 unchar *HelpString;
}COMMAND;

/*ǰ��ɫ*/
#define    F_BLACK 			"\033[30m" 			/* ��ɫ */
#define    F_RED 			"\033[31m" 			/* ��ɫ */
#define    F_GREEN 			"\033[32m" 			/* ��ɫ */
#define    F_YELLOW 		"\033[33m" 			/* ��ɫ */
#define    F_BLUE 			"\033[34m" 			/* ��ɫ */
#define    F_MAGENTA 		"\033[35m" 			/* ��ɫ */
#define    F_CYAN 			"\033[36m" 			/* ��ɫ */
#define    F_WHITE 			"\033[37m" 			/* ��ɫ */
#define    F_LIGHTRED 		"\033[1;31m"    	/* ���� */
#define    F_LIGHTGREEN 	"\033[1;32m"    	/* ���� */
#define    F_LIGHTYELLOW 	"\033[1;33m"    	/* ���� */
#define    F_LIGHITBLUE 	"\033[1;34m"    	/* ���� */
#define    F_LIGHTMAGENTA	"\033[1;35m"    	/* ���� */
#define    F_LIGHTCYAN 		"\033[1;36m"    	/* ���� */
#define    F_LIGHTWHITE 	"\033[1;37m"    	/* ���� */

/*����ɫ*/
#define    B_BLACK 			"\033[40m" 			/* ��ɫ */
#define    B_RED 			"\033[41m" 			/* ��ɫ */
#define    B_GREEN 			"\033[42m" 			/* ��ɫ */
#define    B_YELLOW 		"\033[43m" 			/* ��ɫ */
#define    B_BLUE 			"\033[44m" 			/* ��ɫ */
#define    B_MAGENTA 		"\033[45m" 			/* ��ɫ */
#define    B_CYAN 			"\033[46m" 			/* ��ɫ */
#define    B_LIGHTRED 		"\033[41;1m" 		/* ���� */
#define    B_LIGHTGREEN		"\033[42;1m" 		/* ���� */
#define    B_LIGHTYELLOW 	"\033[43;1m" 		/* ���� */
#define    B_LIGHTBLUE 		"\033[44;1m" 		/* ���� */
#define    B_LIGHTMAGENTA 	"\033[45;1m" 		/* ���� */
#define    B_LIGHTCYAN 		"\033[46;1m" 		/* ���� */
#define    B_LIGHTWHITE 	"\033[47;1m" 		/* ���� */


#define    BOLDFONT 		"\033[1m" 			/* Set blod font */
#define    UNDERLINEFONT 	"\033[4m" 			/* Set underline font */
#define    CLEARSCREEN 		"\033[2J" 			/* Clear screen */
#define    CURSORHOME 		"\033[H" 			/* Restore cursor to home */
#define    SAVECURSOR		"\033[s" 			/* Save cursor position */
#define    RESTORECURSOR	"\033[u" 			/* Restore cursor to saved position */
#define    SINGLEWIDTH 		"\033#5" 			/* Normal, single-width characters */
#define    DOUBLEWIDTH 		"\033#6" 			/* Creates double-width characters */

#define	   FREEZETOP		"\033[2;25r" 		/* Freeze top line */
#define    FREEZEBOTTOM		"\033[1;24r" 		/* Freeze bottom line */
#define    UNFREEZE_TB		"\033[r" 			/* Unfreeze top line and bottom line */

#define    BLINKTEXT		"\033[5m" 			/* Blink text */		
#define    REVERSEMODE		"\033[7m" 			/* Set hyper terminal to reverse mode */
#define    LIGHTREVERSEMODE	"\033[1,7m" 		/* Set hyper terminal to light reverse mode */

/*****************************************************************************/
HYP_EXT   unchar  SerialBuffer[MAX_SERIAL_BUFFER_SIZE];
HYP_EXT   unchar  PromptBuffer[MAX_PROMPT_BUFFER_SIZE];
HYP_EXT   unchar  CursorPosion;
HYP_EXT   unchar  ExecCommandFlag;
HYP_EXT   COMMAND  CommandList[MAX_COMMAND_NUM];

HYP_EXT   void CREATEFILE(unchar argc,unchar **argv);
HYP_EXT   void CREATEDIR(unchar argc,unchar **argv);
HYP_EXT   void DELDIR(unchar argc,unchar **argv);
HYP_EXT   void DELFILE(unchar argc,unchar **argv);
HYP_EXT   void LISTFILE(unchar argc,unchar **argv);
HYP_EXT   void CHGDIR(unchar argc,unchar **argv);
HYP_EXT   void APPENDDATA(unchar argc,unchar **argv);
HYP_EXT   void READFILE(unchar argc,unchar **argv);
HYP_EXT   void MODIFYFILE(unchar argc,unchar **argv);
HYP_EXT   void SENDFILETOUART(unchar argc,unchar **argv);


HYP_EXT   void InitHyperTerminal(void);
HYP_EXT   void RunHyperTerminal(void);

HYP_EXT   void SerialInterrupt(void);



HYP_EXT   void Help(unchar argc,  unchar **argv);
HYP_EXT   void Prompt(unchar argc,  unchar **argv);
HYP_EXT   void Clear(unchar argc,  unchar **argv);
HYP_EXT   void Reboot(unchar argc,  unchar **argv);



HYP_EXT   void Help(unchar argc, unchar **argv);
HYP_EXT   void Prompt(unchar argc, unchar **argv);
HYP_EXT   void Clear(unchar argc,  unchar **argv);
HYP_EXT   void Reboot(unchar argc,  unchar **argv);
HYP_EXT   void InitHyperTerminal(void);
HYP_EXT   void SerialSendByte(unchar dat);
HYP_EXT   void SerialSendStr(unchar *str);
HYP_EXT   void ParseArgs(unchar *argstr,unchar *argc_p,unchar **argv,unchar **resid);
HYP_EXT   void ExecCommand(unchar *buf);
HYP_EXT   void RunHyperTerminal(void);

/*****************************************************************************/


