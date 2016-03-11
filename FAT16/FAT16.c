#define		FAT_GLOBALS
#include	"..\include.h"





void InitFAT16(void )
{
  uint8 i;
  CURPATH.CurrentDirStarClusID =0;//��ǰĿ¼��ʼ��š�
  CURPATH.FatherDirStarClusID =0; 
  CURPATH.pPath =CURPATH.Path;
  for (i = 0;i<MAX_PATH_SIZE;i++ )
  CURPATH.Path[i]=' ';
  CURPATH.Path[0]='\\';
  ReadBPB();
  
}

//********************************************************************************************
//��һ������
void ReadBlock(uint32 LBA)
//********************************************************************************************
{
	mmcReadSector(LBA,BUFFER);
        return;
}

//********************************************************************************************
//дһ������
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
//дBPB���ݽṹ
void FATInit(void)
//********************************************************************************************
{
	FAT_BPB* BPB = (FAT_BPB*)BUFFER;
		//����
	BPB->BPB_BytesPerSec = 0x0200;//ÿ������512�ֽ�
	BPB->BPB_SecPerClus  = 0x08;  //ÿ����8������
	BPB->BPB_RsvdSecCnt  = 0x0004;//һ����������
	BPB->BPB_NumFATs     = 0x02; //2��FAT��
	BPB->BPB_RootEntCnt  = 0x0200;//512Ŀ¼�����
	BPB->BPB_TotSec16    = 0x0000;//��������
	BPB->BPB_FATSz16     = 0xF2;//һ��FAT����ռ��������
	BPB->BPB_HiddSec     = 0x00000000;  //��FAT��ǰ���ص�������
	
	WriteBlock(0);//д������
}
//********************************************************************************************
//��ȡBPB���ݽṹ
void ReadBPB(void)
//********************************************************************************************
{
	FAT_BPB* BPB = (FAT_BPB*)BUFFER;
	ReadBlock(0);

	//��ȡ����
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
//��ȡFAT��ʼ������
uint32 GetFATStartSec(void)
//********************************************************************************************
{
	return BPB_RsvdSecCnt;
}
//********************************************************************************************
//��ȡ��Ŀ¼��ʼ������
//��ȡ��Ŀ¼��ʼ�����ţ�
uint32 DirStartSec(void)
//********************************************************************************************
{
  if (CURPATH.CurrentDirStarClusID==0)
	return BPB_RsvdSecCnt + BPB_NumFATs * BPB_FATSz16;
  else
    
   return  ClusConvLBA(CURPATH.CurrentDirStarClusID);//����Ŀ¼FDT���������š�
     
 }

//********************************************************************************************
//��ȡ��������ʼ������
uint32 DataStartSec(void)
//********************************************************************************************
{
	return (uint32)(BPB_RsvdSecCnt + BPB_NumFATs * BPB_FATSz16 + BPB_RootEntCnt * 32 / BPB_BytesPerSec);
}

//********************************************************************************************
//Ŀ¼��ռ�õ�������
uint16 GetDirSecCount(void)
//********************************************************************************************
{
	return BPB_RootEntCnt * 32 / 512;
}

//********************************************************************************************
//��ȡһ���صĿ�ʼ����
uint32 ClusConvLBA(uint16 ClusID)
//********************************************************************************************
{
	return DataStartSec() + BPB_SecPerClus * (ClusID - 2);
}

//********************************************************************************************
//��ȡ�ļ�������ָ����
uint16 ReadFAT(uint16 Index)
//********************************************************************************************
{
	uint16 *RAM = (uint16*)BUFFER;
	ReadBlock(BPB_RsvdSecCnt + Index / 256);
	return RAM[Index % 256];
}

//********************************************************************************************
//��FAT��ָ��������
void ReadFATSector(uint16 Index)
{

  ReadBlock(BPB_RsvdSecCnt + Index);

}
//д�ļ�������ָ����
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
//дFAT2
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
//�Ƚϴ��������ļ����ļ����Ƿ�ͬ��
//���룺
// *pFileNameCreat :���������ļ���
//*pFileNameExist : �Ѿ����ڵ��ļ���
//�����
//   �ļ�����ͬ���� 0 
//   �ļ�����ͬ����1

//*********************************************
unsigned char  FileNameCmp(unsigned char  *pFileNameCreat,unsigned char  *pFileNameExist)
  {
    unsigned char i;
    for ( i = 0;i<11;i++)
    {
        if(*pFileNameCreat++ !=*pFileNameExist++)
          
          return 0;
    }
     mmc_GoIdle();//�ļ����������ܽ�
    return 1;        
  }
//********************************************************************************************
//��ȡ��Ŀ¼�п���ʹ�õ�һ���Һ���ڵ����ֲ�һ��

//���ڸ�����չ����ʹ�����κ�Ŀ¼�¶����� 
unsigned int  GetEmptyDIR(DIR FileDir)
//********************************************************************************************
{
	uint16 DirSecCut, DirStart, i, m, ID = 0;
        
 if(CURPATH.CurrentDirStarClusID ==0) //��ʾ��ǰΪ��Ŀ¼��
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
   else//��������Ŀ¼�½��в��ҡ��Ҳ�����������һ�������ڴ��FDT��
   {
     //���ݵ�ǰĿ¼�Ĵ����֪��Ŀ¼��FDT�����ڴ�
     uint16 CurrentClus = CURPATH.CurrentDirStarClusID;
     uint32 DirStart1 = ClusConvLBA(CurrentClus);  //�ҵ��ôص��������š�
   contserch: 
     for(i = 0; i < BPB_SecPerClus; i++)//��һ���صķ�Χ�ڲ��ҿ��
     {
       		ReadBlock(DirStart1 + i);
		for(m = 0; m < 16; m++)
		{
                  if(FileNameCmp((unsigned char *)&FileDir.FileName,&BUFFER[m * 32])) return 0xffff;//�ļ����������˳�
                    
			if((BUFFER[m * 32] == 0) || (BUFFER[m * 32] == 0xe5))
                        {
                        //  PutChar(BUFFER[m * 32]);
				return ID;
                        }       
			else
				ID++;
		}
     }
     //��һ���صķ�Χ��û���ҵ����е���ȥ����һ�ء�
     uint16 NextClus=SerchClusChain(CurrentClus);
     if(NextClus !=0xffff)
     {
       CurrentClus=NextClus;
       DirStart1 = ClusConvLBA(CurrentClus) ; //�ҵ��ôص��������š�
         goto contserch;
     }
     else//��������Ŀ¼��û�ѵ���Ҫ������һ���طŵ������ϡ�
     {
     uint16  CurrentClus1= GetNextFAT();//�õ�һ���մء�
      WriteFAT(CurrentClus1, 0xffff);//��ռ�ϸ���;
      WriteFAT2(CurrentClus1, 0xffff);//��ռ�ϸ���;
       
       WriteFAT(CurrentClus, CurrentClus1);//�Ѵ������������Ա����Ŵ������ҡ�
      WriteFAT2(CurrentClus, CurrentClus1);
       //��ո�������´ض�Ӧ�������ռ��Ա�����µ�FDT��
       for (i = 0; i<512;i++)
        BUFFER[i] = 0;
      DirStart1 = ClusConvLBA(CurrentClus);  //�ҵ��ôص��������š�
        
      for(i = 0; i < BPB_SecPerClus; i++)//��һ���صķ�Χ�������������
        WriteBlock(DirStart1 + i);
      return ID++;
        
     } 
        
     
   } 
}



//********************************************************************************************
//��ȡһ���յ�FAT��
uint16 GetNextFAT(void)
//********************************************************************************************
{
  /*
	uint16 FATSectors, i,j;
        FATSectors =BPB_FATSz16;

        for (i = 0; i <FATSectors;i++)
        {
          ReadFATSector(i);
	for(j = 0; j < 512; j++)  //�������FAT ����
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
//��ȡ��Ŀ¼��ָ����
void ReadDIR(uint16 Index, DIR* Value)
//********************************************************************************************
{
	uint32 DirStart = DirStartSec();
	ReadBlock(DirStart + Index / 16);
	CopyBytes(&BUFFER[(Index % 16) * 32], Value, 32);
}

//********************************************************************************************
//д��Ŀ¼��ָ����
//ͬʱд��Ŀ¼��ָ����                               
void WriteDIR(uint16 Index, DIR* Value)
//********************************************************************************************
{
	uint32 LBA = DirStartSec() + Index / 16;
	ReadBlock(LBA);
	CopyBytes(Value, &BUFFER[(Index % 16) * 32], 32);
	WriteBlock(LBA);
}
//********************************************************************************************
//����һ�����ļ�
//˵��:FileName     �ļ������������ֻ��ߴ�д��ĸ����չ��Ҳ�����

//�ļ���С���Ȳ�������,��Ϊ0;
//ԭ������û��ͬ�����������������Ҽ��ϡ�
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
        if(!FormatFileName(filename)) return 0;//�ļ���ʽ����        
        
	//ClusNum = Size / (BPB_SecPerClus * 512) + 1;
	EmptyBytes(&FileDir, sizeof(DIR));
        CopyBytes(filename, &FileDir.FileName, 11);
        
        FileDir.FileAttrib = 0x20;
      
	FileDir.FilePosit.Size = 0;
	FileDir.FilePosit.Start = GetNextFAT();
        
	ClusID = FileDir.FilePosit.Start;
        FreeDirEnty=GetEmptyDIR( FileDir);
        if(FreeDirEnty !=0xffff) //�п������������
        { 
           WriteFAT(ClusID, 0xffff);
           WriteFAT2(ClusID, 0xffff);
           WriteDIR(FreeDirEnty, &FileDir);
           return  1;//no err
        } 
        else
        {
           WriteFAT(ClusID, 0x0000); //�黹�Ѿ�����վ�µĴ�
           WriteFAT2(ClusID, 0x0000);          
        //  PutStr("file not creat");
          return 0;
          
        }  
          

}
//********************************************************************************************
//�ڵ�ǰĿ¼�½�����Ŀ¼
//ϵͳά��һ����ǰĿ¼
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
        if(!FormatFileName(filename)) return 0;//�ļ���ʽ����
	//ClusNum = Size / (BPB_SecPerClus * 512) + 1;
	EmptyBytes(&FileDir, sizeof(DIR));
        CopyBytes(filename, &FileDir.FileName, 11);
        
        FileDir.FileAttrib = 0x10;//Ŀ¼����
      
	FileDir.FilePosit.Size = 0;
	FileDir.FilePosit.Start = GetNextFAT();
        ClusID = FileDir.FilePosit.Start;
           WriteFAT(ClusID, 0xffff);
           WriteFAT2(ClusID, 0xffff);
        

        FreeDirEnty=GetEmptyDIR( FileDir);
        //PutChar((uint16)FreeDirEnty);
        if(FreeDirEnty !=0xffff) //�п������������
        { 

           WriteDIR(FreeDirEnty, &FileDir);
           
           //����Ҫ���½��������Ŀ¼��Ŀ¼�� ��FDT����ա������롣 ����Ŀ¼��
           DirStartSector= ClusConvLBA( ClusID);

       for (i = 0; i<512;i++)
        BUFFER[i] = 0;
           
      for(i = 0; i < BPB_SecPerClus; i++)//��һ���صķ�Χ�������������
        WriteBlock(DirStartSector + i);
        //���½���Ŀ¼��߼��� �� ����Ŀ¼�   
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
//�����ļ������,ʹ��ͱ���һ��
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
//�����ļ�������
//˵����Write       1ʱ��д����,0Ϊ������
//      Name        �ļ���֧�����ֺʹ�д��ĸ
//      Start       ��ʼ�ֽ�
//      Length      ��������
//      Data        ��������
void OperateFile(uint8 Write ,uint8 Name[11], uint32 Start, uint32 Length, void* Data)
//********************************************************************************************
{
	uint8 *data = Data;
	uint16 BytePerClus, SecPerClus, ClusNum, ClusNext,ClusID,ClusIDValue ,m,Index;
	uint32 LBA, i,length;
	DIR FileDir;
	SecPerClus = BPB_SecPerClus;
	BytePerClus = BPB_SecPerClus * 512; // ÿ�ص��ֽ���
	Index = GetFileID(Name, &FileDir);
        length = Length;

	//���㿪ʼλ�����ڴصĴغ�
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

	//���㿪ʼλ��������������ƫ��
	i = (Start % BytePerClus) / 512;//��λ������

	//���㿪ʼλ��������ƫ��
	m = (Start % BytePerClus) % 512;//��λ��������ƫ��

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

				//�����ȡ��ɾ��˳�
				if(--Length == 0)
				{
					if(Write)
                                        {
                                                //��д����
						WriteBlock(LBA); 
                                               //��дĿ¼
                                                FileDir.FilePosit.Size = Start+length;
                                                WriteDIR(Index,&FileDir);
                                        }
					return;
				}
			}
			if(Write)
				WriteBlock(LBA++); //��д����,ָ������
		}
	}
//}
//********************************************************************************************
//��ʽ��
//�������������ϵĸ�ʽ����ֻ�������Ŀ¼�Լ�FAT��
void FormatCard(void)
//********************************************************************************************
{
    uint16 i,DirStart,DirSecNum,FatStart,FatSecNum;
   
    ReadBPB();
    //�����Ŀ¼
    for(i = 0; i <= 512;i++ )BUFFER[i]=0;
    DirSecNum = GetDirSecCount();
    DirStart = DirStartSec();
    for(i=0;i<DirSecNum;i++)
    {
      WriteBlock(DirStart+i);
    }
    
    //���FAT��
    FatStart = BPB_RsvdSecCnt;
    FatSecNum = BPB_FATSz16;
    for(i=0;i<FatSecNum;i++)
    {
      WriteBlock(FatStart+i);
    }
    WriteFAT(0,0xf8ff);
    WriteFAT(1,0xffff);
    
    //����FAT��
    CopyFAT();
}

//********************************************************************************************
//---------------------------------------------------------------------------------------------------------------------------------
        
        
//���Ҵ���
//����  ά��������                             
//���룺����һ���ز�������һ������FAT���λ�á�
                               
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
  if (i ==0) return 0 ;//��Ч�ļ�����
  for (j=i;j <11;j++)
  {
    filename[j]=0x20;
  }
  return 1;//�ļ����Ϸ���
}






//ɾ���ļ� ɾ����ǰĿ¼��ĳ�ļ���
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
        if(!FormatFileName(filename)) return  0xffff;//�ļ���ʽ����         
        
      Index = GetFileID(filename,&FileDir);
      if(Index ==0xffff)return  0xffff;// �ļ�����������ɾ����
      if(!(FileDir.FileAttrib &0x20)) return  0xffff;//�ļ��Ǹ�Ŀ¼����ɾ��ֻ����Ŀ¼ɾ�����
      FileDir.FileName.NAME[0] = 0xE5; 
      ClusID = FileDir.FilePosit.Start;
      
      uint16 tempClusID = SerchClusChain(ClusID);
      
      while(tempClusID !=0xffff)//ɾ���ļ���ռ�õĴ�����
      {
        WriteFAT(tempClusID,0x0000);
        WriteFAT2(tempClusID,0x0000);
        tempClusID = SerchClusChain(tempClusID);
      }
      WriteFAT(ClusID,0x0000); //���״�Ҳɾ��
      WriteFAT2(ClusID,0x0000);

      WriteDIR(Index,&FileDir);
 return DELSUCCESS;     
}



//ɾ����ǰĿ¼�µ�Ŀ¼
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
        if(!FormatFileName(filename)) return  -1;//�ļ���ʽ����         
        
      Index = GetFileID(filename,&FileDir);
      if(Index ==0xffff) return  ERR;//��ǰĿ¼û������ļ���Ŀ¼��
      if(!(FileDir.FileAttrib &0x10)) return  -1;//�ļ�����ͨ�ļ�����ɾ��ֻ�����ļ�ɾ������ɾ����
      FileDir.FileName.NAME[0] = 0xE5; 
      ClusID = FileDir.FilePosit.Start;
      //������Ŀ¼��FDT��鿴��Ŀ¼���Ƿ��ǿյĻ����ѱ�ɾ�����ļ�
      //����ȡ���ôص���ʼ����
     uint32 DirStart1 = ClusConvLBA(ClusID);  //�ҵ��ôص��������š�
     NextClusID=ClusID;
   contserch: 
     for(i = 0; i < BPB_SecPerClus; i++)//��һ���صķ�Χ�ڲ��ҿ��
     {
       		ReadBlock(DirStart1 + i);
		for(m = 0; m < 16; m++)
		{
 
                    
			if( (BUFFER[m * 32] != 0xe5)&&(BUFFER[m * 32] != 0x2e)&&(BUFFER[m * 32] != 0))
                        {

				return -2;//Ҫɾ����Ŀ¼����ûɾ�����ļ�
                        }       

		}
     }
     //��һ���صķ�Χ��û���ҵ���ռ�õ���ȥ����һ�ء�
      NextClusID=SerchClusChain(NextClusID);
     if(NextClusID !=0xffff)
     {
       
       DirStart1 = ClusConvLBA(NextClusID) ; //�ҵ��ôص��������š�
         goto contserch;
     }
     else//����Ŀ¼�ǿյĿ���ɾ����
     {
       tempClusID = SerchClusChain(ClusID);
      
      while(tempClusID !=0xffff)//ɾ���ļ���ռ�õĴ�����
      {
        WriteFAT(tempClusID,0x0000);
        WriteFAT2(tempClusID,0x0000);
        tempClusID = SerchClusChain(tempClusID);
      }
      WriteFAT(ClusID,0x0000); //���״�Ҳɾ��
      WriteFAT2(ClusID,0x0000);
      WriteDIR(Index,&FileDir);        
      return DELSUCCESS; 
     }  
        

}




//********************************************************************************************
//��ú��ļ�����Ӧ��Ŀ¼���Լ��ڸ�Ŀ¼��λ��
uint16 GetFileID(uint8 Name[11], DIR* ID)
//********************************************************************************************
//********************************************************************************************
{
	uint16 DirSecCut, DirStart, i, m, Index=0;
        
 if(CURPATH.CurrentDirStarClusID ==0) //��ʾ��ǰΪ��Ŀ¼��
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
				return Index; //�ҵ���Ӧ��Ŀ¼��,����Ŀ¼��λ��.
			}
		}
	}
        mmc_GoIdle();
	return 0xffff;
    }
   else//��������Ŀ¼�½��в��ҡ�
   {
     //���ݵ�ǰĿ¼�Ĵ����֪��Ŀ¼��FDT�����ڴ�
     uint16 CurrentClus = CURPATH.CurrentDirStarClusID;
     uint32 DirStart1 = ClusConvLBA(CurrentClus);  //�ҵ��ôص��������š�
   contserch: 
     for(i = 0; i < BPB_SecPerClus; i++)//��һ���صķ�Χ�ڲ��ҿ��
     {
       		ReadBlock(DirStart1 + i);
		for(m = 0; m < 16; m++)
		{
 			if(IsEqual(Name, &((DIR*)&BUFFER[m * 32])->FileName, 11))
			{
				*ID = *((DIR*)&BUFFER[m * 32]);
                                Index += (i*16+m);
                                mmc_GoIdle();
				return Index; //�ҵ���Ӧ��Ŀ¼��,����Ŀ¼��λ��.
			}
			
				
		}
     }
     //��һ���صķ�Χ��û���ҵ�ȥ����һ�ء�
     uint16 NextClus=SerchClusChain(CurrentClus);
     if(NextClus !=0xffff)
     {
       CurrentClus=NextClus;
       DirStart1 = ClusConvLBA(CurrentClus) ; //�ҵ��ôص��������š�
         goto contserch;
     }
     else//��������Ŀ¼��û�ѵ���
     {  
      mmc_GoIdle(); 
      return 0xffff;//û�ҵ�
     }
        
     
   } 
}



//��ʾ��ǰ�ļ��б�

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
 if(CURPATH.CurrentDirStarClusID ==0) //��ʾ��ǰΪ��Ŀ¼��
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
                    
                   //��ӡ����Ļ���ļ����֡�
                               
                      uint8 *p =&BUFFER[m * 32];
                    Init_Uart1;  
                    PutStr1("<FILE>");  
                    for (j=0;j<11;j++)
                    putchar1(*p++);
                    PutStr1("</FILE>");
                    putchar1(0x0d);
                    putchar1(0x0a);
                  } 
                  //�����������Ŀ¼�б�д���ļ���

		
		}
	}
        Init_Uart1;
        PutStr1("/FILELIST");
	return ;
    }
   else//��������Ŀ¼�½��в��ҡ�
   {
     //���ݵ�ǰĿ¼�Ĵ����֪��Ŀ¼��FDT�����ڴ�
     uint16 CurrentClus = CURPATH.CurrentDirStarClusID;
     uint32 DirStart1 = ClusConvLBA(CurrentClus);  //�ҵ��ôص��������š�
   contserch: 
     for(i = 0; i < BPB_SecPerClus; i++)//��һ���صķ�Χ�ڲ��ҿ��
     {

                initSPI1(0x02);
              
       		ReadBlock(DirStart1 + i);
		for(m = 0; m < 16; m++)
		{
                  //��ӡ����Ļ���ļ����֡�
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
                 //�����������Ŀ¼�б�д���ļ���
                Delay_ms(2); 
				
		}
           Delay_ms(2);     
         
     }
     //��һ���صķ�Χ��û���ҵ�ȥ����һ�ء�

     initSPI1(0x02);
   
     uint16 NextClus=SerchClusChain(CurrentClus);
     if(NextClus !=0xffff)
     {
       CurrentClus=NextClus;
       DirStart1 = ClusConvLBA(CurrentClus) ; //�ҵ��ôص��������š�
         goto contserch;
     }
     else//��������Ŀ¼��û�ѵ���
     { 
       Init_Uart1;
      PutStr1("</FILELIST>"); 
      return ;//û�ҵ�
     } 
        
     
   }

}



//�ı䵱ǰĿ¼
uint8 ChgDir(uint8 Name[11])
{
      DIR FileDir;
      uint16 Index;
        uint8 filename[11];
        uint8 i,j;
        uint8 *p;
        if(Name[0]!=0x2e)//����Ŀ¼����
        {  
          for (i =0;i<11;i++)
          {
            filename[i]=*Name++;
          }
          if(!FormatFileName(filename)) return  0xff;//�ļ���ʽ����         
          
          Index = GetFileID(filename,&FileDir);  
          if(Index ==0xffff) return  0xff;//Ŀ¼�����ڣ�
          if(FileDir.FileAttrib &0x20) return  0xff;//����Ŀ¼�Ǹ��ļ�
          
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
        else //���Ŀ¼����
        {
          for (i =0;i<11;i++)
          {
            filename[i]=*Name++;
          }
          if(!FormatFileName(filename)) return  0xff;//�ļ���ʽ���� 
         Index = GetFileID(filename,&FileDir);  
          if(Index ==0xffff) return  0xff;
          if(FileDir.FileAttrib &0x20) return  0xff;//����Ŀ¼�Ǹ��ļ�
          
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


 //��ȡһ���صĿ�ʼ������
uint32 GetClusStarSector(uint16 ClusID)
{
return  ClusConvLBA(ClusID);
}


//��FDT��ID����Ϊ����д�޸ĺ���ļ�Ŀ¼����Ӧ����
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
  if(CURPATH.CurrentDirStarClusID==0)//�����ڸ�Ŀ¼�½��в���
  {
    	
	StarSector = DirStartSec()+SectorOffsetInClus;
      ReadBlock(StarSector);
       *((DIR*)&BUFFER[DirOffsetInSector*32])=*FileDir;
      WriteBlock(StarSector);
     return;        
        
  }
  else if(CURPATH.CurrentDirStarClusID !=0)//��������Ŀ¼�½��в���
 ClusID =CURPATH.CurrentDirStarClusID;
 //�ӱ�Ŀ¼���״غſ�ʼ���Ҹ�Ŀ¼�����ڵĴغţ�
 if(CluseNum==0)
 {
   //���Ŀ¼������״����أ�
  StarSector =GetClusStarSector(ClusID)+SectorOffsetInClus;//�ҵ���Ӧ������
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
 StarSector = GetClusStarSector(ClusID)+SectorOffsetInClus;//�ҵ���Ӧ������
  ReadBlock(StarSector);
  *(DIR*)BUFFER[DirOffsetInSector*32]=*FileDir;
  WriteBlock(StarSector);   
 }
 
}






//��һ���ļ�׷�����ݡ�
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
          if(!FormatFileName(filename)) return  0xff;//�ļ���ʽ����         
          
          Index = GetFileID(filename,&FileDir);  
          if(Index ==0xffff) return  0xff;//û�ҵ�ָ���ļ���
          if(FileDir.FileAttrib &0x10) return  0xff;//����Ŀ¼�Ǹ��ļ�
                    
          ClusID=FileDir.FilePosit.Start; 
          //seek���ļ���ĩβ��
          
          SeekPos =FileDir.FilePosit.Size;
           tempClusID=LastClusID =NextClusID=ClusID;
         // tempClusID=NextClusID =SerchClusChain(ClusID);
          next:
           if(NextClusID !=0xffff)//�ҵ�ĩβ��
           {
             tempClusID=NextClusID;
             NextClusID=SerchClusChain(tempClusID);
             goto next;
           }
           else
           {
             //�ҵ���ĩβ�أ�
             LastClusID =tempClusID ;
           }
           InClusOffset = SeekPos %(BPB_SecPerClus*BPB_BytesPerSec);//�ҵ����ļ���ĩ�˵Ĵ���ƫ�ơ�
           SectorOffset = InClusOffset /BPB_BytesPerSec;//�ҵ�������������ƫ�ơ�
           InSectorOffset=InClusOffset %BPB_BytesPerSec;//�ҵ����ļ���ĩ�˵�������ƫ�ơ�
           
           
          //�����ص�������������
           LastSector =GetClusStarSector(LastClusID)+SectorOffset;
           
            ReadBlock(LastSector);   
            if((BPB_BytesPerSec -InSectorOffset )>=length)//��Ҫд����ֽ�û�г������һ��������ʣ��ռ�
            { 
            memcpy(&BUFFER[InSectorOffset],pSource,length);
            pSource+=length;
            WriteBlock(LastSector); 
            FileDir.FilePosit.Size+=length;
              
              //�����dir��size��ˢ�º�Ϳ����˳��ˡ�
            ModfyFDT( Index, &FileDir) ; 
            }
            else
            { //��Ҫд����ֽڳ������һ��������ʣ��ռ��������ݰ������������
              memcpy(&BUFFER[InSectorOffset],pSource,BPB_BytesPerSec -InSectorOffset);
            pSource+=(BPB_BytesPerSec -InSectorOffset);
            WriteBlock(LastSector); 
            FileDir.FilePosit.Size+=BPB_BytesPerSec -InSectorOffset; 
            length-= (BPB_BytesPerSec -InSectorOffset); 
            LastSector++;
            conti:
              while(SectorOffset <BPB_SecPerClus-1)//�ôػ��п����������ʹ�ã�
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
                  else//ʣ�µ������Ѿ�����һ�������ˡ�
                  {
            memcpy(BUFFER,pSource,length );
            pSource+=length;
            WriteBlock(LastSector); 
            FileDir.FilePosit.Size+=length ; 
            //������Ŀ¼��Ϳ����˳��ˡ�
            ModfyFDT( Index, &FileDir) ;
            mmc_GoIdle();
              return 1;      
                  }  
                  
               
              }
                //�ô�ռ���˸�������һ�����ˡ�
             
              LastClusID= GetNextFAT();
              WriteFAT(LastClusID, 0xFFFF);//ռ������õĴ�
              WriteFAT2(LastClusID, 0xFFFF);
        
              WriteFAT(tempClusID,LastClusID);//�Ѵ����ν��ϡ�
              WriteFAT2(tempClusID,LastClusID);
              SectorOffset = 0;
              //��ø�����Ĵص��������š�
              LastSector = GetClusStarSector(LastClusID);
              goto conti;
            
            } 
        mmc_GoIdle();            
        return 1;           
        }


//����ǰĿ¼��ָ���ļ���ָ��λ�õ�ָ�����ȵ����ݿ顣

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
          if(!FormatFileName(filename)) return  0xff;//�ļ���ʽ����         
          
          Index = GetFileID(filename,&FileDir);  
          if(Index ==0xffff) return  0xff;//û�ҵ�ָ���ļ���
          if(FileDir.FileAttrib &0x10) return  0xff;//����Ŀ¼�Ǹ��ļ�
                    
          ClusID=FileDir.FilePosit.Start; 
          //seek���ļ���ĩβ��
          
          SeekPos =FileDir.FilePosit.Size;
          if((Position+length) >SeekPos)//�����ļ��߽磻
            return 0xff;

           LastClusID =ClusID;
         // tempClusID=NextClusID =SerchClusChain(ClusID);

           InClusOffset = Position %(BPB_SecPerClus*BPB_BytesPerSec);//�ҵ��ļ�ָ��λ�õĴ���ƫ�ơ�
           SectorOffset = InClusOffset /BPB_BytesPerSec;//�ҵ��ļ�ָ��λ�õĴ�������ƫ�ơ�
           InSectorOffset=InClusOffset %BPB_BytesPerSec;//�ҵ��ļ�ָ��λ�õ�������ƫ�ơ�
           
           
          //��ָ��λ������������
           LastSector =GetClusStarSector(LastClusID)+SectorOffset;
           
            ReadBlock(LastSector);   
            if((BPB_BytesPerSec -InSectorOffset )>=length)//��Ҫ�������ֽ�û�г������һ��������ʣ��ռ�
            { 
            memcpy(pDist,&BUFFER[InSectorOffset],length);
            pDist+=length;


            }
            else
            { //��Ҫ�������ֽڳ������һ��������ʣ��ռ���Ѵ�����ʣ������ݶ�����
              memcpy(pDist,&BUFFER[InSectorOffset],BPB_BytesPerSec -InSectorOffset);
            pDist+=(BPB_BytesPerSec -InSectorOffset);
 
            length-= (BPB_BytesPerSec -InSectorOffset); 
            LastSector++;//׼������һ������
            conti:
              while(SectorOffset <BPB_SecPerClus-1)//�ôػ��п����������ʹ�ã�
              {
             
                  if(length >BPB_BytesPerSec)
                  {
                    ReadBlock(LastSector++);        
                    memcpy(pDist,BUFFER,BPB_BytesPerSec );
                    pDist+=BPB_BytesPerSec;
                    
                    length-= BPB_BytesPerSec ;  
                    
                    SectorOffset++;
                    
                  }
                  else//ʣ�µ������Ѿ�����һ�������ˡ�
                  {
                    ReadBlock(LastSector);
                    memcpy(pDist,BUFFER,length );
                    pDist+=length;
                    
                    //������Ŀ¼��Ϳ����˳��ˡ�
                    ModfyFDT( Index, &FileDir) ; 
                    mmc_GoIdle();
                    return 1 ;      
                  }  
                  
               
              }
                //�ô�ռ���˸ö���һ�����ˡ�
             
              LastClusID= SerchClusChain(LastClusID);

              SectorOffset = 0;
              //��ø�����Ĵص��������š�
              LastSector = GetClusStarSector(LastClusID);
              goto conti;
            
            }  
 mmc_GoIdle();               
return 1;  
}




//�ı�ָ���ļ���ָ����������ݡ������ı��ļ��Ĵ�Сֻ�Ǹ���ԭ�е�����
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
          if(!FormatFileName(filename)) return  0xff;//�ļ���ʽ����         
          
          Index = GetFileID(filename,&FileDir);  
          if(Index ==0xffff) return  0xff;//û�ҵ�ָ���ļ���
          if(FileDir.FileAttrib &0x10) return  0xff;//����Ŀ¼�Ǹ��ļ�
                    
          ClusID=FileDir.FilePosit.Start; 
          //seek���ļ���ĩβ��
          
          SeekPos =FileDir.FilePosit.Size;
          if((Position+length)>SeekPos) return 0xff;
           LastClusID =ClusID;
         // tempClusID=NextClusID =SerchClusChain(ClusID);

           InClusOffset = Position %(BPB_SecPerClus*BPB_BytesPerSec);//�ҵ��ļ�ָ��λ�õĴ���ƫ�ơ�
           SectorOffset = InClusOffset /BPB_BytesPerSec;//�ҵ��ļ�ָ��λ�õĴ�������ƫ�ơ�
           InSectorOffset=InClusOffset %BPB_BytesPerSec;//�ҵ��ļ�ָ��λ�õ�������ƫ�ơ�
          
          //��ָ��λ�õ�������
           LastSector =GetClusStarSector(LastClusID)+SectorOffset;
           
            ReadBlock(LastSector);   
            if((BPB_BytesPerSec -InSectorOffset )>=length)//��Ҫд����ֽ�û�г������һ��������ʣ��ռ�
            { 
            memcpy(&BUFFER[InSectorOffset],pSource,length);
            pSource+=length;
            WriteBlock(LastSector); 
 
            }
            else
            { //��Ҫд����ֽڳ������һ��������ʣ��ռ��������ݰ������������
              memcpy(&BUFFER[InSectorOffset],pSource,BPB_BytesPerSec -InSectorOffset);
            pSource+=(BPB_BytesPerSec -InSectorOffset);
            WriteBlock(LastSector); 
 
            length-= (BPB_BytesPerSec -InSectorOffset); 
            LastSector++;
            conti:
              while(SectorOffset <BPB_SecPerClus-1)//�ôػ��п����������ʹ�ã�
              {
             
                  if(length >BPB_BytesPerSec)
                  {
            memcpy(BUFFER,pSource,BPB_BytesPerSec );
            pSource+=BPB_BytesPerSec;
            WriteBlock(LastSector++); 

            length-= BPB_BytesPerSec ;  

            SectorOffset++;
                    
                  }
                  else//ʣ�µ������Ѿ�����һ�������ˡ�
                  {
            memcpy(BUFFER,pSource,length );
            pSource+=length;
            WriteBlock(LastSector); 

              return 1;      
                  }  
                  
               
              }
                //�ô�ռ���˸øö���һ�����ˡ�
             
              LastClusID= SerchClusChain(LastClusID);

              SectorOffset = 0;
              //��ø��ѵĴص��������š�
              LastSector = GetClusStarSector(LastClusID);
              goto conti;
            
            }  
        return 1;           
        }

//��ȡ�ļ��й����� 
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
//��Ŀ¼�б��ӡ������׼���õĿ��ļ���
void ListFileToFile(void)
{
  uint8 buf[16*13];
  uint8 *p1 =buf;
  uint16 length = 0;
	uint16 DirSecCut, DirStart, i,j, m ;
        
 if(CURPATH.CurrentDirStarClusID ==0) //��ʾ��ǰΪ��Ŀ¼��
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
                    
                   //��ӡ����Ļ���ļ����֡�
                               
                      uint8 *p =&BUFFER[m * 32];
                    for (j=0;j<11;j++)
                    *p1++ = *p++;
                    *p1++ = 0x0d;
                    *p1++ = 0x0a;
                    length += 13;
                  } 
                  //�����������Ŀ¼�б�д���ļ���
                  AppendData("FILELIST",buf,(uint32)length);
		
		}
	}
	return ;
    }
   else//��������Ŀ¼�½��в��ҡ�
   {
     //���ݵ�ǰĿ¼�Ĵ����֪��Ŀ¼��FDT�����ڴ�
     uint16 CurrentClus = CURPATH.CurrentDirStarClusID;
     uint32 DirStart1 = ClusConvLBA(CurrentClus);  //�ҵ��ôص��������š�
   contserch: 
     for(i = 0; i < BPB_SecPerClus; i++)//��һ���صķ�Χ�ڲ��ҿ��
     {
       		ReadBlock(DirStart1 + i);
		for(m = 0; m < 16; m++)
		{
                  if( (BUFFER[m * 32] != 0xe5)&&(BUFFER[m * 32] != 0x2e)&&(BUFFER[m * 32] != 0))                  
                  {
                    
                   //��ӡ����Ļ���ļ����֡�
                               
                      uint8 *p =&BUFFER[m * 32];
                    for (j=0;j<11;j++)
                    *p1++ = *p++;
                    *p1++ = 0x0d;
                    *p1++ = 0x0a;
                    length += 13;
                  } 
                  //�����������Ŀ¼�б�д���ļ���
                  AppendData("FILELIST",buf,(uint32)length);
				
		}
     }
     //��һ���صķ�Χ��û���ҵ�ȥ����һ�ء�
     uint16 NextClus=SerchClusChain(CurrentClus);
     if(NextClus !=0xffff)
     {
       CurrentClus=NextClus;
       DirStart1 = ClusConvLBA(CurrentClus) ; //�ҵ��ôص��������š�
         goto contserch;
     }
     else//��������Ŀ¼��û�ѵ���
       
      return ;//û�ҵ�
        
     
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
  if(FindFile(FileName)==0)//û�ҵ��ļ��ͽ���֮
  { 
  CreateFile(FileName);
  }
  
  AppendData(FileName,pSource,  length);
  
}

unchar FindFile(unchar *FileName)
{
	uint16 DirSecCut, DirStart, i, m ;

 if(CURPATH.CurrentDirStarClusID ==0) //��ʾ��ǰΪ��Ŀ¼��
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
                    
                    return 1;//�ҵ���ָ���ļ�
                  } 
                 

		
		}
	}

	return 0;//δ�ҵ�ָ�����ļ�
    }
   else//��������Ŀ¼�½��в��ҡ�
   {
     //���ݵ�ǰĿ¼�Ĵ����֪��Ŀ¼��FDT�����ڴ�
     uint16 CurrentClus = CURPATH.CurrentDirStarClusID;
     uint32 DirStart1 = ClusConvLBA(CurrentClus);  //�ҵ��ôص��������š�
   contserch: 
     for(i = 0; i < BPB_SecPerClus; i++)//��һ���صķ�Χ�ڲ��ҿ��
     {
       		ReadBlock(DirStart1 + i);
		for(m = 0; m < 16; m++)
		{
                  //��ӡ����Ļ���ļ����֡�
                 if(IsEqual(FileName, &((DIR*)&BUFFER[m * 32])->FileName, 11))
                 {
                  return 1;//�ҵ���ָ���ļ�
                   
                 }

				
		}
     }
     //��һ���صķ�Χ��û���ҵ�ȥ����һ�ء�
     uint16 NextClus=SerchClusChain(CurrentClus);
     if(NextClus !=0xffff)
     {
       CurrentClus=NextClus;
       DirStart1 = ClusConvLBA(CurrentClus) ; //�ҵ��ôص��������š�
         goto contserch;
     }
     else//��������Ŀ¼��û�ѵ���
     {  
      PutStr1("/FILELIST"); 
      return 0;//û�ҵ�
     } 
        
     
   }

}
