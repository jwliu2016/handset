//---------------------------------------------
// the general menu driver using the  tree struct
// qq:361299856
//---------------------------------------------
#define NORUN   0
#define RUNING  1
#define OVERRUN 2

//----------------------------------------------------------------------------------
// the globle varible declar   here 
//---------------------------------------------------------------------------------
#define	MENU_GLOBALS
#include "..\include.h"
//#define NULL 		(void*)0xffff

struct MENU *pCurrentMenu=&MyMenu[0];
struct MENU *pCurrentMenu1=&MyMenu[0];
//#pragma memory=constseg(MYSEG)
//memory=constseg
 struct MENU    MyMenu[] ={

  //一级菜单                  儿子            父亲              左兄弟              右兄弟
  {0,"1,设置参数"     , 10  ,&MyMenu[5]     , NULL              ,NULL                ,&MyMenu[1]    ,1,NULL},//0
  {1,"2,单只测量"     , 10  ,&MyMenu[10]    , NULL              ,&MyMenu[0]          ,&MyMenu[2]    ,1,NULL},//1
  {2,"3,整组测量"     , 10  ,NULL           , NULL              ,&MyMenu[1]          ,&MyMenu[3]    ,1,TotalGrpMeasure1},//2
  {3,"4,查询数据"     , 10  ,NULL           ,NULL               ,&MyMenu[2]          ,&MyMenu[4]    ,1,SqlData1},//3
  
  {0,"5,管理操作"     , 10  ,&MyMenu[13]    ,NULL               ,&MyMenu[3]          ,NULL          ,2,NULL},//4
  //二级菜单
  {0,"1.1通信参数"    , 11  ,&MyMenu[16]    ,&MyMenu[0]         ,NULL                ,&MyMenu[6]    ,1,NULL},//5
  {1,"1.2用户密码"    , 11  ,NULL           ,&MyMenu[0]         ,&MyMenu[5]          ,&MyMenu[7]    ,1,NULL},	//6	 
  {2,"1.3电池组设置"  , 13  ,&MyMenu[19]    ,&MyMenu[0]         ,&MyMenu[6]          ,&MyMenu[8]    ,1,NULL},//7
  {3,"1.4测量计划"    , 11  ,&MyMenu[29]    ,&MyMenu[0]         ,&MyMenu[7]          ,&MyMenu[9]    ,1,NULL},//8
  
  {0,"1.5设定时间"    , 11  ,&MyMenu[30]    ,&MyMenu[0]         ,&MyMenu[8]          ,NULL          ,2,NULL},//9
  //二级菜单
  {0,"2.1随机测量"    , 11  ,NULL           ,&MyMenu[1]         ,NULL                ,&MyMenu[11]   ,1,RandomMeasure1},//10
  {1,"2.2计划测量"    , 11  ,NULL           ,&MyMenu[1]         ,&MyMenu[10]         ,&MyMenu[12]   ,1,NULL},//11
  {2,"2.3用户密码"    , 11  ,NULL           ,&MyMenu[1]         ,&MyMenu[11]         ,NULL          ,1,NULL},//12
  // 二级菜单
  {0,"5.1清空数据"    , 11  ,NULL           ,&MyMenu[4]         ,NULL                ,&MyMenu[14]   ,1,NULL},//13
  {1,"5.2WIFI控制"    , 11  ,NULL           ,&MyMenu[4]         ,&MyMenu[13]         ,&MyMenu[15]   ,1,OpenWifi1},//14
  {2,"5.3USB控制"     , 10  ,NULL           ,&MyMenu[4]         ,&MyMenu[14]         ,NULL          ,1,NULL},//15
  //三级菜单
  {0,"1.1.1IP地址"    , 11  ,NULL           ,&MyMenu[5]         ,NULL                ,&MyMenu[17]   ,1,SetIp1},//16
  {1,"1.1.2子网掩码"  , 13  ,NULL           ,&MyMenu[5]         ,&MyMenu[16]         ,&MyMenu[18]   ,1,SetIp1},//17
  {2,"1.1.3网关"      , 9  ,NULL           ,&MyMenu[5]         ,&MyMenu[17]         ,NULL          , 1,SetIp1},//18
  //三级菜单
  {0,"1.3.0设置模式"  , 13  ,NULL           ,&MyMenu[7]         ,NULL                ,&MyMenu[20]   ,1,NULL},//19
  {1,"1.3.1电池组编号", 15  ,NULL           ,&MyMenu[7]         ,&MyMenu[19]         ,&MyMenu[21]   ,1,NULL},//20
  {2,"1.3.2电池组数量", 15  ,NULL           ,&MyMenu[7]         ,&MyMenu[20]         ,&MyMenu[22]   ,1,NULL},//21
  {3,"1.3.3电压等级"  , 13  ,NULL           ,&MyMenu[7]         ,&MyMenu[21]         ,&MyMenu[23]   ,1,NULL},//22
  //三级菜单
  {0,"1.3.4基准内阻"  , 13  ,NULL           ,&MyMenu[7]         ,&MyMenu[22]         ,&MyMenu[24]   ,2,NULL},//23
  {1,"1.3.5额定容量"  , 13  ,NULL           ,&MyMenu[7]         ,&MyMenu[23]         ,&MyMenu[25]   ,2,NULL},//24
  {2,"1.3.6电压上限"  , 13  ,NULL           ,&MyMenu[7]         ,&MyMenu[24]         ,&MyMenu[26]   ,2,NULL},//25
  {3,"1.3.7电压下限"  , 13  ,NULL           ,&MyMenu[7]         ,&MyMenu[25]         ,&MyMenu[27]   ,2,NULL},//26
  //三级菜单
  {0,"1.3.8内阻上限"  , 13  ,NULL           ,&MyMenu[7]         ,&MyMenu[26]         ,&MyMenu[28]   ,3,NULL},//27
  {1,"1.3.9内阻下限"  , 13  ,NULL           ,&MyMenu[7]         ,&MyMenu[27]         ,NULL          ,3,NULL},//28
  //三级菜单
  {0,"1.4.1测量间隔"  , 13  ,NULL           ,&MyMenu[8]         ,NULL                ,NULL          ,1,NULL},//29
  //三级菜单
  {0,"1.5.1年月日"    , 11  ,NULL           ,&MyMenu[9]         ,NULL                ,&MyMenu[31]   ,1,SetYYMMDD},//30                             
  {1,"1.5.2时分秒"    , 11  ,NULL           ,&MyMenu[9]         ,&MyMenu[30]         ,NULL          ,1,SetHHMMSS}//31                              
  
};
//#pragma memory= default

unsigned char CurrentPage =1 ;//= pCurrentMenu->page;
unsigned int  MenuStatus=0;
unchar FUNCTION_RUNING = 0; // 0 没有运行， 1，正在运行， 2，运行完毕。

//------------------------------------
//initialzation  menu
//------------------------------------

// pCurrentMenu =&MyMenu[0]; //can't be set here but i don't know why??????????

// pCurrentMenu1 =&MyMenu[0];

void MenuFunction(void)
	{
	 unchar Chars;
	
	switch(MenuStatus)

		{
		  case 0:
			{
			 Chars=GetKey1();
			  if (Chars==ESC_KEY)
				{
				  Screen_Clear();
				  while(pCurrentMenu1->pBrothePrev!=NULL)//找到兄弟中的老大
				  pCurrentMenu1=pCurrentMenu1->pBrothePrev;

	 			           
					  while(pCurrentMenu1->pBrotheNext!=NULL) //显示出所有的兄弟菜单
					   {
					    if(CurrentPage==pCurrentMenu1->page) //只显示当前页的菜单项
                                            { 
        				     DispMenuTile(pCurrentMenu1->Horizon,pCurrentMenu1->MenuTitle);
					   
                                            }
					     pCurrentMenu1=pCurrentMenu1->pBrotheNext;                                            
										      
					   }
                                    //反显CurentMenu；
                                   DispFanBaiMenuTitle(pCurrentMenu->Horizon,pCurrentMenu->Width) ;      
				         
				    
				 MenuStatus=1;
				 
				}

			  break;
			}
		  case 1: 
			{
			  switch(GetKey1())
				{
             		case ENTER_KEY:

					Screen_Clear();
					if (pCurrentMenu->pSonMenu==NULL)
                                        {
					MenuStatus=2;
                                        break;
                                        }
					else 
				        {
                                         
                                         if(pCurrentMenu->CurrentOperation !=NULL)
                                         {
                                          pCurrentMenu->CurrentOperation();//执行下面有子菜单项,但是执行完此函数才能继续下面的菜单项的函数。一般都是密码相关的函数
                                          
                                         }
                                         
					pCurrentMenu=pCurrentMenu->pSonMenu;
                                        CurrentPage=pCurrentMenu->page;
					
				         pCurrentMenu1=pCurrentMenu;

					  while(pCurrentMenu1->pBrothePrev!=NULL)
						pCurrentMenu1=pCurrentMenu1->pBrothePrev;

					  while(pCurrentMenu1->pBrotheNext!=NULL)//显示出所有的兄弟菜单
					   {
					    if(CurrentPage==pCurrentMenu1->page) //只显示当前页的菜单项
                                            { 
					     DispMenuTile(pCurrentMenu1->Horizon,pCurrentMenu1->MenuTitle);

                                            }
					     pCurrentMenu1=pCurrentMenu1->pBrotheNext;                                            
										      
					   }
			           //反显CurentMenu；
                                   if(CurrentPage==pCurrentMenu1->page)       
                                   DispMenuTile(pCurrentMenu1->Horizon,pCurrentMenu1->MenuTitle);       
                                   DispFanBaiMenuTitle(pCurrentMenu->Horizon,pCurrentMenu->Width) ;   
					}
					break;

			        case UP_KEY:
					if (pCurrentMenu->pBrothePrev!=NULL)
					      {
                                                
                                                
                                                // clr the  反白显示
                                                //增加lcm不稳定的补丁
                                                DispMenuTile(pCurrentMenu->Horizon,pCurrentMenu->MenuTitle); 
                                                DispMenuTile(pCurrentMenu->Horizon,pCurrentMenu->MenuTitle); 
                                                
					      //  DispFanBaiMenuTitle(pCurrentMenu->Horizon,pCurrentMenu->Width) ;                                                                                                
                                                
                                    	        pCurrentMenu=pCurrentMenu->pBrothePrev;
                                                if(pCurrentMenu->page==CurrentPage)
                                                {  
			
                                                //使之反白显示
                                                DispFanBaiMenuTitle(pCurrentMenu->Horizon,pCurrentMenu->Width) ;
                                                
                                                
                                                
                                                }
                                                else//菜单项到下一页去了
                                                {
                                                  Screen_Clear();
                                                  CurrentPage=pCurrentMenu->page;
                                                 
                                                  pCurrentMenu1=pCurrentMenu;
			                          while(pCurrentMenu1->pBrothePrev!=NULL)//找到兄弟中的老大
				                   pCurrentMenu1=pCurrentMenu1->pBrothePrev;

	 			           
					  while(pCurrentMenu1->pBrotheNext!=NULL) //显示出所有的兄弟菜单
					   {
					    if(CurrentPage==pCurrentMenu1->page) //只显示当前页的菜单项
                                            { 
					     DispMenuTile(pCurrentMenu1->Horizon,pCurrentMenu1->MenuTitle);

                                            }
					     pCurrentMenu1=pCurrentMenu1->pBrotheNext;                                            
										      
					   }
                                    //反显CurentMenu；
                                            if(CurrentPage==pCurrentMenu1->page)                                        
                                            DispMenuTile(pCurrentMenu1->Horizon,pCurrentMenu1->MenuTitle);  
                                            DispFanBaiMenuTitle(pCurrentMenu->Horizon,pCurrentMenu->Width) ;                                                    
                                                 
                                                 
                                                }  
                                               
					      }
					       break;
					    
				case DOWN_KEY:
					
					if (pCurrentMenu->pBrotheNext!=NULL)
					      {
                                                // 去除反白显示
                                                //增加lcm不稳定的补丁
                                                DispMenuTile(pCurrentMenu->Horizon,pCurrentMenu->MenuTitle); 
                                                DispMenuTile(pCurrentMenu->Horizon,pCurrentMenu->MenuTitle); 
                                                
					      //  DispFanBaiMenuTitle(pCurrentMenu->Horizon,pCurrentMenu->Width) ;                                                
					        pCurrentMenu=pCurrentMenu->pBrotheNext;
                                                if(pCurrentMenu->page==CurrentPage)
                                                {  
		                                 // 使之反白显示
                                                DispFanBaiMenuTitle(pCurrentMenu->Horizon,pCurrentMenu->Width) ;                                                  
                                                }
                                                else//菜单项到下一页去了
                                                {
                                                 Screen_Clear();
                                                 CurrentPage=pCurrentMenu->page;
                                                 
                                                 pCurrentMenu1=pCurrentMenu;
			                    while(pCurrentMenu1->pBrothePrev!=NULL)//找到兄弟中的老大
				            pCurrentMenu1=pCurrentMenu1->pBrothePrev;

	 			           
					  while(pCurrentMenu1->pBrotheNext!=NULL) //显示出所有的兄弟菜单
					   {
					    if(CurrentPage==pCurrentMenu1->page) //只显示当前页的菜单项
                                            { 
					     DispMenuTile(pCurrentMenu1->Horizon,pCurrentMenu1->MenuTitle);

                                            }
					     pCurrentMenu1=pCurrentMenu1->pBrotheNext;                                            
										      
					   }
                                    //反显CurentMenu；
                                   if(CurrentPage==pCurrentMenu1->page)       
                                   DispMenuTile(pCurrentMenu1->Horizon,pCurrentMenu1->MenuTitle);       
                                   DispFanBaiMenuTitle(pCurrentMenu->Horizon,pCurrentMenu->Width) ;                                                   
                                                 
                                                 
                                                }  
					      }

					       break;
				case ESC_KEY:
						
					if(pCurrentMenu->pFatheMenu!=NULL)
                                        {
                                          pCurrentMenu=pCurrentMenu->pFatheMenu;
                                          CurrentPage=pCurrentMenu->page;
                                          Screen_Clear();
                                          pCurrentMenu1=pCurrentMenu;
                                          
					  while(pCurrentMenu1->pBrothePrev!=NULL)
                                            pCurrentMenu1=pCurrentMenu1->pBrothePrev;
                                          
					  while(pCurrentMenu1->pBrotheNext!=NULL) //显示出所有的兄弟菜单
                                          {
					    if(CurrentPage==pCurrentMenu1->page) //只显示当前页的菜单项
                                            { 
                                              DispMenuTile(pCurrentMenu1->Horizon,pCurrentMenu1->MenuTitle);
                                              
                                            }
                                            pCurrentMenu1=pCurrentMenu1->pBrotheNext;                                            
                                            
                                          }
                                          //反显CurentMenu；
                                          if(CurrentPage==pCurrentMenu1->page)
                                          DispMenuTile(pCurrentMenu1->Horizon,pCurrentMenu1->MenuTitle);
                                          DispFanBaiMenuTitle(pCurrentMenu->Horizon,pCurrentMenu->Width) ; 
                                        }
                                          break;
				 

				
			    default: break;
					

				}
			break;

			}
 
		 case 2://执行程序；
                        
                     //   Screen_Clear();

			if((pCurrentMenu->CurrentOperation)!=NULL)  
                          (pCurrentMenu->CurrentOperation)();
                        //执行完应用程序就返回菜单；
                        else
			MenuStatus =3;
			 break;
                case 3://退出应用程序之后屏幕处理
                                  
				  Screen_Clear();
                                  pCurrentMenu1=pCurrentMenu;
                                  CurrentPage=pCurrentMenu->page;
		
                                  while(pCurrentMenu1->pBrothePrev!=NULL)//找到兄弟中的老大
				  pCurrentMenu1=pCurrentMenu1->pBrothePrev;

	 			           
					  while(pCurrentMenu1->pBrotheNext!=NULL) //显示出所有的兄弟菜单
					   {
					    if(CurrentPage==pCurrentMenu1->page) //只显示当前页的菜单项
                                            { 
        				     DispMenuTile(pCurrentMenu1->Horizon,pCurrentMenu1->MenuTitle);
					   
                                            }
					     pCurrentMenu1=pCurrentMenu1->pBrotheNext;                                            
										      
					   }
                                    //反显CurentMenu；
                                   if(CurrentPage==pCurrentMenu1->page)
                                    DispMenuTile(pCurrentMenu1->Horizon,pCurrentMenu1->MenuTitle); 
                                   DispFanBaiMenuTitle(pCurrentMenu->Horizon,pCurrentMenu->Width) ;      
				         
				    
				 MenuStatus=1;                    
                  break;
                case 1000:
			if((pCurrentMenu->CurrentOperation)!=NULL)  
                          (pCurrentMenu->CurrentOperation)();                  
                  break;
                case 2000:
	                if((pCurrentMenu->CurrentOperation)!=NULL)  
                          (pCurrentMenu->CurrentOperation)();                  
                  break;                  
                default:
                  break;
			

		
		}
         
	  
	}



















