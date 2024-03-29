#ifndef __USERINTERFACE_H
#define __USERINTERFACE_H


//#define UsterInterfaceh 0
/**************************************************************************************************************
	UsterInterface.c	UsterInterface.h
**************************************************************************************************************/

#include "app_config.h"
#include "system/includes.h"
#include "wifi/wifi_connect.h"
#include "lwip.h"
#include "dhcp_srv/dhcp_srv.h"
#include "event/net_event.h"
#include "net/assign_macaddr.h"
#include "syscfg_id.h"
#include "lwip/sockets.h"
#include "wifi_app_task_Print.h"
#include "sock_api/sock_api.h"
#include "os/os_api.h"
#include "lwip/netdb.h"
#include "UsbPrinter.h"
#include "device/gpio.h"
#include "system/timer.h"
#include "/system/generic/printf.h"

#include "Driver.h"
#include "McuInt.h"

// 品牌或调试标识，调整程序时需要优先调整
// 0: 调试软件版本
// 1:SHININGER	2:SANKYO		3:TEJIE		4:COMET		5:INSPUR
// 6:世亦		7:三恭		8:特杰		9:科密		10:浪潮
// 255:凯歌
#define DebugMsg			1		// 使用串口返回调试信息
#define DebugAlkeLOGO		0
#define DebugCRGHome		1		// 开机启动不检测HOME位置
#define DebugPaper			1		// 开机启动不检测纸张传感器

//#define DebugUSBClose		0		// 关闭USB
//#define DebugWifiClose	1		// 关闭WIFI
//#define DebugLEClose		1		// 关闭BL
//#define DebugBTClose		1		// 关闭BT


#define	PrinterDPI	180	//
#define	PrinterWide	80//82	// wide列


/********** 	McuIO	**********/

extern volatile unsigned char FlagIOStatus1;

// Power
#define	KeyPower		gpio_read(IO_PORTB_01)	// 1:key power on/off
#define	MainPower		(FlagIOStatus1&0x04)
#define	MainPowerOn		{JL_PORTC->OUT |= 0x0024; FlagIOStatus1 |= 0x04;} // C5/C2
#define	MainPowerOff	{JL_PORTC->OUT &= ~0x0024; FlagIOStatus1 &= ~0x04;}

// SYSTEM
//#define	KeyAdc		gpio_read(IO_PORTC_10)	----> ADC键盘
#define	LedLine			(FlagIOStatus1&0x20)
#define	LedLineClose	{JL_PORTC->OUT |= 0x80; (FlagIOStatus1 |= 0x20);}
#define	LedLineOpen		{JL_PORTC->OUT &= ~0x80; (FlagIOStatus1 &= ~0x20);}
#define	LedErr			(FlagIOStatus1&0x10)
#define	LedErrClose		{JL_PORTC->OUT |= 0x40; (FlagIOStatus1 |= 0x10);}
#define	LedErrOpen		{JL_PORTC->OUT &= ~0x40; (FlagIOStatus1 &= ~0x10);}

// HEAD	16针采用两个595IC串联输出
extern volatile unsigned short FlagHeadIC;
// 后进纸正确的针脚数
#define Needle01 	0x0020//0x2000
#define Needle02 	0x1000//0x0010
#define Needle03 	0x8000//0x0080
#define Needle04 	0x0080//0x8000
#define Needle05 	0x0010//0x1000
#define Needle06 	0x0400//0x0004
#define Needle07 	0x0001//0x0100
#define Needle08 	0x0200//0x0002
#define Needle09 	0x0008//0x0800
#define Needle10 	0x0800//0x0008
#define Needle11 	0x0002//0x0200
#define Needle12 	0x0100//0x0001
#define Needle13 	0x0004//0x0400
#define Needle14 	0x2000//0x0020
#define Needle15 	0x4000//0x0040
#define Needle16 	0x0040//0x4000
extern volatile unsigned char HeadState;
extern volatile unsigned char FlagPrinter;
	#define FlagPRTFast		10	// 3/16真实出针,3/8虚拟半点出针
	#define FlagPRTStand	11	// 4/16真实出针,4/8虚拟半点出针
	//#define FlagPRTDesity	12	// 5/16真实出针,5/8虚拟半点出针
	#define FlagPRTDeep		21	// 5/16真实出针,5/8虚拟半点出针

#define HdDiHigh		JL_PORTA->OUT |= 0x01
#define HdDiLow			JL_PORTA->OUT &= ~0x01
#define HdGoutHigh		JL_PORTA->OUT |= 0x02
#define HdGoutLow		JL_PORTA->OUT &= ~0x02
#define HdRckHigh		JL_PORTA->OUT |= 0x04
#define HdRckLow		JL_PORTA->OUT &= ~0x04
#define HdSckHigh		JL_PORTA->OUT |= 0x08
#define HdSckLow		JL_PORTA->OUT &= ~0x08
#define HdClrHigh		JL_PORTA->OUT |= 0x10
#define HdClrLow		JL_PORTA->OUT &= ~0x10


extern volatile unsigned char FlagMotorMode;
#define CRGLeftDir			(FlagMotorMode&0x80)
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

extern volatile unsigned char FlagMotorIC;
#define MotorEnable		JL_PORTB->OUT |= 0x0020;
#define MotorDisable	JL_PORTB->OUT &= ~0x0020;
// Motor use 595 IC to control CRG & PAP
// bit7-bit0:	PapA	PapB	PapS	PapE			CrgA	CrgB	CrgS	CrgE
// Pap+Crg		phaseA	phaseB	1/3Current	2/3Current	same to Pap
#define MtDiHigh		JL_PORTA->OUT |= 0x0400
#define MtDiLow			JL_PORTA->OUT &= ~0x0400
#define MtGoutHigh		JL_PORTA->OUT |= 0x0200
#define MtGoutLow		JL_PORTA->OUT &= ~0x0200
#define MtRckHigh		JL_PORTA->OUT |= 0x0100
#define MtRckLow		JL_PORTA->OUT &= ~0x0100
#define MtSckHigh		JL_PORTA->OUT |= 0x0040
#define MtSckLow		JL_PORTA->OUT &= ~0x0040
#define MtClrHigh		JL_PORTA->OUT |= 0x0020
#define MtClrLow		JL_PORTA->OUT &= ~0x0020

// Communation
#define	USBin			gpio_read(IO_PORTB_06)	// Adc
//#define	TXD				// H6/H4
//#define	RXD				// H8/H7
//#define	USBM			//
//#define	USBP			//

// Home position
#define	HOME			gpio_read(IO_PORTC_08)
// Paper Sensor
#define	PsFrontOut		gpio_read(IO_PORTB_07)		// 1:paper
#define	PsFrontIn		gpio_read(IO_PORTC_09)		// 1:paper
#define	PsBackIn		gpio_read(IO_PORTC_01)		// 1:paper
#define	PsBackOut		gpio_read(IO_PORTB_08)//C0	// 1:paper

/********** 	McuIO	**********/




extern volatile unsigned char PowerState;

#define	CRGRuning	(FlagMotorIC&0x03 == 0x00)
#define	CRGVolatileStep			18
extern const unsigned short TABLECRGSpeedAccDecOneDirNoprint[1+CRGVolatileStep];
extern const unsigned short  TABLECRGSpeedAccOneDirPrint[1+CRGVolatileStep];
extern const unsigned short  TABLECRGSpeedDecOneDirPrint[1+CRGVolatileStep];
extern const unsigned short  TABLECRGSpeedRightAcc[1+CRGVolatileStep];
#define TABLECRGSpeedRightDec TABLECRGSpeedRightAcc
#define TABLECRGSpeedLeftAcc TABLECRGSpeedRightAcc
#define TABLECRGSpeedLeftDec TABLECRGSpeedLeftAcc
extern volatile unsigned char CRGState;
extern volatile unsigned char cCRGSpeed;
extern volatile unsigned short  CRGStopTimePosition;	// 时间片位置TP
extern volatile unsigned short  CRGNowTimePosition;
#define CRGOneStepTimePosition	16
#define TimeForCRGFree 50 //1*500=1s
#define TimePositionPrintSection		(BufferPRTSize*BufferOneDataTimePosition)//1440*8=11520
#define TimePositionInHome				(60*CRGOneStepTimePosition)//
#define TimePositionAll					((CRGVolatileStep+CRGVolatileStep)*CRGOneStepTimePosition+TimePositionPrintSection)//12096
#define TimePositionMiddleForPaperFeed	(TimePositionAll/2)//6048 打印头停在两个胶辊中间，方便装色带支架

#define	PAPRuning	((FlagMotorIC&0x30) == 0x00)
#define	PAPVolatileStep 			11//40//11
extern const unsigned short  TABLEPAPSpeed[];
extern volatile unsigned char PAPState;
extern volatile unsigned char cPAPSpeed;
extern volatile unsigned short  PAPMoveStep;















// 键盘相关
extern volatile unsigned short  ADCKEY;
//0		1/4		1/2		3/4		1
//NO	*		KeyLine	*		KeyPaper
#define	ADC1F8	(0x400*1/8)	//	128		// 0xfff*1/8  512
#define	ADC2F8	(0x400*2/8)	//	256		// 0xfff*2/8  1024
#define	ADC3F8	(0x400*3/8)	//	384		// 0xfff*3/8  1536
#define	ADC4F8	(0x400*4/8)	//	512		// 0xfff*4/8  2048
#define	ADC5F8	(0x400*5/8)	//	640		// 0xfff*5/8  2560
#define	ADC6F8	(0x400*6/8)	//	768		// 0xfff*6/8  3072
#define	ADC7F8	(0x400*7/8)	//	896		// 0xfff*7/8  3584
#define	ADC8F8	(0x400*8/8)	//	1024	// 0xfff*8/8  4096
extern volatile unsigned char KeyPaperRecorder;
extern volatile unsigned short  KeyPaperDownTime;
extern volatile unsigned char KeyLineRecorder;
extern volatile unsigned short  KeyLineDownTime;


extern volatile u32 DelayTimeCount2mS;
// 系统延时需要的变量


extern volatile unsigned short  ADCTEMP;
//#define	TEMP		gpio_read(IO_PORTA_07)	----> ADC温度



























extern const unsigned char TABLELOGO[];
extern const unsigned char TABLESoftWare[];

extern volatile unsigned char FlagManager;
	#define FlagManagerNormal		0x00
	#define FlagManagerSeftPrint	0x01
	#define FlagManagerHeadDelay	0x02	// no sue
	#define FlagManagerSetMode		0x03	// no use
	#define FlagManagerHexPrint		0x04
	#define FlagManagerTestPrint	0x05	// TestPrint Mode
	#define FlagManagerQR1			0x10	// no use
	#define FlagManagerQR2			0x11	// no use
	#define FlagManagerCODE128		0x20	// no use

/* 接收缓冲区USB/WIFI/LE/BT四个定义在各自的头文件中，此处无需定义
#define	BufferReceiveSIZE			30000	//
extern volatile unsigned char BufferReceive[BufferReceiveSIZE];
struct sBufferReceive
{
	volatile unsigned char *pStart;	// 缓冲区起始地址
	volatile unsigned char State;	// 状态：0:初始状态，！0:标志正在使用等情况
	const unsigned int Size;		// 缓冲区大小
	const unsigned int Full;		// 置忙门槛
	const unsigned int Empty;		// 清忙门槛
	volatile unsigned int cRead;	// 读指针
	volatile unsigned int cWrite;	// 写指针
};
extern struct sBufferReceive BufferReceiveUSB;
extern struct sBufferReceive BufferReceiveWIFI;
extern struct sBufferReceive BufferReceiveBL;
*/




extern volatile unsigned short  FlagSystem;			// 仅支持西文窄字体，此处所有模式取消不用
	#define FlagSystemPrint				(FlagSystem&0x8000)	// Printing or not print when the CRG motor running(carrige moving)
	#define FlagSystemPrintSet			FlagSystem |= 0x8000;
	#define FlagSystemPrintClr			FlagSystem &= ~0x8000;

/*	#define FontLetterKT			(FlagSystem&0x0001)
	#define FontLetterKTSet			(FlagSystem |= 0x0001)
	#define FontLetterKTClr			(FlagSystem &= ~0x0001)
	#define FontLetterLB			(FlagSystem&0x0002)
	#define FontLetterLBSet			(FlagSystem |= 0x0002)
	#define FontLetterLBClr			(FlagSystem &= ~0x0002)
	#define FontLetterDW			(FlagSystem&0x0004)
	#define FontLetterDWSet			(FlagSystem |= 0x0004)
	#define FontLetterDWClr			(FlagSystem &= ~0x0004)
	#define FontLetterDH			(FlagSystem&0x0008)
	#define FontLetterDHSet			(FlagSystem |= 0x0008)
	#define FontLetterDHClr			(FlagSystem &= ~0x0008)
	#define FontChineseLB			(FlagSystem&0x0020)
	#define FontChineseLBSet		(FlagSystem |= 0x0020)
	#define FontChineseLBClr		(FlagSystem &= ~0x0020)
	#define FontChineseDW			(FlagSystem&0x0040)
	#define FontChineseDWSet		(FlagSystem |= 0x0040)
	#define FontChineseDWClr		(FlagSystem &= ~0x0040)
	#define FontChineseDH			(FlagSystem&0x0080)
	#define FontChineseDHSet		(FlagSystem |= 0x0080)
	#define FontChineseDHClr		(FlagSystem &= ~0x0080)
	#define FontAlignLeft			((FlagSystem&0x0300) == 0x0000)
	#define FontAlignLeftSet		(FlagSystem &= ~0x0300)
	#define FontAlignCentre			((FlagSystem&0x0300) == 0x0100)
	#define FontAlignCentreSet		{FlagSystem &= ~0x0300; FlagSystem |= 0x0100;}
	#define FontAlignRight			((FlagSystem&0x0300) == 0x0200)
	#define FontAlignRightSet		{FlagSystem &= ~0x0300; FlagSystem |= 0x0200;}
	#define FontRotateZero			((FlagSystem&0x0c00) == 0x0000)
	#define FontRotateZeroSet		(FlagSystem &= ~0x0c00)
	#define FontRotateRight			((FlagSystem&0x0c00) == 0x0400)
	#define FontRotateRightSet		{FlagSystem &= ~0x0c00; FlagSystem |= 0x0400;}
	#define FontRotateLeft			((FlagSystem&0x0c00) == 0x0800)
	#define FontRotateLeftSet		{FlagSystem &= ~0x0c00; FlagSystem |= 0x0800;}*/
	#define FlagKeyPadNo			(FlagSystem&0x1000)
	#define FlagKeyPadNoUse			(FlagSystem |= 0x1000)
	#define FlagKeyPadYesUse		(FlagSystem &= ~0x1000)

extern volatile unsigned char WifiState;
extern volatile unsigned short DelaytimeForWifiState;


extern volatile unsigned char SpaceBetweenLetter;

// 填写打印缓冲区时使用
#define	BufferOneDataTimePosition		8	//only 8
#define	BufferPRTSize					(PrinterDPI*PrinterWide/10) // 1440
#define	BufferPrintLineNum				10
struct sBufferPrint
{
	volatile unsigned char BufferA[BufferPRTSize];//偶数行数据，高位为高数据
	volatile unsigned char BufferB[BufferPRTSize];//奇数行数据，高位为高数据

	// 中断打印走纸任务中，根据 cBufferPrintLineWrite    cBufferPrintLineRead 确定数据就位，开始计算数据左右边界cLeft cRight，并折算到马达对应的TimePosition
	// 根据cLeft cRight，决定是否需要空跑完成逻辑打印
	volatile unsigned int cLeft;			// 左起非0数据
	volatile unsigned int cRight;			// 右起非0数据

	volatile unsigned char Step;			// 走纸步数默认16步
	volatile unsigned char Dir;				// 默认向前走纸
};
extern struct sBufferPrint BufferPrint[];
extern volatile unsigned int cBufferPrintLineWrite;	// 打印缓冲区当前写入行记录
extern volatile unsigned int cBufferPrintDataWrite;	// 打印缓冲区当前写入行的数据写入位置记录
extern volatile unsigned int cBufferPrintLineRead;		// 打印缓冲区当前打印行记录
extern volatile unsigned int cBufferPrintDataRead;		// 打印缓冲区当前打印行的数据读取位置记录
//enum {sINIT, sStartCRGMotor, sWaitingCRGMotor, sWaitingHeadPrinting, sStartPAPMotor, sWaitingPAPMotor};
enum {sINIT, sStartCRGMotor, sWaitingCRGMotor, sWaitingHeadPrinting};
extern volatile unsigned char PrintingState;

#define StepFrontToPrint 141 // 还需要进一步测量数据
#define StepBackToPrint 150 // 还需要进一步测量数据
#define StepHeadWide 50 // 还需要进一步测量数据
extern unsigned short PaperCount;
// 0x00: no paper	0x1*: 后进纸模式	0x2*: 前进纸模式
extern volatile unsigned short  PaperState;
extern volatile unsigned short  PaperPageLenth;
extern volatile unsigned char SensorRecorderFrontOut;
extern volatile unsigned char SensorRecorderFrontIn;
extern volatile unsigned char SensorRecorderBackOut;
extern volatile unsigned char SensorRecorderBackIn;


#define StepPageLenthFront 2105 // A4纸: 297mm/25.4*180=2105
#define StepPageLenthBack 1980 // 链孔纸: 90*22=1980
extern volatile unsigned char StepBetweenLine;
extern volatile unsigned char FlagPositionCut;
extern volatile unsigned short  StepPageLenth;
extern volatile unsigned short  StepPageCount;
extern volatile unsigned short  StepPapCountTail;


#define	PageLineNumINIT				0x0042	// 279.4mm=1584 1584/24=66Line
#define	PaperPositionFirstPrintINIT	110	// 0x00d1	// 120+16*3	// 120 step // 无纸张边界，由驱动完成	// 0xd1:纸张边界1/4英寸0xc5: 纸张边界1/6英寸		27.4mm~=155步-->170 step
#define	PaperPositionCutINIT		200	// 0x0141	// 2步=.353mm	0x015f	// 55.2mm~=315步
#define	PaperPositionOutINIT		0x00ff	// 保留不用
#define	ADCPAPERLEVERINIT			0x0b00	// 0x0e00 //	0x0e60	// 0x0ee4=3812	NO:>=0x0f6d=3949	YES:<=0x0e5b=3675
#define	ADCTEMPLEVERINIT			0x08c0	//0x0950	// 0x0990	// 0x08fc
#define	HeadDelayCountINIT			22
#define	PaperPositionSlowINIT		0x00aa


extern volatile unsigned short  HeadDelayCount;
extern volatile unsigned short  PageLineNum;
extern volatile unsigned short  PaperPositionFirstPrint;
extern volatile unsigned short  PaperPositionCut;
extern volatile unsigned short  PaperPositionOut;
extern volatile unsigned short  AdcPaperLever;
extern volatile unsigned short  AdcTempLever;


/**************************************************************************************************************
	UsterInterface.c	UsterInterface.h
**************************************************************************************************************/
enum eCommandType  //命令类型
{
	eCmdInit = 0, //初次尚未匹配命令，必须要有这个类型，防止命令解析错误

	eCmd0dPrint, // 打印并回车

	eCmd1bClass,
	eCmd1b0a,
	eCmd1b2aDots, // 点阵图形命令
	eCmd1b4aPAP, // 走纸
	eCmd1b55PrtDir, //单向打印
	eCmd1b7a,
	eCmd1b89,
	eCmd1b9c,
	eCmd1ba0,

	eCmd1dClass,
	eCmd1d56CutPaper // 切纸
};
struct sCommand
{
 	volatile unsigned char Type;	// 命令类型
	volatile unsigned char State;	// 命令状态、步骤、分支等记录
	volatile unsigned int Size;		// 后面数据个数
};
extern struct sCommand Command;




#define McuIOh 0
/**************************************************************************************************************
	McuIO.c	McuIO.h
**************************************************************************************************************/

extern unsigned char  MotorIC(void);
extern void CRGCurrent(unsigned char CrgState);
extern void CRGPhase(void);
extern void PAPCurrent(unsigned char PapState);
extern void PAPPhase(void);
extern unsigned char HeadIC(void);

extern void OutputByteData(unsigned char ByteData);

extern void SendString(unsigned char *p);
//extern unsigned char HeadWrite(unsigned char ByteDataOdd, unsigned char ByteDataEven);
//extern void TimPos_HeadWork3F16_Handler(void);
//extern void TimPos_HeadWork4F16_Handler(void);
//extern void TimPos_HeadWork5F16_Handler(void);
extern volatile unsigned char BufferPrintLineWrited(void);

/**************************************************************************************************************
	McuIO.c	McuIO.h
**************************************************************************************************************/


#if 0
#define McuInth 0
/**************************************************************************************************************
	McuInt.c	McuInt.h
**************************************************************************************************************/
extern void Tim_Sys_Handler(void);
extern unsigned long Tim_PAP_Handler(void);
extern unsigned long TimPos_CRG_Handler(void);

/**************************************************************************************************************
	McuInt.c	McuInt.h
**************************************************************************************************************/
#endif


#define Driverh 0
/**************************************************************************************************************
	Driver.c	Driver.h
**************************************************************************************************************/




/**************************************************************************************************************
	Driver.c	Driver.h
**************************************************************************************************************/


#define Applitionh 0
/**************************************************************************************************************
	Applition.c	Applition.h
**************************************************************************************************************/


extern void SendCmd(unsigned char CMD1, unsigned char CMD2);



/**************************************************************************************************************
	Applition.c	Applition.h
**************************************************************************************************************/
//extern void PrinterInit(void);



#endif
