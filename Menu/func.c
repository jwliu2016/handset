#define FUNC_GLOBALS
#include "..\include.h"


static unint Index=0;
static unchar ModefyModel = 0;
static unint CurrentBatGrp;
const unint BatVoltLevel[] ={2,6,12};
/*
 const unsigned char pucHttpHead[5][] = {
    "HTTP/1.1 200 OK\r\n"
    "Server: microHttp/1.0 Xxxmcu Corporation\r\n"
    "Accept-Ranges: bytes\r\n"
    "Connection: Keep-Close\r\n"
    "Content-Type: text/html\r\n"
    "\r\n"
};
*/
void SendHttpHead(void)
{
  Init_Uart1;
  
  PutStr1( "HTTP/1.1 200 OK\r\n");
  PutStr1( "Content-Type:text/xml\r\n");
  PutStr1( "Last-Modified:Sat,08 Sep 2012 02:57:04 GMT\r\n");
  PutStr1( "Accept-Ranges:bytes\r\n");
  PutStr1( "ETag:\"08010a06d8dcd1:0\"\r\n");
  PutStr1( "Server:Microsoft-IIS/7.0\r\n");
  PutStr1( "X-Powered-By:ASP.NET\r\n");
  PutStr1( "Date:Thu,10 Jan 2013 07:21:19 GMT\r\n");
  PutStr1( "Content-Length:2296\r\n");
 
}
//*************************************
//�ж�ÿ�������ı�Ҫ������
//*************************************
unchar  LEAP1(void)
{
  if((Temp[0]+2000)%4==0)
  {
    if((Temp[0]+2000)%100 ==0)
    {
      if((Temp[0]+2000)%400 ==0)
      return 1;
    else
      return 0;
    }
    else
      return 1;
    
  }
  else
    return  0;  
  
}
unchar  MAX1(void)
{
 
  if((Temp[1]==4)||(Temp[1]==6)||(Temp[1]==9)||(Temp[1]==11))
    return 30;
  else
    if(Temp[1]==2)
    {
      if(LEAP1()==1)
        return 29;
      else
        return 28;
      
    }
  else
    return 31;
}

//**************************************
//���� ������ip��ַ���������룬����
//************************************
void SetIp(void)
{
  unchar s[5];
 switch(MenuStatus)
 {
 case 2:
   if(CURPATH.CurrentDirStarClusID !=0)
   {
     CURPATH.CurrentDirStarClusID = 0;
   }
   unint32 offset;
   if(pCurrentMenu == &MyMenu[16])//�����������IP��ַ��           
   {
    Str_8x16(0, 0,"IP��ַ����" ); 
    Str_8x16(0, 0,"IP��ַ����" );     
    offset =4;
   }
   if(pCurrentMenu == &MyMenu[17])//������������������롣           
   {
    Str_8x16(0, 0,"������������" );
    Str_8x16(0, 0,"������������" );  
    offset=8;
   }
   if(pCurrentMenu == &MyMenu[18])//��������������ص�ַ��           
   {
    Str_8x16(0, 0,"��������" ); 
    Str_8x16(0, 0,"��������" ); 
    offset=12;
   }   
   if((ReadFile("CONFIG  TXT", Temp, offset,4)!=0xff))
   {
     //��ʾip

     s[0] = Temp[0]/100+0x30;
     s[1] = (Temp[0]%100)/10+0x30;
     s[2] = (Temp[0]%100)%10+0x30;
     s[3]= '.';
     s[4] =0;
     Str_8x16(1, 0, s);
     Str_8x16(1, 0, s);
     
     s[0] = Temp[1]/100+0x30;
     s[1] = (Temp[1]%100)/10+0x30;
     s[2] = (Temp[1]%100)%10+0x30;
     s[3]= '.';
     s[4] = 0;
     Str_8x16(1, 4, s);
     Str_8x16(1, 4, s);

     s[0] = Temp[2]/100+0x30;
     s[1] = (Temp[2]%100)/10+0x30;
     s[2] = (Temp[2]%100)%10+0x30;
     s[3]= '.';
     s[4] = 0;
     Str_8x16(1, 8, s); 
     Str_8x16(1, 8, s);

     s[0] = Temp[3]/100+0x30;
     s[1] = (Temp[3]%100)/10+0x30;
     s[2] = (Temp[3]%100)%10+0x30;
     s[3] = '\0';
     Str_8x16(1, 12, s);
     Str_8x16(1, 12, s);
     //������ʾ��һ�
     Index=0;
     DispFanBai(1,0,3);
     MenuStatus =1000;
     
   }
   
   else
   {
     //�ļ���������ʾ�ļ�������ȷ�������أ�
    return; 
   }
   
       

   break;
 case 1000:
     switch(GetKey1())   
     {
       case ENTER_KEY:
         if(Index ==3)
         {
          //�������ݵ�sd�����˵��ϼ��˵���
           if(pCurrentMenu == &MyMenu[16])//�����������IP��ַ��           
           {
             ModifyFile("CONFIG  TXT", Temp,4, 4);
             MenuStatus =3;
             return;
           }
           if(pCurrentMenu == &MyMenu[18])//��������������ص�ַ��           
           {
             ModifyFile("CONFIG  TXT", Temp,12, 4);
             MenuStatus =3;
             return;
           }           
           if(pCurrentMenu == &MyMenu[17])//������������������뻹��֪����������ĺϷ��ԡ�
           {
             unint32 mask;
             mask=(unint32)&Temp[0];
             if(((mask-1)|mask)==0xffffffff)//��mask -1 | mask�� ==  0xFFFFFFFF ,�����TRUE�����������룬��FALSE�Ͳ����������롣
             {
               //�ǺϷ����������룻�����˳���
               ModifyFile("CONFIG  TXT", Temp,8, 4);
               MenuStatus =3;
              return;
             }
             MenuStatus =3;
             return;//�����������˳���
           }
 
           
         }
           

         else
         //�ı䷴����ʾ��λ��
            //����lcm����
          s[0] = Temp[Index]/100+0x30;
          s[1] = (Temp[Index]%100)/10+0x30;
          s[2] = (Temp[Index]%100)%10+0x30;

          s[4] = '\0';
          Str_8x16(1, Index*4, s);
          Str_8x16(1, Index*4, s);            
         //  DispFanBai(1,Index*4,3); //ȡ��ԭ�ȵķ���           
           Index++;
           DispFanBai(1,Index*4,3);//��Ϊ��ǰ�ķ���           
         
         break;
        case UP_KEY:
          if(Temp[Index]==255)
          Temp[Index]=0;
          else
          Temp[Index]++; 
          s[0] = Temp[Index]/100+0x30;
          s[1] = (Temp[Index]%100)/10+0x30;
          s[2] = (Temp[Index]%100)%10+0x30;

          s[4] = '\0';
          Str_8x16(1, Index*4, s);
          Str_8x16(1, Index*4, s);          
          DispFanBai(1,Index*4,3);          
          break;
        case DOWN_KEY:
          if(Temp[Index]==0)
            Temp[Index] = 255;
          else
            Temp[Index]--;
          s[0] = Temp[Index]/100+0x30;
          s[1] = (Temp[Index]%100)/10+0x30;
          s[2] = (Temp[Index]%100)%10+0x30;

          s[4] = '\0';
          Str_8x16(1, Index*4, s); 
          Str_8x16(1, Index*4, s);
          DispFanBai(1,Index*4,3);          
          break;
        case ESC_KEY:
          if(Index ==0)
          {
            
          }
 
         else if(Index==3)
          {
            ////�������ݲ��˵��ϼ��˵�
           MenuStatus = 3;
           return; 
          }

          else
          { 
                     //�ı䷴����ʾ��λ��
            //����lcm����
          s[0] = Temp[Index]/100+0x30;
          s[1] = (Temp[Index]%100)/10+0x30;
          s[2] = (Temp[Index]%100)%10+0x30;

          s[4] = '\0';
          Str_8x16(1, Index*4, s);
          Str_8x16(1, Index*4, s);          
            
            
      //     DispFanBai(1,Index*4,3); //ȡ��ԭ�ȵķ���
           Index--; 
           DispFanBai(1,Index*4,3);//��Ϊ��ǰ�ķ���
          } 
          break;
     default:
          break;
     }
   break;
 default:
   break;
 }
}
  

//*******************************
//���ܣ���������
//***********************************
       
 void SetPass(void)
 {
   unchar s[4];
   switch(MenuStatus)
   {
   case 2:
      Str_8x16(0, 0,"������������" );
      Str_8x16(0, 0,"������������" );
      Temp[0] = 0;
      Temp[1] = 0;
      Temp[2] = 0;
      Temp[3] = 0;
      Index=0;
      
      MenuStatus = 1000;
      break;
   case 1000:
         switch(GetKey1())
         {
         case ENTER_KEY:
           
           break;
         case UP_KEY:
           if(Temp[Index] ==9)
             Temp[Index]=0;
            else 
           Temp[Index]++;
           s[0]=Temp[Index]+0x30;
           s[1]='\0';
           Str_8x16(1, Index, s);
           Str_8x16(1, Index, s);
           Delay_ms(500);
           s[0]='*';
           s[1]='\0';
           Str_8x16(1, Index, s);
           Str_8x16(1, Index, s);           
           Dely_2s=0;           
           break;
         case DOWN_KEY:
           if(Temp[Index] ==0)
             Temp[Index]=9;
            else 
           Temp[Index]++;
           s[0]=Temp[Index]+0x30;
           s[1]='\0';
           Str_8x16(1, Index, s);
           Str_8x16(1, Index, s);           
           Delay_ms(500);
           s[0]='*';
           s[1]='\0';
           Str_8x16(1, Index, s);
           Str_8x16(1, Index, s);           
           Dely_2s=0;            
           break;
         case ESC_KEY:
           break;
         default:
           if(Dely_2s>=3)
           {
             if(Index ==3)
             {
               //�������벢�˳���
               
             }
             else
             {
               Index++;
             }  
           }
           break;
         }
     break;
   }   
   
 }
//**********************************************
  // 1.3.0���� ����ģʽ
  //*******************************************
 void SetModel(void)
 {
   switch(MenuStatus) 
   {
     
   case 2:
     Str_8x16(1, 0,"���/�༭/ɾ��" );
     Str_8x16(1, 0,"���/�༭/ɾ��" );
      DispFanBai(1,0,4) ;
      ModefyModel= 0;//Ĭ�������״̬
      Index = 0;
     DispFanBai(1,0,4); 
     MenuStatus = 1000;
     break;
   case 1000:
        switch(GetKey1())
         {
         case ENTER_KEY:
           ModefyModel = Index;
           return;
         // break;
         case UP_KEY:
           if(Index <3)
           {
             DispFanBai(1,Index*5,4);//ȥ�����еķ���
             Index++; 
            
             DispFanBai(1,Index*5,4);//���ӵ�ǰ���ף�
           }
           
             
           break;
         case DOWN_KEY:
           if(Index >0)
           {
             DispFanBai(1,Index*5,4);//ȥ�����еķ���
             Index--; 
            
             DispFanBai(1,Index*5,4);//���ӵ�ǰ���ף�
           }
           
             
           break;           

         case ESC_KEY:
           return;
           //break;
         default:
           break;           
         }
     break;
   }
   
 }
  //**********************************************
  // 1.3.1���� �������
  //*******************************************
    
 void SetBatGrpNo(void)
 {
   unchar s[5];
   switch(MenuStatus)
   {
   case 2:
     if(ModefyModel ==0)
     {
       if(CURPATH.CurrentDirStarClusID !=0)
       {
         CURPATH.CurrentDirStarClusID = 0;
       } 
       Str_8x16(1, 0,"��ӵ������" );
       Str_8x16(1, 0,"��ӵ������" );
       if((ReadFile("CONFIG  TXT", Temp, 16,2)!=0xff))
       {
         s[0] = (unint)Temp/1000+0x30;         
         s[1] = ((unint)Temp%1000)/100+0x30;
         s[2] = (((unint)Temp%1000)%100)/10+0x30;
         s[3] = (((unint)Temp%1000)%100)%10+0x30;
         s[4] = '\0';
         
       }
     }
     if(ModefyModel ==1)
     {
       Str_8x16(1, 0,"�༭�������" );
       Str_8x16(1, 0,"�༭�������" );
     }
     if(ModefyModel ==2)
     {
       Str_8x16(1, 0,"ɾ���������" );
       Str_8x16(1, 0,"ɾ���������" );
     }
      Temp[0] = 0;
      Temp[1] = 0;
      Temp[2] = 0;
      Temp[3] = 0;
      s[0] =0x30;
      s[1] =0x30;
      s[2] =0x30;
      s[3] =0x30;
      s[4] =0;
      Index=0;
      Str_8x16(1, 0,s );
      Str_8x16(1, 0,s );
      DispFanBai(1,0,1);
      MenuStatus = 1000;
      break;
   case 1000:
         switch(GetKey1())
         {
         case ENTER_KEY:
         if(ModefyModel ==0) 
        {
          if(AppendData("CONFIG  TXT", Temp, 2) !=0xff)
          {
            CurrentBatGrp =(unint)Temp;
            Str_8x16(1,0,"�����Ѿ�����ɹ�");
            Str_8x16(1,0,"�����Ѿ�����ɹ�");
            return;
          }
          
        }
           return;
             
           //break;
         case UP_KEY:
            if((unint)Temp==9999)
              *(unint*)Temp = 0;
           else
             (unint)Temp[0]++;
         s[0] = (unint)Temp/1000+0x30;         
         s[1] = ((unint)Temp%1000)/100+0x30;
         s[2] = (((unint)Temp%1000)%100)/10+0x30;
         s[3] = (((unint)Temp%1000)%100)%10+0x30;
         s[4] = '\0';
         Str_8x16(1,0, Temp);
         Str_8x16(1,0, Temp);
         DispFanBai(1,0,4); 

           
           
           break;
         case DOWN_KEY:
            if((unint)Temp ==0)
             *(unint*)Temp=9999;
             else
              (unint)Temp[0]--;
         s[0] = (unint)Temp/1000+0x30;         
         s[1] = ((unint)Temp%1000)/100+0x30;
         s[2] = (((unint)Temp%1000)%100)/10+0x30;
         s[3] = (((unint)Temp%1000)%100)%10+0x30;
         s[4] = '\0';
         Str_8x16(1,0 ,Temp);
         Str_8x16(1,0, Temp);         
         DispFanBai(1,0,4);      
           break;
         case ESC_KEY:

            
           
           break;
         default:
           break;
         } 
              break;  
         
         }     

   }
   

 
           
  //**********************************************
  // 1.3.2���� �������
  //���ܣ����õ�ǰ�����ĵ��������           
  //******************************
   SetCurrentBatNumbers()
   {
        unchar s[5];
        switch(MenuStatus)
        {
        case 2:

     
          
       if(CURPATH.CurrentDirStarClusID !=0)
       {
         CURPATH.CurrentDirStarClusID = 0;
       } 
       Str_8x16(1, 0,"��ӵ������" ); 
      if(ReadFile("CONFIG  TXT", Temp, 20,2)!=0xff)
       {
         s[0] = (unint)Temp/1000+0x30;         
         s[1] = ((unint)Temp%1000)/100+0x30;
         s[2] = (((unint)Temp%1000)%100)/10+0x30;
         s[3] = (((unint)Temp%1000)%100)%10+0x30;
         s[4] = '\0';
         Str_8x16(1,0, Temp);
         Str_8x16(1,0, Temp);
         DispFanBai(1,0,4);
         MenuStatus=1000;
         
       }
       else
       {
         //�����ļ������ڡ�
       }  
        
          
          break;
        case 1000:
         switch(GetKey1())
         {
         case ENTER_KEY:
        if(ModefyModel ==0) 
        {
          if(ModifyFile("CONFIG  TXT", Temp,64+CurrentBatGrp*64, 2) !=0xff)
          {
            Str_8x16(1,0,"�����Ѿ�����ɹ�");
            Str_8x16(1,0,"�����Ѿ�����ɹ�");
            return;
          }
          
        }
           return;
           //break;
         case UP_KEY:
           if((unint)Temp==9999)
             *(unint*)Temp=0;
           else
             (unint)Temp[0]++;
         s[0] =(unint) Temp/1000+0x30;         
         s[1] = ((unint)Temp%1000)/100+0x30;
         s[2] = (((unint)Temp%1000)%100)/10+0x30;
         s[3] = (((unint)Temp%1000)%100)%10+0x30;
         s[4] = '\0';
         Str_8x16(1,0, Temp);
         Str_8x16(1,0, Temp);
         DispFanBai(1,0,4);           
           
           break;
         case DOWN_KEY:
           if((unint)Temp ==0)
             *(unint*)Temp=9999;
             else
              (unint)Temp[0]--;
         s[0] =(unint) Temp/1000+0x30;         
         s[1] = ((unint)Temp%1000)/100+0x30;
         s[2] = (((unint)Temp%1000)%100)/10+0x30;
         s[3] = (((unint)Temp%1000)%100)%10+0x30;
         s[4] = '\0';
         Str_8x16(1,0, Temp);
         Str_8x16(1,0, Temp);         
         DispFanBai(1,0,4);               
             
           break;
         case ESC_KEY:
           break;
         default:
           break;          
          //break;          
          
     }
   }        
 }        

  //**********************************************
  // 1.3.3���� ��ѹ�ȼ�
  //���ܣ����õ�ǰ�����ĵ�ѹ�ȼ���           
  //******************************    
   
void SetVoltLevel(void)

{
        unchar s[5];
        switch(MenuStatus)
        {
        case 2:
 
       if(CURPATH.CurrentDirStarClusID !=0)
       {
         CURPATH.CurrentDirStarClusID = 0;
       } 
       Str_8x16(1, 0,"��ӵ�ѹ�ȼ�" ); 
       Str_8x16(1, 0,"��ӵ�ѹ�ȼ�" );       
      if((ReadFile("CONFIG  TXT", Temp, 22,2)!=0xff))//ȡ��ϵͳĬ�ϵĵ�ѹ�ȼ�
       {
         s[0] = *(unint*)Temp/1000+0x30;         
         s[1] = (*(unint*)Temp%1000)/100+0x30;
         s[2] = ((*(unint*)Temp%1000)%100)/10+0x30;
         s[3] = ((*(unint*)Temp%1000)%100)%10+0x30;
         s[4] = '\0';
         if(((unint)Temp !=2)||((unint)Temp !=6)||((unint)Temp !=12))
         {  
         Str_8x16(1,0, "�Ƿ��ĵ�ѹ�ȼ�");
         Str_8x16(1,0, "�Ƿ��ĵ�ѹ�ȼ�");         
         return;
         }
         Str_8x16(1,0, Temp);
         if(*(unint*)Temp ==2) Index =0;
         if((unint)Temp[0]==6 ) Index = 1;
         if((unint)Temp[0]==12) Index =2;
         
         DispFanBai(1,0,4);
         MenuStatus=1000;
         
       }
       else
       {
         //�����ļ������ڡ�
         return;
         
       }  
        
          
          break;
        case 1000:
         switch(GetKey1())
         {
         case ENTER_KEY:
        if(ModefyModel ==0) 
        {
          if(ModifyFile("CONFIG  TXT", &Temp[Index],64+CurrentBatGrp*64+16, 2) !=0xff)
          {
            Str_8x16(1,0,"�����Ѿ�����ɹ�");
            Str_8x16(1,0,"�����Ѿ�����ɹ�");
            return;
          }
          
        }
           return;
           //break;
         case UP_KEY:
          if (Index ==2)
            Index =0;
           else
            Index++;
           s[0] = BatVoltLevel[Index]/10;
           s[1] = BatVoltLevel[Index]%10;
           s[2] = 0;
           Str_8x16(1,0,s);
           DispFanBai(1,0,2);
           break;
         case DOWN_KEY:
           if (Index ==0)
            Index =2;
           else
            Index--;
           s[0] = BatVoltLevel[Index]/10;
           s[1] = BatVoltLevel[Index]%10;
           s[2] = 0;
           Str_8x16(1,0,s);
           DispFanBai(1,0,2);         
             
           break;
         case ESC_KEY:
           return;
           //break;
         default:
           break;          
          //break;          
          
     }
   }          
}


  //**************************8
  // 1.3.4 ��׼���衣          
  //���ܣ� ���û�׼���衣
  //*************************          
void SetResLevel(void )
{
  unchar s[10];

  float f;

  
 switch(MenuStatus)
 {
 case 2:
     if(CURPATH.CurrentDirStarClusID !=0)
       {
         CURPATH.CurrentDirStarClusID = 0;
       } 
       Str_8x16(1, 0,"��ӻ�׼����" ); 
       Str_8x16(1, 0,"��ӻ�׼����" ); 
     if((ReadFile("CONFIG  TXT", Temp, 24,4)!=0xff))//ȡ��ϵͳĬ�ϵĻ�׼����
       {
  /*       
  unchar *p =(unchar*)&f;         
         *p++=Temp[0];
         *p++=Temp[1];
         *p++=Temp[2];
         *p++=Temp[3];         
 */        
         memcpy(&f,Temp,4);
         memcpy(s,&f,4);
      //   *s=*(unchar*)&f;���ַ�������ȡֻ����memcpy ��һ�ֽ�һ�ֽڵ��ֶ���ȡ������ע�͵��Ĳ��֡�

         sprintf((char*)s,"%f",f);
 
          Str_8x16(1,0,s);
         Str_8x16(1,0,s);
         DispFanBai(1,0,4);
         MenuStatus=1000;
         
       }
       else
       {
         //�����ļ������ڡ�
         return;
         
       }     
   break;
 case 1000:
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
   break;
 default:
   break;
   
 } 
}


  //**************************8
  // 3���������          
  //���ܣ� ʵ��ĳ���ص����������
  //************************* 
void TotalGrpMeasure(void )
{
    unint i;
    unchar s[20];
     
     unint fint;
     float fres;
     float f,arf[3];    

 switch(MenuStatus)
 {
 case 2:
     if(CURPATH.CurrentDirStarClusID !=0)
       {
         CURPATH.CurrentDirStarClusID = 0;
       } 
     if((ReadFile("CONFIG  TXT", (unchar*)pCfgHead, 0,sizeof (CFG_HEAD))!=0xff))//ȡ��ϵͳͷ
     {
      
      Index = 1;

     // t=*(unint*)Temp;�ٴγ����ֽڶ�������⣬��ʼ���Ե�ʱ����û����������ֳ��ˡ�����
         s[0] = Index/1000+0x30;         
         s[1] = (Index%1000)/100+0x30;
         s[2] = ((Index%1000)%100)/10+0x30;
         s[3] = ((Index%1000)%100)%10+0x30;
         s[4] = '\0'; 
       

      Str_8x16(0,0,"��:");
      Str_8x16(0,0,"��:");      
      Str_8x16(0,3,s);
 


      DispFanBai(0,3,4);
      MenuStatus =1000;
     }
     else
     {
       Str_8x16(1,0 ,"ϵͳ�ļ�������");
       Str_8x16(1,0 ,"ϵͳ�ļ�������");
       Delay_ms(2000);
       MenuStatus = 3;
       return;
     } 
  break;
 case 1000:
   
         switch(GetKey1())
         {
         case ENTER_KEY:
           DispFanBai(0,3,4);//��λ��ȡ������
           //�ȴ�config�ļ����������ض�Ӧ����������
          
          if( ReadFile("CONFIG  TXT",(unchar*)pGrpHead,Index*64,sizeof (GRP_HEAD))==0xff)
          {
            Str_8x16(2,0,"ϵͳ�����ļ�����");
            Str_8x16(2,0,"ϵͳ�����ļ�����");            
            Delay_ms(2000);
            MenuStatus =3;
            return;
          }
     
           
           
           //����������ڵ�Ŀ¼���û�и���ͽ��������Ŀ¼�����ڸ�Ŀ¼�¸���ʱ�佨������Ĳ����ļ�
           
        if(CURPATH.CurrentDirStarClusID !=0)
       {
         CURPATH.CurrentDirStarClusID = 0;
       }
         s[0] = Index/1000+0x30;         
         s[1] = (Index%1000)/100+0x30;
         s[2] = ((Index%1000)%100)/10+0x30;
         s[3] = ((Index%1000)%100)%10+0x30;
         s[4] = '\0';
       if(ChgDir(s)==0xff)//û�и�Ŀ¼������Ŀ¼������
       {
         CreateDir(s);
            
       }
       ChgDir(s);
       DIR *ID=(DIR *)&Temp[12];
       s[0] = year/1000+0x30;
       s[1] = (year%1000)/100+0x30;
       s[2] = ((year%1000)%100)/10+0x30;
       s[3] = ((year%1000)%100)%10+0x30;    
       s[4]=month/10+0x30;
       s[5]= month%10+0x30;
       s[6]= day/10+0x30;
       s[7]=day%10+0x30;
       s[8]='D';
       s[9]='A';
       s[10]='T';
       s[11]=0;
       
         
         
       if(GetFileID(s, ID)==0xffff)//�ļ������ڽ���֮������config.txt�������ļ���Ĭ��ͷ��
          {
            CreateFile(s);
         
            pMeasureHead=(MEASURE_HEAD*)Temp;
            (pMeasureHead->DeviceID)=(pCfgHead->DeviceID);
            pMeasureHead->BatGrpID=pGrpHead->BatGrpID;
            pMeasureHead->BatNumbers=pGrpHead->BatNumbers;
            AppendData(s,(unchar*)pMeasureHead,sizeof (MEASURE_HEAD));
          }
       GetFileID(s, ID);
       //�ж������ؽ����Ѿ������˶��ٿ��Ա��������Ĳ�����
           Index =(ID->FilePosit.Size-16)/8+1;//�Ѿ����ڵĵ�ز���������
           if(Index>=pGrpHead->BatNumbers)
           {
             Str_8x16(1,0,"�����Ѿ��������");
             Str_8x16(1,0,"�����Ѿ��������");
             Delay_ms(2000);
            MenuStatus =3;
            return;
             
           }
        s[0] = Index/1000+0x30;
       s[1] = (Index%1000)/100+0x30;
       s[2] = ((Index%1000)%100)/10+0x30;
       s[3] = ((Index%1000)%100)%10+0x30;           
       s[4] = 0;
       Str_8x16(0,12,s);
       Str_8x16(0,12,s);//��ʾ��һ����Ҫ�����ĵ�ء�
       
         
         //��cy��Ƭ���ϵ�򿪴��ڲ����д��ڳ�ʼ����
         pRecv0Str=&Temp[20];//���ý��ջ������ĵ�ַ��       
         pAck->tail = 0;
         pAck->head = 0;
         pAck->cBufFlag = 0;
         POWRON_CY;
         Init_Uart0;

         Uart0RxTimeOut = 0;
           Recv0State =0;
           Recv0Index =0;         
         //�ȴ�cy��Ƭ������Ӧ����
         while(1)
         {
           if(GetKey2( pAck,Temp,10)!=NULL)
           {
             //�ж��õ��������Ƿ�Ϸ�
             if((Temp[4]==0x04)&&(Temp[5]==0x03)&&(Temp[6]==0x02)&&(Temp[7]==0x01))
             {
               unchar  data[4];
               data[0] =10;//�趨��ַΪ10
               data[1] = 0;
               data[2] = 0;
               data[3] = 0;
               Recv0State =0;
               Recv0Index =0;
               SendCmd(SET_ADRESS,0,data);  
               Uart0RxTimeOut =0;
             break;
             }
              Str_8x16(1,0,"����ģ�����");
              Str_8x16(1,0,"����ģ�����");
             break;
           }
           
          if(Uart0RxTimeOut>=300)
          {
            //cy��Ƭ��û����Ӧ�˳���
            Str_8x16(1,0,"����ģ��û����Ӧ");
            Str_8x16(1,0,"����ģ��û����Ӧ");
            Delay_ms(2000);
            POWROFF_CY;
            MenuStatus =3;
            return;
          }
         }


             while(1)
             {
               if(GetKey2( pAck,Temp,10)!=NULL)//�õ�cy���趨��ַ��Ӧ
                 
               {
                 if(Temp[2]==10)
                 {
 
                    break;
                 }
               Str_8x16(1,0,"������Ԫ����");                 
               Str_8x16(1,0,"������Ԫ����"); 
                break; 
               }
         if(Uart0RxTimeOut>=300)
          {
            //cy��Ƭ��û����Ӧ�˳���
            Str_8x16(1,0,"����ģ��û����Ӧ");
            Str_8x16(1,0,"����ģ��û����Ӧ");
            Delay_ms(2000);
            POWROFF_CY;
            MenuStatus =3;
            return;
          }               
             }         
         
         
         
        //   DispFanBai(0,12,4);//��ر��λ�üӷ��ף�
         //Index = 1;

           MenuStatus =2000;
           break;
         case UP_KEY:
       
         if(Index ==pCfgHead->NumberBatGrps)
         { 
          Index=1;
          
         } 
         else
         {  
          Index++;
          
         }
         s[0] = Index/1000+0x30;         
         s[1] = (Index%1000)/100+0x30;
         s[2] = ((Index%1000)%100)/10+0x30;
         s[3] = ((Index%1000)%100)%10+0x30;
         s[4] = '\0';
         
         Str_8x16(0,3,s);
         Str_8x16(0,3,s);
         DispFanBai(0,3,4);
           
           break;
         case DOWN_KEY:
          
         if(Index==1)
         { 
          Index = pCfgHead->NumberBatGrps;
         
         } 
         else
         {  
          Index--; 
          
         }  
         s[0] = Index/1000+0x30;         
         s[1] = (Index%1000)/100+0x30;
         s[2] = ((Index%1000)%100)/10+0x30;
         s[3] = ((Index%1000)%100)%10+0x30;
         s[4] = '\0';
         
         Str_8x16(0,3,s);
         Str_8x16(0,3,s);
         DispFanBai(0,3,4);           
           break;
         case ESC_KEY:
           break;
         default:
           break;
         }     
     
   
   
  break;
 case 2000: //��ʼ��������Ӧ�κΰ���
   //���Ͳ��������cy
   
   i = 0;
   while(1)
   {
   cont:    
   Uart0RxTimeOut=0;
   Recv0State =0;
   Recv0Index =0;
                  unchar  data[4];
               data[0] =0;//�趨��ַΪ10
               data[1] = 0;
               data[2] = 0;
               data[3] = 0;
     SendCmd(MEASURE_VOLT,10,data);
     while(1)
     { 
       if(GetKey2( pAck,Temp,10)!=NULL)
       {
         memcpy((unchar*)&fint,&Temp[4],2);
         f=(float)fint/1000;//ע���ֽڶ������⣻
        
         if((f>=pGrpHead->BatVoltMin)&&(f<=pGrpHead->BatVoltMax))
         {
           arf[i]=f;
           
           i++;
           break;
         }
         else
         {
            Str_8x16(1,0,"�������");
            Str_8x16(1,0,"�������");
             goto cont;        
           
         }  
       }
       else if(Uart0RxTimeOut>=300)//��ʱ�˳���
          {
            //cy��Ƭ��û����Ӧ�˳���
            Str_8x16(1,0,"����ģ��û����Ӧ");
            Str_8x16(1,0,"����ģ��û����Ӧ");
            Delay_ms(5000);
            POWROFF_CY;
            MenuStatus =3;
            return;
          }

       
     }

     if(i==3)
     {
       //�Ѿ�������3��
       i =0;
      float f1 = fabsf(arf[0]-arf[1]);
      float f2 = fabsf(arf[0]-arf[2]);
      float f3 = fabsf(arf[2]-arf[1]);
                       
                        
      if((f1<=0.01)&&(f2<=0.01)&&(f3<=0.01))//�Ƚ���Ч��
      {
        
      //���Ӽк��˲������õ����κϸ�ĵ�ѹֵ��
        //�˳���ѹ�������е��������

//*******************8
   
        //��������ֵ�����
        Uart0RxTimeOut = 0;
        
        SendCmd(MEASURE_RES,10,data);
       // DispFanBai(0,12,4);//ȡ�����ס�
        f=(arf[0]+arf[1]+arf[2])/3;
        while(1)
        {
         if(GetKey2( pAck,Temp,10)!=NULL)
         { 
         
         memcpy(Temp,&f,4);
         memcpy(&fint,&Temp[4],2);
         fres =(float)fint/1000;
         memcpy(&Temp[4],&fres,4);
        // ��ʾ����ѹ������ֵ��
         sprintf(s,"%5.3f",f);
       // unint j =i;
         Str_8x16(1,0,"��ѹ:");
         Str_8x16(1,0,"��ѹ:");
         Str_8x16(1,10,"V");
         Str_8x16(1,5,s);

         sprintf((char*)s,"%5.3f",fres);
         Str_8x16(2,0,"����:");
         Str_8x16(2,0,"����:");
         Str_8x16(2,10,"m��");         
         Str_8x16(2,5,s);

           
         //��������洢���ļ���
       s[0] = year/1000+0x30;
       s[1] = (year%1000)/100+0x30;
       s[2] = ((year%1000)%100)/10+0x30;
       s[3] = ((year%1000)%100)%10+0x30;    
       s[4]=month/10+0x30;
       s[5]= month%10+0x30;
       s[6]= day/10+0x30;
       s[7]=day%10+0x30;
       s[8]='D';
       s[9]='A';
       s[10]='T';
       s[11]=0;
    
       AppendData(s,Temp,8);
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
       //��ʼ��һ����صĲ���
       //  Str_8x16(2,0, "ENT����һ�� ESC���˳�");

         unchar key;
       s[0] = Index/1000+0x30;
       s[1] = (Index%1000)/100+0x30;
       s[2] = ((Index%1000)%100)/10+0x30;
       s[3] = ((Index%1000)%100)%10+0x30;         
       s[4] = 0;
       Str_8x16(0,12,s);
       Str_8x16(0,12,s);       
       DispFanBai(0,12,4); //�����˵�ǰ����ٴη��״˵�ر���������        
        while(1)//�ȴ��û�������
          
        {
          KeyScan();
          
          key=GetKey1();
         if(key==ENTER_KEY) 
         {
           if(Index==pGrpHead->BatNumbers)
           {
             //�⵽�����һ�����˳�ϵͳ��
             Str_8x16(1,0,"����������");
             Str_8x16(1,0,"����������");
             Delay_ms(2000);
             POWROFF_CY;
             MenuStatus =3;
             return;         
             
             
           }
           else
           {  
             Index++;
           }
           s[0] = Index/1000+0x30;
           s[1] = (Index%1000)/100+0x30;
           s[2] = ((Index%1000)%100)/10+0x30;
           s[3] = ((Index%1000)%100)%10+0x30;         
           s[4] = 0;
           Str_8x16(0,12,s);
           Str_8x16(0,12,s);
         //  DispFanBai(0,12,4);
           Str_8x16(1,0,"                ");           
           Str_8x16(2,0,"                ");           
           
           Uart0RxTimeOut=0;
           Recv0State =0;
           Recv0Index =0;
           goto cont;
         }
         else if(key==ESC_KEY)
         {
            Delay_ms(2000);
            POWROFF_CY;
            MenuStatus =3;
            return;            
           
         
         }
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^         
           
        }        
       

        
        
     //     break;//�˳�������ֵ��Ӧ��ѭ����
          }
       else if(Uart0RxTimeOut>=300)//��ʱ�˳���
          {
            //cy��Ƭ��û����Ӧ�˳���
            Str_8x16(1,0,"����ģ��û����Ӧ");
            Str_8x16(1,0,"����ģ��û����Ӧ");
            Delay_ms(5000);
            POWROFF_CY;
            MenuStatus =3;
            return;
          }
          
      
        }

        
//***************************************        


      //break;  //�˳�case2000
       
      }
      else
      {
        //�ж�Ϊ����û�мк�
        Str_8x16(3,0,"û�к������²���");
        Str_8x16(3,0,"û�к������²���");
        unchar key;
       while(1)
       {
         KeyScan();
         key=GetKey1();
         if(key==ENTER_KEY)
         {
           i=0;
          Str_8x16(3,0,"                "); 
          Str_8x16(3,0,"                "); 
           break;//�������Ե�ǰ��صĲ���
         }
         else if(key==ESC_KEY)           //�����Ե�ǰ��ز�����Ϊ�����ؿ����Ѿ����˽��Ų�����ĵ�ء�
         {
           i =0;
       //��ռ���ļ���Ӧ��λ�õȽ������е���������
       s[0] = year/1000+0x30;
       s[1] = (year%1000)/100+0x30;
       s[2] = ((year%1000)%100)/10+0x30;
       s[3] = ((year%1000)%100)%10+0x30;    
       s[4]=month/10+0x30;
       s[5]= month%10+0x30;
       s[6]= day/10+0x30;
       s[7]=day%10+0x30;
       s[8]='D';
       s[9]='A';
       s[10]='T'; 
       for (unchar j=0;j<8;j++)
         Temp[j]=0;
          AppendData(s,Temp,8);    
       if(Index < pGrpHead->BatNumbers)
       {
         Index++;
        Str_8x16(3,0,"����������һֻ      "); 
        Str_8x16(3,0,"����������һֻ      "); 
       }
       else
       {
                //�������;
            Str_8x16(1,0,"����������");
            Str_8x16(1,0,"����������");
            Delay_ms(2000);
            POWROFF_CY;
            MenuStatus =3;
            return;         

       }
       s[0] = Index/1000+0x30;
       s[1] = (Index%1000)/100+0x30;
       s[2] = ((Index%1000)%100)/10+0x30;
       s[3] = ((Index%1000)%100)%10+0x30;         
       s[4] = 0;
       Str_8x16(0,12,s); 
       Str_8x16(0,12,s); 
       DispFanBai(0,12,4);

           
         }
           
       }
      // break;
      }
     // break;
     }
    

   }


        


   
   
   
  
 default:
   break;
 } 
}
//***************************************
// ���ܣ� 1.5.1����������
//*******************************


void SetYYMMDD(void)
{
  unchar s[5];
 switch(MenuStatus)
 {
 case 2:
 
     Temp[0] =year-2000;
     s[0] = Temp[0]/10+0x30;
     s[1] = (Temp[0]%10)+0x30;
     s[2]= 0xc4;//��
     s[3] =0xea;
     s[4]=0;
     Str_8x16(1, 0, s);
     Str_8x16(1, 0, s);
     Temp[1] =month;
     s[0] = Temp[1]/10+0x30;
     s[1] = (Temp[1]%10)+0x30;
     s[2]= 0xd4;//��
     s[3] =0xc2;
     s[4] = 0;
     Str_8x16(1, 4, s); 
     Str_8x16(1, 4, s); 
     Temp[2] =day;
     s[0] = Temp[2]/10+0x30;
     s[1] = (Temp[2]%10)+0x30;
     s[2]= 0xc8;//��
     s[3]=0xd5;
     s[4] = 0;
     Str_8x16(1, 8, s); 
     Str_8x16(1, 8, s); 

     //������ʾ��һ�
     Index=0;
     DispFanBai(1,0,2);
     MenuStatus =1000;
     

      

   break;
 case 1000:
     switch(GetKey1())   
     {
       case ENTER_KEY:
         if(Index ==2)
         {
 
          year=Temp[0]+2000;
          month =Temp[1];
          day=Temp[2];
          MenuStatus = 3;
          return;
           
         }
           

         else
         //�ı䷴����ʾ��λ��
          //����lcm���ȶ�����
          s[0] = Temp[Index]/10+0x30;
          s[1] = (Temp[Index]%10)+0x30;
          s[2] = '\0';          
          Str_8x16(1, Index*4, s);
          Str_8x16(1, Index*4, s);           
       //   DispFanBai(1,Index*4,2); //ȡ��ԭ�ȵķ���           
           Index++;
           DispFanBai(1,Index*4,2);//��Ϊ��ǰ�ķ���           
         
         break;
        case UP_KEY:
         
          
          
          if(Index ==0) //��
          {
          if(Temp[Index]==99)
          Temp[Index]=0;
          else
          Temp[Index]++; 
          s[0] = Temp[Index]/10+0x30;
          s[1] = (Temp[Index]%10)+0x30;
          s[2] = '\0';
          }
          if(Index ==1)//��
          {
          if(Temp[Index]==12)
          Temp[Index]=1;
          else
          Temp[Index]++; 
          s[0] = Temp[Index]/10+0x30;
          s[1] = (Temp[Index]%10)+0x30;
          s[2] = '\0';
          } 
          if(Index ==2)//��
          {
            
          if(Temp[Index]==MAX1())
          Temp[Index]=1;
          else
          Temp[Index]++; 
          s[0] = Temp[Index]/10+0x30;
          s[1] = (Temp[Index]%10)+0x30;
          s[2] = '\0';
          }            
          
          Str_8x16(1, Index*4, s);
          Str_8x16(1, Index*4, s);
          DispFanBai(1,Index*4,2);
          
          break;
        case DOWN_KEY:
         
          if(Index ==0) //��
          {
          if(Temp[Index]==0)
          Temp[Index]=99;
          else
          Temp[Index]--; 
          s[0] = Temp[Index]/10+0x30;
          s[1] = (Temp[Index]%10)+0x30;
          s[2] = '\0';
          }
          if(Index ==1)//��
          {
          if(Temp[Index]==1)
          Temp[Index]=12;
          else
          Temp[Index]--; 
          s[0] = Temp[Index]/10+0x30;
          s[1] = (Temp[Index]%10)+0x30;
          s[2] = '\0';
          } 
          if(Index ==2)//��
          {
            
          if(Temp[Index]==1)
          Temp[Index]=MAX1();
          else
          Temp[Index]--; 
          s[0] = Temp[Index]/10+0x30;
          s[1] = (Temp[Index]%10)+0x30;
          s[2] = '\0';
          }            
          
          Str_8x16(1, Index*4, s);
          Str_8x16(1, Index*4, s);
          DispFanBai(1,Index*4,2); ;          
          break;
        case ESC_KEY:
          if(Index ==0)
          {
            
          }
 
         else if(Index==2)
          {
            ////�������ݲ��˵��ϼ��˵�
           MenuStatus = 3;
           return; 
          }

          else
          { 
                     //�ı䷴����ʾ��λ��
          //����lcm���ȶ�����
          s[0] = Temp[Index]/10+0x30;
          s[1] = (Temp[Index]%10)+0x30;
          s[2] = '\0';          
          Str_8x16(1, Index*4, s);
          Str_8x16(1, Index*4, s);
             
            
          // DispFanBai(1,Index*4,2); //ȡ��ԭ�ȵķ���
           Index--; 
           DispFanBai(1,Index*4,2);//��Ϊ��ǰ�ķ���
          } 
          break;
     default:
          break;
     }
   break;
 default:
   break;
 }
}




//***************************************
// ���ܣ� 1.5.2����ʱ����
//*******************************


void SetHHMMSS(void)
{
  unchar s[5];
 switch(MenuStatus)
 {
 case 2:
 
     Temp[0] =hour;
     s[0] = Temp[0]/10+0x30;
     s[1] = (Temp[0]%10)+0x30;
     s[2]= 0xcA;//ʱ
     s[3] =0xB1;
     s[4]=0;
     Str_8x16(1, 0, s);
     Str_8x16(1, 0, s);
     Temp[1] =min;
     s[0] = Temp[1]/10+0x30;
     s[1] = (Temp[1]%10)+0x30;
     s[2]= 0xB7; //��
     s[3] =0xd6;
     s[4] = 0;
     Str_8x16(1, 4, s); 
     Str_8x16(1, 4, s); 
     Temp[2] =sec;
     s[0] = Temp[2]/10+0x30;
     s[1] = (Temp[2]%10)+0x30;
     s[2]= 0xc3; //��
     s[3]=0xeb;
     s[4] = 0;
     Str_8x16(1, 8, s); 
     Str_8x16(1, 8, s); 

     //������ʾ��һ�
     Index=0;
     DispFanBai(1,0,2);
     MenuStatus =1000;
     

      

   break;
 case 1000:
     switch(GetKey1())   
     {
       case ENTER_KEY:
         if(Index ==2)
         {
 
          hour=Temp[0];
          min =Temp[1];
          sec=Temp[2];
          MenuStatus = 3;
          return;
           
         }
           

         else
         //�ı䷴����ʾ��λ��
          //����lcm���ȶ�����
          s[0] = Temp[Index]/10+0x30;
          s[1] = (Temp[Index]%10)+0x30;
          s[2] = '\0';          
          Str_8x16(1, Index*4, s);
          Str_8x16(1, Index*4, s);           
        //   DispFanBai(1,Index*4,2); //ȡ��ԭ�ȵķ���           
           Index++;
           DispFanBai(1,Index*4,2);//��Ϊ��ǰ�ķ���           
         
         break;
        case UP_KEY:
        
          if(Index ==0) //ʱ
          {
          if(Temp[Index]==23)
          Temp[Index]=0;
          else
          Temp[Index]++; 
          s[0] = Temp[Index]/10+0x30;
          s[1] = (Temp[Index]%10)+0x30;
          s[2] = '\0';
          }
          if(Index ==1)//��
          {
          if(Temp[Index]==59)
          Temp[Index]=0;
          else
          Temp[Index]++; 
          s[0] = Temp[Index]/10+0x30;
          s[1] = (Temp[Index]%10)+0x30;
          s[2] = '\0';
          } 
          if(Index ==2)//��
          {
            
          if(Temp[Index]==59)
          Temp[Index]=0;
          else
          Temp[Index]++; 
          s[0] = Temp[Index]/10+0x30;
          s[1] = (Temp[Index]%10)+0x30;
          s[2] = '\0';
          }            
          
          Str_8x16(1, Index*4, s);
          Str_8x16(1, Index*4, s);
          DispFanBai(1,Index*4,2);          
          break;
        case DOWN_KEY:
          
          if(Index ==0) //ʱ
          {
          if(Temp[Index]==0)
          Temp[Index]=23;
          else
          Temp[Index]--; 
          s[0] = Temp[Index]/10+0x30;
          s[1] = (Temp[Index]%10)+0x30;
          s[2] = '\0';
          }
          if(Index ==1)//��
          {
          if(Temp[Index]==0)
          Temp[Index]=59;
          else
          Temp[Index]--; 
          s[0] = Temp[Index]/10+0x30;
          s[1] = (Temp[Index]%10)+0x30;
          s[2] = '\0';
          } 
          if(Index ==2)//��
          {
            
          if(Temp[Index]==0)
          Temp[Index]=59;
          else
          Temp[Index]--; 
          s[0] = Temp[Index]/10+0x30;
          s[1] = (Temp[Index]%10)+0x30;
          s[2] = '\0';
          }            
          
          Str_8x16(1, Index*4, s);
          Str_8x16(1, Index*4, s);
          DispFanBai(1,Index*4,2); ;          
          break;
        case ESC_KEY:
          if(Index ==0)
          {
            
          }

         else if(Index==2)
          {
            
            ////�������ݲ��˵��ϼ��˵�
           MenuStatus = 3;
           return; 
          }

          else
          { 
                     //�ı䷴����ʾ��λ��
          //����lcm���ȶ�����
          s[0] = Temp[Index]/10+0x30;
          s[1] = (Temp[Index]%10)+0x30;
          s[2] = '\0';          
          Str_8x16(1, Index*4, s);
          Str_8x16(1, Index*4, s);
           
        //   DispFanBai(1,Index*4,2); //ȡ��ԭ�ȵķ���
           Index--; 
           DispFanBai(1,Index*4,2);//��Ϊ��ǰ�ķ���
          } 
          break;
     default:
          break;
     }
   break;
 default:
   break;
 }
}






//*************************************
//��Ϊ����������Ⲣ��Ӧ�����ڲ˵���һ�������Էŵ�����
//���ܣ� ��֤�û�������������ȷ�ԡ���ȷ�˳�
//����ȷ�������ѭ����
//*******************************************
void PutInPass(void) 
{
  unchar s[5];
  switch(MenuStatus)
  {
  case 1:
    Str_8x16(1, 0,"����������" );
    Str_8x16(1, 0,"����������" );
      Temp[0] = 0;
      Temp[1] = 0;
      Temp[2] = 0;
      Temp[3] = 0;
      Index=0;
    MenuStatus = 2000;
    break;
  case 2000:
         switch(GetKey1())
         {
         case ENTER_KEY:
           break;
         case UP_KEY:
           if(Temp[Index] ==9)
             Temp[Index]=0;
            else 
           Temp[Index]++;
           s[0]=Temp[Index]+0x30;
           s[1]='\0';
           Str_8x16(1, Index, s);
           Str_8x16(1, Index, s);
           Delay_ms(500);
           s[0]='*';
           s[1]='\0';
           Str_8x16(1, Index, s);
           Str_8x16(1, Index, s);
           Dely_2s=0;             
           break;
         case DOWN_KEY:
           if(Temp[Index] ==0)
             Temp[Index]=9;
            else 
           Temp[Index]++;
           s[0]=Temp[Index]+0x30;
           s[1]='\0';
           Str_8x16(1, Index, s);
           Str_8x16(1, Index, s);
           Delay_ms(500);
           s[0]='*';
           s[1]='\0';
           Str_8x16(1, Index, s);
           Str_8x16(1, Index, s);
           Dely_2s=0;             
           break;
         case ESC_KEY:
           break;
         default:
           if(Dely_2s>=2)
           {
             if(Index ==3)
             {
               //�ж�������ȷ��
               if((Temp[0]==1)&&(Temp[1]==2)&&(Temp[2]==3)&&(Temp[3]==4))
                  {
                    MenuStatus = 3;
                    return;
                  }
               else
               {
                Screen_Clear(); 
                Screen_Clear();                
                MenuStatus = 1; 
                break;
               }
             }
             else
             {
               Index++;
             }  
           }
           break;           
         }    
    break;
  }
}





//******************************8\
//
//5.2ʵ�� WIFIͨ�Ź���
void OpenWifi(void)
{
  unchar    i = 0;
  unchar key;
  
  switch (MenuStatus)
  {
  case 2:
             Str_8x16(1,0, "��������WIFI  ");
             Str_8x16(1,0, "��������WIFI  ");
             POWRON_WIFI;
             Delay_ms(5000);
            pWIFI_RX=Temp;//�������ܻ�������
             Init_Uart1;
             WIFI_RX_NUM=0;

             WIFI_RX_CONNECT=0;
             
             while(1)
             {  
             if(DialWIFI())
             
             {
               Delay_ms(2000);
               WIFI_RX_CONNECT=2;
             
               Str_8x16(1,0,"WIFI�����ɹ�");
               Str_8x16(1,0,"WIFI�����ɹ�");
               break;
             }
             else
             {
               i++;
               if(i>8)
               {
                 Str_8x16(1,0,"WIFI����ʧ��");
                  Str_8x16(1,0,"WIFI����ʧ��");
                 POWROFF_WIFI;
                 IE2 &= ~URXIE1;//��ֹ�����жϣ�
                 Delay_ms(5000);
                 MenuStatus = 3;
                 return;
                 
               }
               continue;
             } 
             
             }    

      MenuStatus = 1000;
    break;

  case 1000:
    while(1)
    {
      if(RecvCmd ==1)
      {
        RecvCmd = 0;
        //�����յ������
        //if(IsEqual(&Temp[4], "CONFIG.TXT", 10))
       
        {
          //����config.txt�ļ�������1
          
          if(CURPATH.CurrentDirStarClusID !=0)
            
          {
            CURPATH.CurrentDirStarClusID = 0;
          }
          
          SendHttpHead();
          SendFileToUart("CONFIG  TXT");
          
          
         // IE2 |= URXIE1;//�򿪽����ж����½������
        }
        /*
        else if((Temp[3]=='/')&&(Temp[8]==' '))//��Ŀ¼��
                
        {
         if(CURPATH.CurrentDirStarClusID !=0)
            
          {
            CURPATH.CurrentDirStarClusID = 0;
          } 
         unchar temp[11];
         temp[0]=Temp[4];
         temp[1]=Temp[5];
         temp[2]=Temp[6];
         temp[3]=Temp[7];
         temp[4]=0;
         temp[5]=0;
         temp[6]=0;
 
         temp[7]=0;
         temp[8]=0;
         temp[9]=0;
         temp[10]=0;
        
         
         
         ChgDir(temp);
         SendFileToUart(&Temp[9]);
          
        }
        */
        
      }
      if(RecvCmd ==2)//��ʾĿ¼�б�����
      {
        RecvCmd = 0;
        ListFile();
      }  
      if(RecvCmd ==3)//��ʾ���������ļ������ڡ�
      {
        RecvCmd = 0;
        SendFileToUart("");
      } 
      if(RecvCmd==4)//�ӿͻ��������ļ���SD�� ��Ҫ��CONFIGTXT
      {
        
      }          
    KeyScan();
    key=GetKey1();
    if(key!=NULL)                 
    {
      if(key ==ESC_KEY)
      {
        POWROFF_WIFI;
        IE2 &= ~URXIE1;//��ֹ�����жϣ�//�ָ�spi�������Ա����sd��
        initSPI(2);
        pWIFI_RX=(unchar *)0xffff;//���ջ�����ָ��ָ��ա�
        MenuStatus =3;
        return;
      }
    }
                 
                 
                 
      
    } 
     

   
  }
}




//***********************************************8
//���ݲ�ѯ���ܣ�
//4.���ݲ�ѯ���ܣ�
//*****************************
void SqlData(void)
{
   unchar s[12];
   unint year1;
   unchar month1;
   unchar day1;
  switch (MenuStatus)
  {
  case 2:
//**************************88
     if(CURPATH.CurrentDirStarClusID !=0)
       {
         CURPATH.CurrentDirStarClusID = 0;
       }
    if((ReadFile("CONFIG  TXT", (unchar*)pCfgHead, 0,sizeof (CFG_HEAD))!=0xff))//ȡ��ϵͳͷ
     {     
           Index = 1;
         s[0] = Index/1000+0x30;         
         s[1] = (Index%1000)/100+0x30;
         s[2] = ((Index%1000)%100)/10+0x30;
         s[3] = ((Index%1000)%100)%10+0x30;
         s[4] = '\0'; 
      Str_8x16(0,0,"��:");
      Str_8x16(0,0,"��:");
      Str_8x16(0,3,s);
      DispFanBai(0,3,4);
     }
     else
     {
       Str_8x16(1,0 ,"ϵͳ�ļ�������");
        Str_8x16(1,0 ,"ϵͳ�ļ�������");
       Delay_ms(2000);
       MenuStatus = 3;
       return;
     }     
      while(1)
      {
        KeyScan();
         switch(GetKey1())
         {
         case ENTER_KEY:
         DispFanBai(0,3,4);//��λ��ȡ������  
         s[0] = Index/1000+0x30;         
         s[1] = (Index%1000)/100+0x30;
         s[2] = ((Index%1000)%100)/10+0x30;
         s[3] = ((Index%1000)%100)%10+0x30;
         s[4] = '\0';
       if(ChgDir(s)==0xff)//û�и�Ŀ¼              
       {
         Str_8x16(1,0,"�����ڸ��������    ");
         Str_8x16(1,0,"�����ڸ��������    ");
         Delay_ms(4000);
         MenuStatus = 3;
         return;
       }
       
        //������ָ����Ŀ¼��   
         goto next ; 
         
       
       
         case UP_KEY:
         if(Index ==pCfgHead->NumberBatGrps)
         { 
          Index=1;
          
         } 
         else
         {  
          Index++;
          
         }
         s[0] = Index/1000+0x30;         
         s[1] = (Index%1000)/100+0x30;
         s[2] = ((Index%1000)%100)/10+0x30;
         s[3] = ((Index%1000)%100)%10+0x30;
         s[4] = '\0';
         
         Str_8x16(0,3,s);
         Str_8x16(0,3,s);
         DispFanBai(0,3,4);
           
           break;           
           
           
          
         case DOWN_KEY:
        if(Index==1)
         { 
          Index = pCfgHead->NumberBatGrps;
         
         } 
         else
         {  
          Index--; 
          
         }  
         s[0] = Index/1000+0x30;         
         s[1] = (Index%1000)/100+0x30;
         s[2] = ((Index%1000)%100)/10+0x30;
         s[3] = ((Index%1000)%100)%10+0x30;
         s[4] = '\0';
         
         Str_8x16(0,3,s);
         Str_8x16(0,3,s);
         DispFanBai(0,3,4);              
           break;
         case ESC_KEY:
           break;
         default:
           break;
         }         
      }  
    
//������µ��ļ� 
  next:
     Temp[0] =year-2000;
     s[0] = Temp[0]/10+0x30;
     s[1] = (Temp[0]%10)+0x30;
     s[2]= 0xc4;//��
     s[3] =0xea;
     s[4]=0;
     Str_8x16(1, 0, s);
     Str_8x16(1, 0, s);
     Temp[1] =month;
     s[0] = Temp[1]/10+0x30;
     s[1] = (Temp[1]%10)+0x30;
     s[2]= 0xd4;//��
     s[3] =0xc2;
     s[4] = 0;
     Str_8x16(1, 4, s); 
     Str_8x16(1, 4, s); 
     Temp[2] =day;
     s[0] = Temp[2]/10+0x30;
     s[1] = (Temp[2]%10)+0x30;
     s[2]= 0xc8;//��
     s[3]=0xd5;
     s[4] = 0;
     Str_8x16(1, 8, s); 
     Str_8x16(1, 8, s); 

     //������ʾ��һ�
     Index=0;
     DispFanBai(1,0,2);  
   while(1)
   {
     KeyScan();
     switch(GetKey1())   
     {
       case ENTER_KEY:
         if(Index ==2)
         {
           
           Str_8x16(1,0,"                ");
           Str_8x16(1,0,"                ");
           year1=Temp[0]+2000;
           month1 =Temp[1];
           day1=Temp[2];
           DIR *ID=(DIR*)&Temp[12];
           s[0] = year1/1000+0x30;
           s[1] = (year1%1000)/100+0x30;
           s[2] = ((year1%1000)%100)/10+0x30;
           s[3] = ((year1%1000)%100)%10+0x30;    
           s[4]=month1/10+0x30;
           s[5]= month1%10+0x30;
           s[6]= day1/10+0x30;
           s[7]=day1%10+0x30;
           s[8]='D';
           s[9]='A';
           s[10]='T';
           s[11]=0; 
           for(unchar i =0;i<11;i++)//�����ļ�������;
           {
             Temp[200+i]=s[i];
           }
           if(GetFileID(s, ID)==0xffff) //�ļ�������
           {
             Str_8x16(1,0,"��ѯ���ļ�������");
             Str_8x16(1,0,"��ѯ���ļ�������");
             Delay_ms(4000);
             MenuStatus =3;
             return;
             
           }
           memcpy(&Temp[100],(unchar*)&(ID->FilePosit.Size),4);//�����ļ����ȱ���
           //   *(unint32*)&Temp[100]=*(&(ID->FilePosit.Size));
           if(ID->FilePosit.Size >=16+8) 
           {
             if(ReadFile(s,Temp,16,8)!=0xff)
             {  
               float f;
               memcpy(&f,Temp,4);
               sprintf((char*)s,"%6.3f",f); 
               Str_8x16(1,0,"��ѹ:");
               Str_8x16(1,0,"��ѹ:");
               Str_8x16(1,12,"V");
               Str_8x16(1,5,s);
               memcpy(&f,&Temp[4],4);
               sprintf((char*)s,"%6.3f",f); 
               Str_8x16(2,0,"����:");
               Str_8x16(2,0,"����:");
               Str_8x16(2,12,"m��");
               Str_8x16(2,5,s);
               Index =1;
               s[0] = Index/1000+0x30;         
               s[1] = (Index%1000)/100+0x30;
               s[2] = ((Index%1000)%100)/10+0x30;
               s[3] = ((Index%1000)%100)%10+0x30;
               s[4] = '\0'; 
               Str_8x16(0,12,s); 
               Str_8x16(0,12,s); 
               DispFanBai(0,12,4);
               //*********************8
               MenuStatus =1000;
               return;

               
               //*****************         
               
               
               
             }
             else
             {
               Str_8x16(1,0,"���ļ�����");
               Str_8x16(1,0,"���ļ�����");
               MenuStatus =3;
               return;           
             }  
             
           }
           else
           {
             Str_8x16(1,0,"�ļ��ǿյ�");
             Str_8x16(1,0,"�ļ��ǿյ�");
             Delay_ms(4000);
             MenuStatus =3;
             return;         
           }  
           
         }
           

         else
         //�ı䷴����ʾ��λ��
           DispFanBai(1,Index*4,2); //ȡ��ԭ�ȵķ���           
           Index++;
           DispFanBai(1,Index*4,2);//��Ϊ��ǰ�ķ���           
         
         break;
        case UP_KEY:
          if(Index ==0) //��
          {
          if(Temp[Index]==99)
          Temp[Index]=0;
          else
          Temp[Index]++; 
          s[0] = Temp[Index]/10+0x30;
          s[1] = (Temp[Index]%10)+0x30;
          s[2] = '\0';
          }
          if(Index ==1)//��
          {
          if(Temp[Index]==12)
          Temp[Index]=1;
          else
          Temp[Index]++; 
          s[0] = Temp[Index]/10+0x30;
          s[1] = (Temp[Index]%10)+0x30;
          s[2] = '\0';
          } 
          if(Index ==2)//��
          {
            
          if(Temp[Index]==MAX1())
          Temp[Index]=1;
          else
          Temp[Index]++; 
          s[0] = Temp[Index]/10+0x30;
          s[1] = (Temp[Index]%10)+0x30;
          s[2] = '\0';
          }            
          
          Str_8x16(1, Index*4, s);
          Str_8x16(1, Index*4, s);
          DispFanBai(1,Index*4,2);          
          break;
        case DOWN_KEY:
          if(Index ==0) //��
          {
          if(Temp[Index]==0)
          Temp[Index]=99;
          else
          Temp[Index]--; 
          s[0] = Temp[Index]/10+0x30;
          s[1] = (Temp[Index]%10)+0x30;
          s[2] = '\0';
          }
          if(Index ==1)//��
          {
          if(Temp[Index]==1)
          Temp[Index]=12;
          else
          Temp[Index]--; 
          s[0] = Temp[Index]/10+0x30;
          s[1] = (Temp[Index]%10)+0x30;
          s[2] = '\0';
          } 
          if(Index ==2)//��
          {
            
          if(Temp[Index]==1)
          Temp[Index]=MAX1();
          else
          Temp[Index]--; 
          s[0] = Temp[Index]/10+0x30;
          s[1] = (Temp[Index]%10)+0x30;
          s[2] = '\0';
          }            
          
          Str_8x16(1, Index*4, s);
          Str_8x16(1, Index*4, s);
          DispFanBai(1,Index*4,2); ;          
          break;
        case ESC_KEY:
          if(Index ==0)
          {
            
          }
 
         else if(Index==2)
          {
            ////�������ݲ��˵��ϼ��˵�
           MenuStatus = 3;
           return; 
          }

          else
          { 
                     //�ı䷴����ʾ��λ��
            
           DispFanBai(1,Index*4,2); //ȡ��ԭ�ȵķ���
           Index--; 
           DispFanBai(1,Index*4,2);//��Ϊ��ǰ�ķ���
          } 
          break;

     }     
   }     
    

  case 1000:
    switch(GetKey1())
    {
         unint32 k; 
         case ENTER_KEY:
           MenuStatus =3;
           return;

         case UP_KEY:

           memcpy(&k,&Temp[100],4);
           if(Index ==((k-16)/8 ))
         { 
          Index=1;
          
         } 
         else
         {  
          Index++;
          
         }
         s[0] = Index/1000+0x30;         
         s[1] = (Index%1000)/100+0x30;
         s[2] = ((Index%1000)%100)/10+0x30;
         s[3] = ((Index%1000)%100)%10+0x30;
         s[4] = '\0';
         
         Str_8x16(0,12,s);
         Str_8x16(0,12,s);
         if(ReadFile(&Temp[200],Temp,16+Index*8-8,8)!=0xff)
         {  
           float f;
           memcpy(&f,Temp,4);
           Str_8x16(1,0,"��ѹ:");
            Str_8x16(1,0,"��ѹ:");
           sprintf((char*)s,"%6.3f",f); 

           Str_8x16(1,5,s);
           Str_8x16(1,5,s);
           memcpy(&f,&Temp[4],4);
           Str_8x16(2,0,"����:");           
           Str_8x16(2,0,"����:");           
           sprintf((char*)s,"%6.3f",f); 

           Str_8x16(2,5,s);
           Str_8x16(2,5,s);
         //  Index =1;
         s[0] = Index/1000+0x30;         
         s[1] = (Index%1000)/100+0x30;
         s[2] = ((Index%1000)%100)/10+0x30;
         s[3] = ((Index%1000)%100)%10+0x30;
         s[4] = '\0';
         Str_8x16(0,12,s); 
         Str_8x16(0,12,s); 
         DispFanBai(0,12,4);
         DispFanBai(0,12,4);
       
         
         
            
         }
         else
         {
         Str_8x16(1,0,"���ļ�����");
         Str_8x16(1,0,"���ļ�����");
         MenuStatus =3;
         return;           
         }          
         
         DispFanBai(0,12,4);
           
           break;
         case DOWN_KEY:

           memcpy(&k,&Temp[100],4);      
           if(Index ==1)
         { 
          Index=(k-16)/8 ;
          
         } 
         else
         {  
          Index--;
          
         }
         s[0] = Index/1000+0x30;         
         s[1] = (Index%1000)/100+0x30;
         s[2] = ((Index%1000)%100)/10+0x30;
         s[3] = ((Index%1000)%100)%10+0x30;
         s[4] = '\0';
         Str_8x16(0,12,s);         
         Str_8x16(0,12,s);
         if(ReadFile(&Temp[200],Temp,16+Index*8-8,8)!=0xff)
         {  
           float f;
           memcpy(&f,Temp,4);
           Str_8x16(1,0,"��ѹ:");
           Str_8x16(1,0,"��ѹ:");
           sprintf((char*)s,"%6.3f",f); 

           Str_8x16(1,5,s);
           Str_8x16(1,5,s);
           memcpy(&f,&Temp[4],4);
           Str_8x16(2,0,"����:");           
           Str_8x16(2,0,"����:");           
           sprintf((char*)s,"%6.3f",f); 

           Str_8x16(2,5,s);
           Str_8x16(2,5,s);
         s[0] = Index/1000+0x30;         
         s[1] = (Index%1000)/100+0x30;
         s[2] = ((Index%1000)%100)/10+0x30;
         s[3] = ((Index%1000)%100)%10+0x30;
         s[4] = '\0';

         Str_8x16(0,12,s); 
         Str_8x16(0,12,s); 
         DispFanBai(0,12,4);
         DispFanBai(0,12,4);
       
           
         }
         else
         {
         Str_8x16(1,0,"���ļ�����");
         MenuStatus =3;
         return;           
         }          
         
         DispFanBai(0,12,4);           
           break;
         case ESC_KEY:
           MenuStatus =3;
           return;           


    }
    break;
  case 2000:
    break;
    
  }
}


//********************************
//2.1�������
//*******************************

void RandomMeasure(void)
{
      unchar i=0;
    unchar s[12];
     
     unint fint;
     float fres;
     float f,arf[3]; 
  
  switch(MenuStatus)
  {
    case 2:    
              //��cy��Ƭ���ϵ�򿪴��ڲ����д��ڳ�ʼ����
         pRecv0Str=&Temp[20];//���ý��ջ������ĵ�ַ��       
         POWRON_CY;
         Init_Uart0;

         Uart0RxTimeOut = 0;
           Recv0State =0;
           Recv0Index =0;         
         //�ȴ�cy��Ƭ������Ӧ����
         while(1)
         {
           if(GetKey2( pAck,Temp,10)!=NULL)
           {
             //�ж��õ��������Ƿ�Ϸ�
             if((Temp[4]==0x04)&&(Temp[5]==0x03)&&(Temp[6]==0x02)&&(Temp[7]==0x01))
             {
               unchar  data[4];
               data[0] =10;//�趨��ַΪ10
               data[1] = 0;
               data[2] = 0;
               data[3] = 0;
               Recv0State =0;
               Recv0Index =0;
               SendCmd(SET_ADRESS,0,data);  
               Uart0RxTimeOut =0;
             break;
             }
              Str_8x16(1,0,"����ģ�����");
              Str_8x16(1,0,"����ģ�����");
             break;
           }
           
          if(Uart0RxTimeOut>=300)
          {
            //cy��Ƭ��û����Ӧ�˳���
            Str_8x16(1,0,"����ģ��û����Ӧ");
            Str_8x16(1,0,"����ģ��û����Ӧ");
            Delay_ms(2000);
            POWROFF_CY;
            MenuStatus =3;
            return;
          }
         }
             while(1)
             {
               if(GetKey2( pAck,Temp,10)!=NULL)//�õ�cy���趨��ַ��Ӧ
                 
               {
                 if(Temp[2]==10)
                 {
 
                    break;
                 }
               Str_8x16(1,0,"������Ԫ����");                 
               Str_8x16(1,0,"������Ԫ����"); 
                break; 
               }
         if(Uart0RxTimeOut>=300)
          {
            //cy��Ƭ��û����Ӧ�˳���
            Str_8x16(1,0,"����ģ��û����Ӧ");
            Str_8x16(1,0,"����ģ��û����Ӧ");
            Delay_ms(2000);
            POWROFF_CY;
            MenuStatus =3;
            return;
          }               
             }          
        MenuStatus =1000;
      break;
  case 1000:


   while(1)
   {
   cont:    
   Uart0RxTimeOut=0;
   Recv0State =0;
   Recv0Index =0;
                  unchar  data[4];
               data[0] =0;//�趨��ַΪ10
               data[1] = 0;
               data[2] = 0;
               data[3] = 0;
    Str_8x16(0,0,"���ڲ�����...     "); 
    Str_8x16(0,0,"���ڲ�����...     ");     
    Str_8x16(1,0,"                  ");     
     SendCmd(MEASURE_VOLT,10,data);
     while(1)
     { 
       if(GetKey2( pAck,Temp,10)!=NULL)
       {
         memcpy((unchar*)&fint,&Temp[4],2);
         f=(float)fint/1000;//ע���ֽڶ������⣻
        
       //  if((f>=pGrpHead->BatVoltMin)&&(f<=pGrpHead->BatVoltMax))
         {
           arf[i]=f;
           
           i++;
           break;
         }
        /* 
         else
         {
            Str_8x16(1,0,"�������");
            Str_8x16(1,0,"�������");
             goto cont;        
           
         }
         */
       }
       else if(Uart0RxTimeOut>=300)//��ʱ�˳���
          {
            //cy��Ƭ��û����Ӧ�˳���
            Str_8x16(1,0,"����ģ��û����Ӧ");
            Str_8x16(1,0,"����ģ��û����Ӧ");
            Delay_ms(5000);
            POWROFF_CY;
            MenuStatus =3;
            return;
          }

       
     } 


     if(i==3)
     {
       //�Ѿ�������3��
       i =0;
       float f1 = fabsf(arf[0]-arf[1]);
       float f2 = fabsf(arf[0]-arf[2]);
       float f3 = fabsf(arf[2]-arf[1]);
       
       
       if((f1<=0.01)&&(f2<=0.01)&&(f3<=0.01))//�Ƚ���Ч��
       {
         
         //���Ӽк��˲������õ����κϸ�ĵ�ѹֵ��
         //�˳���ѹ�������е��������
         
         //*******************8
         
         //��������ֵ�����
         Uart0RxTimeOut = 0;
         
         SendCmd(MEASURE_RES,10,data);
         // DispFanBai(0,12,4);//ȡ�����ס�
         f=(arf[0]+arf[1]+arf[2])/3;
         while(1)
         {
           if(GetKey2( pAck,Temp,10)!=NULL)
           { 
             
             memcpy(Temp,&f,4);
             memcpy(&fint,&Temp[4],2);
             fres =(float)fint/1000;
             memcpy(&Temp[4],&fres,4);
             // ��ʾ����ѹ������ֵ��
             sprintf((char*)s,"%5.3f",f);
             Str_8x16(1,0,"��ѹ:");
             Str_8x16(1,0,"��ѹ:");
             Str_8x16(1,10,"V");
             Str_8x16(1,5,s);
             
             sprintf((char*)s,"%5.3f",fres);
             Str_8x16(2,0,"����:");
             Str_8x16(2,0,"����:");
             Str_8x16(2,10,"m��");         
             Str_8x16(2,5,s);
             
 
             //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
             //��ʼ��һ����صĲ���
             //  Str_8x16(2,0, "ENT����һ�� ESC���˳�");
             
             unchar key;
              Str_8x16(0,0,"�������        ");
              Str_8x16(0,0,"�������        ");              
             while(1)//�ȴ��û�������
               
             {
               KeyScan();
               
               key=GetKey1();
               if(key==ENTER_KEY) 
               {


                 Str_8x16(1,0,"                ");           
                 Str_8x16(1,0,"                ");           
                 Str_8x16(2,0,"                ");  
                 Str_8x16(3,0,"                "); 
                 
                 Uart0RxTimeOut=0;
                 Recv0State =0;
                 Recv0Index =0;
                 goto cont;
               }
               else if(key==ESC_KEY)
               {
                 Delay_ms(2000);
                 POWROFF_CY;
                 MenuStatus =3;
                 return;            
                 
                 
               }
               //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^         
               
             }        
             
             
             
             
             //     break;//�˳�������ֵ��Ӧ��ѭ����
           }
           else if(Uart0RxTimeOut>=300)//��ʱ�˳���
           {
             //cy��Ƭ��û����Ӧ�˳���
             Str_8x16(1,0,"����ģ��û����Ӧ");
             Str_8x16(1,0,"����ģ��û����Ӧ");             
             Delay_ms(5000);
             POWROFF_CY;
             MenuStatus =3;
             return;
           }
           
           
         }
         
         
         //***************************************        
         
         
         //break;  //�˳�case2000
         
       }
       else
       {
         //�ж�Ϊ����û�мк�
         Str_8x16(3,0,"û�к������²���");
         Str_8x16(3,0,"û�к������²���");         
         unchar key;
         while(1)
         {
           KeyScan();
           key=GetKey1();
           if(key==ENTER_KEY)
           {
             i=0;
             Str_8x16(3,0,"                "); 
             Str_8x16(3,0,"                ");
             break;//�������Ե�ǰ��صĲ���
           }
           else if(key==ESC_KEY)           //�����Ե�ǰ��ز�����Ϊ�����ؿ����Ѿ����˽��Ų�����ĵ�ء�
           {


               Delay_ms(2000);
               POWROFF_CY;
               MenuStatus =3;
               return;         
               
            }

             
             
           }
           
         }
         // break;
       }
       // break;
     }       

     
  

    
  }

}






//***************************
//����������°汾��
//2.1
//*****************************

void RandomMeasure1(void )
{
      unchar i=0;
    unchar s[12];
     
     unint fint;
     float fres;
     float f,arf[3]; 
  
  switch(MenuStatus)
  {
    case 2:    
              //��cy��Ƭ���ϵ�򿪴��ڲ����д��ڳ�ʼ����
         pRecv0Str=&Temp[20];//���ý��ջ������ĵ�ַ��       
         POWRON_CY;
         Init_Uart0;
          WIFI_RX_CONNECT=100;
         Uart0RxTimeOut = 0;
           Recv0State =0;
           Recv0Index =0;         
         //�ȴ�cy��Ƭ������Ӧ����
         while(1)
         {
           if(GetKey2( pAck,Temp,10)!=NULL)
           {
             //�ж��õ��������Ƿ�Ϸ�
             if((Temp[4]==0x04)&&(Temp[5]==0x03)&&(Temp[6]==0x02)&&(Temp[7]==0x01))
             {
               unchar  data[4];
               data[0] =10;//�趨��ַΪ10
               data[1] = 0;
               data[2] = 0;
               data[3] = 0;
               Recv0State =0;
               Recv0Index =0;
               SendCmd(SET_ADRESS,0,data);  
               Uart0RxTimeOut =0;
             break;
             }
              Str_8x16(1,0,"����ģ�����");
              Str_8x16(1,0,"����ģ�����");
             break;
           }
           
          if(Uart0RxTimeOut>=300)
          {
            //cy��Ƭ��û����Ӧ�˳���
            Str_8x16(1,0,"����ģ��û����Ӧ");
            Str_8x16(1,0,"����ģ��û����Ӧ");
            Delay_ms(2000);
            POWROFF_CY;
            MenuStatus =3;
            return;
          }
         }
             while(1)
             {
               if(GetKey2( pAck,Temp,10)!=NULL)//�õ�cy���趨��ַ��Ӧ
                 
               {
                 if(Temp[2]==10)
                 {
 
                    break;
                 }
               Str_8x16(1,0,"������Ԫ����");                 
               Str_8x16(1,0,"������Ԫ����"); 
                break; 
               }
         if(Uart0RxTimeOut>=300)
          {
            //cy��Ƭ��û����Ӧ�˳���
            Str_8x16(1,0,"����ģ��û����Ӧ");
            Str_8x16(1,0,"����ģ��û����Ӧ");
            Delay_ms(2000);
            POWROFF_CY;
            MenuStatus =3;
            return;
          }               
             }          
        MenuStatus =1000;
      break;
  case 1000:


   while(1)
   {
    cont:   
   Uart0RxTimeOut=0;
   Recv0State =0;
   Recv0Index =0;
                  unchar  data[4];
               data[0] =0;//�趨��ַΪ10
               data[1] = 0;
               data[2] = 0;
               data[3] = 0;
    Str_8x16(0,0,"���ڲ�����...     "); 
    Str_8x16(0,0,"���ڲ�����...     ");     
    Str_8x16(1,0,"                  ");     
     SendCmd(MEASURE_VOLT,10,data);
     while(1)
     { 
       if(GetKey2( pAck,Temp,10)!=NULL)
       {
         memcpy((unchar*)&fint,&Temp[4],2);
         f=(float)fint/1000;//ע���ֽڶ������⣻
        
       //  if((f>=pGrpHead->BatVoltMin)&&(f<=pGrpHead->BatVoltMax))
         {
           arf[i]=f;
           
           i++;

           break;
         }
        /* 
         else
         {
            Str_8x16(1,0,"�������");
            Str_8x16(1,0,"�������");
             goto cont;        
           
         }
         */
       }
       else if(Uart0RxTimeOut>=300)//��ʱ�˳���
          {
            //cy��Ƭ��û����Ӧ�˳���
            Str_8x16(1,0,"����ģ��û����Ӧ");
            Str_8x16(1,0,"����ģ��û����Ӧ");
            Delay_ms(5000);
            POWROFF_CY;
            MenuStatus =3;
            return;
          }

       
     } 


     if(i==3)
     {
       //�Ѿ�������3��
       i =0;
       float f1 = fabsf(arf[0]-arf[1]);
       float f2 = fabsf(arf[0]-arf[2]);
       float f3 = fabsf(arf[2]-arf[1]);
       
       if((arf[0]>=1)&&(arf[0]<=4)&&(arf[1]>=1)&&(arf[1]<=4)&&(arf[2]>=1)&&(arf[2]<=4))//�Ƚ���Ч��
      // if((f1<=0.01)&&(f2<=0.01)&&(f3<=0.01))//�Ƚ���Ч��
       {
         
         //���Ӽк��˲������õ����κϸ�ĵ�ѹֵ��
         //�˳���ѹ�������е��������
         
         //*******************8
         
         //��������ֵ�����
         Uart0RxTimeOut = 0;
         
         SendCmd(MEASURE_RES,10,data);
         // DispFanBai(0,12,4);//ȡ�����ס�
         f=(arf[0]+arf[1]+arf[2])/3;
         while(1)
         {
           if(GetKey2( pAck,Temp,10)!=NULL)
           { 
             
             memcpy(Temp,&f,4);
             memcpy(&fint,&Temp[4],2);
             fres =(float)fint/1000;
             memcpy(&Temp[4],&fres,4);
             // ��ʾ����ѹ������ֵ��
             sprintf((char*)s,"%5.3f",f);
             Str_8x16(1,0,"��ѹ:");
             Str_8x16(1,0,"��ѹ:");
             Str_8x16(1,10,"V");
             Str_8x16(1,5,s);
             Str_8x16(3,0,s);
             
             sprintf((char*)s,"%5.3f",fres);
             Str_8x16(2,0,"����:");
             Str_8x16(2,0,"����:");
             Str_8x16(2,11,"m��");         
             Str_8x16(2,5,s);
             Str_8x16(3,8,s);             
             
 
             //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
             //��ʼ��һ����صĲ���
             //  Str_8x16(2,0, "ENT����һ�� ESC���˳�");
             
       //      unchar key;
              Str_8x16(0,0,"�������        ");
              Str_8x16(0,0,"�������        ");
              Delay_ms(1000);
                 Str_8x16(1,0,"                ");           
                 Str_8x16(1,0,"                ");           
                 Str_8x16(2,0,"                ");  
              

                 goto cont;
/*                 
             while(1)//�ȴ��û�������
               
             {
               KeyScan();
               
               key=GetKey1();
               if(key==ENTER_KEY) 
               {


                 Str_8x16(1,0,"                ");           
                 Str_8x16(1,0,"                ");           
                 Str_8x16(2,0,"                ");  
                 Str_8x16(3,0,"                "); 
                 
                 Uart0RxTimeOut=0;
                 Recv0State =0;
                 Recv0Index =0;
                 goto cont;
               }
               else if(key==ESC_KEY)
               {
                 Delay_ms(2000);
                 POWROFF_CY;
                 MenuStatus =3;
                 return;            
                 
                 
               }
               //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^         
               
             }
*/             
             
             
             
             
             //     break;//�˳�������ֵ��Ӧ��ѭ����
           }
           else if(Uart0RxTimeOut>=300)//��ʱ�˳���
           {
             //cy��Ƭ��û����Ӧ�˳���
             Str_8x16(1,0,"����ģ��û����Ӧ");
             Str_8x16(1,0,"����ģ��û����Ӧ");             
             Delay_ms(5000);
             POWROFF_CY;
             MenuStatus =3;
             return;
           }
           
           
         }
         
         
         //***************************************        
         
         
         //break;  //�˳�case2000
         
       }
       else
       {
         //�ж�Ϊ����û�мк�
                 Str_8x16(0,0,"                ");  
                 Str_8x16(0,0,"                ");
                 Str_8x16(1,0,"                ");           
                 Str_8x16(2,0,"                ");          
       //    Str_8x16(3,0,"                ");
      //   Str_8x16(3,0,"û�кÿ�ʼ���²�");
         Delay_ms(3000);         
       //  unchar key;
 /*       
         while(1)
         {
           KeyScan();
           key=GetKey1();
           if(key==ENTER_KEY)
           {
             i=0;
             Str_8x16(3,0,"                "); 
             Str_8x16(3,0,"                ");
             break;//�������Ե�ǰ��صĲ���
           }
           else if(key==ESC_KEY)           //�����Ե�ǰ��ز�����Ϊ�����ؿ����Ѿ����˽��Ų�����ĵ�ء�
           {


               Delay_ms(2000);
               POWROFF_CY;
               MenuStatus =3;
               return;         
               
            }

             
             
           }
*/         
           
         }
         // break;
       }
       // break;
     }       

     
  

    
  }

}


