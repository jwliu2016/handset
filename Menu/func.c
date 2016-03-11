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
//判定每月天数的必要函数。
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
//功能 ：设置ip地址，子网掩码，网关
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
   if(pCurrentMenu == &MyMenu[16])//如果这是设置IP地址。           
   {
    Str_8x16(0, 0,"IP地址设置" ); 
    Str_8x16(0, 0,"IP地址设置" );     
    offset =4;
   }
   if(pCurrentMenu == &MyMenu[17])//如果这是设置子网掩码。           
   {
    Str_8x16(0, 0,"子网掩码设置" );
    Str_8x16(0, 0,"子网掩码设置" );  
    offset=8;
   }
   if(pCurrentMenu == &MyMenu[18])//如果这是设置网关地址。           
   {
    Str_8x16(0, 0,"网关设置" ); 
    Str_8x16(0, 0,"网关设置" ); 
    offset=12;
   }   
   if((ReadFile("CONFIG  TXT", Temp, offset,4)!=0xff))
   {
     //显示ip

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
     //反白显示第一项。
     Index=0;
     DispFanBai(1,0,3);
     MenuStatus =1000;
     
   }
   
   else
   {
     //文件不存在提示文件不存在确定键返回；
    return; 
   }
   
       

   break;
 case 1000:
     switch(GetKey1())   
     {
       case ENTER_KEY:
         if(Index ==3)
         {
          //保存数据到sd卡并退到上级菜单。
           if(pCurrentMenu == &MyMenu[16])//如果这是设置IP地址。           
           {
             ModifyFile("CONFIG  TXT", Temp,4, 4);
             MenuStatus =3;
             return;
           }
           if(pCurrentMenu == &MyMenu[18])//如果这是设置网关地址。           
           {
             ModifyFile("CONFIG  TXT", Temp,12, 4);
             MenuStatus =3;
             return;
           }           
           if(pCurrentMenu == &MyMenu[17])//如果这是设置子网掩码还需知道子网掩码的合法性。
           {
             unint32 mask;
             mask=(unint32)&Temp[0];
             if(((mask-1)|mask)==0xffffffff)//（mask -1 | mask） ==  0xFFFFFFFF ,如果是TRUE就是子网掩码，是FALSE就不是子网掩码。
             {
               //是合法的子网掩码；保存退出。
               ModifyFile("CONFIG  TXT", Temp,8, 4);
               MenuStatus =3;
              return;
             }
             MenuStatus =3;
             return;//放弃不保存退出。
           }
 
           
         }
           

         else
         //改变反白显示的位置
            //增加lcm补丁
          s[0] = Temp[Index]/100+0x30;
          s[1] = (Temp[Index]%100)/10+0x30;
          s[2] = (Temp[Index]%100)%10+0x30;

          s[4] = '\0';
          Str_8x16(1, Index*4, s);
          Str_8x16(1, Index*4, s);            
         //  DispFanBai(1,Index*4,3); //取消原先的反白           
           Index++;
           DispFanBai(1,Index*4,3);//改为当前的反白           
         
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
            ////放弃数据并退到上级菜单
           MenuStatus = 3;
           return; 
          }

          else
          { 
                     //改变反白显示的位置
            //增加lcm补丁
          s[0] = Temp[Index]/100+0x30;
          s[1] = (Temp[Index]%100)/10+0x30;
          s[2] = (Temp[Index]%100)%10+0x30;

          s[4] = '\0';
          Str_8x16(1, Index*4, s);
          Str_8x16(1, Index*4, s);          
            
            
      //     DispFanBai(1,Index*4,3); //取消原先的反白
           Index--; 
           DispFanBai(1,Index*4,3);//改为当前的反白
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
//功能：设置密码
//***********************************
       
 void SetPass(void)
 {
   unchar s[4];
   switch(MenuStatus)
   {
   case 2:
      Str_8x16(0, 0,"请输入新密码" );
      Str_8x16(0, 0,"请输入新密码" );
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
               //保存密码并退出。
               
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
  // 1.3.0功能 设置模式
  //*******************************************
 void SetModel(void)
 {
   switch(MenuStatus) 
   {
     
   case 2:
     Str_8x16(1, 0,"添加/编辑/删除" );
     Str_8x16(1, 0,"添加/编辑/删除" );
      DispFanBai(1,0,4) ;
      ModefyModel= 0;//默认是添加状态
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
             DispFanBai(1,Index*5,4);//去掉旧有的反白
             Index++; 
            
             DispFanBai(1,Index*5,4);//增加当前反白；
           }
           
             
           break;
         case DOWN_KEY:
           if(Index >0)
           {
             DispFanBai(1,Index*5,4);//去掉旧有的反白
             Index--; 
            
             DispFanBai(1,Index*5,4);//增加当前反白；
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
  // 1.3.1功能 电池组编号
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
       Str_8x16(1, 0,"添加电池组编号" );
       Str_8x16(1, 0,"添加电池组编号" );
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
       Str_8x16(1, 0,"编辑电池组编号" );
       Str_8x16(1, 0,"编辑电池组编号" );
     }
     if(ModefyModel ==2)
     {
       Str_8x16(1, 0,"删除电池组编号" );
       Str_8x16(1, 0,"删除电池组编号" );
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
            Str_8x16(1,0,"设置已经保存成功");
            Str_8x16(1,0,"设置已经保存成功");
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
  // 1.3.2功能 电池数量
  //功能：设置当前电池组的电池数量。           
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
       Str_8x16(1, 0,"添加电池数量" ); 
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
         //配置文件不存在。
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
            Str_8x16(1,0,"设置已经保存成功");
            Str_8x16(1,0,"设置已经保存成功");
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
  // 1.3.3功能 电压等级
  //功能：设置当前电池组的电压等级。           
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
       Str_8x16(1, 0,"添加电压等级" ); 
       Str_8x16(1, 0,"添加电压等级" );       
      if((ReadFile("CONFIG  TXT", Temp, 22,2)!=0xff))//取出系统默认的电压等级
       {
         s[0] = *(unint*)Temp/1000+0x30;         
         s[1] = (*(unint*)Temp%1000)/100+0x30;
         s[2] = ((*(unint*)Temp%1000)%100)/10+0x30;
         s[3] = ((*(unint*)Temp%1000)%100)%10+0x30;
         s[4] = '\0';
         if(((unint)Temp !=2)||((unint)Temp !=6)||((unint)Temp !=12))
         {  
         Str_8x16(1,0, "非法的电压等级");
         Str_8x16(1,0, "非法的电压等级");         
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
         //配置文件不存在。
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
            Str_8x16(1,0,"设置已经保存成功");
            Str_8x16(1,0,"设置已经保存成功");
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
  // 1.3.4 基准内阻。          
  //功能： 设置基准内阻。
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
       Str_8x16(1, 0,"添加基准内阻" ); 
       Str_8x16(1, 0,"添加基准内阻" ); 
     if((ReadFile("CONFIG  TXT", Temp, 24,4)!=0xff))//取出系统默认的基准内阻
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
      //   *s=*(unchar*)&f;这种方法不可取只能用memcpy 或一字节一字节的手动存取如上面注释掉的部分。

         sprintf((char*)s,"%f",f);
 
          Str_8x16(1,0,s);
         Str_8x16(1,0,s);
         DispFanBai(1,0,4);
         MenuStatus=1000;
         
       }
       else
       {
         //配置文件不存在。
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
  // 3整组测量。          
  //功能： 实现某组电池的整组测量。
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
     if((ReadFile("CONFIG  TXT", (unchar*)pCfgHead, 0,sizeof (CFG_HEAD))!=0xff))//取出系统头
     {
      
      Index = 1;

     // t=*(unint*)Temp;再次出现字节对齐的问题，开始调试的时候是没问题的现在又出了。。。
         s[0] = Index/1000+0x30;         
         s[1] = (Index%1000)/100+0x30;
         s[2] = ((Index%1000)%100)/10+0x30;
         s[3] = ((Index%1000)%100)%10+0x30;
         s[4] = '\0'; 
       

      Str_8x16(0,0,"组:");
      Str_8x16(0,0,"组:");      
      Str_8x16(0,3,s);
 


      DispFanBai(0,3,4);
      MenuStatus =1000;
     }
     else
     {
       Str_8x16(1,0 ,"系统文件不存在");
       Str_8x16(1,0 ,"系统文件不存在");
       Delay_ms(2000);
       MenuStatus = 3;
       return;
     } 
  break;
 case 1000:
   
         switch(GetKey1())
         {
         case ENTER_KEY:
           DispFanBai(0,3,4);//组位置取消反白
           //先从config文件读出该组电池对应的组属性域；
          
          if( ReadFile("CONFIG  TXT",(unchar*)pGrpHead,Index*64,sizeof (GRP_HEAD))==0xff)
          {
            Str_8x16(2,0,"系统配置文件错误");
            Str_8x16(2,0,"系统配置文件错误");            
            Delay_ms(2000);
            MenuStatus =3;
            return;
          }
     
           
           
           //进入该组所在的目录如果没有该组就建立该组的目录。并在该目录下根据时间建立该组的测量文件
           
        if(CURPATH.CurrentDirStarClusID !=0)
       {
         CURPATH.CurrentDirStarClusID = 0;
       }
         s[0] = Index/1000+0x30;         
         s[1] = (Index%1000)/100+0x30;
         s[2] = ((Index%1000)%100)/10+0x30;
         s[3] = ((Index%1000)%100)%10+0x30;
         s[4] = '\0';
       if(ChgDir(s)==0xff)//没有该目录建立该目录并进入
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
       
         
         
       if(GetFileID(s, ID)==0xffff)//文件不存在建立之并根据config.txt建立该文件的默认头部
          {
            CreateFile(s);
         
            pMeasureHead=(MEASURE_HEAD*)Temp;
            (pMeasureHead->DeviceID)=(pCfgHead->DeviceID);
            pMeasureHead->BatGrpID=pGrpHead->BatGrpID;
            pMeasureHead->BatNumbers=pGrpHead->BatNumbers;
            AppendData(s,(unchar*)pMeasureHead,sizeof (MEASURE_HEAD));
          }
       GetFileID(s, ID);
       //判定该组电池今天已经测量了多少块以便进行下面的测量。
           Index =(ID->FilePosit.Size-16)/8+1;//已经存在的电池测量数据组
           if(Index>=pGrpHead->BatNumbers)
           {
             Str_8x16(1,0,"该组已经测量完毕");
             Str_8x16(1,0,"该组已经测量完毕");
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
       Str_8x16(0,12,s);//显示第一个需要测量的电池。
       
         
         //给cy单片机上电打开串口并进行串口初始化；
         pRecv0Str=&Temp[20];//设置接收缓冲区的地址。       
         pAck->tail = 0;
         pAck->head = 0;
         pAck->cBufFlag = 0;
         POWRON_CY;
         Init_Uart0;

         Uart0RxTimeOut = 0;
           Recv0State =0;
           Recv0Index =0;         
         //等待cy单片机的响应包；
         while(1)
         {
           if(GetKey2( pAck,Temp,10)!=NULL)
           {
             //判定得到的数据是否合法
             if((Temp[4]==0x04)&&(Temp[5]==0x03)&&(Temp[6]==0x02)&&(Temp[7]==0x01))
             {
               unchar  data[4];
               data[0] =10;//设定地址为10
               data[1] = 0;
               data[2] = 0;
               data[3] = 0;
               Recv0State =0;
               Recv0Index =0;
               SendCmd(SET_ADRESS,0,data);  
               Uart0RxTimeOut =0;
             break;
             }
              Str_8x16(1,0,"测量模块出错");
              Str_8x16(1,0,"测量模块出错");
             break;
           }
           
          if(Uart0RxTimeOut>=300)
          {
            //cy单片机没有响应退出；
            Str_8x16(1,0,"测量模块没有响应");
            Str_8x16(1,0,"测量模块没有响应");
            Delay_ms(2000);
            POWROFF_CY;
            MenuStatus =3;
            return;
          }
         }


             while(1)
             {
               if(GetKey2( pAck,Temp,10)!=NULL)//得到cy的设定地址回应
                 
               {
                 if(Temp[2]==10)
                 {
 
                    break;
                 }
               Str_8x16(1,0,"测量单元出错");                 
               Str_8x16(1,0,"测量单元出错"); 
                break; 
               }
         if(Uart0RxTimeOut>=300)
          {
            //cy单片机没有响应退出；
            Str_8x16(1,0,"测量模块没有响应");
            Str_8x16(1,0,"测量模块没有响应");
            Delay_ms(2000);
            POWROFF_CY;
            MenuStatus =3;
            return;
          }               
             }         
         
         
         
        //   DispFanBai(0,12,4);//电池编号位置加反白；
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
 case 2000: //开始测量不响应任何按键
   //发送测量命令给cy
   
   i = 0;
   while(1)
   {
   cont:    
   Uart0RxTimeOut=0;
   Recv0State =0;
   Recv0Index =0;
                  unchar  data[4];
               data[0] =0;//设定地址为10
               data[1] = 0;
               data[2] = 0;
               data[3] = 0;
     SendCmd(MEASURE_VOLT,10,data);
     while(1)
     { 
       if(GetKey2( pAck,Temp,10)!=NULL)
       {
         memcpy((unchar*)&fint,&Temp[4],2);
         f=(float)fint/1000;//注意字节对齐问题；
        
         if((f>=pGrpHead->BatVoltMin)&&(f<=pGrpHead->BatVoltMax))
         {
           arf[i]=f;
           
           i++;
           break;
         }
         else
         {
            Str_8x16(1,0,"请检查接线");
            Str_8x16(1,0,"请检查接线");
             goto cont;        
           
         }  
       }
       else if(Uart0RxTimeOut>=300)//超时退出；
          {
            //cy单片机没有响应退出；
            Str_8x16(1,0,"测量模块没有响应");
            Str_8x16(1,0,"测量模块没有响应");
            Delay_ms(5000);
            POWROFF_CY;
            MenuStatus =3;
            return;
          }

       
     }

     if(i==3)
     {
       //已经测量了3次
       i =0;
      float f1 = fabsf(arf[0]-arf[1]);
      float f2 = fabsf(arf[0]-arf[2]);
      float f3 = fabsf(arf[2]-arf[1]);
                       
                        
      if((f1<=0.01)&&(f2<=0.01)&&(f3<=0.01))//比较有效性
      {
        
      //夹子夹好了并连续得到三次合格的电压值；
        //退出电压测量进行电阻测量。

//*******************8
   
        //发测量阻值的命令；
        Uart0RxTimeOut = 0;
        
        SendCmd(MEASURE_RES,10,data);
       // DispFanBai(0,12,4);//取消反白。
        f=(arf[0]+arf[1]+arf[2])/3;
        while(1)
        {
         if(GetKey2( pAck,Temp,10)!=NULL)
         { 
         
         memcpy(Temp,&f,4);
         memcpy(&fint,&Temp[4],2);
         fres =(float)fint/1000;
         memcpy(&Temp[4],&fres,4);
        // 显示出电压和内阻值；
         sprintf(s,"%5.3f",f);
       // unint j =i;
         Str_8x16(1,0,"电压:");
         Str_8x16(1,0,"电压:");
         Str_8x16(1,10,"V");
         Str_8x16(1,5,s);

         sprintf((char*)s,"%5.3f",fres);
         Str_8x16(2,0,"内阻:");
         Str_8x16(2,0,"内阻:");
         Str_8x16(2,10,"mΩ");         
         Str_8x16(2,5,s);

           
         //测量结果存储到文件里
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
       //开始下一个电池的测量
       //  Str_8x16(2,0, "ENT：下一个 ESC：退出");

         unchar key;
       s[0] = Index/1000+0x30;
       s[1] = (Index%1000)/100+0x30;
       s[2] = ((Index%1000)%100)/10+0x30;
       s[3] = ((Index%1000)%100)%10+0x30;         
       s[4] = 0;
       Str_8x16(0,12,s);
       Str_8x16(0,12,s);       
       DispFanBai(0,12,4); //测完了当前电池再次反白此电池表明测量完        
        while(1)//等待用户操作；
          
        {
          KeyScan();
          
          key=GetKey1();
         if(key==ENTER_KEY) 
         {
           if(Index==pGrpHead->BatNumbers)
           {
             //测到了最后一块电池退出系统。
             Str_8x16(1,0,"整组测量完毕");
             Str_8x16(1,0,"整组测量完毕");
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
       

        
        
     //     break;//退出测量阻值的应答循环。
          }
       else if(Uart0RxTimeOut>=300)//超时退出；
          {
            //cy单片机没有响应退出；
            Str_8x16(1,0,"测量模块没有响应");
            Str_8x16(1,0,"测量模块没有响应");
            Delay_ms(5000);
            POWROFF_CY;
            MenuStatus =3;
            return;
          }
          
      
        }

        
//***************************************        


      //break;  //退出case2000
       
      }
      else
      {
        //判定为夹子没有夹好
        Str_8x16(3,0,"没夹好请重新测量");
        Str_8x16(3,0,"没夹好请重新测量");
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
           break;//继续重试当前电池的测量
         }
         else if(key==ESC_KEY)           //放弃对当前电池测量因为这个电池可能已经坏了接着测下面的电池。
         {
           i =0;
       //先占据文件相应的位置等将来进行单独测量。
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
        Str_8x16(3,0,"跳过测量下一只      "); 
        Str_8x16(3,0,"跳过测量下一只      "); 
       }
       else
       {
                //测量完毕;
            Str_8x16(1,0,"整组测量完毕");
            Str_8x16(1,0,"整组测量完毕");
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
// 功能： 1.5.1设置年月日
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
     s[2]= 0xc4;//年
     s[3] =0xea;
     s[4]=0;
     Str_8x16(1, 0, s);
     Str_8x16(1, 0, s);
     Temp[1] =month;
     s[0] = Temp[1]/10+0x30;
     s[1] = (Temp[1]%10)+0x30;
     s[2]= 0xd4;//月
     s[3] =0xc2;
     s[4] = 0;
     Str_8x16(1, 4, s); 
     Str_8x16(1, 4, s); 
     Temp[2] =day;
     s[0] = Temp[2]/10+0x30;
     s[1] = (Temp[2]%10)+0x30;
     s[2]= 0xc8;//日
     s[3]=0xd5;
     s[4] = 0;
     Str_8x16(1, 8, s); 
     Str_8x16(1, 8, s); 

     //反白显示第一项。
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
         //改变反白显示的位置
          //增加lcm不稳定补丁
          s[0] = Temp[Index]/10+0x30;
          s[1] = (Temp[Index]%10)+0x30;
          s[2] = '\0';          
          Str_8x16(1, Index*4, s);
          Str_8x16(1, Index*4, s);           
       //   DispFanBai(1,Index*4,2); //取消原先的反白           
           Index++;
           DispFanBai(1,Index*4,2);//改为当前的反白           
         
         break;
        case UP_KEY:
         
          
          
          if(Index ==0) //年
          {
          if(Temp[Index]==99)
          Temp[Index]=0;
          else
          Temp[Index]++; 
          s[0] = Temp[Index]/10+0x30;
          s[1] = (Temp[Index]%10)+0x30;
          s[2] = '\0';
          }
          if(Index ==1)//月
          {
          if(Temp[Index]==12)
          Temp[Index]=1;
          else
          Temp[Index]++; 
          s[0] = Temp[Index]/10+0x30;
          s[1] = (Temp[Index]%10)+0x30;
          s[2] = '\0';
          } 
          if(Index ==2)//日
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
         
          if(Index ==0) //年
          {
          if(Temp[Index]==0)
          Temp[Index]=99;
          else
          Temp[Index]--; 
          s[0] = Temp[Index]/10+0x30;
          s[1] = (Temp[Index]%10)+0x30;
          s[2] = '\0';
          }
          if(Index ==1)//月
          {
          if(Temp[Index]==1)
          Temp[Index]=12;
          else
          Temp[Index]--; 
          s[0] = Temp[Index]/10+0x30;
          s[1] = (Temp[Index]%10)+0x30;
          s[2] = '\0';
          } 
          if(Index ==2)//日
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
            ////放弃数据并退到上级菜单
           MenuStatus = 3;
           return; 
          }

          else
          { 
                     //改变反白显示的位置
          //增加lcm不稳定补丁
          s[0] = Temp[Index]/10+0x30;
          s[1] = (Temp[Index]%10)+0x30;
          s[2] = '\0';          
          Str_8x16(1, Index*4, s);
          Str_8x16(1, Index*4, s);
             
            
          // DispFanBai(1,Index*4,2); //取消原先的反白
           Index--; 
           DispFanBai(1,Index*4,2);//改为当前的反白
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
// 功能： 1.5.2设置时分秒
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
     s[2]= 0xcA;//时
     s[3] =0xB1;
     s[4]=0;
     Str_8x16(1, 0, s);
     Str_8x16(1, 0, s);
     Temp[1] =min;
     s[0] = Temp[1]/10+0x30;
     s[1] = (Temp[1]%10)+0x30;
     s[2]= 0xB7; //分
     s[3] =0xd6;
     s[4] = 0;
     Str_8x16(1, 4, s); 
     Str_8x16(1, 4, s); 
     Temp[2] =sec;
     s[0] = Temp[2]/10+0x30;
     s[1] = (Temp[2]%10)+0x30;
     s[2]= 0xc3; //秒
     s[3]=0xeb;
     s[4] = 0;
     Str_8x16(1, 8, s); 
     Str_8x16(1, 8, s); 

     //反白显示第一项。
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
         //改变反白显示的位置
          //增加lcm不稳定补丁
          s[0] = Temp[Index]/10+0x30;
          s[1] = (Temp[Index]%10)+0x30;
          s[2] = '\0';          
          Str_8x16(1, Index*4, s);
          Str_8x16(1, Index*4, s);           
        //   DispFanBai(1,Index*4,2); //取消原先的反白           
           Index++;
           DispFanBai(1,Index*4,2);//改为当前的反白           
         
         break;
        case UP_KEY:
        
          if(Index ==0) //时
          {
          if(Temp[Index]==23)
          Temp[Index]=0;
          else
          Temp[Index]++; 
          s[0] = Temp[Index]/10+0x30;
          s[1] = (Temp[Index]%10)+0x30;
          s[2] = '\0';
          }
          if(Index ==1)//分
          {
          if(Temp[Index]==59)
          Temp[Index]=0;
          else
          Temp[Index]++; 
          s[0] = Temp[Index]/10+0x30;
          s[1] = (Temp[Index]%10)+0x30;
          s[2] = '\0';
          } 
          if(Index ==2)//秒
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
          
          if(Index ==0) //时
          {
          if(Temp[Index]==0)
          Temp[Index]=23;
          else
          Temp[Index]--; 
          s[0] = Temp[Index]/10+0x30;
          s[1] = (Temp[Index]%10)+0x30;
          s[2] = '\0';
          }
          if(Index ==1)//分
          {
          if(Temp[Index]==0)
          Temp[Index]=59;
          else
          Temp[Index]--; 
          s[0] = Temp[Index]/10+0x30;
          s[1] = (Temp[Index]%10)+0x30;
          s[2] = '\0';
          } 
          if(Index ==2)//秒
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
            
            ////放弃数据并退到上级菜单
           MenuStatus = 3;
           return; 
          }

          else
          { 
                     //改变反白显示的位置
          //增加lcm不稳定补丁
          s[0] = Temp[Index]/10+0x30;
          s[1] = (Temp[Index]%10)+0x30;
          s[2] = '\0';          
          Str_8x16(1, Index*4, s);
          Str_8x16(1, Index*4, s);
           
        //   DispFanBai(1,Index*4,2); //取消原先的反白
           Index--; 
           DispFanBai(1,Index*4,2);//改为当前的反白
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
//因为这个函数特殊并且应当属于菜单的一部分所以放到这里
//功能： 验证用户输入的密码的正确性。正确退出
//不正确就在里边循环；
//*******************************************
void PutInPass(void) 
{
  unchar s[5];
  switch(MenuStatus)
  {
  case 1:
    Str_8x16(1, 0,"请输入密码" );
    Str_8x16(1, 0,"请输入密码" );
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
               //判定密码正确否
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
//5.2实现 WIFI通信功能
void OpenWifi(void)
{
  unchar    i = 0;
  unchar key;
  
  switch (MenuStatus)
  {
  case 2:
             Str_8x16(1,0, "正在启动WIFI  ");
             Str_8x16(1,0, "正在启动WIFI  ");
             POWRON_WIFI;
             Delay_ms(5000);
            pWIFI_RX=Temp;//给定接受缓冲区。
             Init_Uart1;
             WIFI_RX_NUM=0;

             WIFI_RX_CONNECT=0;
             
             while(1)
             {  
             if(DialWIFI())
             
             {
               Delay_ms(2000);
               WIFI_RX_CONNECT=2;
             
               Str_8x16(1,0,"WIFI启动成功");
               Str_8x16(1,0,"WIFI启动成功");
               break;
             }
             else
             {
               i++;
               if(i>8)
               {
                 Str_8x16(1,0,"WIFI启动失败");
                  Str_8x16(1,0,"WIFI启动失败");
                 POWROFF_WIFI;
                 IE2 &= ~URXIE1;//禁止接收中断；
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
        //解析收到的命令。
        //if(IsEqual(&Temp[4], "CONFIG.TXT", 10))
       
        {
          //发送config.txt文件到串口1
          
          if(CURPATH.CurrentDirStarClusID !=0)
            
          {
            CURPATH.CurrentDirStarClusID = 0;
          }
          
          SendHttpHead();
          SendFileToUart("CONFIG  TXT");
          
          
         // IE2 |= URXIE1;//打开接收中断重新接收命令。
        }
        /*
        else if((Temp[3]=='/')&&(Temp[8]==' '))//子目录项
                
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
      if(RecvCmd ==2)//显示目录列表到串口
      {
        RecvCmd = 0;
        ListFile();
      }  
      if(RecvCmd ==3)//显示测量数据文件到串口。
      {
        RecvCmd = 0;
        SendFileToUart("");
      } 
      if(RecvCmd==4)//从客户端下载文件到SD卡 主要是CONFIGTXT
      {
        
      }          
    KeyScan();
    key=GetKey1();
    if(key!=NULL)                 
    {
      if(key ==ESC_KEY)
      {
        POWROFF_WIFI;
        IE2 &= ~URXIE1;//禁止接收中断；//恢复spi的设置以便操作sd卡
        initSPI(2);
        pWIFI_RX=(unchar *)0xffff;//接收缓冲区指针指向空。
        MenuStatus =3;
        return;
      }
    }
                 
                 
                 
      
    } 
     

   
  }
}




//***********************************************8
//数据查询功能：
//4.数据查询功能；
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
    if((ReadFile("CONFIG  TXT", (unchar*)pCfgHead, 0,sizeof (CFG_HEAD))!=0xff))//取出系统头
     {     
           Index = 1;
         s[0] = Index/1000+0x30;         
         s[1] = (Index%1000)/100+0x30;
         s[2] = ((Index%1000)%100)/10+0x30;
         s[3] = ((Index%1000)%100)%10+0x30;
         s[4] = '\0'; 
      Str_8x16(0,0,"组:");
      Str_8x16(0,0,"组:");
      Str_8x16(0,3,s);
      DispFanBai(0,3,4);
     }
     else
     {
       Str_8x16(1,0 ,"系统文件不存在");
        Str_8x16(1,0 ,"系统文件不存在");
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
         DispFanBai(0,3,4);//组位置取消反白  
         s[0] = Index/1000+0x30;         
         s[1] = (Index%1000)/100+0x30;
         s[2] = ((Index%1000)%100)/10+0x30;
         s[3] = ((Index%1000)%100)%10+0x30;
         s[4] = '\0';
       if(ChgDir(s)==0xff)//没有该目录              
       {
         Str_8x16(1,0,"不存在该组的数据    ");
         Str_8x16(1,0,"不存在该组的数据    ");
         Delay_ms(4000);
         MenuStatus = 3;
         return;
       }
       
        //进入了指定的目录；   
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
    
//电池组下的文件 
  next:
     Temp[0] =year-2000;
     s[0] = Temp[0]/10+0x30;
     s[1] = (Temp[0]%10)+0x30;
     s[2]= 0xc4;//年
     s[3] =0xea;
     s[4]=0;
     Str_8x16(1, 0, s);
     Str_8x16(1, 0, s);
     Temp[1] =month;
     s[0] = Temp[1]/10+0x30;
     s[1] = (Temp[1]%10)+0x30;
     s[2]= 0xd4;//月
     s[3] =0xc2;
     s[4] = 0;
     Str_8x16(1, 4, s); 
     Str_8x16(1, 4, s); 
     Temp[2] =day;
     s[0] = Temp[2]/10+0x30;
     s[1] = (Temp[2]%10)+0x30;
     s[2]= 0xc8;//日
     s[3]=0xd5;
     s[4] = 0;
     Str_8x16(1, 8, s); 
     Str_8x16(1, 8, s); 

     //反白显示第一项。
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
           for(unchar i =0;i<11;i++)//保存文件名备用;
           {
             Temp[200+i]=s[i];
           }
           if(GetFileID(s, ID)==0xffff) //文件不存在
           {
             Str_8x16(1,0,"查询的文件不存在");
             Str_8x16(1,0,"查询的文件不存在");
             Delay_ms(4000);
             MenuStatus =3;
             return;
             
           }
           memcpy(&Temp[100],(unchar*)&(ID->FilePosit.Size),4);//保存文件长度备用
           //   *(unint32*)&Temp[100]=*(&(ID->FilePosit.Size));
           if(ID->FilePosit.Size >=16+8) 
           {
             if(ReadFile(s,Temp,16,8)!=0xff)
             {  
               float f;
               memcpy(&f,Temp,4);
               sprintf((char*)s,"%6.3f",f); 
               Str_8x16(1,0,"电压:");
               Str_8x16(1,0,"电压:");
               Str_8x16(1,12,"V");
               Str_8x16(1,5,s);
               memcpy(&f,&Temp[4],4);
               sprintf((char*)s,"%6.3f",f); 
               Str_8x16(2,0,"内阻:");
               Str_8x16(2,0,"内阻:");
               Str_8x16(2,12,"mΩ");
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
               Str_8x16(1,0,"读文件出错");
               Str_8x16(1,0,"读文件出错");
               MenuStatus =3;
               return;           
             }  
             
           }
           else
           {
             Str_8x16(1,0,"文件是空的");
             Str_8x16(1,0,"文件是空的");
             Delay_ms(4000);
             MenuStatus =3;
             return;         
           }  
           
         }
           

         else
         //改变反白显示的位置
           DispFanBai(1,Index*4,2); //取消原先的反白           
           Index++;
           DispFanBai(1,Index*4,2);//改为当前的反白           
         
         break;
        case UP_KEY:
          if(Index ==0) //年
          {
          if(Temp[Index]==99)
          Temp[Index]=0;
          else
          Temp[Index]++; 
          s[0] = Temp[Index]/10+0x30;
          s[1] = (Temp[Index]%10)+0x30;
          s[2] = '\0';
          }
          if(Index ==1)//月
          {
          if(Temp[Index]==12)
          Temp[Index]=1;
          else
          Temp[Index]++; 
          s[0] = Temp[Index]/10+0x30;
          s[1] = (Temp[Index]%10)+0x30;
          s[2] = '\0';
          } 
          if(Index ==2)//日
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
          if(Index ==0) //年
          {
          if(Temp[Index]==0)
          Temp[Index]=99;
          else
          Temp[Index]--; 
          s[0] = Temp[Index]/10+0x30;
          s[1] = (Temp[Index]%10)+0x30;
          s[2] = '\0';
          }
          if(Index ==1)//月
          {
          if(Temp[Index]==1)
          Temp[Index]=12;
          else
          Temp[Index]--; 
          s[0] = Temp[Index]/10+0x30;
          s[1] = (Temp[Index]%10)+0x30;
          s[2] = '\0';
          } 
          if(Index ==2)//日
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
            ////放弃数据并退到上级菜单
           MenuStatus = 3;
           return; 
          }

          else
          { 
                     //改变反白显示的位置
            
           DispFanBai(1,Index*4,2); //取消原先的反白
           Index--; 
           DispFanBai(1,Index*4,2);//改为当前的反白
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
           Str_8x16(1,0,"电压:");
            Str_8x16(1,0,"电压:");
           sprintf((char*)s,"%6.3f",f); 

           Str_8x16(1,5,s);
           Str_8x16(1,5,s);
           memcpy(&f,&Temp[4],4);
           Str_8x16(2,0,"内阻:");           
           Str_8x16(2,0,"内阻:");           
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
         Str_8x16(1,0,"读文件出错");
         Str_8x16(1,0,"读文件出错");
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
           Str_8x16(1,0,"电压:");
           Str_8x16(1,0,"电压:");
           sprintf((char*)s,"%6.3f",f); 

           Str_8x16(1,5,s);
           Str_8x16(1,5,s);
           memcpy(&f,&Temp[4],4);
           Str_8x16(2,0,"内阻:");           
           Str_8x16(2,0,"内阻:");           
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
         Str_8x16(1,0,"读文件出错");
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
//2.1随机测量
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
              //给cy单片机上电打开串口并进行串口初始化；
         pRecv0Str=&Temp[20];//设置接收缓冲区的地址。       
         POWRON_CY;
         Init_Uart0;

         Uart0RxTimeOut = 0;
           Recv0State =0;
           Recv0Index =0;         
         //等待cy单片机的响应包；
         while(1)
         {
           if(GetKey2( pAck,Temp,10)!=NULL)
           {
             //判定得到的数据是否合法
             if((Temp[4]==0x04)&&(Temp[5]==0x03)&&(Temp[6]==0x02)&&(Temp[7]==0x01))
             {
               unchar  data[4];
               data[0] =10;//设定地址为10
               data[1] = 0;
               data[2] = 0;
               data[3] = 0;
               Recv0State =0;
               Recv0Index =0;
               SendCmd(SET_ADRESS,0,data);  
               Uart0RxTimeOut =0;
             break;
             }
              Str_8x16(1,0,"测量模块出错");
              Str_8x16(1,0,"测量模块出错");
             break;
           }
           
          if(Uart0RxTimeOut>=300)
          {
            //cy单片机没有响应退出；
            Str_8x16(1,0,"测量模块没有响应");
            Str_8x16(1,0,"测量模块没有响应");
            Delay_ms(2000);
            POWROFF_CY;
            MenuStatus =3;
            return;
          }
         }
             while(1)
             {
               if(GetKey2( pAck,Temp,10)!=NULL)//得到cy的设定地址回应
                 
               {
                 if(Temp[2]==10)
                 {
 
                    break;
                 }
               Str_8x16(1,0,"测量单元出错");                 
               Str_8x16(1,0,"测量单元出错"); 
                break; 
               }
         if(Uart0RxTimeOut>=300)
          {
            //cy单片机没有响应退出；
            Str_8x16(1,0,"测量模块没有响应");
            Str_8x16(1,0,"测量模块没有响应");
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
               data[0] =0;//设定地址为10
               data[1] = 0;
               data[2] = 0;
               data[3] = 0;
    Str_8x16(0,0,"正在测量中...     "); 
    Str_8x16(0,0,"正在测量中...     ");     
    Str_8x16(1,0,"                  ");     
     SendCmd(MEASURE_VOLT,10,data);
     while(1)
     { 
       if(GetKey2( pAck,Temp,10)!=NULL)
       {
         memcpy((unchar*)&fint,&Temp[4],2);
         f=(float)fint/1000;//注意字节对齐问题；
        
       //  if((f>=pGrpHead->BatVoltMin)&&(f<=pGrpHead->BatVoltMax))
         {
           arf[i]=f;
           
           i++;
           break;
         }
        /* 
         else
         {
            Str_8x16(1,0,"请检查接线");
            Str_8x16(1,0,"请检查接线");
             goto cont;        
           
         }
         */
       }
       else if(Uart0RxTimeOut>=300)//超时退出；
          {
            //cy单片机没有响应退出；
            Str_8x16(1,0,"测量模块没有响应");
            Str_8x16(1,0,"测量模块没有响应");
            Delay_ms(5000);
            POWROFF_CY;
            MenuStatus =3;
            return;
          }

       
     } 


     if(i==3)
     {
       //已经测量了3次
       i =0;
       float f1 = fabsf(arf[0]-arf[1]);
       float f2 = fabsf(arf[0]-arf[2]);
       float f3 = fabsf(arf[2]-arf[1]);
       
       
       if((f1<=0.01)&&(f2<=0.01)&&(f3<=0.01))//比较有效性
       {
         
         //夹子夹好了并连续得到三次合格的电压值；
         //退出电压测量进行电阻测量。
         
         //*******************8
         
         //发测量阻值的命令；
         Uart0RxTimeOut = 0;
         
         SendCmd(MEASURE_RES,10,data);
         // DispFanBai(0,12,4);//取消反白。
         f=(arf[0]+arf[1]+arf[2])/3;
         while(1)
         {
           if(GetKey2( pAck,Temp,10)!=NULL)
           { 
             
             memcpy(Temp,&f,4);
             memcpy(&fint,&Temp[4],2);
             fres =(float)fint/1000;
             memcpy(&Temp[4],&fres,4);
             // 显示出电压和内阻值；
             sprintf((char*)s,"%5.3f",f);
             Str_8x16(1,0,"电压:");
             Str_8x16(1,0,"电压:");
             Str_8x16(1,10,"V");
             Str_8x16(1,5,s);
             
             sprintf((char*)s,"%5.3f",fres);
             Str_8x16(2,0,"内阻:");
             Str_8x16(2,0,"内阻:");
             Str_8x16(2,10,"mΩ");         
             Str_8x16(2,5,s);
             
 
             //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
             //开始下一个电池的测量
             //  Str_8x16(2,0, "ENT：下一个 ESC：退出");
             
             unchar key;
              Str_8x16(0,0,"测量完成        ");
              Str_8x16(0,0,"测量完成        ");              
             while(1)//等待用户操作；
               
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
             
             
             
             
             //     break;//退出测量阻值的应答循环。
           }
           else if(Uart0RxTimeOut>=300)//超时退出；
           {
             //cy单片机没有响应退出；
             Str_8x16(1,0,"测量模块没有响应");
             Str_8x16(1,0,"测量模块没有响应");             
             Delay_ms(5000);
             POWROFF_CY;
             MenuStatus =3;
             return;
           }
           
           
         }
         
         
         //***************************************        
         
         
         //break;  //退出case2000
         
       }
       else
       {
         //判定为夹子没有夹好
         Str_8x16(3,0,"没夹好请重新测量");
         Str_8x16(3,0,"没夹好请重新测量");         
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
             break;//继续重试当前电池的测量
           }
           else if(key==ESC_KEY)           //放弃对当前电池测量因为这个电池可能已经坏了接着测下面的电池。
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
//随机测量的新版本；
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
              //给cy单片机上电打开串口并进行串口初始化；
         pRecv0Str=&Temp[20];//设置接收缓冲区的地址。       
         POWRON_CY;
         Init_Uart0;
          WIFI_RX_CONNECT=100;
         Uart0RxTimeOut = 0;
           Recv0State =0;
           Recv0Index =0;         
         //等待cy单片机的响应包；
         while(1)
         {
           if(GetKey2( pAck,Temp,10)!=NULL)
           {
             //判定得到的数据是否合法
             if((Temp[4]==0x04)&&(Temp[5]==0x03)&&(Temp[6]==0x02)&&(Temp[7]==0x01))
             {
               unchar  data[4];
               data[0] =10;//设定地址为10
               data[1] = 0;
               data[2] = 0;
               data[3] = 0;
               Recv0State =0;
               Recv0Index =0;
               SendCmd(SET_ADRESS,0,data);  
               Uart0RxTimeOut =0;
             break;
             }
              Str_8x16(1,0,"测量模块出错");
              Str_8x16(1,0,"测量模块出错");
             break;
           }
           
          if(Uart0RxTimeOut>=300)
          {
            //cy单片机没有响应退出；
            Str_8x16(1,0,"测量模块没有响应");
            Str_8x16(1,0,"测量模块没有响应");
            Delay_ms(2000);
            POWROFF_CY;
            MenuStatus =3;
            return;
          }
         }
             while(1)
             {
               if(GetKey2( pAck,Temp,10)!=NULL)//得到cy的设定地址回应
                 
               {
                 if(Temp[2]==10)
                 {
 
                    break;
                 }
               Str_8x16(1,0,"测量单元出错");                 
               Str_8x16(1,0,"测量单元出错"); 
                break; 
               }
         if(Uart0RxTimeOut>=300)
          {
            //cy单片机没有响应退出；
            Str_8x16(1,0,"测量模块没有响应");
            Str_8x16(1,0,"测量模块没有响应");
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
               data[0] =0;//设定地址为10
               data[1] = 0;
               data[2] = 0;
               data[3] = 0;
    Str_8x16(0,0,"正在测量中...     "); 
    Str_8x16(0,0,"正在测量中...     ");     
    Str_8x16(1,0,"                  ");     
     SendCmd(MEASURE_VOLT,10,data);
     while(1)
     { 
       if(GetKey2( pAck,Temp,10)!=NULL)
       {
         memcpy((unchar*)&fint,&Temp[4],2);
         f=(float)fint/1000;//注意字节对齐问题；
        
       //  if((f>=pGrpHead->BatVoltMin)&&(f<=pGrpHead->BatVoltMax))
         {
           arf[i]=f;
           
           i++;

           break;
         }
        /* 
         else
         {
            Str_8x16(1,0,"请检查接线");
            Str_8x16(1,0,"请检查接线");
             goto cont;        
           
         }
         */
       }
       else if(Uart0RxTimeOut>=300)//超时退出；
          {
            //cy单片机没有响应退出；
            Str_8x16(1,0,"测量模块没有响应");
            Str_8x16(1,0,"测量模块没有响应");
            Delay_ms(5000);
            POWROFF_CY;
            MenuStatus =3;
            return;
          }

       
     } 


     if(i==3)
     {
       //已经测量了3次
       i =0;
       float f1 = fabsf(arf[0]-arf[1]);
       float f2 = fabsf(arf[0]-arf[2]);
       float f3 = fabsf(arf[2]-arf[1]);
       
       if((arf[0]>=1)&&(arf[0]<=4)&&(arf[1]>=1)&&(arf[1]<=4)&&(arf[2]>=1)&&(arf[2]<=4))//比较有效性
      // if((f1<=0.01)&&(f2<=0.01)&&(f3<=0.01))//比较有效性
       {
         
         //夹子夹好了并连续得到三次合格的电压值；
         //退出电压测量进行电阻测量。
         
         //*******************8
         
         //发测量阻值的命令；
         Uart0RxTimeOut = 0;
         
         SendCmd(MEASURE_RES,10,data);
         // DispFanBai(0,12,4);//取消反白。
         f=(arf[0]+arf[1]+arf[2])/3;
         while(1)
         {
           if(GetKey2( pAck,Temp,10)!=NULL)
           { 
             
             memcpy(Temp,&f,4);
             memcpy(&fint,&Temp[4],2);
             fres =(float)fint/1000;
             memcpy(&Temp[4],&fres,4);
             // 显示出电压和内阻值；
             sprintf((char*)s,"%5.3f",f);
             Str_8x16(1,0,"电压:");
             Str_8x16(1,0,"电压:");
             Str_8x16(1,10,"V");
             Str_8x16(1,5,s);
             Str_8x16(3,0,s);
             
             sprintf((char*)s,"%5.3f",fres);
             Str_8x16(2,0,"内阻:");
             Str_8x16(2,0,"内阻:");
             Str_8x16(2,11,"mΩ");         
             Str_8x16(2,5,s);
             Str_8x16(3,8,s);             
             
 
             //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
             //开始下一个电池的测量
             //  Str_8x16(2,0, "ENT：下一个 ESC：退出");
             
       //      unchar key;
              Str_8x16(0,0,"测量完成        ");
              Str_8x16(0,0,"测量完成        ");
              Delay_ms(1000);
                 Str_8x16(1,0,"                ");           
                 Str_8x16(1,0,"                ");           
                 Str_8x16(2,0,"                ");  
              

                 goto cont;
/*                 
             while(1)//等待用户操作；
               
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
             
             
             
             
             //     break;//退出测量阻值的应答循环。
           }
           else if(Uart0RxTimeOut>=300)//超时退出；
           {
             //cy单片机没有响应退出；
             Str_8x16(1,0,"测量模块没有响应");
             Str_8x16(1,0,"测量模块没有响应");             
             Delay_ms(5000);
             POWROFF_CY;
             MenuStatus =3;
             return;
           }
           
           
         }
         
         
         //***************************************        
         
         
         //break;  //退出case2000
         
       }
       else
       {
         //判定为夹子没有夹好
                 Str_8x16(0,0,"                ");  
                 Str_8x16(0,0,"                ");
                 Str_8x16(1,0,"                ");           
                 Str_8x16(2,0,"                ");          
       //    Str_8x16(3,0,"                ");
      //   Str_8x16(3,0,"没夹好开始重新测");
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
             break;//继续重试当前电池的测量
           }
           else if(key==ESC_KEY)           //放弃对当前电池测量因为这个电池可能已经坏了接着测下面的电池。
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


