#ifdef FUNC_GLOBALS
#define   FUNC_EXT
#else
#define   FUNC_EXT    extern
#endif
#define POWRON_CY      P6OUT|=(1<<3);P6DIR|=(1<<3)
#define POWROFF_CY     P6OUT&=~(1<<3);P6DIR|=(1<<3)
#define POWRON_WIFI   P6OUT|=(1<<4);P6DIR|=(1<<4)
#define POWROFF_WIFI  P6DIR|=(1<<4);P6OUT&=~(1<<4)
FUNC_EXT  unchar Temp[267];//267
FUNC_EXT  unchar Dely_2s;
FUNC_EXT  unchar WifiState;
FUNC_EXT  unchar WifiState1;
//**************************************
//功能 ：设置ip地址，子网掩码，网关
//************************************
FUNC_EXT    void SetIp(void);
//*******************************
//功能：设置密码
//***********************************
FUNC_EXT    void SetPass(void);

//***************************************
// 功能： 1.5.1设置年月日
//*******************************


FUNC_EXT    void SetYYMMDD(void);
//***************************************
// 功能： 1.5.2设置时分秒
//*******************************


FUNC_EXT    void SetHHMMSS(void);
  //**************************8
  // 1.3.4 基准内阻。          
  //功能： 设置基准内阻。
  //*************************          
FUNC_EXT    void SetResLevel(void );



//***************************************
// 功能： 1.5.1设置年月日
//*******************************


FUNC_EXT  void SetYYMMDD(void);


//***************************************
// 功能： 1.5.2设置时分秒
//*******************************


FUNC_EXT  void SetHHMMSS(void);



  //**************************8
  // 3整组测量。          
  //功能： 实现某组电池的整组测量。
  //************************* 
FUNC_EXT void TotalGrpMeasure(void );

//***********************************************8
//数据查询功能：
//4.数据查询功能；
//*****************************
FUNC_EXT void SqlData(void);





//********************************
//2.1随机测量
//*******************************

FUNC_EXT void RandomMeasure(void);

//******************************8\
//
//5.2实现 WIFI通信功能
FUNC_EXT void OpenWifi(void);

/*
         switch(GetKey1())
         {
         case ENTER_KEY:
           break;
         case UP_KEY:
           
           break;
         case DOWN_KEY:
           break;
         case ESC_KEY:
           break;
         default:
           break;
         }  
*/
//***************************
//随机测量的新版本；
//2.1
//*****************************

FUNC_EXT void RandomMeasure1(void );
FUNC_EXT void SendHttpHead(void);
FUNC_EXT unchar  MAX1(void);
