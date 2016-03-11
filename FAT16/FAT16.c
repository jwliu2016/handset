#define		FAT_GLOBALS
#include	"..\include.h"





void InitFAT16(void )
{
  uint8 i;
  CURPATH.CurrentDirStarClusID =0;//当前目录初始族号。
  CURPATH.FatherDirStarClusID =0; 
  CURPATH.pPath =CURPATH.Path;
  for (i = 0;i<MAX_PATH_SIZE;i++ )
  CURPATH.Path[i]=' ';
  CURPATH.Path[0]='\\';
  ReadBPB();
  
}

//********************************************************************************************
//读一个扇区
void ReadBlock(uint32 LBA)
//********************************************************************************************
{
	mmcReadSector(LBA,BUFFER);
        return;
}

//********************************************************************************************
//写一个扇区
void WriteBlock(uint32 LBA)
//********************************************************************************************
{
	mmcWriteSector(LBA,BUFFER);
        return;
}

//********************************************************************************************
void CopyBytes(void* S, void* D, uint16 size)
//********************************************************************************************
{
	uint8 *s = S, *d = D;
	uint16 i;
	for(i = 0; i < size; i++)
		*d++ = *s++;
}

//********************************************************************************************
uint8 IsEqual(void* A, void* B, uint8 Size)
//********************************************************************************************
{
	uint8 i, *a = A, *b = B;
	for(i = 0; i < Size; i++)
		if(a[i] != b[i])
			return 0;
	return 1;
}

//********************************************************************************************
void EmptyBytes(void* D, uint16 size)
//********************************************************************************************
{
	uint16 i;
	uint8* data = (uint8*)D;
	for(i = 0; i < size; i++)
	{
		*data++ = 0;
	}
}
//********************************************************************************************
//写BPB数据结构
void FATInit(void)
//********************************************************************************************
{
	FAT_BPB* BPB = (FAT_BPB*)BUFFER;
		//参数
	BPB->BPB_BytesPerSec = 0x0200;//每个扇区512字节
	BPB->BPB_SecPerClus  = 0x08;  //每个簇8个扇区
	BPB->BPB_RsvdSecCnt  = 0x0004;//一个保留扇区
	BPB->BPB_NumFATs     = 0x02; //2个FAT表
	BPB->BPB_RootEntCnt  = 0x0200;//512目录项个数
	BPB->BPB_TotSec16    = 0x0000;//总扇区数
	BPB->BPB_FATSz16     = 0xF2;//一个FAT表所占的扇区数
	BPB->BPB_HiddSec     = 0x00000000;  //本FAT表前隐藏的扇区数
	
	WriteBlock(0);//写引导区
}
//********************************************************************************************
//读取BPB数据结构
void ReadBPB(void)
//********************************************************************************************
{
	FAT_BPB* BPB = (FAT_BPB*)BUFFER;
	ReadBlock(0);

	//获取参数
	BPB_BytesPerSec = BPB->BPB_BytesPerSec;
	BPB_SecPerClus = BPB->BPB_SecPerClus;
	BPB_RsvdSecCnt = BPB->BPB_RsvdSecCnt;
	BPB_NumFATs = BPB->BPB_NumFATs;
	BPB_RootEntCnt = BPB->BPB_RootEntCnt;
	BPB_TotSec16 = BPB->BPB_TotSec16;
	BPB_FATSz16 = BPB->BPB_FATSz16;
	BPB_HiddSec = BPB->BPB_HiddSec;

}
//********************************************************************************************
//获取FAT表开始扇区号
uint32 GetFATStartSec(void)
//********************************************************************************************
{
	return BPB_RsvdSecCnt;
}
//********************************************************************************************
//获取根目录开始扇区号
//获取子目录开始扇区号；
uint32 DirStartSec(void)
//********************************************************************************************
{
  if (CURPATH.CurrentDirStarClusID==0)
	return BPB_RsvdSecCnt + BPB_NumFATs * BPB_FATSz16;
  else
    
   return  ClusConvLBA(CURPATH.CurrentDirStarClusID);//该子目录FDT表首扇区号。
     
 }

//********************************************************************************************
//获取数据区开始扇区号
uint32 DataStartSec(void)
//********************************************************************************************
{
	return (uint32)(BPB_RsvdSecCnt + BPB_NumFATs * BPB_FATSz16 + BPB_RootEntCnt * 32 / BPB_BytesPerSec);
}

//********************************************************************************************
//目录项占用的扇区数
uint16 GetDirSecCount(void)
//********************************************************************************************
{
	return BPB_RootEntCnt * 32 / 512;
}

//********************************************************************************************
//获取一个簇的开始扇区
uint32 ClusConvLBA(uint16 ClusID)
//********************************************************************************************
{
	return DataStartSec() + BPB_SecPerClus * (ClusID - 2);
}

//********************************************************************************************
//读取文件分配表的指定项
uint16 ReadFAT(uint16 Index)
//********************************************************************************************
{
	uint16 *RAM = (uint16*)BUFFER;
	ReadBlock(BPB_RsvdSecCnt + Index / 256);
	return RAM[Index % 256];
}

//********************************************************************************************
//读FAT表指定的扇区
void ReadFATSector(uint16 Index)
{

  ReadBlock(BPB_RsvdSecCnt + Index);

}
//写文件分配表的指定项
void WriteFAT(uint16 Index, uint16 Value)
//********************************************************************************************
{
	uint16 *RAM = (uint16*)BUFFER;
	uint32 SecID;
	SecID = BPB_RsvdSecCnt + Index / 256;
	ReadBlock(SecID);
	RAM[Index % 256] = Value;
	WriteBlock(SecID);
}
//********************************************************************************************
//写FAT2
void WriteFAT2(uint16 Index, uint16 Value)
//********************************************************************************************
{
	uint16 *RAM = (uint16*)BUFFER;
	uint32 SecID;
	SecID = BPB_RsvdSecCnt + BPB_FATSz16+ Index / 256;
	ReadBlock(SecID);
	RAM[Index % 256] = Value;
	WriteBlock(SecID);
}
//******************************************
//比较待建立的文件或文件夹是否同名
//输入：
// *pFileNameCreat :待建立的文件名
//*pFileNameExist : 已经存在的文件名
//输出：
//   文件名不同返回 0 
//   文件名相同返回1

//*********************************************
unsigned char  FileNameCmp(unsigned char  *pFileNameCreat,unsigned char  *pFileNameExist)
  {
    unsigned char i;
    for ( i = 0;i<11;i++)
    {
        if(*pFileNameCreat++ !=*pFileNameExist++)
          
          return 0;
    }
     mmc_GoIdle();//文件名重名不能建
    return 1;        
  }
//********************************************************************************************
//获取根目录中可以使用的一项且和入口的名字不一样

//现在给他扩展功能使其在任何目录下都能用 
unsigned int  GetEmptyDIR(DIR FileDir)
//********************************************************************************************
{
	uint16 DirSecCut, DirStart, i, m, ID = 0;
        
 if(CURPATH.CurrentDirStarClusID ==0) //表示当前为根目录。
    {
	DirSecCut = GetDirSecCount();
	DirStart = DirStartSec();
	for(i = 0; i < DirSecCut; i++)
	{
		ReadBlock(DirStart + i);
		for(m = 0; m < 16; m++)
		{
                  if(FileNameCmp((unsigned char *)&FileDir.FileName,&BUFFER[m * 32])) return 0xffff;
                    
			if((BUFFER[m * 32] == 0) || (BUFFER[m * 32] == 0xe5))
				return ID;
			else
				ID++;
		}
	}
	return 0xffff;
    }
   else//现在在子目录下进行查找。找不到就再增加一个簇用于存放FDT表。
   {
     //根据当前目录的此域得知此目录的FDT表所在簇
     uint16 CurrentClus = CURPATH.CurrentDirStarClusID;
     uint32 DirStart1 = ClusConvLBA(CurrentClus);  //找到该簇的首扇区号。
   contserch: 
     for(i = 0; i < BPB_SecPerClus; i++)//在一个簇的范围内查找空项。
     {
       		ReadBlock(DirStart1 + i);
		for(m = 0; m < 16; m++)
		{
                  if(FileNameCmp((unsigned char *)&FileDir.FileName,&BUFFER[m * 32])) return 0xffff;//文件重名立即退出
                    
			if((BUFFER[m * 32] == 0) || (BUFFER[m * 32] == 0xe5))
                        {
                        //  PutChar(BUFFER[m * 32]);
				return ID;
                        }       
			else
				ID++;
		}
     }
     //在一个簇的范围内没有找到空闲的项去找下一簇。
     uint16 NextClus=SerchClusChain(CurrentClus);
     if(NextClus !=0xffff)
     {
       CurrentClus=NextClus;
       DirStart1 = ClusConvLBA(CurrentClus) ; //找到该簇的首扇区号。
         goto contserch;
     }
     else//搜完整个目录都没搜到就要再申请一个簇放到簇链上。
     {
     uint16  CurrentClus1= GetNextFAT();//得到一个空簇。
      WriteFAT(CurrentClus1, 0xffff);//先占上该族;
      WriteFAT2(CurrentClus1, 0xffff);//先占上该族;
       
       WriteFAT(CurrentClus, CurrentClus1);//把簇链连接起来以备沿着簇链查找。
      WriteFAT2(CurrentClus, CurrentClus1);
       //清空刚申请的新簇对应的扇区空间以便插入新的FDT表。
       for (i = 0; i<512;i++)
        BUFFER[i] = 0;
      DirStart1 = ClusConvLBA(CurrentClus);  //找到该簇的首扇区号。
        
      for(i = 0; i < BPB_SecPerClus; i++)//在一个簇的范围内清空其扇区。
        WriteBlock(DirStart1 + i);
      return ID++;
        
     } 
        
     
   } 
}



//********************************************************************************************
//获取一个空的FAT项
uint16 GetNextFAT(void)
//********************************************************************************************
{
  /*
	uint16 FATSectors, i,j;
        FATSectors =BPB_FATSz16;

        for (i = 0; i <FATSectors;i++)
        {
          ReadFATSector(i);
	for(j = 0; j < 512; j++)  //查读出的FAT 扇区
	{
          if((BUFFER[j]==0)&&(BUFFER[j+1]==0))
          {
            return i*256+j;
          }
          else
          {
            j++;
          }  
	}          
        }
          

	return 0;
  */
  
  uint16 *RAM = (uint16*)BUFFER;
  uint16 FATSectors, i,j;
  FATSectors =BPB_FATSz16;

  for (i = 0; i <FATSectors;i++)
  { 
    ReadFATSector(i);
  for (j=0;j <256 ;j++)
    if (*RAM++==0x0000)
      {
        return i*256+j;
      }
  }
  return 0xffff;
}     

//********************************************************************************************
//读取根目录的指定项
void ReadDIR(uint16 Index, DIR* Value)
//********************************************************************************************
{
	uint32 DirStart = DirStartSec();
	ReadBlock(DirStart + Index / 16);
	CopyBytes(&BUFFER[(Index % 16) * 32], Value, 32);
}

//********************************************************************************************
//写根目录的指定项
//同时写子目录的指定项                               
void WriteDIR(uint16 Index, DIR* Value)
//********************************************************************************************
{
	uint32 LBA = DirStartSec() + Index / 16;
	ReadBlock(LBA);
	CopyBytes(Value, &BUFFER[(Index % 16) * 32], 32);
	WriteBlock(LBA);
}
//********************************************************************************************
//创建一个空文件
//说明:FileName     文件名必须是数字或者大写字母，扩展名也是如此

//文件大小事先不用设置,均为0;
//原创作者没做同名检测的问题在这里我加上。
unsigned char   CreateFile(uint8 FileName[11])
//********************************************************************************************
{
	uint16 ClusID,FreeDirEnty;
        DIR FileDir;
        uint8 filename[11];
        uint8 i;
        for (i =0;i<11;i++)
        {
          filename[i]=*FileName++;
        }
        if(!FormatFileName(filename)) return 0;//文件格式不对        
        
	//ClusNum = Size / (BPB_SecPerClus * 512) + 1;
	EmptyBytes(&FileDir, sizeof(DIR));
        CopyBytes(filename, &FileDir.FileName, 11);
        
        FileDir.FileAttrib = 0x20;
      
	FileDir.FilePosit.Size = 0;
	FileDir.FilePosit.Start = GetNextFAT();
        
	ClusID = FileDir.FilePosit.Start;
        FreeDirEnty=GetEmptyDIR( FileDir);
        if(FreeDirEnty !=0xffff) //有空闲项不重名的项
        { 
           WriteFAT(ClusID, 0xffff);
           WriteFAT2(ClusID, 0xffff);
           WriteDIR(FreeDirEnty, &FileDir);
           return  1;//no err
        } 
        else
        {
           WriteFAT(ClusID, 0x0000); //归还已经事先站下的簇
           WriteFAT2(ClusID, 0x0000);          
        //  PutStr("file not creat");
          return 0;
          
        }  
          

}
//********************************************************************************************
//在当前目录下建立空目录
//系统维护一个当前目录
//******************************************************
unsigned char   CreateDir(uint8 *FileName)
{
  uint16 ClusID,FreeDirEnty;
  uint16 i;
  uint32 DirStartSector;
  DIR FileDir;
  uint8 filename[11];
        for (i =0;i<11;i++)
        {
          filename[i]=*FileName++;
        }
        if(!FormatFileName(filename)) return 0;//文件格式不对
	//ClusNum = Size / (BPB_SecPerClus * 512) + 1;
	EmptyBytes(&FileDir, sizeof(DIR));
        CopyBytes(filename, &FileDir.FileName, 11);
        
        FileDir.FileAttrib = 0x10;//目录属性
      
	FileDir.FilePosit.Size = 0;
	FileDir.FilePosit.Start = GetNextFAT();
        ClusID = FileDir.FilePosit.Start;
           WriteFAT(ClusID, 0xffff);
           WriteFAT2(ClusID, 0xffff);
        

        FreeDirEnty=GetEmptyDIR( FileDir);
        //PutChar((uint16)FreeDirEnty);
        if(FreeDirEnty !=0xffff) //有空闲项不重名的项
        { 

           WriteDIR(FreeDirEnty, &FileDir);
           
           //还需要把新建的这个子目录的目录项 即FDT表清空。并加入。 。。目录项
           DirStartSector= ClusConvLBA( ClusID);

       for (i = 0; i<512;i++)
        BUFFER[i] = 0;
           
      for(i = 0; i < BPB_SecPerClus; i++)//在一个簇的范围内清空其扇区。
        WriteBlock(DirStartSector + i);
        //在新建的目录里边加入 。 。。目录项。   
          BUFFER[0]  ='.';
          BUFFER[1]  =' ';
          BUFFER[2]  =' ';
          BUFFER[3]  =' ';
          BUFFER[4]  =' ';
          BUFFER[5]  =' ';
          BUFFER[6] =' ';
          BUFFER[7] =' ';
          BUFFER[8] =' ';
          BUFFER[9] =' ';
          BUFFER[10]=' ';
          BUFFER[11]=0x10;
          
          BUFFER[26] =ClusID;
          BUFFER[27] =(ClusID>>8);
          BUFFER[32]  ='.';
          BUFFER[33] = '.';
          BUFFER[34] = ' ';
          BUFFER[35] = ' ';
          BUFFER[36] = ' ';
          BUFFER[37] = ' ';
          BUFFER[38] = ' ';
          BUFFER[39] = ' ';
          BUFFER[40] = ' ';
          BUFFER[41] = ' ';
          BUFFER[42] = ' ';
          BUFFER[43] = 0x10;          
          BUFFER[58] = CURPATH.CurrentDirStarClusID;
          BUFFER[59] = (CURPATH.CurrentDirStarClusID>>8);
         WriteBlock(DirStartSector );
        // PutStr("write . .. dir");
           return  1;//no err
        } 
        else
        {
           WriteFAT(ClusID, 0x0000);
           WriteFAT2(ClusID, 0x0000);          
         
          return 0;
          
        }  
  
}


//********************************************************************************************
//********************************************************************************************
//复制文件分配表,使其和备份一致
void CopyFAT(void)
//********************************************************************************************
{
	uint16 FATSz16, RsvdSecCnt, i;
	FATSz16 = BPB_FATSz16;
	RsvdSecCnt = BPB_RsvdSecCnt;
	for(i = 0; i < FATSz16; i++)
	{
		ReadBlock(RsvdSecCnt + i);
		WriteBlock(RsvdSecCnt + FATSz16 + i);
	}
}

//********************************************************************************************
//操作文件的数据
//说明：Write       1时是写操作,0为读操作
//      Name        文件名支持数字和大写字母
//      Start       开始字节
//      Length      操作长度
//      Data        操作数据
void OperateFile(uint8 Write ,uint8 Name[11], uint32 Start, uint32 Length, void* Data)
//********************************************************************************************
{
	uint8 *data = Data;
	uint16 BytePerClus, SecPerClus, ClusNum, ClusNext,ClusID,ClusIDValue ,m,Index;
	uint32 LBA, i,length;
	DIR FileDir;
	SecPerClus = BPB_SecPerClus;
	BytePerClus = BPB_SecPerClus * 512; // 每簇的字节数
	Index = GetFileID(Name, &FileDir);
        length = Length;

	//计算开始位置所在簇的簇号
	ClusNum = Start / BytePerClus;
	ClusID = FileDir.FilePosit.Start;

	for(i = 0; i < ClusNum; i++)
	{
                ClusIDValue = ReadFAT(ClusID);
		WriteFAT(ClusID, 0xffff);
                WriteFAT2(ClusID, 0xffff);
                if(ClusIDValue==0xffff)
                  ClusNext = GetNextFAT();
                else
                  ClusNext = ClusIDValue; 
                
		WriteFAT(ClusID, ClusNext);
                WriteFAT2(ClusID, ClusNext);
		ClusID = ClusNext;
	}
        WriteFAT(ClusID, 0xffff);
        WriteFAT2(ClusID, 0xffff);

	//计算开始位置所在扇区簇内偏移
	i = (Start % BytePerClus) / 512;//定位到扇区

	//计算开始位置扇区内偏移
	m = (Start % BytePerClus) % 512;//定位到扇区内偏移

	LBA = ClusConvLBA(ClusID) + i;


		for(i = 0; i < SecPerClus; i++)
		{
			if(Write)
				ReadBlock(LBA);
			else
				ReadBlock(LBA++);

			for(m = 0; m < 512; m++)
			{

				if(Write)
					BUFFER[m] = *data++;
				else
					*data++ = BUFFER[m];

				//如果读取完成就退出
				if(--Length == 0)
				{
					if(Write)
                                        {
                                                //回写扇区
						WriteBlock(LBA); 
                                               //回写目录
                                                FileDir.FilePosit.Size = Start+length;
                                                WriteDIR(Index,&FileDir);
                                        }
					return;
				}
			}
			if(Write)
				WriteBlock(LBA++); //回写扇区,指针下移
		}
	}
//}
//********************************************************************************************
//格式化
//不是真正意义上的格式化，只是清除根目录以及FAT表
void FormatCard(void)
//********************************************************************************************
{
    uint16 i,DirStart,DirSecNum,FatStart,FatSecNum;
   
    ReadBPB();
    //清除根目录
    for(i = 0; i <= 512;i++ )BUFFER[i]=0;
    DirSecNum = GetDirSecCount();
    DirStart = DirStartSec();
    for(i=0;i<DirSecNum;i++)
    {
      WriteBlock(DirStart+i);
    }
    
    //清除FAT表
    FatStart = BPB_RsvdSecCnt;
    FatSecNum = BPB_FATSz16;
    for(i=0;i<FatSecNum;i++)
    {
      WriteBlock(FatStart+i);
    }
    WriteFAT(0,0xf8ff);
    WriteFAT(1,0xffff);
    
    //备份FAT表
    CopyFAT();
}

//********************************************************************************************
//---------------------------------------------------------------------------------------------------------------------------------
        
        
//查找簇链
//用于  维护簇链；                             
//输入：给定一个簇查找其下一个簇在FAT表的位置。
                               
 //**************************************************                              
uint16     SerchClusChain(uint16 CurrentClus)                   
           {
               return  ReadFAT(CurrentClus);
            
          }



//**********************************************
uint8 FormatFileName(uint8 *filename)
{
  uint8 i,j;
  for (i = 0;i<11;i++)
  {
    if(((filename[i]>='0')&&(filename[i]<='9'))||((filename[i]<='Z')&&(filename[i]>='A'))||(filename[i]==0x2e)||(filename[i]==0x20))
    continue;
    break;
  }
  if (i ==0) return 0 ;//无效文件名；
  for (j=i;j <11;j++)
  {
    filename[j]=0x20;
  }
  return 1;//文件名合法。
}






//删除文件 删除当前目录的某文件。
unsigned int DelFile(uint8 Name[11])
{
      DIR FileDir;
      uint16 ClusID,Index;
        uint8 filename[11];
        uint8 i;
        for (i =0;i<11;i++)
        {
          filename[i]=*Name++;
        }
        if(!FormatFileName(filename)) return  0xffff;//文件格式不对         
        
      Index = GetFileID(filename,&FileDir);
      if(Index ==0xffff)return  0xffff;// 文件不存在无需删除。
      if(!(FileDir.FileAttrib &0x20)) return  0xffff;//文件是个目录不能删除只能用目录删除命令。
      FileDir.FileName.NAME[0] = 0xE5; 
      ClusID = FileDir.FilePosit.Start;
      
      uint16 tempClusID = SerchClusChain(ClusID);
      
      while(tempClusID !=0xffff)//删除文件所占用的簇链。
      {
        WriteFAT(tempClusID,0x0000);
        WriteFAT2(tempClusID,0x0000);
        tempClusID = SerchClusChain(tempClusID);
      }
      WriteFAT(ClusID,0x0000); //把首簇也删除
      WriteFAT2(ClusID,0x0000);

      WriteDIR(Index,&FileDir);
 return DELSUCCESS;     
}



//删除当前目录下的目录
signed int  DelDir(uint8 Name[11])
{
       DIR FileDir;
      uint16 Index,ClusID,tempClusID,NextClusID;

        uint8 filename[11];
        uint8 i,m;
        for (i =0;i<11;i++)
        {
          filename[i]=*Name++;
        }
        if(!FormatFileName(filename)) return  -1;//文件格式不对         
        
      Index = GetFileID(filename,&FileDir);
      if(Index ==0xffff) return  ERR;//当前目录没有这个文件或目录。
      if(!(FileDir.FileAttrib &0x10)) return  -1;//文件个普通文件不能删除只能用文件删除命令删除。
      FileDir.FileName.NAME[0] = 0xE5; 
      ClusID = FileDir.FilePosit.Start;
      //调出该目录的FDT表查看其目录项是否都是空的或有已被删除的文件
      //首先取出该簇的起始扇区
     uint32 DirStart1 = ClusConvLBA(ClusID);  //找到该簇的首扇区号。
     NextClusID=ClusID;
   contserch: 
     for(i = 0; i < BPB_SecPerClus; i++)//在一个簇的范围内查找空项。
     {
       		ReadBlock(DirStart1 + i);
		for(m = 0; m < 16; m++)
		{
 
                    
			if( (BUFFER[m * 32] != 0xe5)&&(BUFFER[m * 32] != 0x2e)&&(BUFFER[m * 32] != 0))
                        {

				return -2;//要删除的目录还有没删除的文件
                        }       

		}
     }
     //在一个簇的范围内没有找到被占用的项去找下一簇。
      NextClusID=SerchClusChain(NextClusID);
     if(NextClusID !=0xffff)
     {
       
       DirStart1 = ClusConvLBA(NextClusID) ; //找到该簇的首扇区号。
         goto contserch;
     }
     else//整个目录是空的可以删除。
     {
       tempClusID = SerchClusChain(ClusID);
      
      while(tempClusID !=0xffff)//删除文件所占用的簇链。
      {
        WriteFAT(tempClusID,0x0000);
        WriteFAT2(tempClusID,0x0000);
        tempClusID = SerchClusChain(tempClusID);
      }
      WriteFAT(ClusID,0x0000); //把首簇也删除
      WriteFAT2(ClusID,0x0000);
      WriteDIR(Index,&FileDir);        
      return DELSUCCESS; 
     }  
        

}




//********************************************************************************************
//获得和文件名对应的目录项以及在该目录的位置
uint16 GetFileID(uint8 Name[11], DIR* ID)
//********************************************************************************************
//********************************************************************************************
{
	uint16 DirSecCut, DirStart, i, m, Index=0;
        
 if(CURPATH.CurrentDirStarClusID ==0) //表示当前为根目录。
    {
	DirSecCut = GetDirSecCount();
	DirStart = DirStartSec();
	for(i = 0; i < DirSecCut; i++)
	{
		ReadBlock(DirStart + i);
		for(m = 0; m < 16; m++)
		{
			if(IsEqual(Name, &((DIR*)&BUFFER[m * 32])->FileName, 11))
			{
				*ID = *((DIR*)&BUFFER[m * 32]);
                                Index = i*16+m;
                                mmc_GoIdle();
				return Index; //找到对应的目录项,返回目录的位置.
			}
		}
	}
        mmc_GoIdle();
	return 0xffff;
    }
   else//现在在子目录下进行查找。
   {
     //根据当前目录的此域得知此目录的FDT表所在簇
     uint16 CurrentClus = CURPATH.CurrentDirStarClusID;
     uint32 DirStart1 = ClusConvLBA(CurrentClus);  //找到该簇的首扇区号。
   contserch: 
     for(i = 0; i < BPB_SecPerClus; i++)//在一个簇的范围内查找空项。
     {
       		ReadBlock(DirStart1 + i);
		for(m = 0; m < 16; m++)
		{
 			if(IsEqual(Name, &((DIR*)&BUFFER[m * 32])->FileName, 11))
			{
				*ID = *((DIR*)&BUFFER[m * 32]);
                                Index += (i*16+m);
                                mmc_GoIdle();
				return Index; //找到对应的目录项,返回目录的位置.
			}
			
				
		}
     }
     //在一个簇的范围内没有找到去找下一簇。
     uint16 NextClus=SerchClusChain(CurrentClus);
     if(NextClus !=0xffff)
     {
       CurrentClus=NextClus;
       DirStart1 = ClusConvLBA(CurrentClus) ; //找到该簇的首扇区号。
         goto contserch;
     }
     else//搜完整个目录都没搜到。
     {  
      mmc_GoIdle(); 
      return 0xffff;//没找到
     }
        
     
   } 
}



//显示当前文件列表；

void ListFile(void)
{
	uint16 DirSecCut, DirStart, i,j, m ;
  PutStr1("<?xml version=\"1.0\"?>");
                      putchar1(0x0d);
                    putchar1(0x0a);
  PutStr1("<FILELIST>");
                      putchar1(0x0d);
                    putchar1(0x0a);


  Delay_ms(4);
 if(CURPATH.CurrentDirStarClusID ==0) //表示当前为根目录。
    { 
	DirSecCut = GetDirSecCount();
	DirStart = DirStartSec();
	for(i = 0; i < DirSecCut; i++)
	{
                
                initSPI(0x02);
                
		ReadBlock(DirStart + i);
		for(m = 0; m < 16; m++)
		{
                
                  if( (BUFFER[m * 32] != 0xe5)&&(BUFFER[m * 32] != 0x2e)&&(BUFFER[m * 32] != 0))
                  {
                    
                   //打印到屏幕把文件名字。
                               
                      uint8 *p =&BUFFER[m * 32];
                    Init_Uart1;  
                    PutStr1("<FILE>");  
                    for (j=0;j<11;j++)
                    putchar1(*p++);
                    PutStr1("</FILE>");
                    putchar1(0x0d);
                    putchar1(0x0a);
                  } 
                  //可以在这里把目录列表写入文件里

		
		}
	}
        Init_Uart1;
        PutStr1("/FILELIST");
	return ;
    }
   else//现在在子目录下进行查找。
   {
     //根据当前目录的此域得知此目录的FDT表所在簇
     uint16 CurrentClus = CURPATH.CurrentDirStarClusID;
     uint32 DirStart1 = ClusConvLBA(CurrentClus);  //找到该簇的首扇区号。
   contserch: 
     for(i = 0; i < BPB_SecPerClus; i++)//在一个簇的范围内查找空项。
     {

                initSPI1(0x02);
              
       		ReadBlock(DirStart1 + i);
		for(m = 0; m < 16; m++)
		{
                  //打印到屏幕把文件名字。
                 Init_Uart1; 
                 if( (BUFFER[m * 32] != 0xe5)&&(BUFFER[m * 32] != 0x2e)&&(BUFFER[m * 32] != 0)&&(BUFFER[m * 32] >='0')&&(BUFFER[m * 32] <='9'))
                 {
                    uint8 *p =&BUFFER[m * 32];
                    
                    PutStr1("<FILE>");  
                    for (j=0;j<11;j++)
                    putchar1(*p++);
                    PutStr1("</FILE>");
                    putchar1(0x0d);
                    putchar1(0x0a);
                   Delay_ms(2);
                 }
                 //可以在这里把目录列表写入文件里
                Delay_ms(2); 
				
		}
           Delay_ms(2);     
         
     }
     //在一个簇的范围内没有找到去找下一簇。

     initSPI1(0x02);
   
     uint16 NextClus=SerchClusChain(CurrentClus);
     if(NextClus !=0xffff)
     {
       CurrentClus=NextClus;
       DirStart1 = ClusConvLBA(CurrentClus) ; //找到该簇的首扇区号。
         goto contserch;
     }
     else//搜完整个目录都没搜到。
     { 
       Init_Uart1;
      PutStr1("</FILELIST>"); 
      return ;//没找到
     } 
        
     
   }

}



//改变当前目录
uint8 ChgDir(uint8 Name[11])
{
      DIR FileDir;
      uint16 Index;
        uint8 filename[11];
        uint8 i,j;
        uint8 *p;
        if(Name[0]!=0x2e)//向子目录方向
        {  
          for (i =0;i<11;i++)
          {
            filename[i]=*Name++;
          }
          if(!FormatFileName(filename)) return  0xff;//文件格式不对         
          
          Index = GetFileID(filename,&FileDir);  
          if(Index ==0xffff) return  0xff;//目录不存在；
          if(FileDir.FileAttrib &0x20) return  0xff;//不是目录是个文件
          
          CURPATH.CurrentDirStarClusID=FileDir.FilePosit.Start;
           p =CURPATH.Path;
          for (i = 0;i < MAX_PATH_SIZE;i++)
          {
            if((p[i])==0x20)
            {
              
              for (j = 0;j< 11;j++)
              {
                if(filename[j]!=0x20)
                { 
                  p[i]=filename[j];
                  i++;
                }
                else
                  break; 
              }
              
              break;
            }
            
          }
          p[i]='\\';
          
          i = 0;
          while ((*p !=' ' )&&(i<5))
          {
            PromptBuffer[i] = *p++;
            i++;
          }
          PromptBuffer[i] ='\0'; 
        }
        else //向根目录方向
        {
          for (i =0;i<11;i++)
          {
            filename[i]=*Name++;
          }
          if(!FormatFileName(filename)) return  0xff;//文件格式不对 
         Index = GetFileID(filename,&FileDir);  
          if(Index ==0xffff) return  0xff;
          if(FileDir.FileAttrib &0x20) return  0xff;//不是目录是个文件
          
          CURPATH.CurrentDirStarClusID=FileDir.FilePosit.Start;          
          
         // if(CURPATH.CurrentDirStarClusID !=0)
          {
            p =CURPATH.Path;
            j=0;
            for (i=0;i< MAX_PATH_SIZE; i++)
            {
              if (p[j] !=0x20)
                
              {
                j++;
               
              }
              else
              {
                p[j--] =0x20;
                break;
              } 
            }
          }
                 p[j]= ' ';
                for (;j>0;j--)
                {
                  if (p[j]=='\\')break;
                  else p[j]= ' ';
                }
           i = 0;     
          while ((*p !=' ' )&&(i<5))
          {
            PromptBuffer[i] = *p++;
            i++;
          }
          PromptBuffer[i] ='\0';                 
                
          
          
        }  
  return 1;
}


 //获取一个簇的开始扇区号
uint32 GetClusStarSector(uint16 ClusID)
{
return  ClusConvLBA(ClusID);
}


//以FDT的ID号作为索引写修改后的文件目录表相应的域
void ModfyFDT(unsigned int  Index,DIR* FileDir)
{
  unsigned int CluseNum;
  unsigned int Sector =Index/16;
  unsigned int SectorOffsetInClus;
  unsigned long StarSector;
  unsigned int ClusID,DirOffsetInSector;
  CluseNum =Sector/BPB_SecPerClus;
  
  SectorOffsetInClus =Sector%BPB_SecPerClus;
  DirOffsetInSector =Sector =Index%16;
  if(CURPATH.CurrentDirStarClusID==0)//这是在根目录下进行操作
  {
    	
	StarSector = DirStartSec()+SectorOffsetInClus;
      ReadBlock(StarSector);
       *((DIR*)&BUFFER[DirOffsetInSector*32])=*FileDir;
      WriteBlock(StarSector);
     return;        
        
  }
  else if(CURPATH.CurrentDirStarClusID !=0)//这是在子目录下进行操作
 ClusID =CURPATH.CurrentDirStarClusID;
 //从本目录的首簇号开始查找该目录项所在的簇号；
 if(CluseNum==0)
 {
   //这个目录项就在首簇上呢；
  StarSector =GetClusStarSector(ClusID)+SectorOffsetInClus;//找到相应的扇区
  ReadBlock(StarSector);
  *((DIR*)&BUFFER[DirOffsetInSector*32])=*FileDir;
  WriteBlock(StarSector);
  return;
 }
 else
 { 
   unsigned int i;
   for (i = 0;i <CluseNum ;i++)
   {
     ClusID =SerchClusChain(ClusID);
   }
 StarSector = GetClusStarSector(ClusID)+SectorOffsetInClus;//找到相应的扇区
  ReadBlock(StarSector);
  *(DIR*)BUFFER[DirOffsetInSector*32]=*FileDir;
  WriteBlock(StarSector);   
 }
 
}






//向一个文件追加数据。
uint8 AppendData(uint8 *FileName, uint8 *pSource, uint32 length)
{
     DIR FileDir;
      uint16 ClusID,Index,tempClusID,NextClusID,LastClusID;
      uint16 InClusOffset,SectorOffset,InSectorOffset;
        uint8 filename[11];
        uint16 i;

        uint32  SeekPos,LastSector;
  
          for (i =0;i<11;i++)
          {
            filename[i]=*FileName++;
          }
          if(!FormatFileName(filename)) return  0xff;//文件格式不对         
          
          Index = GetFileID(filename,&FileDir);  
          if(Index ==0xffff) return  0xff;//没找到指定文件。
          if(FileDir.FileAttrib &0x10) return  0xff;//不是目录是个文件
                    
          ClusID=FileDir.FilePosit.Start; 
          //seek到文件的末尾。
          
          SeekPos =FileDir.FilePosit.Size;
           tempClusID=LastClusID =NextClusID=ClusID;
         // tempClusID=NextClusID =SerchClusChain(ClusID);
          next:
           if(NextClusID !=0xffff)//找到末尾簇
           {
             tempClusID=NextClusID;
             NextClusID=SerchClusChain(tempClusID);
             goto next;
           }
           else
           {
             //找到了末尾簇；
             LastClusID =tempClusID ;
           }
           InClusOffset = SeekPos %(BPB_SecPerClus*BPB_BytesPerSec);//找到在文件最末端的簇内偏移。
           SectorOffset = InClusOffset /BPB_BytesPerSec;//找到在最后簇内扇区偏移。
           InSectorOffset=InClusOffset %BPB_BytesPerSec;//找到在文件最末端的扇区内偏移。
           
           
          //打开最后簇的最后面的扇区。
           LastSector =GetClusStarSector(LastClusID)+SectorOffset;
           
            ReadBlock(LastSector);   
            if((BPB_BytesPerSec -InSectorOffset )>=length)//需要写入的字节没有超出最后一个扇区的剩余空间
            { 
            memcpy(&BUFFER[InSectorOffset],pSource,length);
            pSource+=length;
            WriteBlock(LastSector); 
            FileDir.FilePosit.Size+=length;
              
              //这里把dir项size项刷新后就可以退出了。
            ModfyFDT( Index, &FileDir) ; 
            }
            else
            { //需要写入的字节超出最后一个扇区的剩余空间则用数据把这个扇区填满
              memcpy(&BUFFER[InSectorOffset],pSource,BPB_BytesPerSec -InSectorOffset);
            pSource+=(BPB_BytesPerSec -InSectorOffset);
            WriteBlock(LastSector); 
            FileDir.FilePosit.Size+=BPB_BytesPerSec -InSectorOffset; 
            length-= (BPB_BytesPerSec -InSectorOffset); 
            LastSector++;
            conti:
              while(SectorOffset <BPB_SecPerClus-1)//该簇还有空余的扇区供使用；
              {
             
                  if(length >BPB_BytesPerSec)
                  {
            memcpy(BUFFER,pSource,BPB_BytesPerSec );
            pSource+=BPB_BytesPerSec;
            WriteBlock(LastSector++); 
            FileDir.FilePosit.Size+=BPB_BytesPerSec ; 
            length-= BPB_BytesPerSec ;  

            SectorOffset++;
                    
                  }
                  else//剩下的数据已经不足一个扇区了。
                  {
            memcpy(BUFFER,pSource,length );
            pSource+=length;
            WriteBlock(LastSector); 
            FileDir.FilePosit.Size+=length ; 
            //更新了目录项就可以退出了。
            ModfyFDT( Index, &FileDir) ;
            mmc_GoIdle();
              return 1;      
                  }  
                  
               
              }
                //该簇占满了该申请下一个簇了。
             
              LastClusID= GetNextFAT();
              WriteFAT(LastClusID, 0xFFFF);//占领申请好的簇
              WriteFAT2(LastClusID, 0xFFFF);
        
              WriteFAT(tempClusID,LastClusID);//把簇链衔接上。
              WriteFAT2(tempClusID,LastClusID);
              SectorOffset = 0;
              //获得刚申请的簇的首扇区号。
              LastSector = GetClusStarSector(LastClusID);
              goto conti;
            
            } 
        mmc_GoIdle();            
        return 1;           
        }


//读当前目录下指定文件的指定位置的指定长度的数据块。

uint8 ReadFile(uint8 *FileName, uint8 *pDist, uint32 Position,uint32 length)
{
       DIR FileDir;
      uint16 ClusID,Index,LastClusID;
      uint16 InClusOffset,SectorOffset,InSectorOffset;
        uint8 filename[11];
        uint16 i;

        uint32  SeekPos,LastSector;
  
          for (i =0;i<11;i++)
          {
            filename[i]=*FileName++;
          }
          if(!FormatFileName(filename)) return  0xff;//文件格式不对         
          
          Index = GetFileID(filename,&FileDir);  
          if(Index ==0xffff) return  0xff;//没找到指定文件。
          if(FileDir.FileAttrib &0x10) return  0xff;//不是目录是个文件
                    
          ClusID=FileDir.FilePosit.Start; 
          //seek到文件的末尾。
          
          SeekPos =FileDir.FilePosit.Size;
          if((Position+length) >SeekPos)//超出文件边界；
            return 0xff;

           LastClusID =ClusID;
         // tempClusID=NextClusID =SerchClusChain(ClusID);

           InClusOffset = Position %(BPB_SecPerClus*BPB_BytesPerSec);//找到文件指定位置的簇内偏移。
           SectorOffset = InClusOffset /BPB_BytesPerSec;//找到文件指定位置的簇内扇区偏移。
           InSectorOffset=InClusOffset %BPB_BytesPerSec;//找到文件指定位置的扇区内偏移。
           
           
          //打开指定位置所在扇区。
           LastSector =GetClusStarSector(LastClusID)+SectorOffset;
           
            ReadBlock(LastSector);   
            if((BPB_BytesPerSec -InSectorOffset )>=length)//需要读出的字节没有超出最后一个扇区的剩余空间
            { 
            memcpy(pDist,&BUFFER[InSectorOffset],length);
            pDist+=length;


            }
            else
            { //需要读出的字节超出最后一个扇区的剩余空间则把此扇区剩余的数据读出来
              memcpy(pDist,&BUFFER[InSectorOffset],BPB_BytesPerSec -InSectorOffset);
            pDist+=(BPB_BytesPerSec -InSectorOffset);
 
            length-= (BPB_BytesPerSec -InSectorOffset); 
            LastSector++;//准备读下一扇区。
            conti:
              while(SectorOffset <BPB_SecPerClus-1)//该簇还有空余的扇区供使用；
              {
             
                  if(length >BPB_BytesPerSec)
                  {
                    ReadBlock(LastSector++);        
                    memcpy(pDist,BUFFER,BPB_BytesPerSec );
                    pDist+=BPB_BytesPerSec;
                    
                    length-= BPB_BytesPerSec ;  
                    
                    SectorOffset++;
                    
                  }
                  else//剩下的数据已经不足一个扇区了。
                  {
                    ReadBlock(LastSector);
                    memcpy(pDist,BUFFER,length );
                    pDist+=length;
                    
                    //更新了目录项就可以退出了。
                    ModfyFDT( Index, &FileDir) ; 
                    mmc_GoIdle();
                    return 1 ;      
                  }  
                  
               
              }
                //该簇占满了该读下一个簇了。
             
              LastClusID= SerchClusChain(LastClusID);

              SectorOffset = 0;
              //获得刚申请的簇的首扇区号。
              LastSector = GetClusStarSector(LastClusID);
              goto conti;
            
            }  
 mmc_GoIdle();               
return 1;  
}




//改变指定文件的指定区域的数据。而不改变文件的大小只是覆盖原有的区域。
uint8 ModifyFile(uint8 *FileName, uint8 *pSource,uint32 Position, uint32 length)
{
     DIR FileDir;
      uint16 ClusID,Index,LastClusID;
      uint16 InClusOffset,SectorOffset,InSectorOffset;
        uint8 filename[11];
        uint16 i;

        uint32  SeekPos,LastSector;
  
          for (i =0;i<11;i++)
          {
            filename[i]=*FileName++;
          }
          if(!FormatFileName(filename)) return  0xff;//文件格式不对         
          
          Index = GetFileID(filename,&FileDir);  
          if(Index ==0xffff) return  0xff;//没找到指定文件。
          if(FileDir.FileAttrib &0x10) return  0xff;//不是目录是个文件
                    
          ClusID=FileDir.FilePosit.Start; 
          //seek到文件的末尾。
          
          SeekPos =FileDir.FilePosit.Size;
          if((Position+length)>SeekPos) return 0xff;
           LastClusID =ClusID;
         // tempClusID=NextClusID =SerchClusChain(ClusID);

           InClusOffset = Position %(BPB_SecPerClus*BPB_BytesPerSec);//找到文件指定位置的簇内偏移。
           SectorOffset = InClusOffset /BPB_BytesPerSec;//找到文件指定位置的簇内扇区偏移。
           InSectorOffset=InClusOffset %BPB_BytesPerSec;//找到文件指定位置的扇区内偏移。
          
          //打开指定位置的扇区。
           LastSector =GetClusStarSector(LastClusID)+SectorOffset;
           
            ReadBlock(LastSector);   
            if((BPB_BytesPerSec -InSectorOffset )>=length)//需要写入的字节没有超出最后一个扇区的剩余空间
            { 
            memcpy(&BUFFER[InSectorOffset],pSource,length);
            pSource+=length;
            WriteBlock(LastSector); 
 
            }
            else
            { //需要写入的字节超出最后一个扇区的剩余空间则用数据把这个扇区填满
              memcpy(&BUFFER[InSectorOffset],pSource,BPB_BytesPerSec -InSectorOffset);
            pSource+=(BPB_BytesPerSec -InSectorOffset);
            WriteBlock(LastSector); 
 
            length-= (BPB_BytesPerSec -InSectorOffset); 
            LastSector++;
            conti:
              while(SectorOffset <BPB_SecPerClus-1)//该簇还有空余的扇区供使用；
              {
             
                  if(length >BPB_BytesPerSec)
                  {
            memcpy(BUFFER,pSource,BPB_BytesPerSec );
            pSource+=BPB_BytesPerSec;
            WriteBlock(LastSector++); 

            length-= BPB_BytesPerSec ;  

            SectorOffset++;
                    
                  }
                  else//剩下的数据已经不足一个扇区了。
                  {
            memcpy(BUFFER,pSource,length );
            pSource+=length;
            WriteBlock(LastSector); 

              return 1;      
                  }  
                  
               
              }
                //该簇占满了该该读下一个簇了。
             
              LastClusID= SerchClusChain(LastClusID);

              SectorOffset = 0;
              //获得刚搜的簇的首扇区号。
              LastSector = GetClusStarSector(LastClusID);
              goto conti;
            
            }  
        return 1;           
        }

//获取文件有关属性 
//****************************************************

unsigned long GetFileLength(unsigned char *FileName)
{
  DIR ID;
  if (GetFileID(FileName,  &ID) !=0xffff)
  {
    return ID.FilePosit.Size;
  }
  return 0;
}







//******************************************************8
// send a file to uart 
void SendFileToUart(unsigned char *FileName)
{
  unsigned long m  ;
  unsigned int i;
    
  unsigned long FileLength;
 
   initSPI(0x02);
  FileLength =GetFileLength(FileName);
  if(FileLength ==0)return;
  m =FileLength/256;
  for (i = 0;i<m; i++)
  {
    initSPI(0x02);
   if( ReadFile(FileName, Temp, i*256,256)==0xff)return;
   {
    Init_Uart1; 
    SendToUart1(Temp,256);
    
   }
    
  }
  m=FileLength%256;
  initSPI(0x02);
  ReadFile(FileName, Temp, FileLength-m,m)  ;
  Init_Uart1;
  SendToUart1(Temp,(unsigned int)m);
  
}






//*****************************
//把目录列表打印到事先准备好的空文件里
void ListFileToFile(void)
{
  uint8 buf[16*13];
  uint8 *p1 =buf;
  uint16 length = 0;
	uint16 DirSecCut, DirStart, i,j, m ;
        
 if(CURPATH.CurrentDirStarClusID ==0) //表示当前为根目录。
    {
	DirSecCut = GetDirSecCount();
	DirStart = DirStartSec();
	for(i = 0; i < DirSecCut; i++)
	{
		ReadBlock(DirStart + i);
		for(m = 0; m < 16; m++)
		{
                
                  if( (BUFFER[m * 32] != 0xe5)&&(BUFFER[m * 32] != 0x2e)&&(BUFFER[m * 32] != 0))
                  {
                    
                   //打印到屏幕把文件名字。
                               
                      uint8 *p =&BUFFER[m * 32];
                    for (j=0;j<11;j++)
                    *p1++ = *p++;
                    *p1++ = 0x0d;
                    *p1++ = 0x0a;
                    length += 13;
                  } 
                  //可以在这里把目录列表写入文件里
                  AppendData("FILELIST",buf,(uint32)length);
		
		}
	}
	return ;
    }
   else//现在在子目录下进行查找。
   {
     //根据当前目录的此域得知此目录的FDT表所在簇
     uint16 CurrentClus = CURPATH.CurrentDirStarClusID;
     uint32 DirStart1 = ClusConvLBA(CurrentClus);  //找到该簇的首扇区号。
   contserch: 
     for(i = 0; i < BPB_SecPerClus; i++)//在一个簇的范围内查找空项。
     {
       		ReadBlock(DirStart1 + i);
		for(m = 0; m < 16; m++)
		{
                  if( (BUFFER[m * 32] != 0xe5)&&(BUFFER[m * 32] != 0x2e)&&(BUFFER[m * 32] != 0))                  
                  {
                    
                   //打印到屏幕把文件名字。
                               
                      uint8 *p =&BUFFER[m * 32];
                    for (j=0;j<11;j++)
                    *p1++ = *p++;
                    *p1++ = 0x0d;
                    *p1++ = 0x0a;
                    length += 13;
                  } 
                  //可以在这里把目录列表写入文件里
                  AppendData("FILELIST",buf,(uint32)length);
				
		}
     }
     //在一个簇的范围内没有找到去找下一簇。
     uint16 NextClus=SerchClusChain(CurrentClus);
     if(NextClus !=0xffff)
     {
       CurrentClus=NextClus;
       DirStart1 = ClusConvLBA(CurrentClus) ; //找到该簇的首扇区号。
         goto contserch;
     }
     else//搜完整个目录都没搜到。
       
      return ;//没找到
        
     
   } 
}



unchar ReName(unchar *pSourceFileName,unchar*pNewFileName)
{
  DIR ID;
  unint index=GetFileID(pSourceFileName,  &ID);
  if (index !=0xffff)
  {
    for (unchar i =0;i<8;i++)
     {
       ID.FileName.NAME[i]= pNewFileName[i];
     }
        for (unchar i =0;i<3;i++)
     {
       ID.FileName.TYPE[i]= pNewFileName[9+i];
     }
    WriteDIR(index, &ID);
    return 1;
  }
  else
  return 0;
    
}

void RcvFileFromUart(unchar *FileName,uint8 *pSource,uint32 length)
{
  if(FindFile(FileName)==0)//没找到文件就建立之
  { 
  CreateFile(FileName);
  }
  
  AppendData(FileName,pSource,  length);
  
}

unchar FindFile(unchar *FileName)
{
	uint16 DirSecCut, DirStart, i, m ;

 if(CURPATH.CurrentDirStarClusID ==0) //表示当前为根目录。
    {
	DirSecCut = GetDirSecCount();
	DirStart = DirStartSec();
	for(i = 0; i < DirSecCut; i++)
	{
		ReadBlock(DirStart + i);
		for(m = 0; m < 16; m++)
		{
                
                  if(IsEqual(FileName, &((DIR*)&BUFFER[m * 32])->FileName, 11))
                  {
                    
                    return 1;//找到了指定文件
                  } 
                 

		
		}
	}

	return 0;//未找到指定的文件
    }
   else//现在在子目录下进行查找。
   {
     //根据当前目录的此域得知此目录的FDT表所在簇
     uint16 CurrentClus = CURPATH.CurrentDirStarClusID;
     uint32 DirStart1 = ClusConvLBA(CurrentClus);  //找到该簇的首扇区号。
   contserch: 
     for(i = 0; i < BPB_SecPerClus; i++)//在一个簇的范围内查找空项。
     {
       		ReadBlock(DirStart1 + i);
		for(m = 0; m < 16; m++)
		{
                  //打印到屏幕把文件名字。
                 if(IsEqual(FileName, &((DIR*)&BUFFER[m * 32])->FileName, 11))
                 {
                  return 1;//找到了指定文件
                   
                 }

				
		}
     }
     //在一个簇的范围内没有找到去找下一簇。
     uint16 NextClus=SerchClusChain(CurrentClus);
     if(NextClus !=0xffff)
     {
       CurrentClus=NextClus;
       DirStart1 = ClusConvLBA(CurrentClus) ; //找到该簇的首扇区号。
         goto contserch;
     }
     else//搜完整个目录都没搜到。
     {  
      PutStr1("/FILELIST"); 
      return 0;//没找到
     } 
        
     
   }

}
