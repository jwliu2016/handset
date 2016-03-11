#define   HYP_GLOBALS
#include "..\include.h"

void CREATEFILE(unchar argc,unchar **argv)
{
  switch(argc)
  {
  case 2:
    CreateFile(argv[1]);
     mmc_GoIdle();
     Delay_us(100);
    break;
  default: 
    break;
    
  }
  
}




void CREATEDIR(unchar argc,unchar **argv)
{
  switch(argc)
  {
  case 2:
    CreateDir(argv[1]);
     mmc_GoIdle();
     Delay_us(100);
    break;
  default: 
    break;
    
  }
  
}




void DELDIR(unchar argc,unchar **argv)
{
  switch(argc)
  {
  case 2:
    if((DelDir(argv[1]))==-1) 
       SerialSendStr("This is a file  not a dir");
       SerialSendByte(0x0d); 
       SerialSendByte(0x0a);
     mmc_GoIdle();
     Delay_us(100);
    break;
  default: 
    break;
    
  }
  
}


void DELFILE(unchar argc,unchar **argv)
{
  switch(argc)
  {
  case 2:
    if((DelFile(argv[1]))==0xffff) 
       SerialSendStr("This is a Dir  not a file");
       SerialSendByte(0x0d); 
       SerialSendByte(0x0a);
     mmc_GoIdle();
     Delay_us(100);
    break;
  default: 
    break;
    
  }
  
}


void LISTFILE(unchar argc,unchar **argv)
{
  argv =argv;
  switch(argc)
  {
  case 1:
    ListFile();
     mmc_GoIdle();
     Delay_us(100);
    break;
  default: 
    break;
    
  }
  
}



void CHGDIR(unchar argc,unchar **argv)
{
  switch(argc)
  {
  case 2:
    if((ChgDir(argv[1]))==0xff) 
       SerialSendStr("This is a file  not a dir");
       SerialSendByte(0x0d); 
       SerialSendByte(0x0a);
     mmc_GoIdle();
     Delay_us(100);
    break;
  default: 
    break;
    
  }
  
}





void APPENDDATA(unchar argc,unchar **argv)
{
  unsigned long q = 0;
  unsigned char *p=argv[2];
  while (*p !='\0')
  {
    q=q*10+(*p-'0');
    p++;
  } 
  FillData(Datas,*argv[3],200);
  switch(argc)
  {
  case 4:
        AppendData(argv[1], Datas,q);


     mmc_GoIdle();
     Delay_us(100);
    break;
  default: 
    break;
    
  }
  
}





void READFILE(unchar argc,unchar **argv)
{
  unsigned long q = 0;
  unsigned char *p=argv[2];
  while (*p !='\0')
  {
    q=q*10+(*p-'0');
    p++;
  } 

  switch(argc)
  {
  case 3:
        ReadFile(argv[1], Datas,q,123);


     mmc_GoIdle();
     Delay_us(100);
    break;
  default: 
    break;
    
  }
  
}




void MODIFYFILE(unchar argc,unchar **argv)
{
  unsigned long q = 0;
  unsigned char *p=argv[2];
  while (*p !='\0')
  {
    q=q*10+(*p-'0');
    p++;
  } 
  FillData(Datas,*argv[3],200);
  switch(argc)
  {
  case 3:
        ModifyFile(argv[1], Datas,q,123);


     mmc_GoIdle();
     Delay_us(100);
    break;
  default: 
    break;
    
  }
  
}





void SENDFILETOUART(unchar argc,unchar **argv)
{

  switch(argc)
  {
  case 2:
        SendFileToUart(argv[1]);


     mmc_GoIdle();
     Delay_us(100);
    break;
  default: 
    break;
    
  }
  
}



/*****************************************************************************/


COMMAND  CommandList[MAX_COMMAND_NUM] = {
	{"createfile",CREATEFILE,"create a file at current directory"},
	{"mkdir",CREATEDIR," prompt <string> -- Change a prompt"},
	{"dd",DELDIR,"delete  directory"},
	{"df",DELFILE,"delete a file at current directory"},
        {"ls",LISTFILE,"DispFile Table"},
        {"cd",CHGDIR,"Change Current dirctory"},
        {"append",APPENDDATA,"append data to a file "},
        {"readfile",READFILE,"read some data from the properate file "},
        {"modifyfile",MODIFYFILE,"change some data of a file but not effect file length"},
        {"sendfiletouart",SENDFILETOUART,"sned a file to uart "}
        
};

/*****************************************************************************/
void Help(unchar argc,unchar **argv)
{
	char i;
	argv = argv;
	switch(argc)
	{
	case 1:
		for(i = 0; i < MAX_COMMAND_NUM; i++)
		{
			SerialSendStr(CommandList[i].HelpString);
			SerialSendStr("\r\n");
		}
		SerialSendStr("\r\n");
		break;
	default:
		SerialSendStr(" Invalid 'help' command: too many arguments\r\n");
		SerialSendStr(" Usage:\r\n");
		SerialSendStr("     help\r\n");
		break;
	}
}

void Prompt(unchar argc,  unchar **argv)
{
	switch(argc)
	{
	case 2:
		if(strlen(argv[1]) >= MAX_PROMPT_BUFFER_SIZE)
		{
			SerialSendStr(" Warnning:Your argument is too long!\r\n\r\n");
			break;
		}
		memcpy(PromptBuffer,argv[1],MAX_PROMPT_BUFFER_SIZE);
		SerialSendStr(" Prompt is chagned to \"");
		sprintf(&SerialBuffer[0],"%s\"\r\n\r\n",&PromptBuffer[0]);
		SerialSendStr(&SerialBuffer[0]);
		break;
	default:
		SerialSendStr(" Invalid 'prompt' command: too few or many arguments\r\n");
		SerialSendStr(" Usage:\r\n");
		SerialSendStr("     prompt <string>\r\n");
		break;
	}
}

void Clear(unchar argc,  unchar **argv)
{
	argv = argv;
	switch(argc)
	{
	case 1:
		SerialSendStr(CLEARSCREEN);	
		break;
	default:
		SerialSendStr(" Invalid 'clear' command: too many arguments\r\n");
		SerialSendStr(" Usage:\r\n");
		SerialSendStr("     clear\r\n");
		break;
	}
}

void Reboot(unchar argc,  unchar **argv)
{
	argv = argv;
	switch(argc)
	{
	case 1:
		(*(void(*)())0)(); 
		break;
	default:
		SerialSendStr(" Invalid 'reboot' command: too many arguments\r\n");
		SerialSendStr(" Usage:\r\n");
		SerialSendStr("     reboot\r\n");
		break;
	}
}
/*****************************************************************************/
void InitHyperTerminal(void)
{


	CursorPosion = 0;
	ExecCommandFlag = 0;
	memset(&SerialBuffer[0],'\0',MAX_SERIAL_BUFFER_SIZE);
	memcpy(&PromptBuffer[0],"\\",MAX_PROMPT_BUFFER_SIZE);
	SerialSendStr(F_LIGHTGREEN);
	SerialSendStr(B_BLACK);
	SerialSendStr(CLEARSCREEN);
	SerialSendStr("-----------------------------\r\n");
	SerialSendStr("  FAT16 TEST Hyper Terminal   \r\n");
	SerialSendStr("  Designed By JianWeiLiu  \r\n");
	SerialSendStr("-----------------------------\r\n");
	SerialSendStr("\r\n");
	SerialSendStr(&PromptBuffer[0]);
}

void SerialSendByte(unchar dat)
{
    while((IFG1&UTXIFG0)==0);	//
    TXBUF0=dat;
}

void SerialSendStr(unchar *str)
{
  while (*str!='\0')
  {
    while((IFG1&UTXIFG0)==0);	//
    TXBUF0=*str++;
  }
    
}
	
#pragma vector =UART0RX_VECTOR
__interrupt void usart0_rx (void)
{
	char SbufTemp;

	

		SbufTemp = RXBUF0;
		switch(SbufTemp)
		{
		case 0x08:
		case 0x06:
		case 0x07:
		case 0x7E:
		case 0x7F:
			if(CursorPosion > 0)
			{
				CursorPosion--;
				SerialSendByte(0x08);
				SerialSendByte(' ');
				SerialSendByte(0x08);
			}
			SerialBuffer[CursorPosion] = '\0';
			break;
		case '\r':
		case '\n':
		case '\0':
			SerialSendByte('\r');
			SerialSendByte('\n');
			ExecCommandFlag = 1;
			break;
	   	case '\t':
			break;
		default:
			if(CursorPosion < MAX_SERIAL_BUFFER_SIZE)
			{
				SerialBuffer[CursorPosion] = SbufTemp;
				SerialSendByte(SbufTemp);
				CursorPosion++;
			}
			else
			{
				CursorPosion = 0;
				memset(&SerialBuffer[0],'\0',MAX_SERIAL_BUFFER_SIZE);
				SerialSendStr("\r\n Warnning:Your command string is too long!\r\n\r\n");
				SerialSendStr(&PromptBuffer[0]);
			}
			break;
		}
	
}

void ParseArgs(unchar *argstr,unchar *argc_p,unchar **argv, unchar **resid)
{
	char argc = 0;
	char c;
	PARSESTATE stackedState,lastState = PS_WHITESPACE;

	while ((c = *argstr) != 0)
	{
		PARSESTATE newState;

		if (c == ';' && lastState != PS_STRING && lastState != PS_ESCAPE)
			break;

		if (lastState == PS_ESCAPE)
		{
			newState = stackedState;
		}
		else if (lastState == PS_STRING)
		{
			if (c == '"')
		 	{
				newState = PS_WHITESPACE;
				*argstr = 0;
			}
		 	else 
			{
				newState = PS_STRING;
			}
		}
	 	else if ((c == ' ') || (c == '\t'))
		{
			*argstr = 0;
			newState = PS_WHITESPACE;
		}
	 	else if (c == '"') 
		{
			newState = PS_STRING;
			*argstr++ = 0;
			argv[argc++] = argstr;
		}
	 	else if (c == '\\') 
		{
			stackedState = lastState;
			newState = PS_ESCAPE;
		}
	 	else 
		{
			if (lastState == PS_WHITESPACE) 
			{
				argv[argc++] = argstr;
			}
			newState = PS_TOKEN;
		}

		lastState = newState;
		argstr++;
	}

	argv[argc] = NULL;
	if (argc_p != NULL)
		*argc_p = argc;

	if (*argstr == ';') 
	{
		*argstr++ = '\0';
	}
	*resid = argstr;
}

void ExecCommand(unchar *buf)
{
	unchar argc,*argv[8],*resid,i;
	COMMAND *Command = 0;

	while(*buf)
 	{
		memset(argv,0,sizeof(argv));
		ParseArgs(buf, &argc, argv, &resid);
		if(argc > 0)
		{
			for(i = 0; i < MAX_COMMAND_NUM; i++)
			{
				Command = &CommandList[i];	
				if(strncmp(Command->CommandName,argv[0],strlen(argv[0])) == 0)
					break;
				else
					Command = 0;
			}
			if(Command == 0)
			{
				SerialSendStr(" Could not found \"");
				SerialSendStr(argv[0]);
				SerialSendStr("\" command\r\n");
				SerialSendStr(" If you want to konw available commands, type 'help'\r\n\r\n"); 
		   	}
			else
			{
				Command->CommandFunc(argc,argv);
			}
		}
		buf = resid;
	}
}

void RunHyperTerminal(void)
{
	if(ExecCommandFlag)
	{
		ExecCommand(&SerialBuffer[0]);
		SerialSendStr(&PromptBuffer[0]);
		memset(&SerialBuffer[0],'\0',MAX_SERIAL_BUFFER_SIZE);
		CursorPosion = 0;
		ExecCommandFlag = 0;
	}
}
