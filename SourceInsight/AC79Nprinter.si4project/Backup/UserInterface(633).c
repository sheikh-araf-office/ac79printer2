
/**************************************************************************************************************************
	NeedlePrinter_AC7911B8_WIFI	SM720
**************************************************************************************************************************/

#define UsterInterfacec 0
/**************************************************************************************************************
	UsterInterface.c	UsterInterface.h
**************************************************************************************************************/

#include "UserInterface.h"


/**************************************************
	���ļ���ϵͳ����ȫ�ֱ������ж��庯��

**************************************************/


/**********		IO	**********/


/**************************************************
 ��������˿ڲ���ֱ�Ӷ�ȡ�˿�״̬����õ�ǰ״̬
 (���������ⲿ��·��·���߸��ع��ص��µ�ƽ״̬���ܿɿ�ʶ��)
 ��������˿ڲ��ܼ򵥶�ȡ�˿�״̬����õ�ǰ״̬
 (���������ⲿ��·���ŵ��²���ʶ����Ч�仯�������ǰ���)
**************************************************/
volatile unsigned char FlagIOStatus1 = 0xf8;
// 7:-	6:-		5:LedLine		4:LedErr		3:-	2:Power	10:-



volatile unsigned short FlagHeadIC = 0x0000;
// bit15-bit0:	Print head needle
// 03	15	00	04			08	12	10	06			02	14	13	01			09	05	07	11
// 04	16	01	05			09	13	11	07			03	15	14	02			10	06	08	12
/*#define Needle01 0x2000
#define Needle02 0x0010
#define Needle03 0x0080
#define Needle04 0x8000
#define Needle05 0x1000
#define Needle06 0x0004
#define Needle07 0x0100
#define Needle08 0x0002
#define Needle09 0x0800
#define Needle10 0x0008
#define Needle11 0x0200
#define Needle12 0x0001
#define Needle13 0x0400
#define Needle14 0x0020
#define Needle15 0x0040
#define Needle16 0x4000*/
volatile unsigned char HeadState = 0xff;
volatile unsigned char FlagPrinter = FlagPRTFast;
/*	#define FlagPRTFast		10	// 3/16��ʵ����,3/8���������
	#define FlagPRTStand	11	// 4/16��ʵ����,4/8���������
	//#define FlagPRTDesity	12	// 5/16��ʵ����,5/8���������
	#define FlagPRTDeep		21	// 5/16��ʵ����,5/8���������*/


// ����������
volatile unsigned char FlagMotorMode = 0x00;
/*#
define CRGLeftDir			(FlagMotorMode&0x80)
#define CRGLeftSet			FlagMotorMode |= 0x80;
#define CRGRightDir			((FlagMotorMode&0x80) == 0x00)
#define CRGRightSet			FlagMotorMode &= ~0x80;
#define PAPBackDir			(FlagMotorMode&0x40)
#define PAPBackSet			FlagMotorMode |= 0x40;
#define PAPFeedDir			((FlagMotorMode&0x40) == 0x00)
#define PAPFeedSet			FlagMotorMode &= ~0x40;
//#define PAPKong				(FlagMotorMode&0x20)
//#define PAPKongSet			FlagMotorMode |= 0x20;
//#define PAPPingtuiSet		FlagMotorMode &= ~0x20;
#define CMDOneDir			(FlagMotorMode&0x02)
#define CMDOneDirSet		FlagMotorMode |= 0x02;
#define CMDTwoDirSet		FlagMotorMode &= ~0x02;
#define TMPOneDir			(FlagMotorMode&0x01)
#define TMPOneDirSet		FlagMotorMode |= 0x01;
#define TMPTwoDirSet		FlagMotorMode &= ~0x01;
*/

volatile unsigned char FlagMotorIC = 0xff;
// bit7-bit0:	PapA	PapB	PapS	PapE	CrgA	CrgB	CrgS	CrgE
// Pap+Crg		phaseA	phaseB	1/3Current		2/3Current	same to Pap


volatile unsigned char PowerState = 0;//0xff;


// @20220518 ���򷵳̲���ӡ���Լӿ��ٶ�
const unsigned short TABLECRGSpeedAccDecOneDirNoprint[1+CRGVolatileStep] =
{
	13300,
	2700,	2301,	2014,	1798,	1631,
	1499,	1394,	1308,	1238,	1180,
	1133,	1095,	1064,	1040,	1022,
	1009,	1002,	1000
};
// @202205211958 �����ӡȫ���������ٱ�֤ƽ��
const unsigned short TABLECRGSpeedAccOneDirPrint[1+CRGVolatileStep] =
{
	13300,
	2770,	2397,	2121,	1910,	1745,
	1613,	1506,	1419,	1347,	1287,
	1239,	1199,	1167,	1142,	1123,
	1110,	1102,	1100
};
// �����������г���
const unsigned short TABLECRGSpeedDecOneDirPrint[1+CRGVolatileStep] =
{
	13300,
	2700,	1692,	1311,	1147,	1100,
	1100,	1100,	1100,	1100,	1100,
	1100,	1100,	1100,	1100,	1100,
	1100,	1100,	1100
};
const unsigned short TABLECRGSpeedRightAcc[1+CRGVolatileStep] =
{
	13300,
	2700,	1692,	1311,	1147,	1100,
	1100,	1100,	1100,	1100,	1100,
	1100,	1100,	1100,	1100,	1100,
	1100,	1100,	1100
};
volatile unsigned char 	CRGState				= 0xff;		// ���ڼ�¼��ʼ���ȸ���״̬��ȫ������״̬���ƽ����ź����ȴ� 0x55:��ʼ�����
volatile unsigned char	cCRGSpeed				= 0;		// �������ļӼ���ʹ��
volatile unsigned short	CRGStopTimePosition		= 0;		// ����ֹͣʱ��Ƭλ�ã�
volatile unsigned short	CRGNowTimePosition		= 0;		// �ֳ���ǰʱ��Ƭλ��

const unsigned short TABLEPAPSpeed[1+PAPVolatileStep] =
{
	13300,
	2320,	1831,	1549,	1377,	1273,
	1217,	1200,	1200,	1200,	1200,
	1200
};
volatile unsigned char PAPState 				= 0xff;
volatile unsigned char cPAPSpeed 				= 0;		// �������ļӼ���ʹ��
volatile unsigned short PAPMoveStep 			= 0;		// ��ֽ����  +��ǰ��ֽ  -�����ֽ  0ֹͣ




/**********		McuInt	**********/

// �������
volatile unsigned short ADCKEY;
// 0		1/4		1/2		3/4		4/4
// NO		*		KeyLine	*		KeyPaper
// ������ϵͳ�жϺ����ж�ȡ������״̬��¼�ڴˣ��Ա��жϰ���
// T = 50ms; 	1111 1111:H	111* *000:Down	0000 0000:L	000* *111:Up
// ʹ�ñ����¼�ʱ����Ҫ�ȴ����¼����������������һ��ɨ�������ܻ���Ӧ��θ��¼�
volatile unsigned char KeyPaperRecorder 	= 0x00;
volatile unsigned short  KeyPaperDownTime 	= 0;
volatile unsigned char KeyLineRecorder 		= 0x00;
volatile unsigned short  KeyLineDownTime 	= 0;

volatile u32 DelayTimeCount2mS = 0;
// ϵͳ��ʱ��Ҫ�ı���


volatile unsigned short  ADCTEMP;
// ��ӡͷ�¶�


/**********		Manager	**********/

const unsigned char TABLELOGO[] =
{
#if	(DebugAlkeLOGO == 0)
	"DebugVersion: Alke Printer AC7911B8",
#endif

#if	(DebugAlkeLOGO == 1)
	"Brank: SHININGER   Mode: SM690 AC7911B8",
#endif

/*	add another	*/

#if	(DebugAlkeLOGO == 255)
	"Brank: ALKE   Mode: SM690 WiIFI"
#endif
};

const unsigned char TABLESoftWare[] =
{
	"HardWare:SM720-M V10 20221127 \n",
	"SoftWare: NeedlePrinter_AC7911B8_WIFI \n"
};

volatile unsigned char FlagManager;
/*	#define FlagManagerNormal		0x00
	#define FlagManagerSeftPrint	0x01
	#define FlagManagerHeadDelay	0x02	// no sue
	#define FlagManagerSetMode		0x03	// no use
	#define FlagManagerHexPrint		0x04
	#define FlagManagerTestPrint	0x05	// TestPrint Mode
	#define FlagManagerQR1			0x10	// no use
	#define FlagManagerQR2			0x11	// no use
	#define FlagManagerCODE128		0x20	// no use
*/


// ͳһ������ջ�����������ʵ�ʹ�����С����̬�������Դ�С
volatile unsigned char BufferReceive[BufferReceiveSIZE] =
{
	0,0,0
};
struct sBufferReceive BufferReceiveUSB =
{
	BufferReceive,
	0,
	(BufferReceiveSIZE/3),
	(BufferReceiveSIZE/3)*9/10,
	(BufferReceiveSIZE/3)*1/10,
	0,
	0
};
struct sBufferReceive BufferReceiveWIFI =
{
	BufferReceive+BufferReceiveSIZE/3, //BufferReceiveUSB.Size
	0,
	//0,
	(BufferReceiveSIZE/3),
	(BufferReceiveSIZE/3)*9/10,
	(BufferReceiveSIZE/3)*1/10,
	0,
	0
};
struct sBufferReceive BufferReceiveBL =
{
	BufferReceive+BufferReceiveSIZE/3+BufferReceiveSIZE/3, // BufferReceiveUSB.Size+BufferReceiveWIFI.Size,
	0,
	//0,
	(BufferReceiveSIZE/3),
	(BufferReceiveSIZE/3)*9/10,
	(BufferReceiveSIZE/3)*1/10,
	0,
	0
};
/*	���ջ�������֯�ɻ��λ����������·��ž���ʾΪ�߼���ϵ��=:��� <:�ͺ�>:��ǰ	*/
// r=w:		empty
// r<w:		normal
// r=w+1:	full



volatile unsigned short  FlagSystem	= 0x0000;
//#define FlagSystemPrint				(FlagSystem&0x8000)	// Printing or not print when the CRG motor running(carrige moving)
//#define FlagSystemPrintSet			FlagSystem |= 0x8000;
//#define FlagSystemPrintClr			FlagSystem &= ~0x8000;
//#define FlagSystemBack				(FlagSystem&0x8000)
//#define FlagSystemFeedSet			FlagSystem |= 0x8000;
//#define FlagSystemBackSet			FlagSystem &= ~0x8000;
/*
#define FontLetterKT				(FlagSystem&0x0001)
#define FontLetterKTSet				FlagSystem |= 0x0001;
#define FontLetterKTClr				FlagSystem &= ~0x0001;
#define FontLetterLB				(FlagSystem&0x0002)
#define FontLetterLBSet				FlagSystem |= 0x0002;
#define FontLetterLBClr				FlagSystem &= ~0x0002;
#define FontLetterDW				(FlagSystem&0x0004)
#define FontLetterDWSet			FlagSystem |= 0x0004;
#define FontLetterDWClr			FlagSystem &= ~0x0004;
#define FontLetterDH				(FlagSystem&0x0008)
#define FontLetterDHSet				FlagSystem |= 0x0008;
#define FontLetterDHClr				FlagSystem &= ~0x0008;
#define FontChineseLB				(FlagSystem&0x0020)
#define FontChineseLBSet			FlagSystem |= 0x0020;
#define FontChineseLBClr			FlagSystem &= ~0x0020;
#define FontChineseDW				(FlagSystem&0x0040)
#define FontChineseDWSet			FlagSystem |= 0x0040;
#define FontChineseDWClr			FlagSystem &= ~0x0040;
#define FontChineseDH				(FlagSystem&0x0080)
#define FontChineseDHSet			FlagSystem |= 0x0080;
#define FontChineseDHClr			FlagSystem &= ~0x0080;
#define FontAlignCentre				((FlagSystem&0x0300) == 0x0000)
#define FontAlignCentreSet			FlagSystem &= ~0x0300;
#define FontAlignRight				((FlagSystem&0x0300) == 0x0100)
#define FontAlignRightSet			(FlagSystem &= ~0x0300; FlagSystem |= 0x0100;)
#define FontAlignLeft				((FlagSystem&0x0300) == 0x0200)
#define FontAlignLeftSet			(FlagSystem &= ~0x0300; FlagSystem |= 0x0200;)
#define FontRotateZero				((FlagSystem&0x0c00) == 0x0000)
#define FontRotateZeroSet			FlagSystem &= ~0x0c00;
#define FontRotateRight				((FlagSystem&0x0c00) == 0x0400)
#define FontRotateRightSet			(FlagSystem &= ~0x0c00; FlagSystem |= 0x0400;)
#define FontRotateLeft				((FlagSystem&0x0c00) == 0x0800)
#define FontRotateLeftSet			(FlagSystem &= ~0x0c00; FlagSystem |= 0x0800;)
#define FlagKeyPadNo				(FlagSystem&0x1000)
#define FlagKeyPadSet				FlagSystem |= 0x1000;
#define FlagKeyPadClr				FlagSystem &= ~0x1000;
*/

volatile unsigned char WifiState = 0xff;
volatile unsigned short DelaytimeForWifiState = 0;


volatile unsigned char SpaceBetweenLetter = 2;		// 0


// �˴�ӡ���������У�Ϊtimer�ж�����ֽ��ӡ�¼� �� ���յ����ݺ���������������������ͨѶ������ͨѶȫ�������û�������ͬʱ������ر�־λ����ɶ��ߵ�ͬ��Э������
// ��������ӡ�ٶ�Զ�����ݴ����ٶ������ʴ��յ�16�����ݾͿ�ʼ��ӡ��������������Ҫ�ȴ��϶����ݲſ�ʼ������ӡ
struct sBufferPrint BufferPrint[BufferPrintLineNum] =
{
	{{0,0,0},{0,0,0},	0,0,	0,0,},
	{{0,0,0},{0,0,0},	0,0,	0,0,},
	{{0,0,0},{0,0,0},	0,0,	0,0}
};
volatile unsigned int cBufferPrintLineWrite = 0;	// ��ӡ��������ǰд���м�¼
volatile unsigned int cBufferPrintDataWrite = 0;	// ��ӡ��������ǰд���е�����д��λ�ü�¼
volatile unsigned int cBufferPrintLineRead = 0;	// ��ӡ��������ǰ��ӡ�м�¼
volatile unsigned int cBufferPrintDataRead = 0;	// ��ӡ��������ǰ��ӡ�е����ݶ�ȡλ�ü�¼
// ��ӡ���̵�״̬��¼�������߼����ܵ� 0:��ʼ����׼�� 1: �����ֳ������ܻ��ӡ (2): �ȴ��ֳ����ܽ��� 3: �ȴ��ֳ�����ӡ���� 4: ������ֽ����˶� 5: �ȴ���ֽ���
// enum {sStart, sStartCRGMotor, sWaitingCRGMotor, sWaitingHeadPrinting, sStartPAPMotor, sWaitingPAPMotor};
volatile unsigned char PrintingState = sINIT;

unsigned short PaperCount;
// 0x00: no paper	0x1*: ǰ��ֽģʽ	0x2*: ���ֽģʽ
volatile unsigned short  PaperState		= 0x00;
volatile unsigned short  PaperPageLenth	= 1580;
volatile unsigned char SensorRecorderFrontOut;
volatile unsigned char SensorRecorderFrontIn;
volatile unsigned char SensorRecorderBackOut;
volatile unsigned char SensorRecorderBackIn;


volatile unsigned char StepBetweenLine			= 24;	// 24/144=1/6Ӣ��
volatile unsigned char FlagPositionCut 			= 0;		// 1:�ϴ�Ϊ˺��ֽ������ֽ
// ǰ��ֽĬ��297mm(2105Step A4)�����ֽĬ��12.7*22=279.4mm(90Step*22=1980Step 180DPI=90Step/�׾�12.7mm)��
volatile unsigned short  StepPageLenth			= 1584;	// StepBetweenLine*PageLineNumINIT
volatile unsigned short  StepPageCount			= 0;		// 0~PageLineNum-1
volatile unsigned short  StepPapCountTail		= PaperPositionSlowINIT;

volatile unsigned short  HeadDelayCount;
volatile unsigned short  PageLineNum = PageLineNumINIT;					// ����	ÿ����24������ҳ��
volatile unsigned short  PaperPositionFirstPrint = PaperPositionFirstPrintINIT;	// ��ʼ��ӡλ��
volatile unsigned short  PaperPositionCut = PaperPositionCutINIT;		// ˺ֽλ��
volatile unsigned short  PaperPositionOut = PaperPositionOutINIT;		// ˺ֽ��λ��
volatile unsigned short  AdcPaperLever = ADCPAPERLEVERINIT;				// ֽ���ż���ѹ��SM880��5��������
volatile unsigned short  AdcTempLever = ADCTEMPLEVERINIT;				// ��ӡͷ�¶ȴ�����

/**************************************************************************************************************
	UsterInterface.c	UsterInterface.h
**************************************************************************************************************/

struct sCommand Command = {0,0,0};



//#define McuIOc 0
/**************************************************************************************************************
	McuIO.c	McuIO.h
**************************************************************************************************************/

// IC�ϵ��ʼ����Ҫ�ʵ���ʱ���˺�����Ϊ��������Ѿ����
// ���ز���Ϊ0ʱ����ʾ������IC�˿����������
unsigned char MotorIC(void)
{
	static unsigned char MotorICState = 1; // 0: Normal	!0: init or Err
	unsigned char c8c1 = 0;

	if(MotorICState == 0)
	{
		for(c8c1 = 0x80; c8c1; c8c1 >>= 1)
		{
			if(FlagMotorIC&c8c1)
				MtDiHigh;
			else
				MtDiLow;
			MtSckHigh;
			MtSckLow;
		}
		MtRckHigh;
		MtRckLow;
	}

	else if(MotorICState == 1)
	{
		MtClrLow;
		MtGoutHigh;

		MtDiLow;
		MtSckLow;
		MtRckLow;

		MotorICState++;
	}
	else if(MotorICState == 2) // shift in 0*8
	{
		for(c8c1 = 0; c8c1 < 8; c8c1++)
		{
			MtSckHigh;
			//MtDiLow;
			MtSckLow;
			//MtDiLow;
		}
		MtRckHigh;
		MtRckLow;

		MtGoutLow;
		MtClrHigh;

		MotorICState++;
		//MotorICState = 0;
	}
	else if(MotorICState < 10) // ��������ʱѭ��������������ʱ���̾����ڲ���ϵͳʱ��Ƭ����
	{
		MotorICState++;
	}
	else
	{
		MotorICState = 0;
	}

	return MotorICState;
}

// 0: close	1:stand	2:enable
void CRGCurrent(unsigned char CrgState)
{
	if(CrgState == 2)
		FlagMotorIC &= ~0x03;
	else if(CrgState == 1)
	{
		FlagMotorIC &= ~0x03;
		FlagMotorIC |= 0x01;
	}
	else
		FlagMotorIC |= 0x03;

	MotorIC();
}
void CRGPhase(void)
{
	static unsigned char FlagCRGPhaseCount = 2;

	if(CRGRightDir)
	{
		if(FlagCRGPhaseCount)
			FlagCRGPhaseCount--;
		else
			FlagCRGPhaseCount = 3;
	}
	else
	{
		if(FlagCRGPhaseCount < 3)
			FlagCRGPhaseCount++;
		else
			FlagCRGPhaseCount = 0;
	}

	if(FlagCRGPhaseCount == 0)
	{
		FlagMotorIC &= ~0x0c;
	}
	else if(FlagCRGPhaseCount == 1)
	{
		FlagMotorIC &= ~0x0c;
		FlagMotorIC |= 0x04;
	}
	else if(FlagCRGPhaseCount == 2)
	{
		FlagMotorIC |= 0x0c;
	}
	else// if(FlagCRGPhaseCount == 3)
	{
		FlagMotorIC &= ~0x0c;
		FlagMotorIC |= 0x08;
	}

	MotorIC();
}
// 0: close	1:stand	2:enable
void PAPCurrent(unsigned char PapState)
{
	if(PapState == 2)
		FlagMotorIC &= ~0x30;
	else if(PapState == 1)
	{
		FlagMotorIC &= ~0x30;
		FlagMotorIC |= 0x10;
	}
	else
		FlagMotorIC |= 0x30;

	MotorIC();
}
void PAPPhase(void)
{
	static unsigned char FlagPAPPhaseCount = 2;

	if(PAPFeedDir)
	{
		if(FlagPAPPhaseCount)
			FlagPAPPhaseCount--;
		else
			FlagPAPPhaseCount = 3;
	}
	else
	{
		if(FlagPAPPhaseCount < 3)
			FlagPAPPhaseCount++;
		else
			FlagPAPPhaseCount = 0;
	}

	if(FlagPAPPhaseCount == 0)
	{
		FlagMotorIC &= ~0xc0;
	}
	else if(FlagPAPPhaseCount == 1)
	{
		FlagMotorIC &= ~0xc0;
		FlagMotorIC |= 0x40;
	}
	else if(FlagPAPPhaseCount == 2)
	{
		FlagMotorIC |= 0xc0;
	}
	else// if(FlagPAPPhaseCount == 3)
	{
		FlagMotorIC &= ~0xc0;
		FlagMotorIC |= 0x80;
	}

	MotorIC();
}

unsigned char HeadIC(void)
{
	static unsigned char HeadICState = 1; // 0: Normal	!0: init or Err
	unsigned short  c8c1;

	if(HeadICState == 0)
	{
		for(c8c1 = 0x8000; c8c1; c8c1 >>= 1)
		{
			if(FlagHeadIC&c8c1)
				HdDiHigh;
			else
				HdDiLow;
			HdSckHigh;
			HdSckLow;
		}
		HdRckHigh;
		HdRckLow;
	}

	else if(HeadICState == 1)
	{
		HdClrLow;
		HdGoutHigh;

		HdDiLow;
		HdSckLow;
		HdRckLow;

		HeadICState++;
	}
	else if(HeadICState == 2) // shift in 0*16
	{
		for(c8c1 = 0; c8c1 < 16; c8c1++)
		{
			HdSckHigh;
			//HdDiLow;
			HdSckLow;
			//HdDiLow;
		}
		HdRckHigh;
		HdRckLow;

		HdGoutLow;
		HdClrHigh;

		HeadICState++;
	}
	else if(HeadICState < 10) // ��������ʱѭ��������������ʱ���̾����ڲ���ϵͳʱ��Ƭ����
	{
		HeadICState++;
	}
	else
	{
		HeadICState = 0;
	}

	return HeadICState;
}
/*	@@@@@@@@@@@@@@@@@@@�˴�����ΪӲ���ײ���������@@@@@@@@@@@@@@@@@@	*/
/*	����������	*/

//	USART1 always working
void OutputByteData(unsigned char ByteData)	// ����Ҳ����ͨ��USB��������
{
	if(DebugMsg)
	{
		//putbyte(ByteData);
		printf("%c", ByteData);
		printf("%x", ByteData);
	}
}

void SendString(unsigned char *p)
{
	if(DebugMsg)
	{
		while(*p)
		{
			//putbyte(*p);
			printf("%c", *p);
			printf("%x", *p);
			p++;
		}
	}
}


// return 0x00:	-----	no key
//		0x01:	--|__	key down
//		0x02:	_____	resevered
//		0x03:	__|--	key up
//		0x0f:	*****	key *




volatile unsigned char BufferPrintLineWrited(void)
{
	if(cBufferPrintLineRead <= cBufferPrintLineWrite)
		return cBufferPrintLineWrite-cBufferPrintLineRead;
	else
		return BufferPrintLineNum+cBufferPrintLineWrite-cBufferPrintLineRead;
}






#define Driverc 0
/**************************************************************************************************************
	Driver.c	Driver.h
**************************************************************************************************************/






/**************************************************************************************************************
	Driver.c	Driver.h
**************************************************************************************************************/







#define Applitionc 0
/**************************************************************************************************************
	Applition.c	Applition.h
**************************************************************************************************************/
void SendCmd(unsigned char CMD1, unsigned char CMD2)
{
	if(DebugMsg)
	{
		//put_u8hex(CMD1); // user_put_u8hex(CMD1);
		//put_u8hex(CMD2); // user_put_u8hex(CMD2);

		printf("%c", CMD1);
		printf("%c", CMD1);

		printf("%x", CMD1);
		printf("%x", CMD1);

		printf(" \r\n");
	}
}

/**************************************************************************************************************
	Applition.c	Applition.h
**************************************************************************************************************/



/**************************************************************************************************************
	UserInterface.c	UserInterface.h
**************************************************************************************************************/



