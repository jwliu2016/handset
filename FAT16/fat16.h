#ifdef  FAT_GLOBALS
#define	 FAT_EXT
#else
#define  FAT_EXT	extern
#endif


#define DELSUCCESS 0
#define ERR        -1
#define MAX_PATH_SIZE     30



FAT_EXT		uint8  BUFFER[512];
FAT_EXT		uint16 BPB_BytesPerSec;
FAT_EXT		uint8 BPB_SecPerClus;
FAT_EXT		uint16 BPB_RsvdSecCnt;
FAT_EXT		uint8 BPB_NumFATs;
FAT_EXT		uint16 BPB_RootEntCnt;
FAT_EXT		uint16 BPB_TotSec16;
FAT_EXT		uint16 BPB_FATSz16;
FAT_EXT		uint32 BPB_HiddSec;

FAT_EXT		uint32 FileSize;





#pragma pack(1)

typedef struct
{
	uint8 BS_jmpBoot[3];
	uint8 BS_OEMName[8];
	uint16 BPB_BytesPerSec;
	uint8 BPB_SecPerClus;
	uint16 BPB_RsvdSecCnt;
	uint8 BPB_NumFATs;
	uint16 BPB_RootEntCnt;
	uint16 BPB_TotSec16;
	uint8 BPB_Media;
	uint16 BPB_FATSz16;
	uint16 BPB_SecPerTrk;
	uint16 BPB_NumHeads;
	uint32 BPB_HiddSec;
	uint32 BPB_TotSec32;
	uint8 BS_DrvNum;
	uint8 BS_Reservedl;
	uint8 BS_BootSig;
	uint32 BS_VolID;
	uint8 BS_VolLab[11];
	uint8 BS_FilSysType[8];
	uint8 ExecutableCode[448];
	uint8 ExecutableMarker[2];
} FAT_BPB;
typedef struct
{
	uint8 NAME[8];
	uint8 TYPE[3];
} FILE_NAME;
typedef struct
{
	uint16 Start;
	uint32 Size;
} FILE_POSIT;
typedef struct
{
	FILE_NAME FileName;
	uint8 FileAttrib;
	uint8 UnUsed[10];
	uint8 FileUpdateTime[2];
	uint8 FileUpdateData[2];
	FILE_POSIT FilePosit;
} DIR;
typedef struct
{
	uint16 ClusID;
	uint16 SecOfClus;
	uint16 ByteOfSec;
} DATA_POSIT;

typedef struct 
{
  uint16 CurrentDirStarClusID;
  uint16 FatherDirStarClusID;
  unsigned char *pPath;
  unsigned char Path[MAX_PATH_SIZE];
} PATH;
#pragma pack()
FAT_EXT         unchar PromptBuffer[6];
FAT_EXT		PATH  CURPATH;





FAT_EXT		uint16     SerchClusChain(uint16 CurrentClus); 


//初始化FAT相关的数据结构。
FAT_EXT		void InitFAT16(void );
//********************************************************************************************
//读一个扇区
FAT_EXT		void ReadBlock(uint32 LBA);
//********************************************************************************************
//写一个扇区
FAT_EXT		void WriteBlock(uint32 LBA);
//********************************************************************************************
FAT_EXT		void CopyBytes(void* S, void* D, uint16 size);
//********************************************************************************************
FAT_EXT		uint8 IsEqual(void* A, void* B, uint8 Size);
//********************************************************************************************
FAT_EXT		void EmptyBytes(void* D, uint16 size);
//********************************************************************************************
//写BPB数据结构
FAT_EXT		void FATInit(void);
//********************************************************************************************
//读取BPB数据结构
FAT_EXT		void ReadBPB(void);
//********************************************************************************************
//获取FAT表开始扇区号
FAT_EXT		uint32 GetFATStartSec(void);
//********************************************************************************************
//获取根目录开始扇区号
FAT_EXT		uint32 DirStartSec(void);
//********************************************************************************************
//获取数据区开始扇区号
FAT_EXT		uint32 DataStartSec(void);
//********************************************************************************************
//目录项占用的扇区数
FAT_EXT		uint16 GetDirSecCount(void);
//********************************************************************************************
//获取一个簇的开始扇区
FAT_EXT		uint32 ClusConvLBA(uint16 ClusID);
//********************************************************************************************
//读取文件分配表的指定项
FAT_EXT		uint16 ReadFAT(uint16 Index);
//********************************************************************************************
//写文件分配表的指定项
FAT_EXT		void WriteFAT(uint16 Index, uint16 Value);
//********************************************************************************************
//写备份文件分配表
FAT_EXT		void WriteFAT2(uint16 Index, uint16 Value);
//********************************************************************************************
//获取根目录中可以使用的一项
FAT_EXT		uint16 GetEmptyDIR(DIR FileDir);
//********************************************************************************************
//获得和文件名对应的目录项
FAT_EXT	 	uint16 GetFileID(uint8 Name[11], DIR* ID);
//********************************************************************************************
//获取一个空的FAT项
FAT_EXT		uint16 GetNextFAT(void);
//********************************************************************************************
//读取根目录的指定项
FAT_EXT		void ReadDIR(uint16 Index, DIR* Value);
//********************************************************************************************
//写根目录的指定项
FAT_EXT		void WriteDIR(uint16 Index, DIR* Value);
//********************************************************************************************
//创建一个空文件
FAT_EXT		unsigned char  CreateFile(uint8 FileName[11]);
//********************************************************************************************
//复制文件分配表,使其和备份一致
FAT_EXT		void CopyFAT(void);
//********************************************************************************************
//操作文件的数据
FAT_EXT		void OperateFile(uint8 Write ,uint8 Name[11], uint32 Start, uint32 Length, void* Data);
//********************************************************************************************
//删除文件
FAT_EXT		unsigned int  DelFile(uint8 Name[11]);
//********************************************************************************************
//格式化
FAT_EXT		void FormatCard(void);
//********************************************************************************************
//---------------------------------------------------------------------------------------------------------------------------------

//比较文件名
FAT_EXT		unsigned char  FileNameCmp(unsigned char   *pFileNameCreat,unsigned char   *pFileNameExist);



//创建目录：
FAT_EXT		unsigned char   CreateDir(uint8 *FileName);

//格式化文件名；
FAT_EXT		unsigned char  FormatFileName(unsigned char  *filename);



//向一个文件追加数据。
FAT_EXT	 	uint8 AppendData(uint8 *FileName, uint8 *pSource, uint32 length);



//读当前目录下指定文件的指定位置的指定长度的数据块。

FAT_EXT		uint8 ReadFile(uint8 *FileName, uint8 *pDist, uint32 Position,uint32 length);


//改变指定文件的指定区域的数据。而不改变文件的大小只是覆盖原有的区域。
FAT_EXT		uint8 ModifyFile(uint8 *FileName, uint8 *pSource,uint32 Position, uint32 length);


//获取文件有关属性 
//****************************************************

FAT_EXT		unsigned long GetFileLength(unsigned char *FileName);


//******************************************************8
// send a file to uart 
FAT_EXT		void SendFileToUart(unsigned char *FileName);



FAT_EXT		void RecvFileFromUart(void);

FAT_EXT         void ListFile(void);

//*****************************
//把目录列表打印到事先准备好的空文件里
FAT_EXT		void ListFileToFile(void);


FAT_EXT         signed int  DelDir(uint8 Name[11]);

FAT_EXT         uint8 ChgDir(uint8 Name[11]);
FAT_EXT         unchar FindFile(unchar *FileName);
FAT_EXT         void RcvFileFromUart(unchar *FileName,uint8 *pSource,uint32 length);
FAT_EXT         unchar ReName(unchar *pSourceFileName,unchar*pNewFileName);

