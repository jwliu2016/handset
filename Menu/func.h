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
//���� ������ip��ַ���������룬����
//************************************
FUNC_EXT    void SetIp(void);
//*******************************
//���ܣ���������
//***********************************
FUNC_EXT    void SetPass(void);

//***************************************
// ���ܣ� 1.5.1����������
//*******************************


FUNC_EXT    void SetYYMMDD(void);
//***************************************
// ���ܣ� 1.5.2����ʱ����
//*******************************


FUNC_EXT    void SetHHMMSS(void);
  //**************************8
  // 1.3.4 ��׼���衣          
  //���ܣ� ���û�׼���衣
  //*************************          
FUNC_EXT    void SetResLevel(void );



//***************************************
// ���ܣ� 1.5.1����������
//*******************************


FUNC_EXT  void SetYYMMDD(void);


//***************************************
// ���ܣ� 1.5.2����ʱ����
//*******************************


FUNC_EXT  void SetHHMMSS(void);



  //**************************8
  // 3���������          
  //���ܣ� ʵ��ĳ���ص����������
  //************************* 
FUNC_EXT void TotalGrpMeasure(void );

//***********************************************8
//���ݲ�ѯ���ܣ�
//4.���ݲ�ѯ���ܣ�
//*****************************
FUNC_EXT void SqlData(void);





//********************************
//2.1�������
//*******************************

FUNC_EXT void RandomMeasure(void);

//******************************8\
//
//5.2ʵ�� WIFIͨ�Ź���
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
//����������°汾��
//2.1
//*****************************

FUNC_EXT void RandomMeasure1(void );
FUNC_EXT void SendHttpHead(void);
FUNC_EXT unchar  MAX1(void);
