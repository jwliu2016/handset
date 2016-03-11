#ifdef	MENU_GLOBALS
#define		MENU_EXT
#else
#define		MENU_EXT    extern
#endif
	
#define ENTER_KEY		0x0d
#define ESC_KEY			0x01
#define UP_KEY			0x81
#define DOWN_KEY		0x80
#define LEFT_KEY		0x82
#define RIGHT_KEY		0x83
#define NULLF           ((void(*)(void))0x0000)

#define DispMenuTile(line,s)  Str_8x16(line, 0, s)
#define DispFanBai(line,row,width)      Area_Data(2, line*16, row*8,width, 0, 16)
#define DispFanBaiMenuTitle(line,width)  Area_Data(2, line*16, 0,width,0, 16)

 struct  MENU { 
                  unchar  Horizon;  //菜单在屏幕的哪行显示 
                   unchar  *MenuTitle ;//菜单字符串名字
                  unchar   Width;
                  struct MENU  *pSonMenu;//指向最左边子菜单项的指针
                  struct MENU  *pFatheMenu;//上级目录指针
                  struct MENU  *pBrothePrev;//指向从右向左的兄弟菜单项                  
                  struct MENU  *pBrotheNext;//指向从左向右的兄弟菜单项

                  unchar  page;//当前屏幕显示不下所有的同级菜单项需要分页页数从1-n，如能同屏显示下同级菜单则此处为0
	         void (*CurrentOperation)();
		};

MENU_EXT       struct MENU   MyMenu[32];

MENU_EXT	struct MENU *pCurrentMenu;
MENU_EXT	struct MENU *pCurrentMenu1;
MENU_EXT         unsigned char CurrentPage ;//= pCurrentMenu->page;
MENU_EXT	unsigned int  MenuStatus;
MENU_EXT        unchar FUNCTION_RUNING;
MENU_EXT	void MenuFunction(void);



