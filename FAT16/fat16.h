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


//��ʼ��FAT��ص����ݽṹ��
FAT_EXT		void InitFAT16(void );
//********************************************************************************************
//��һ������
FAT_EXT		void ReadBlock(uint32 LBA);
//********************************************************************************************
//дһ������
FAT_EXT		void WriteBlock(uint32 LBA);
//********************************************************************************************
FAT_EXT		void CopyBytes(void* S, void* D, uint16 size);
//********************************************************************************************
FAT_EXT		uint8 IsEqual(void* A, void* B, uint8 Size);
//********************************************************************************************
FAT_EXT		void EmptyBytes(void* D, uint16 size);
//********************************************************************************************
//дBPB���ݽṹ
FAT_EXT		void FATInit(void);
//********************************************************************************************
//��ȡBPB���ݽṹ
FAT_EXT		void ReadBPB(void);
//********************************************************************************************
//��ȡFAT��ʼ������
FAT_EXT		uint32 GetFATStartSec(void);
//********************************************************************************************
//��ȡ��Ŀ¼��ʼ������
FAT_EXT		uint32 DirStartSec(void);
//********************************************************************************************
//��ȡ��������ʼ������
FAT_EXT		uint32 DataStartSec(void);
//********************************************************************************************
//Ŀ¼��ռ�õ�������
FAT_EXT		uint16 GetDirSecCount(void);
//********************************************************************************************
//��ȡһ���صĿ�ʼ����
FAT_EXT		uint32 ClusConvLBA(uint16 ClusID);
//********************************************************************************************
//��ȡ�ļ�������ָ����
FAT_EXT		uint16 ReadFAT(uint16 Index);
//********************************************************************************************
//д�ļ�������ָ����
FAT_EXT		void WriteFAT(uint16 Index, uint16 Value);
//********************************************************************************************
//д�����ļ������
FAT_EXT		void WriteFAT2(uint16 Index, uint16 Value);
//********************************************************************************************
//��ȡ��Ŀ¼�п���ʹ�õ�һ��
FAT_EXT		uint16 GetEmptyDIR(DIR FileDir);
//********************************************************************************************
//��ú��ļ�����Ӧ��Ŀ¼��
FAT_EXT	 	uint16 GetFileID(uint8 Name[11], DIR* ID);
//********************************************************************************************
//��ȡһ���յ�FAT��
FAT_EXT		uint16 GetNextFAT(void);
//********************************************************************************************
//��ȡ��Ŀ¼��ָ����
FAT_EXT		void ReadDIR(uint16 Index, DIR* Value);
//********************************************************************************************
//д��Ŀ¼��ָ����
FAT_EXT		void WriteDIR(uint16 Index, DIR* Value);
//********************************************************************************************
//����һ�����ļ�
FAT_EXT		unsigned char  CreateFile(uint8 FileName[11]);
//********************************************************************************************
//�����ļ������,ʹ��ͱ���һ��
FAT_EXT		void CopyFAT(void);
//********************************************************************************************
//�����ļ�������
FAT_EXT		void OperateFile(uint8 Write ,uint8 Name[11], uint32 Start, uint32 Length, void* Data);
//********************************************************************************************
//ɾ���ļ�
FAT_EXT		unsigned int  DelFile(uint8 Name[11]);
//********************************************************************************************
//��ʽ��
FAT_EXT		void FormatCard(void);
//********************************************************************************************
//---------------------------------------------------------------------------------------------------------------------------------

//�Ƚ��ļ���
FAT_EXT		unsigned char  FileNameCmp(unsigned char   *pFileNameCreat,unsigned char   *pFileNameExist);



//����Ŀ¼��
FAT_EXT		unsigned char   CreateDir(uint8 *FileName);

//��ʽ���ļ�����
FAT_EXT		unsigned char  FormatFileName(unsigned char  *filename);



//��һ���ļ�׷�����ݡ�
FAT_EXT	 	uint8 AppendData(uint8 *FileName, uint8 *pSource, uint32 length);



//����ǰĿ¼��ָ���ļ���ָ��λ�õ�ָ�����ȵ����ݿ顣

FAT_EXT		uint8 ReadFile(uint8 *FileName, uint8 *pDist, uint32 Position,uint32 length);


//�ı�ָ���ļ���ָ����������ݡ������ı��ļ��Ĵ�Сֻ�Ǹ���ԭ�е�����
FAT_EXT		uint8 ModifyFile(uint8 *FileName, uint8 *pSource,uint32 Position, uint32 length);


//��ȡ�ļ��й����� 
//****************************************************

FAT_EXT		unsigned long GetFileLength(unsigned char *FileName);


//******************************************************8
// send a file to uart 
FAT_EXT		void SendFileToUart(unsigned char *FileName);



FAT_EXT		void RecvFileFromUart(void);

FAT_EXT         void ListFile(void);

//*****************************
//��Ŀ¼�б��ӡ������׼���õĿ��ļ���
FAT_EXT		void ListFileToFile(void);


FAT_EXT         signed int  DelDir(uint8 Name[11]);

FAT_EXT         uint8 ChgDir(uint8 Name[11]);
FAT_EXT         unchar FindFile(unchar *FileName);
FAT_EXT         void RcvFileFromUart(unchar *FileName,uint8 *pSource,uint32 length);
FAT_EXT         unchar ReName(unchar *pSourceFileName,unchar*pNewFileName);

