
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
volatile unsigned char FlagPrinter = eFlagPRTFast;
/*	#define eFlagPRTFast		10	// 3/16��ʵ����,3/8���������
	#define eFlagPRTStand		11	// 4/16��ʵ����,4/8���������
	//#define eFlagPRTDesity	12	// 5/16��ʵ����,5/8���������
	#define eFlagPRTDeep		21	// 5/16��ʵ����,5/8���������*/


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
volatile unsigned char PAPState 				= 0xff;		// 0x55: ��������λ�л�
volatile unsigned char cPAPSpeed 				= 0;		// �������ļӼ���ʹ��
volatile unsigned short PAPMoveStep 			= 0;		// ��ֽ����  +��ǰ��ֽ  -�����ֽ  0ֹͣ


volatile unsigned char Debugc8 = 0;



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


#if 0
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
#endif


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
// enum {sINIT, sStartCRGMotor, sWaitingCRGMotor, sWaitingHeadPrinting};
volatile unsigned char PrintingState = sINIT;

unsigned short PaperCount;
// 0x00: no paper	0x1*: ǰ��ֽģʽ	0x2*: ���ֽģʽ
volatile unsigned short  PaperState		= 0xf0;		// 0x00: ǰ����ֽ 0xf0: ��⵽ǰ��ֽ(��ֽ�������ٴ���ֽ) 0x0f: ��⵽���ֽ 0xff: ǰ����ֽ����Ϊ���ֽ
volatile unsigned short  PaperPageLenth	= 1580;
volatile unsigned char SensorRecorderFrontOut = 0x00;
volatile unsigned char SensorRecorderFrontIn = 0x00;
volatile unsigned char SensorRecorderBackOut = 0x00;
volatile unsigned char SensorRecorderBackIn = 0x00;


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



#define McuIOc 0
/**************************************************************************************************************
	McuIO.c	McuIO.h
**************************************************************************************************************/

// IC�ϵ��ʼ����Ҫ�ʵ���ʱ���˺�����Ϊ��������Ѿ����
// ���ز���Ϊ0ʱ����ʾ������IC�˿����������
void MotorIC(void)
{
	u8 c8c1;

	MtClrHigh;
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

/*unsigned char MotorIC(void)
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
}*/

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

	if(0)//if(PAPFeedDir)
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
void HeadIC(void)
{
	u16  c16c1;

	HdClrHigh;
	for(c16c1 = 0x8000; c16c1; c16c1 >>= 1)
	{
		if(FlagHeadIC&c16c1)
			HdDiHigh;
		else
			HdDiLow;
		HdSckHigh;
		HdSckLow;
	}
	HdRckHigh;
	HdRckLow;
}

/*unsigned char HeadIC(void)
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

		HeadICState++;
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
}*/

/**************************************************************************************************************
	McuIO.c	McuIO.h
**************************************************************************************************************/

/*	@@@@@@@@@@@@@@@@@@@�˴�����ΪӲ���ײ���������@@@@@@@@@@@@@@@@@@	*/
/*	����������	*/















#define Driverc 0
/**************************************************************************************************************
	Driver.c	Driver.h
**************************************************************************************************************/

volatile unsigned char BufferPrintLineWrited(void)
{
	if(cBufferPrintLineRead <= cBufferPrintLineWrite)
		return cBufferPrintLineWrite-cBufferPrintLineRead;
	else
		return BufferPrintLineNum+cBufferPrintLineWrite-cBufferPrintLineRead;
}





/**************************************************************************************************************
	Driver.c	Driver.h
**************************************************************************************************************/







#define Applitionc 0
/**************************************************************************************************************
	Applition.c	Applition.h
**************************************************************************************************************/


#if 1
 void InitialIO(void)
 {
	gpio_set_die(IO_PORTC_01,1);		// 1: gpio 0: another function, for examge adc
	gpio_set_pull_up(IO_PORTC_01,1);	// 1��up resister
	gpio_set_pull_down(IO_PORTC_01, 0);	// 1��down resister
	gpio_set_direction(IO_PORTC_01,0);	// 0: output mode 1:input mode

	gpio_set_die(IO_PORTA_00,1);
	gpio_set_pull_up(IO_PORTA_00,1);
	gpio_set_pull_down(IO_PORTA_00, 0);
	gpio_set_direction(IO_PORTA_00,0);

	gpio_set_die(IO_PORTA_01,1);
	gpio_set_pull_up(IO_PORTA_01,1);
	gpio_set_pull_down(IO_PORTA_01, 0);
	gpio_set_direction(IO_PORTA_01,0);

	gpio_set_die(IO_PORTA_02,1);
	gpio_set_pull_up(IO_PORTA_02,1);
	gpio_set_pull_down(IO_PORTA_02, 0);
	gpio_set_direction(IO_PORTA_02,0);

	gpio_set_die(IO_PORTA_03,1);
	gpio_set_pull_up(IO_PORTA_03,1);
	gpio_set_pull_down(IO_PORTA_03, 0);
	gpio_set_direction(IO_PORTA_03,0);

	gpio_set_die(IO_PORTA_04,1);
	gpio_set_pull_up(IO_PORTA_04,1);
	gpio_set_pull_down(IO_PORTA_04, 0);
	gpio_set_direction(IO_PORTA_04,0);

	gpio_set_die(IO_PORTA_05,1);
	gpio_set_pull_up(IO_PORTA_05,1);
	gpio_set_pull_down(IO_PORTA_05, 0);
	gpio_set_direction(IO_PORTA_05,0);

	gpio_set_die(IO_PORTA_06,1);
	gpio_set_pull_up(IO_PORTA_06,1);
	gpio_set_pull_down(IO_PORTA_06, 0);
	gpio_set_direction(IO_PORTA_06,0);

	gpio_set_die(IO_PORTA_07, 0);
	gpio_set_direction(IO_PORTA_07, 1);  //������Ϊ����
    gpio_set_pull_up(IO_PORTA_07, 0);    //������10K
    gpio_set_pull_down(IO_PORTA_07, 0);  //������10K

	gpio_set_die(IO_PORTA_08,1);
	gpio_set_pull_up(IO_PORTA_08,1);
	gpio_set_pull_down(IO_PORTA_08, 0);
	gpio_set_direction(IO_PORTA_08,0);

	gpio_set_die(IO_PORTA_09,1);
	gpio_set_pull_up(IO_PORTA_09,1);
	gpio_set_pull_down(IO_PORTA_09, 0);
	gpio_set_direction(IO_PORTA_09,0);

	gpio_set_die(IO_PORTA_10,1);
	gpio_set_pull_up(IO_PORTA_10,1);
	gpio_set_pull_down(IO_PORTA_10, 0);
	gpio_set_direction(IO_PORTA_10,0);

	gpio_set_die(IO_PORTB_01,1);
	gpio_set_pull_up(IO_PORTB_01,1);
	gpio_set_pull_down(IO_PORTB_01, 0);
	gpio_set_direction(IO_PORTB_01,1);

	gpio_set_die(IO_PORTB_05,1);
	gpio_set_pull_up(IO_PORTB_05,1);
	gpio_set_pull_down(IO_PORTB_05, 0);
	gpio_set_direction(IO_PORTB_05,0);

	// FrontOut
	gpio_set_die(IO_PORTB_07, 0);
	gpio_set_pull_up(IO_PORTB_07, 0);
	gpio_set_pull_down(IO_PORTB_07, 0);
	gpio_set_direction(IO_PORTB_07, 1);


	// BackOut
 	gpio_set_die(IO_PORTC_00, 0);
	gpio_set_pull_up(IO_PORTC_00, 0);
	gpio_set_pull_down(IO_PORTC_00, 0);
	gpio_set_direction(IO_PORTC_00, 1);

	// BackIn
 	gpio_set_die(IO_PORTC_01, 0);
	gpio_set_pull_up(IO_PORTC_01, 0);
	gpio_set_pull_down(IO_PORTC_01, 0);
	gpio_set_direction(IO_PORTC_01, 1);






	gpio_set_die(IO_PORTC_02,1);
	gpio_set_pull_up(IO_PORTC_02,0);
	gpio_set_pull_down(IO_PORTC_02, 1);
	gpio_set_direction(IO_PORTC_02,0);

	gpio_set_die(IO_PORTC_05,1);
	gpio_set_pull_up(IO_PORTC_05,0);
	gpio_set_pull_down(IO_PORTC_05, 1);
	gpio_set_direction(IO_PORTC_05,0);

	gpio_set_die(IO_PORTC_06,1);
	gpio_set_pull_up(IO_PORTC_06,1);
	gpio_set_pull_down(IO_PORTC_06, 0);
	gpio_set_direction(IO_PORTC_06,0);

	gpio_set_die(IO_PORTC_07,1);
	gpio_set_pull_up(IO_PORTC_07,1);
	gpio_set_pull_down(IO_PORTC_07, 0);
	gpio_set_direction(IO_PORTC_07,0);

	// HOME
	gpio_set_die(IO_PORTC_08,1);
	gpio_set_pull_up(IO_PORTC_08,1);
	gpio_set_pull_down(IO_PORTC_08, 0);
	gpio_set_direction(IO_PORTC_08,1);

	// FrontIn
	gpio_set_die(IO_PORTC_09,0);
	gpio_set_pull_up(IO_PORTC_09,0);
	gpio_set_pull_down(IO_PORTC_09, 0);
	gpio_set_direction(IO_PORTC_09,1);

	// KeyAdc for test
	gpio_set_die(IO_PORTC_10, 0);
	gpio_set_pull_up(IO_PORTC_10, 0);
	gpio_set_pull_down(IO_PORTC_10, 1);
	gpio_set_direction(IO_PORTC_10, 1);

 	MtClrLow;
	MtGoutLow;
	MtSckLow;
	MtRckLow;

 	HdClrLow;
	HdGoutLow;
	HdSckLow;
	HdRckLow;


 /*gpio_set_die(IO_PORTB_07, 0);
 gpio_set_pull_up(IO_PORTB_07, 0);
 gpio_set_pull_down(IO_PORTB_07, 0);
 gpio_set_direction(IO_PORTB_07, 1);

 gpio_set_die(IO_PORTC_00, 0);
 gpio_set_pull_up(IO_PORTC_00, 0);
 gpio_set_pull_down(IO_PORTC_00, 0);
 gpio_set_direction(IO_PORTC_00, 1);

 gpio_set_die(IO_PORTC_01, 0);
 gpio_set_pull_up(IO_PORTC_01, 0);
 gpio_set_pull_down(IO_PORTC_01, 0);
 gpio_set_direction(IO_PORTC_01, 1);

 gpio_set_die(IO_PORTC_09, 0);
 gpio_set_pull_up(IO_PORTC_09, 0);
 gpio_set_pull_down(IO_PORTC_09, 0);
 gpio_set_direction(IO_PORTC_09, 1);*/

 }



#define AWINLINE   __attribute__((always_inline))
#define TSEC SEC_USED(.volatile_ram_code)
 #if 1
static SEC_USED(.volatile_ram) JL_TIMER_TypeDef *TMR2 = JL_TIMER2;
static SEC_USED(.volatile_ram) u8 timer_irq2 = IRQ_TIMER2_IDX;
static SEC_USED(.volatile_ram) u32 timer_clk2 = 0;
static AWINLINE TSEC void timer2_cfg(u32 freq, u32 us)
{
    u8 timer_index[16] =  {0, 4, 1, 5, 2,  6,  3,  7,   8,   12,  9,    13,   10,   14,   11,    15};
    u32 timer_table[16] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768};
    u32 clock = timer_clk2;
    u8 psc = 0;
    u8 tmp = 0;
    u32 prd = 0;
    u32 ts = us / (1000 * 1000);
    u32 tu = us % (1000 * 1000);
    u8 i;
    float tp = 0;

    if (freq >= clock) {
        freq = clock;
    } else if (freq <= 1) {
        freq = 1;
        if (ts) {
            tp = (float)tu / (1000 * 1000);
        }
    }
    prd = clock / freq;
    if (prd > 65535) {
        for (psc = 0; psc < 16; psc++) {
            prd = (u32)(clock / (timer_table[psc]) / freq);
            if (prd <= 65535) {
                break;
            } else if (psc == 15) {
                prd = 65535;
                break;
            }
        }
    }
    prd = ts ? (prd * ts + tp * prd) : prd;
    psc = timer_index[psc];
    TMR2->CON = 0;
    TMR2->CNT = 0;
    TMR2->CON |= BIT(14);
    TMR2->PRD = prd;
    TMR2->CON |= psc << 4;
    TMR2->CON |= BIT(0);
}
int tt2;
static void TSEC timer_set2(u32 us);
static ___interrupt TSEC void timer_isr2(void)
{
	unsigned long c32 = 500000;

	if(TMR2->CON & BIT(15))
    {
		TMR2->CON |= BIT(14);
	#if 0
		if(USBin)
		{
			if(LedLine)
				LedLineOpen
			else
				LedLineClose
		}
		else
			LedLineClose
	#endif
	#if 1
		Tim_Sys_Handler();
	#endif

		/******************
		if(FlagHeadIC == 0xaaaa)
			FlagHeadIC = 0x5555;
		else
			FlagHeadIC = 0xaaaa;
		HeadIC();******************/

		timer_set2(1000*20);// 1000*20=20ms 2000us=2ms
    }
}
// u32 us: <5000000//5s
static void TSEC timer_set2(u32 us)
{
    u32 freq = 1000000 / us;
    timer2_cfg(freq, us);
}
static void TSEC timer_freq_set2(u32 freq)
{
    timer2_cfg(freq, 0);
}
//example test
static int Timer2Initial(void)
{
    timer_clk2 = clk_get("timer");
    request_irq(timer_irq2, 3, timer_isr2, 0);
    timer_set2(2000);
    return 0;
}
#endif
 #if 1
static SEC_USED(.volatile_ram) JL_TIMER_TypeDef *TMR3 = JL_TIMER3;
static SEC_USED(.volatile_ram) u8 timer_irq3 = IRQ_TIMER3_IDX;
static SEC_USED(.volatile_ram) u32 timer_clk3 = 0;
static AWINLINE TSEC void timer3_cfg(u32 freq, u32 us)
{
    u8 timer_index[16] =  {0, 4, 1, 5, 2,  6,  3,  7,   8,   12,  9,    13,   10,   14,   11,    15};
    u32 timer_table[16] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768};
    u32 clock = timer_clk3;
    u8 psc = 0;
    u8 tmp = 0;
    u32 prd = 0;
    u32 ts = us / (1000 * 1000);
    u32 tu = us % (1000 * 1000);
    u8 i;
    float tp = 0;

    if (freq >= clock) {
        freq = clock;
    } else if (freq <= 1) {
        freq = 1;
        if (ts) {
            tp = (float)tu / (1000 * 1000);
        }
    }
    prd = clock / freq;
    if (prd > 65535) {
        for (psc = 0; psc < 16; psc++) {
            prd = (u32)(clock / (timer_table[psc]) / freq);
            if (prd <= 65535) {
                break;
            } else if (psc == 15) {
                prd = 65535;
                break;
            }
        }
    }
    prd = ts ? (prd * ts + tp * prd) : prd;
    psc = timer_index[psc];
    TMR3->CON = 0;
    TMR3->CNT = 0;
    TMR3->CON |= BIT(14);
    TMR3->PRD = prd;
    TMR3->CON |= psc << 4;
    TMR3->CON |= BIT(0);
}

int tt3;
static void TSEC timer_set3(u32 us);
static ___interrupt TSEC void timer_isr3(void)
{
	unsigned long c32 = 500000;

	if(TMR3->CON & BIT(15))
    {
		TMR3->CON |= BIT(14);


	#if 1
		c32 = Tim_PAP_Handler();
		timer_set3(c32);
	#else
		c32 = TimPos_CRG_Handler();
		timer_set3(c32);
	#endif
    }
}
// u32 us: <5000000//5s
static void TSEC timer_set3(u32 us)
{
    u32 freq = 1000000 / us;
    timer3_cfg(freq, us);
}
static void TSEC timer_freq_set3(u32 freq)
{
    timer3_cfg(freq, 0);
}

//example test
static int Timer3Initial(void)
{
    timer_clk3 = clk_get("timer");
    request_irq(timer_irq3, 3, timer_isr3, 0);
    timer_set3(3300); //1000=1ms
    return 0;
}
#endif
#if 1
static SEC_USED(.volatile_ram) JL_TIMER_TypeDef *TMR = JL_TIMER4;
static SEC_USED(.volatile_ram) u8 timer_irq4 = IRQ_TIMER4_IDX;
static SEC_USED(.volatile_ram) u32 timer_clk4 = 0;
#define AWINLINE   __attribute__((always_inline))
#define TSEC SEC_USED(.volatile_ram_code)
static AWINLINE TSEC void timer_cfg(u32 freq, u32 us)
{
    u8 timer_index[16] =  {0, 4, 1, 5, 2,  6,  3,  7,   8,   12,  9,    13,   10,   14,   11,    15};
    u32 timer_table[16] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768};
    u32 clock = timer_clk4;
    u8 psc = 0;
    u8 tmp = 0;
    u32 prd = 0;
    u32 ts = us / (1000 * 1000);
    u32 tu = us % (1000 * 1000);
    u8 i;
    float tp = 0;

    if (freq >= clock) {
        freq = clock;
    } else if (freq <= 1) {
        freq = 1;
        if (ts) {
            tp = (float)tu / (1000 * 1000);
        }
    }
    prd = clock / freq;
    if (prd > 65535) {
        for (psc = 0; psc < 16; psc++) {
            prd = (u32)(clock / (timer_table[psc]) / freq);
            if (prd <= 65535) {
                break;
            } else if (psc == 15) {
                prd = 65535;
                break;
            }
        }
    }
    prd = ts ? (prd * ts + tp * prd) : prd;
    psc = timer_index[psc];
    TMR->CON = 0;
    TMR->CNT = 0;
    TMR->CON |= BIT(14);
    TMR->PRD = prd;
    TMR->CON |= psc << 4;
    TMR->CON |= BIT(0);
}

int tt;
static void TSEC timer_set4(u32 us);
static ___interrupt TSEC void timer_isr4(void)
{
	unsigned long c32 = 500000;

	if(TMR->CON & BIT(15))
    {
		TMR->CON |= BIT(14);

	#if 0
		c32 = Tim_PAP_Handler();
		timer_set4(c32);
	#else
		c32 = TimPos_CRG_Handler();
		timer_set4(c32);
	#endif
    }
}
// u32 us: <5000000//5s
static void TSEC timer_set4(u32 us)
{
    u32 freq = 1000000 / us;
    timer_cfg(freq, us);
}
static void TSEC timer_freq_set4(u32 freq)
{
    timer_cfg(freq, 0);
}

//example test
static int Timer4Initial(void)
{
    timer_clk4 = clk_get("timer");
    request_irq(timer_irq4, 3, timer_isr4, 0);
    timer_set4(3300); //1000=1ms
    return 0;
}
#endif

void PrinterInit(void)
{
	InitialIO();
	Timer2Initial();
	Timer3Initial();
	Timer4Initial();

	adc_init();
	adc_add_sample_ch(AdcChKey); // AdcKey
	adc_add_sample_ch(AdcChTemp);
	adc_add_sample_ch(AdcChFrontOut);
	adc_add_sample_ch(AdcChFrontIn);
	adc_add_sample_ch(AdcChBackIn);
	adc_add_sample_ch(AdcChBackOut);

//    return;
	FlagMotorIC = 0xff;
	MotorIC();
	FlagHeadIC = 0x0000;
	HeadIC();

	MainPowerOn
	MotorEnable

	LedErrClose
	LedLineOpen

/*	if(DebugCRGHome)
		CRGState = 0x55;
	else if(CRGState != 0x55)
	{
		FlagSystemPrintClr

		if(HOME)
		{
			CRGState = 0xff;
			CRGNowTimePosition = 0;
			CRGStopTimePosition = 0;
			CRGRightSet
		}
		else
		{
			CRGState = 0x00;
			CRGNowTimePosition = TimePositionAll;
			CRGStopTimePosition = TimePositionAll;
			CRGLeftSet
		}
	}*/
}
#endif

/**************************************************************************************************************
	Applition.c	Applition.h
**************************************************************************************************************/



/**************************************************************************************************************
	UserInterface.c	UserInterface.h
**************************************************************************************************************/



