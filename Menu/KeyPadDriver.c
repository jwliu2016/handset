#define		KEY_GLOBALS
#include	"..\include.h"



//-------------------------------------------
// the globle varible declar   here 
//---------------------------------------------
static unchar  KeyBuf[8];
static  unchar BufFlag = EMPTY;
static unchar Real=0;
static  unchar Front = 0;
unchar KeyPadState=0;
volatile static  unchar temp = 0;
static unchar Dely20ms;
//--------------------------------------
// put the  char into  the  key buffer
//--------------------------------------
static	void PutInBuf1(unchar x)
 		{
		  if (BufFlag!=FULL)
           		{
				 KeyBuf[Real]=x;
				 
				 Real++;
				 Real&=0x07;
                                 
				 BufFlag=NOTEMPTY;
				 if (Real==Front)
					BufFlag=FULL;
				}
		}
//----------------------------------------
//  get char from the key bufffer
//---------------------------------------
 
 unchar GetKey1(void)
	     { unchar x;
		  if (BufFlag!=EMPTY)
			  {
			    x=KeyBuf[Front];
                            
			    Front++;
			    Front&=0x07;
                                
				BufFlag=NOTFULL;
				if (Real==Front)
					BufFlag=EMPTY;
				return(x);	
			  }
			 //else 	
    		return(0);	
	     }
  
//--------------------------------------
// key scan  function 
// scan the key and put the key into key  buffer 
//-----------------------------------------------
 void KeyScan(void)
	{
          switch(KeyPadState)
          {
          case 0:
            temp =(P2IN&0x78);
            if(temp !=0x78)//有按键按下了
            {
             KeyPadState = 1;//去抖 
             Dely20ms = 0;
            }
            break;
          case 1:
            
            if(Dely20ms >=2)
             KeyPadState = 2;//20ms去抖完成；
            break;
          case 2:
              temp =(P2IN&0x78);
              if(temp ==0x78)
              {
                KeyPadState =0;//键盘只是抖动了一下没有真正按下去。
                break;
              }  
              switch(temp)
              {
              case 0x70:
                PutInBuf1(DOWN_KEY);
                break;
              case 0x68:
                PutInBuf1(UP_KEY);
                break;
              case 0x58:
                PutInBuf1(ENTER_KEY);
                break;
              case 0x38:
                PutInBuf1(ESC_KEY);
                break;
              default:
                break;
                            
              }
            KeyPadState =3;
            break;
          case 3://释放按键；
            temp=(P2IN&0x78);
            if(temp ==0x78)
              KeyPadState =0;
              
            break;
          default:
            break;
            
             
          }
          
	}
 
        
void ClrKeyBuf(void)
{
  BufFlag=EMPTY;
  Real=Front;
    
}

        
void Init_TimerB(void )
{
  WDTCTL=WDTPW+WDTHOLD;
    //FLL_CTL0|=XCAP14PF;
    TBCTL=MC_1+TASSEL_2+ID_3+CNTL_0+TBIE+TBCLR;//sMclk作为时钟源8分频 1mhz 输入
    TBCCR0=50000;//1000000/50000=20次每秒 5ms一次中断。

    
}



#pragma vector =TIMERB1_VECTOR
__interrupt void TimerB1(void)
{

TBCTL&=~TBIFG;
Dely20ms++;
Uart0RxTimeOut++;

}
