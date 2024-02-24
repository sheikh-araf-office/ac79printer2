
#define McuInt 0
/**************************************************************************************************************
	McuInt.c	McuInt.h
**************************************************************************************************************/

#include "Driver.h"
#include "McuInt.h"
#include "UserInterface.h"

/*UserInterface.h
wifi_app_task_printer.h
wifi_app_task_print.h
wifi_app_task_printer.c
wifi_app_task_print.c*/

// 此处定义的数据必须加括号，否则时序错误
#define DR0810	0
#define DR0612	0
#define DR0414	3
#define DR0216	6
#define DR0115	(9+1)
#define DR0313	(12+1)
#define DR0511	(15+1)
#define DR0709	(15+1)
/*#define DL0907	0
#define DL1105	0
#define DL1303	3
#define DL1501	6
#define DL1602	(9+1)
#define DL1404	(12+1)
#define DL1206	(15+1)
#define DL1008	(15+1)*/
void TimPos_HeadWork3F16_Handler(void)
{
	unsigned char c8 = 0;
	unsigned char cCFCINIT = 0xff;
	unsigned char ByteDataOdd = 0x00;
	unsigned char ByteDataEven = 0xff;
	u32 cc1;
	u32 cc2;
	u32 c32c1;
	u32 c32c2;

	FlagHeadIC = 0x0000;

	// 需要限制左右边界，防止计算时指针超出缓冲区范围
	// 从左向右打印，此处检查最早出针即左起边界，结束边界考虑各针滞后延时分别考虑
	if(CRGRightDir)
		if(CRGNowTimePosition < CRGVolatileStep*CRGOneStepTimePosition)
			goto HeadWork3F16End;

	// 需要限制左右边界，防止计算时指针超出缓冲区范围
	// 从右向左打印，此处检查最早出针即右起边界，结束边界考虑各针滞后延时分别考虑
	if(CRGLeftDir)
		if(CRGNowTimePosition >= (CRGVolatileStep*CRGOneStepTimePosition+TimePositionPrintSection))
			goto HeadWork3F16End;

	c32c1 = CRGNowTimePosition/BufferOneDataTimePosition-CRGVolatileStep*CRGOneStepTimePosition/BufferOneDataTimePosition;
	c32c2 = CRGNowTimePosition%BufferOneDataTimePosition;

	if((c32c2 == 0)||(c32c2 == 1)||(c32c2 == 2))
		if((c32c1 >= DR0810) && (c32c1 < (BufferPRTSize+DR0810)))
			if(*(BufferPrint[cBufferPrintLineWrite].BufferA+c32c1-DR0810)&0x10)		FlagHeadIC |= Needle08;
	if((c32c2 == 1)||(c32c2 == 2)||(c32c2 == 3))
		if((c32c1 >= DR0810) && (c32c1 < (BufferPRTSize+DR0810)))
			if(*(BufferPrint[cBufferPrintLineWrite].BufferA+c32c1-DR0810)&0x08)		FlagHeadIC |= Needle10;
	if((c32c2 == 2)||(c32c2 == 3)||(c32c2 == 4))
		if((c32c1 >= DR0612) && (c32c1 < (BufferPRTSize+DR0612)))
			if(*(BufferPrint[cBufferPrintLineWrite].BufferA+c32c1-DR0612)&0x20)		FlagHeadIC |= Needle06;
	if((c32c2 == 3)||(c32c2 == 4)||(c32c2 == 5))
		if((c32c1 >= DR0612) && (c32c1 < (BufferPRTSize+DR0612)))
			if(*(BufferPrint[cBufferPrintLineWrite].BufferA+c32c1-DR0612)&0x04)		FlagHeadIC |= Needle12;
	if((c32c2 == 4)||(c32c2 == 5)||(c32c2 == 6))
		if((c32c1 >= DR0414) && (c32c1 < (BufferPRTSize+DR0414)))
			if(*(BufferPrint[cBufferPrintLineWrite].BufferA+c32c1-DR0414)&0x40)		FlagHeadIC |= Needle04;
	if((c32c2 == 5)||(c32c2 == 6)||(c32c2 == 7))
		if((c32c1 >= DR0414) && (c32c1 < (BufferPRTSize+DR0414)))
			if(*(BufferPrint[cBufferPrintLineWrite].BufferA+c32c1-DR0414)&0x02)		FlagHeadIC |= Needle14;

	if((c32c2 == 6)||(c32c2 == 7))
		if((c32c1 >= DR0216) && (c32c1 < (BufferPRTSize+DR0216)))
			if(*(BufferPrint[cBufferPrintLineWrite].BufferA+c32c1-DR0216)&0x80)		FlagHeadIC |= Needle02;
	if(c32c2 == 0)
		if((c32c1 >= (DR0216+1)) && (c32c1 < (BufferPRTSize+(DR0216+1))))
			if(*(BufferPrint[cBufferPrintLineWrite].BufferA+c32c1-(DR0216+1))&0x80)	FlagHeadIC |= Needle02;

	if(c32c2 == 7)
		if((c32c1 >= DR0216) && (c32c1 < (BufferPRTSize+DR0216)))
			if(*(BufferPrint[cBufferPrintLineWrite].BufferA+c32c1-DR0216)&0x01)		FlagHeadIC |= Needle16;
	if((c32c2 == 0)||(c32c2 == 1))
		if((c32c1 >= (DR0216+1)) && (c32c1 < (BufferPRTSize+(DR0216+1))))
			if(*(BufferPrint[cBufferPrintLineWrite].BufferA+c32c1-(DR0216+1))&0x01)	FlagHeadIC |= Needle16;

	if((c32c2 == 0)||(c32c2 == 1)||(c32c2 == 2))
		if((c32c1 >= DR0115) && (c32c1 < (BufferPRTSize+DR0115)))
			if(*(BufferPrint[cBufferPrintLineWrite].BufferB+c32c1-DR0115)&0x80)		FlagHeadIC |= Needle01;
	if((c32c2 == 1)||(c32c2 == 2)||(c32c2 == 3))
		if((c32c1 >= DR0115) && (c32c1 < (BufferPRTSize+DR0115)))
			if(*(BufferPrint[cBufferPrintLineWrite].BufferB+c32c1-DR0115)&0x01)		FlagHeadIC |= Needle15;
	if((c32c2 == 2)||(c32c2 == 3)||(c32c2 == 4))
		if((c32c1 >= DR0313) && (c32c1 < (BufferPRTSize+DR0313)))
			if(*(BufferPrint[cBufferPrintLineWrite].BufferB+c32c1-DR0313)&0x40)		FlagHeadIC |= Needle03;
	if((c32c2 == 3)||(c32c2 == 4)||(c32c2 == 5))
		if((c32c1 >= DR0313) && (c32c1 < (BufferPRTSize+DR0313)))
			if(*(BufferPrint[cBufferPrintLineWrite].BufferB+c32c1-DR0313)&0x02)		FlagHeadIC |= Needle13;
	if((c32c2 == 4)||(c32c2 == 5)||(c32c2 == 6))
		if((c32c1 >= DR0511) && (c32c1 < (BufferPRTSize+DR0511)))
			if(*(BufferPrint[cBufferPrintLineWrite].BufferB+c32c1-DR0511)&0x20)		FlagHeadIC |= Needle05;
	if((c32c2 == 5)||(c32c2 == 6)||(c32c2 == 7))
		if((c32c1 >= DR0511) && (c32c1 < (BufferPRTSize+DR0511)))
			if(*(BufferPrint[cBufferPrintLineWrite].BufferB+c32c1-DR0511)&0x04)		FlagHeadIC |= Needle11;

	if((c32c2 == 6)||(c32c2 == 7))
		if((c32c1 >= DR0709) && (c32c1 < (BufferPRTSize+DR0709)))
			if(*(BufferPrint[cBufferPrintLineWrite].BufferB+c32c1-DR0709)&0x10)		FlagHeadIC |= Needle07;
	if(c32c2 == 0)
		if((c32c1 >= (DR0709+1)) && (c32c1 < (BufferPRTSize+(DR0709+1))))
			if(*(BufferPrint[cBufferPrintLineWrite].BufferB+c32c1-(DR0709+1))&0x10)	FlagHeadIC |= Needle07;

	if(c32c2 == 7)
		if((c32c1 >= DR0709) && (c32c1 < (BufferPRTSize+DR0709)))
			if(*(BufferPrint[cBufferPrintLineWrite].BufferB+c32c1-DR0709)&0x08)		FlagHeadIC |= Needle09;
	if((c32c2 == 0)||(c32c2 == 1))
		if((c32c1 >= (DR0709+1)) && (c32c1 < (BufferPRTSize+(DR0709+1))))
			if(*(BufferPrint[cBufferPrintLineWrite].BufferB+c32c1-(DR0709+1))&0x08)	FlagHeadIC |= Needle09;

	HeadWork3F16End:
	HeadIC();
}
void TimPos_HeadWork4F16_Handler(void)
{
	unsigned char c8 = 0;
	unsigned char cCFCINIT = 0xff;
	unsigned char ByteDataOdd = 0x00;
	unsigned char ByteDataEven = 0xff;
	u32 cc1;
	u32 cc2;
	u32 c32c1;
	u32 c32c2;

	FlagHeadIC = 0x0000;

	// 需要限制左右边界，防止计算时指针超出缓冲区范围
	// 从左向右打印，此处检查最早出针即左起边界，结束边界考虑各针滞后延时分别考虑
	if(CRGRightDir)
		if(CRGNowTimePosition < CRGVolatileStep*CRGOneStepTimePosition)
			goto HeadWork4F16End;

	// 需要限制左右边界，防止计算时指针超出缓冲区范围
	// 从右向左打印，此处检查最早出针即右起边界，结束边界考虑各针滞后延时分别考虑
	if(CRGLeftDir)
		if(CRGNowTimePosition >= (CRGVolatileStep*CRGOneStepTimePosition+TimePositionPrintSection))
			goto HeadWork4F16End;

	c32c1 = CRGNowTimePosition/BufferOneDataTimePosition-CRGVolatileStep*CRGOneStepTimePosition/BufferOneDataTimePosition;
	c32c2 = CRGNowTimePosition%BufferOneDataTimePosition;

	if((c32c2 == 0)||(c32c2 == 1)||(c32c2 == 2)||(c32c2 == 3))
		if((c32c1 >= DR0810) && (c32c1 < (BufferPRTSize+DR0810)))
			if(*(BufferPrint[cBufferPrintLineWrite].BufferA+c32c1-DR0810)&0x10)		FlagHeadIC |= Needle08;
	if((c32c2 == 1)||(c32c2 == 2)||(c32c2 == 3)||(c32c2 == 4))
		if((c32c1 >= DR0810) && (c32c1 < (BufferPRTSize+DR0810)))
			if(*(BufferPrint[cBufferPrintLineWrite].BufferA+c32c1-DR0810)&0x08)		FlagHeadIC |= Needle10;
	if((c32c2 == 2)||(c32c2 == 3)||(c32c2 == 4)||(c32c2 == 5))
		if((c32c1 >= DR0612) && (c32c1 < (BufferPRTSize+DR0612)))
			if(*(BufferPrint[cBufferPrintLineWrite].BufferA+c32c1-DR0612)&0x20)		FlagHeadIC |= Needle06;
	if((c32c2 == 3)||(c32c2 == 4)||(c32c2 == 5)||(c32c2 == 6))
		if((c32c1 >= DR0612) && (c32c1 < (BufferPRTSize+DR0612)))
			if(*(BufferPrint[cBufferPrintLineWrite].BufferA+c32c1-DR0612)&0x04)		FlagHeadIC |= Needle12;
	if((c32c2 == 4)||(c32c2 == 5)||(c32c2 == 6)||(c32c2 == 7))
		if((c32c1 >= DR0414) && (c32c1 < (BufferPRTSize+DR0414)))
			if(*(BufferPrint[cBufferPrintLineWrite].BufferA+c32c1-DR0414)&0x40)		FlagHeadIC |= Needle04;

	if((c32c2 == 5)||(c32c2 == 6)||(c32c2 == 7))
		if((c32c1 >= DR0414) && (c32c1 < (BufferPRTSize+DR0414)))
			if(*(BufferPrint[cBufferPrintLineWrite].BufferA+c32c1-DR0414)&0x02)		FlagHeadIC |= Needle14;
	if(c32c2 == 0)
		if((c32c1 >= (DR0414+1)) && (c32c1 < (BufferPRTSize+(DR0414+1))))
			if(*(BufferPrint[cBufferPrintLineWrite].BufferA+c32c1-(DR0414+1))&0x02)		FlagHeadIC |= Needle14;

	if((c32c2 == 6)||(c32c2 == 7))
		if((c32c1 >= DR0216) && (c32c1 < (BufferPRTSize+DR0216)))
			if(*(BufferPrint[cBufferPrintLineWrite].BufferA+c32c1-DR0216)&0x80)		FlagHeadIC |= Needle02;
	if((c32c2 == 0)||(c32c2 == 1))
		if((c32c1 >= (DR0216+1)) && (c32c1 < (BufferPRTSize+(DR0216+1))))
			if(*(BufferPrint[cBufferPrintLineWrite].BufferA+c32c1-(DR0216+1))&0x80)	FlagHeadIC |= Needle02;

	if(c32c2 == 7)
		if((c32c1 >= DR0216) && (c32c1 < (BufferPRTSize+DR0216)))
			if(*(BufferPrint[cBufferPrintLineWrite].BufferA+c32c1-DR0216)&0x01)		FlagHeadIC |= Needle16;
	if((c32c2 == 0)||(c32c2 == 1)||(c32c2 == 2))
		if((c32c1 >= (DR0216+1)) && (c32c1 < (BufferPRTSize+(DR0216+1))))
			if(*(BufferPrint[cBufferPrintLineWrite].BufferA+c32c1-(DR0216+1))&0x01)	FlagHeadIC |= Needle16;

	if((c32c2 == 0)||(c32c2 == 1)||(c32c2 == 2)||(c32c2 == 3))
		if((c32c1 >= DR0115) && (c32c1 < (BufferPRTSize+DR0115)))
			if(*(BufferPrint[cBufferPrintLineWrite].BufferB+c32c1-DR0115)&0x80)		FlagHeadIC |= Needle01;
	if((c32c2 == 1)||(c32c2 == 2)||(c32c2 == 3)||(c32c2 == 4))
		if((c32c1 >= DR0115) && (c32c1 < (BufferPRTSize+DR0115)))
			if(*(BufferPrint[cBufferPrintLineWrite].BufferB+c32c1-DR0115)&0x01)		FlagHeadIC |= Needle15;
	if((c32c2 == 2)||(c32c2 == 3)||(c32c2 == 4)||(c32c2 == 5))
		if((c32c1 >= DR0313) && (c32c1 < (BufferPRTSize+DR0313)))
			if(*(BufferPrint[cBufferPrintLineWrite].BufferB+c32c1-DR0313)&0x40)		FlagHeadIC |= Needle03;
	if((c32c2 == 3)||(c32c2 == 4)||(c32c2 == 5)||(c32c2 == 6))
		if((c32c1 >= DR0313) && (c32c1 < (BufferPRTSize+DR0313)))
			if(*(BufferPrint[cBufferPrintLineWrite].BufferB+c32c1-DR0313)&0x02)		FlagHeadIC |= Needle13;
	if((c32c2 == 4)||(c32c2 == 5)||(c32c2 == 6)||(c32c2 == 7))
		if((c32c1 >= DR0511) && (c32c1 < (BufferPRTSize+DR0511)))
			if(*(BufferPrint[cBufferPrintLineWrite].BufferB+c32c1-DR0511)&0x20)		FlagHeadIC |= Needle05;

	if((c32c2 == 5)||(c32c2 == 6)||(c32c2 == 7))
		if((c32c1 >= DR0511) && (c32c1 < (BufferPRTSize+DR0511)))
			if(*(BufferPrint[cBufferPrintLineWrite].BufferB+c32c1-DR0511)&0x04)		FlagHeadIC |= Needle11;
	if(c32c2 == 0)
		if((c32c1 >= (DR0511+1)) && (c32c1 < (BufferPRTSize+(DR0511+1))))
			if(*(BufferPrint[cBufferPrintLineWrite].BufferB+c32c1-(DR0511+1))&0x04)		FlagHeadIC |= Needle11;

	if((c32c2 == 6)||(c32c2 == 7))
		if((c32c1 >= DR0709) && (c32c1 < (BufferPRTSize+DR0709)))
			if(*(BufferPrint[cBufferPrintLineWrite].BufferB+c32c1-DR0709)&0x10)		FlagHeadIC |= Needle07;
	if((c32c2 == 0)||(c32c2 == 1))
		if((c32c1 >= (DR0709+1)) && (c32c1 < (BufferPRTSize+(DR0709+1))))
			if(*(BufferPrint[cBufferPrintLineWrite].BufferB+c32c1-(DR0709+1))&0x10)	FlagHeadIC |= Needle07;

	if(c32c2 == 7)
		if((c32c1 >= DR0709) && (c32c1 < (BufferPRTSize+DR0709)))
			if(*(BufferPrint[cBufferPrintLineWrite].BufferB+c32c1-DR0709)&0x08)		FlagHeadIC |= Needle09;
	if((c32c2 == 0)||(c32c2 == 1)||(c32c2 == 2))
		if((c32c1 >= (DR0709+1)) && (c32c1 < (BufferPRTSize+(DR0709+1))))
			if(*(BufferPrint[cBufferPrintLineWrite].BufferB+c32c1-(DR0709+1))&0x08)	FlagHeadIC |= Needle09;

	HeadWork4F16End:
	HeadIC();
}
void TimPos_HeadWork5F16_Handler(void)
{
	unsigned char c8 = 0;
	unsigned char cCFCINIT = 0xff;
	unsigned char ByteDataOdd = 0x00;
	unsigned char ByteDataEven = 0xff;
	u32 cc1;
	u32 cc2;
	u32 c32c1;
	u32 c32c2;

	FlagHeadIC = 0x0000;

	// 需要限制左右边界，防止计算时指针超出缓冲区范围
	// 从左向右打印，此处检查最早出针即左起边界，结束边界考虑各针滞后延时分别考虑
	if(CRGRightDir)
		if(CRGNowTimePosition < CRGVolatileStep*CRGOneStepTimePosition)
			goto HeadWork5F16End;

	// 需要限制左右边界，防止计算时指针超出缓冲区范围
	// 从右向左打印，此处检查最早出针即右起边界，结束边界考虑各针滞后延时分别考虑
	if(CRGLeftDir)
		if(CRGNowTimePosition >= (CRGVolatileStep*CRGOneStepTimePosition+TimePositionPrintSection))
			goto HeadWork5F16End;

	c32c1 = CRGNowTimePosition/BufferOneDataTimePosition-CRGVolatileStep*CRGOneStepTimePosition/BufferOneDataTimePosition;
	c32c2 = CRGNowTimePosition%BufferOneDataTimePosition;

	if((c32c2 == 0)||(c32c2 == 1)||(c32c2 == 2)||(c32c2 == 3)||(c32c2 == 4))
		if((c32c1 >= DR0810) && (c32c1 < (BufferPRTSize+DR0810)))
			if(*(BufferPrint[cBufferPrintLineWrite].BufferA+c32c1-DR0810)&0x10)		FlagHeadIC |= Needle08;
	if((c32c2 == 1)||(c32c2 == 2)||(c32c2 == 3)||(c32c2 == 4)||(c32c2 == 5))
		if((c32c1 >= DR0810) && (c32c1 < (BufferPRTSize+DR0810)))
			if(*(BufferPrint[cBufferPrintLineWrite].BufferA+c32c1-DR0810)&0x08)		FlagHeadIC |= Needle10;
	if((c32c2 == 2)||(c32c2 == 3)||(c32c2 == 4)||(c32c2 == 5)||(c32c2 == 6))
		if((c32c1 >= DR0612) && (c32c1 < (BufferPRTSize+DR0612)))
			if(*(BufferPrint[cBufferPrintLineWrite].BufferA+c32c1-DR0612)&0x20)		FlagHeadIC |= Needle06;
	if((c32c2 == 3)||(c32c2 == 4)||(c32c2 == 5)||(c32c2 == 6)||(c32c2 == 7))
		if((c32c1 >= DR0612) && (c32c1 < (BufferPRTSize+DR0612)))
			if(*(BufferPrint[cBufferPrintLineWrite].BufferA+c32c1-DR0612)&0x04)		FlagHeadIC |= Needle12;

	if((c32c2 == 4)||(c32c2 == 5)||(c32c2 == 6)||(c32c2 == 7))
		if((c32c1 >= DR0414) && (c32c1 < (BufferPRTSize+DR0414)))
			if(*(BufferPrint[cBufferPrintLineWrite].BufferA+c32c1-DR0414)&0x40)		FlagHeadIC |= Needle04;
	if(c32c2 == 0)
		if((c32c1 >= (DR0414+1)) && (c32c1 < (BufferPRTSize+(DR0414+1))))
			if(*(BufferPrint[cBufferPrintLineWrite].BufferA+c32c1-(DR0414+1))&0x40)		FlagHeadIC |= Needle04;

	if((c32c2 == 5)||(c32c2 == 6)||(c32c2 == 7))
		if((c32c1 >= DR0414) && (c32c1 < (BufferPRTSize+DR0414)))
			if(*(BufferPrint[cBufferPrintLineWrite].BufferA+c32c1-DR0414)&0x02)		FlagHeadIC |= Needle14;
	if((c32c2 == 0)||(c32c2 == 1))
		if((c32c1 >= (DR0414+1)) && (c32c1 < (BufferPRTSize+(DR0414+1))))
			if(*(BufferPrint[cBufferPrintLineWrite].BufferA+c32c1-(DR0414+1))&0x02)		FlagHeadIC |= Needle14;

	if((c32c2 == 6)||(c32c2 == 7))
		if((c32c1 >= DR0216) && (c32c1 < (BufferPRTSize+DR0216)))
			if(*(BufferPrint[cBufferPrintLineWrite].BufferA+c32c1-DR0216)&0x80)		FlagHeadIC |= Needle02;
	if((c32c2 == 0)||(c32c2 == 1)||(c32c2 == 2))
		if((c32c1 >= (DR0216+1)) && (c32c1 < (BufferPRTSize+(DR0216+1))))
			if(*(BufferPrint[cBufferPrintLineWrite].BufferA+c32c1-(DR0216+1))&0x80)	FlagHeadIC |= Needle02;

	if(c32c2 == 7)
		if((c32c1 >= DR0216) && (c32c1 < (BufferPRTSize+DR0216)))
			if(*(BufferPrint[cBufferPrintLineWrite].BufferA+c32c1-DR0216)&0x01)		FlagHeadIC |= Needle16;
	if((c32c2 == 0)||(c32c2 == 1)||(c32c2 == 2)||(c32c2 == 3))
		if((c32c1 >= (DR0216+1)) && (c32c1 < (BufferPRTSize+(DR0216+1))))
			if(*(BufferPrint[cBufferPrintLineWrite].BufferA+c32c1-(DR0216+1))&0x01)	FlagHeadIC |= Needle16;

	if((c32c2 == 0)||(c32c2 == 1)||(c32c2 == 2)||(c32c2 == 3)||(c32c2 == 4))
		if((c32c1 >= DR0115) && (c32c1 < (BufferPRTSize+DR0115)))
			if(*(BufferPrint[cBufferPrintLineWrite].BufferB+c32c1-DR0115)&0x80)		FlagHeadIC |= Needle01;
	if((c32c2 == 1)||(c32c2 == 2)||(c32c2 == 3)||(c32c2 == 4)||(c32c2 == 5))
		if((c32c1 >= DR0115) && (c32c1 < (BufferPRTSize+DR0115)))
			if(*(BufferPrint[cBufferPrintLineWrite].BufferB+c32c1-DR0115)&0x01)		FlagHeadIC |= Needle15;
	if((c32c2 == 2)||(c32c2 == 3)||(c32c2 == 4)||(c32c2 == 5)||(c32c2 == 6))
		if((c32c1 >= DR0313) && (c32c1 < (BufferPRTSize+DR0313)))
			if(*(BufferPrint[cBufferPrintLineWrite].BufferB+c32c1-DR0313)&0x40)		FlagHeadIC |= Needle03;
	if((c32c2 == 3)||(c32c2 == 4)||(c32c2 == 5)||(c32c2 == 6)||(c32c2 == 7))
		if((c32c1 >= DR0313) && (c32c1 < (BufferPRTSize+DR0313)))
			if(*(BufferPrint[cBufferPrintLineWrite].BufferB+c32c1-DR0313)&0x02)		FlagHeadIC |= Needle13;

	if((c32c2 == 4)||(c32c2 == 5)||(c32c2 == 6)||(c32c2 == 7))
		if((c32c1 >= DR0511) && (c32c1 < (BufferPRTSize+DR0511)))
			if(*(BufferPrint[cBufferPrintLineWrite].BufferB+c32c1-DR0511)&0x20)		FlagHeadIC |= Needle05;
	if(c32c2 == 0)
		if((c32c1 >= (DR0511+1)) && (c32c1 < (BufferPRTSize+(DR0511+1))))
			if(*(BufferPrint[cBufferPrintLineWrite].BufferB+c32c1-(DR0511+1))&0x20)		FlagHeadIC |= Needle05;

	if((c32c2 == 5)||(c32c2 == 6)||(c32c2 == 7))
		if((c32c1 >= DR0511) && (c32c1 < (BufferPRTSize+DR0511)))
			if(*(BufferPrint[cBufferPrintLineWrite].BufferB+c32c1-DR0511)&0x04)		FlagHeadIC |= Needle11;
	if((c32c2 == 0)||(c32c2 == 1))
		if((c32c1 >= (DR0511+1)) && (c32c1 < (BufferPRTSize+(DR0511+1))))
			if(*(BufferPrint[cBufferPrintLineWrite].BufferB+c32c1-(DR0511+1))&0x04)		FlagHeadIC |= Needle11;

	if((c32c2 == 6)||(c32c2 == 7))
		if((c32c1 >= DR0709) && (c32c1 < (BufferPRTSize+DR0709)))
			if(*(BufferPrint[cBufferPrintLineWrite].BufferB+c32c1-DR0709)&0x10)		FlagHeadIC |= Needle07;
	if((c32c2 == 0)||(c32c2 == 1)||(c32c2 == 2))
		if((c32c1 >= (DR0709+1)) && (c32c1 < (BufferPRTSize+(DR0709+1))))
			if(*(BufferPrint[cBufferPrintLineWrite].BufferB+c32c1-(DR0709+1))&0x10)	FlagHeadIC |= Needle07;

	if(c32c2 == 7)
		if((c32c1 >= DR0709) && (c32c1 < (BufferPRTSize+DR0709)))
			if(*(BufferPrint[cBufferPrintLineWrite].BufferB+c32c1-DR0709)&0x08)		FlagHeadIC |= Needle09;
	if((c32c2 == 0)||(c32c2 == 1)||(c32c2 == 2)||(c32c2 == 3))
		if((c32c1 >= (DR0709+1)) && (c32c1 < (BufferPRTSize+(DR0709+1))))
			if(*(BufferPrint[cBufferPrintLineWrite].BufferB+c32c1-(DR0709+1))&0x08)	FlagHeadIC |= Needle09;

	HeadWork5F16End:
	HeadIC();
}


/**************************************************************************************************************
	McuInt.c	McuInt.h
**************************************************************************************************************/
// 系统定时器，需要每2毫秒执行一次
// 可以不再放到定时器中，由于没有死循环，可在大循环中执行
void Tim_Sys_Handler(void)
{
	static volatile unsigned char c8ForKeyAdc = 5;		// KEYADC按键扫描
	static volatile unsigned char c8ForPsPaper = 0;	// 纸张传感器扫描
	static volatile unsigned int c16ForPower = 0;	// KEYPOWER开关机处理

return;
/*if(LedLine)
{
	LedLineClose
	LedErrOpen
}
else
{
	LedLineOpen
	LedErrClose
}*/

	// 系统延时计数器
	if(DelayTimeCount2mS)
		DelayTimeCount2mS--;

	// Ledline 不同闪烁状态，表示WIFI配置过程的不同阶段
	// 常亮：WiFi已经OK
	// 闪烁1/2/3/4/5：表示各种不同的配置阶段
	if(WifiState == 0xff)
	{
		if((DelaytimeForWifiState%500) < 200*1)
		{
			if((DelaytimeForWifiState%200) < 100)
				LedLineOpen
		}
		else
			LedLineClose
	}
	else if(WifiState == 0xfe)
	{
		if((DelaytimeForWifiState%500) < 200*2)
		{
			if((DelaytimeForWifiState%200) < 100)
				LedLineOpen
		}
		else
			LedLineClose
	}
	else if(WifiState <= 2)
	{
		if((DelaytimeForWifiState%2000) < 200*1)
		{
			if((DelaytimeForWifiState%200) < 100)
				LedLineOpen
		}
		else
			LedLineClose
	}
	else if(WifiState <= 4)
	{
		if((DelaytimeForWifiState%2000) < 200*2)
		{
			if((DelaytimeForWifiState%200) < 100)
				LedLineOpen
		}
		else
			LedLineClose
	}
	else if(WifiState <= 6)
	{
		if((DelaytimeForWifiState%2000) < 200*3)
		{
			if((DelaytimeForWifiState%200) < 100)
				LedLineOpen
		}
		else
			LedLineClose
	}
	else if(WifiState <= 8)
	{
		if((DelaytimeForWifiState%2000) < 200*4)
		{
			if((DelaytimeForWifiState%200) < 100)
				LedLineOpen
		}
		else
			LedLineClose
	}
	else if(WifiState <= 10)
	{
		if((DelaytimeForWifiState%2000) < 200*5)
		{
			if((DelaytimeForWifiState%200) < 100)
				LedLineOpen
		}
		else
			LedLineClose
	}
	else if(WifiState <= 12)
	{
		if((DelaytimeForWifiState%2000) < 200*6)
		{
			if((DelaytimeForWifiState%200) < 100)
				LedLineOpen
		}
		else
			LedLineClose
	}
	else if(WifiState <= 13)
	{
		if((DelaytimeForWifiState%2000) < 200*7)
		{
			if((DelaytimeForWifiState%200) < 150)
				LedLineOpen
		}
		else
			LedLineClose
	}
	else if(WifiState <= 14)
	{
		if((DelaytimeForWifiState%2000) < 200*8)
		{
			if((DelaytimeForWifiState%200) < 150)
				LedLineOpen
		}
		else
			LedLineClose
	}
	else
	{
		LedLineOpen
	}

	// StateUsb
/*	if(DelaytimeForStateUsb < 250)
		DelaytimeForStateUsb++;
	else
		StateUsb = 0;*/

	// power off function
	if(c16ForPower < 0xffff)
		c16ForPower++;
	if(PowerState == 0)		// 上电确认状态
	{
		if(!KeyPower)
			c16ForPower = 0;
		if(c16ForPower >= 50) // 50*2mS=100ms
		{
			PowerState = 1;
		}
	}
	else if(PowerState == 1)// 上电已经确认，等待按键松开
	{
		if(!KeyPower)
		{
			MainPowerOn
			PowerState = 10;
		}
	}
	else if(PowerState == 10)// 上电过程完成，正常开机保持状态
	{
		c16ForPower = 0;
		if(KeyPower)
		{
			PowerState = 21;
		}
	}
	else if(PowerState == 21)// 1s内松开按键没有影响
	{
		if(!KeyPower)
			PowerState = 10;
		if(c16ForPower >= 500)// 500*2ms=1000ms
		{
			c16ForPower = 0;
			PowerState = 22;
		}
	}
	else if(PowerState == 22)// 2s闪烁led等待人工关机确认
	{
		if(!KeyPower)
			PowerState = 10;
		if(c16ForPower%100 == 0)
		{
			if(LedErr)
				LedErrOpen
			else
				LedErrClose
		}
		if(c16ForPower >= 100)// 100*2ms=200ms 1000->100立马关机不确认
		{
			MainPowerOff
			PowerState = 23;
		}
	}
	else if(PowerState == 23)
	{
		if(c16ForPower%100 == 0) // 50->100闪烁频率与之前保持一直
		{
			if(LedErr)
				LedErrOpen
			else
				LedErrClose
		}
		MainPowerOff

//		LedLineClose
//		USBPause
//		BusyUartSetup
		if(c16ForPower >= 500)
			c16ForPower = 0;
	}
#if 0
	//如果有按键按下则不检测纸张错误
	//if(UPF==0)
	{
		if(!KeyPower)
		{
			if(PsFrontIn)
			{
				LedErrClose;
				DeviceStatus[0]=0x18;
			}
			else
			{
				LedErrOpen;
				DeviceStatus[0]=0xff;
			}
		}
	}
#endif
#if 0
	// 纸张传感器状态扫描
//	c8ForPsPaper
	if(PrinttingMode==PrintForward)
	{
		if(RecordPaperInstep>500&&SangYi==0)
		{
			SangYi=1;
		//	SendString("位置 到了\r\n");
			if(PAPMoveStep>PAPVolatileStep)
			{

				PaperFeedBack=PAPMoveStep-PAPVolatileStep;
				PAPMoveStep = PAPVolatileStep;
			}
			RecordPaperInstep=0;
		}
	}
	if(PsFrontIn == 0x00)//no paper
	{
		LightOnF=1;
		LightOnCount=0;
	}
	else
	{
		if(LightOnF)
		{
			LightOnCount++;
			if(LightOnCount>4000)
			{
				LightOnCount=0;
//				BOXLow
				LightOnF=0;
			}
		}

	}
#endif

	// 键盘扫描
	if(FlagKeyPadNo == 0x0000)
	{
		if(c8ForKeyAdc)
		{
			c8ForKeyAdc--;
		}
		else
		{
			// 正常2*5=10ms扫描一次
			// 某个按键弹起时，需要延时较长时间，以便ADC电压回落到0位，防止误判按键
			c8ForKeyAdc = 5;

			KeyPaperRecorder <<= 1;
			KeyLineRecorder <<= 1;
			if(ADCKEY >= ADC6F8)
			{
				KeyPaperRecorder |= 0x01;
			}
			else if(ADCKEY >= ADC2F8)
			{
				KeyLineRecorder |= 0x01;
			}

			if(KeyPaperRecorder == 0xff)
			{
				if(KeyPaperDownTime < 0xffff)
					KeyPaperDownTime++;
			}
			else
			{
				KeyPaperDownTime = 0;
			}
			if(KeyLineRecorder == 0xff)
			{
				if(KeyLineDownTime < 0xffff)
					KeyLineDownTime++;
			}
			else
				KeyLineDownTime = 0;

			/*if(	( (KeyPaperRecorder&0xe0) && ((KeyPaperRecorder&0x07) == 0x00) )	||
				( (KeyLineRecorder&0xe0) && ((KeyLineRecorder&0x07) == 0x00) )
			)*/
			if( (KeyLineRecorder == 0xff) || (KeyPaperRecorder == 0xff) )
			{
				c8ForKeyAdc = 50;
			}
		}
	}

#if 0
	// 忙信号定时检查
	if((NumBufferReceive() < BufferReceiveSizeEmpty)&&sangT==0)
	{
		BUSYUSBClear;
	//	BusyUartClear;
	}
#endif
#if 0
	if(ReceiveBufferSize() < BufferReceiveSizeEmpty)
	//if(NumBufferReceive() < BufferReceiveSizeEmpty)ReceiveBufferSize
	{
		;//BUSYUSBClear;
		//BusyUartClear;
	}
#endif
}
// 走纸定时器扫描时间必须要少于字车扫描时间：字车timer会启动走纸，需要保证走纸timer及时启动
// @20230616对上面的策略进行调整：考虑到可能会使用0a，所以走纸启动，必须要经过字车启动后，再启动走纸；可以是打印后再走纸，或者无需打印启动走纸
// 只有  0D  0A走纸结束后，才会调整打印缓冲区指针
// 后续优化的话，可以改用启动timer的方式进行
#define TimePAPScan	20000	//
#define TimeCRGScan	25000	//
unsigned long Tim_PAP_Handler(void)
{
	unsigned long c32time = TimePAPScan;

	if(PAPMoveStep == 0)		// stop
	{
		cPAPSpeed = 0;

		// 停止状态下启动走纸
		if((cBufferPrintLineRead != cBufferPrintLineWrite) && (BufferPrint[cBufferPrintLineRead].Step) && (!CRGRuning))
		//if(cBufferPrintLineRead != cBufferPrintLineWrite)
		{
			PAPCurrent(2);
			PAPMoveStep = BufferPrint[cBufferPrintLineRead].Step;

			BufferPrint[cBufferPrintLineRead].Step = 0;
			
			if(cBufferPrintLineRead == (BufferPrintLineNum-1))
				cBufferPrintLineRead = 0;
			else
				cBufferPrintLineRead++;
		}
		else if((FlagMotorIC&0x30) == 0x00)
		{
			PAPCurrent(1);
		}
		else// if(FlagMotorIC&0x30) // 10 or 01
		{
			PAPCurrent(0);
		}
	}
	else
	{
		// 走纸状态下补充走纸步数
		if((cBufferPrintLineRead != cBufferPrintLineWrite) && (BufferPrint[cBufferPrintLineRead].Step))
		{
			PAPMoveStep += BufferPrint[cBufferPrintLineRead].Step;

			BufferPrint[cBufferPrintLineRead].Step = 0;
			
			if(cBufferPrintLineRead == (BufferPrintLineNum-1))
				cBufferPrintLineRead = 0;
			else
				cBufferPrintLineRead++;
		}
		
		PAPPhase();
//CRGCurrent(2);
//CRGPhase();
		if(PAPMoveStep > (cPAPSpeed+1))
		{
			if(cPAPSpeed < PAPVolatileStep)
				cPAPSpeed++;
		}
		else if(PAPMoveStep <= (cPAPSpeed+1))
		{
			if(cPAPSpeed)
				cPAPSpeed--;
		}
		PAPMoveStep--;

		c32time = *(TABLEPAPSpeed+cPAPSpeed);
	}
	return c32time;
}
// return Pamameter	0:timer close	!0:unit:us
unsigned long TimPos_CRG_Handler(void)
{
	static unsigned long c32time = TimeCRGScan; // 50ms 字车停止运动后定时器中断时间
	unsigned int Count;

static unsigned int cc;
if(cc < 500)
	cc++;
else
{
	cc = 0;
	if(CRGNowTimePosition == CRGStopTimePosition)//(PrintingState == sINIT)
	{
		if(LedErr)
			LedErrOpen
		else
			LedErrClose
	}
}

	/** 马达匀速运动时，可以在适当时间进行出针操作，每个时间片位置都需要计算考虑出针 **/
if(0)//	if(FlagSystemPrint)
	{
		if(FlagPrinter == FlagPRTFast)
			TimPos_HeadWork3F16_Handler();
		else if(FlagPrinter == FlagPRTStand)
			TimPos_HeadWork4F16_Handler();
		else// if(FlagPrinter == FlagPRTDeep)
			TimPos_HeadWork5F16_Handler();
	}

	// 只有在马达运动结束后才进行下个阶段的活动
	if(CRGNowTimePosition == CRGStopTimePosition)	// Stop
	{
		cCRGSpeed = 0;

		// 因为打印优先，故此不必考虑走纸等状态
 		if((FlagMotorIC&0x03) == 0x00)
		{
			CRGCurrent(1);
		}
		else //if((FlagMotorIC&0x03) == 0x01) // 10 or 01 or 11
		{
			CRGCurrent(0);
		}
		
			if(PrintingState == sINIT)
			{
				if((cBufferPrintLineRead != cBufferPrintLineWrite) && (!PAPRuning))
				{
				
 			/* // 计算左右非0数据位置
 					cBufferPrintDataRead = 0;
					while((BufferPrint[cBufferPrintLineRead].BufferA[cBufferPrintDataRead] == 0) && (BufferPrint[cBufferPrintLineRead].BufferB[cBufferPrintDataRead] == 0))
					{
						if(cBufferPrintDataRead < (BufferPRTSize-1))
							cBufferPrintDataRead++;
						else
							break;
					}
					BufferPrint[cBufferPrintLineRead].cLeft = cBufferPrintDataRead;
					cBufferPrintDataRead = BufferPRTSize-1;
					while((BufferPrint[cBufferPrintLineRead].BufferA[cBufferPrintDataRead] == 0) && (BufferPrint[cBufferPrintLineRead].BufferB[cBufferPrintDataRead] == 0))
					{
						if(cBufferPrintDataRead)
							cBufferPrintDataRead--;
						else
							break;
					}
					BufferPrint[cBufferPrintLineRead].cRight = cBufferPrintDataRead;
					BufferPrint[cBufferPrintLineRead].cLeft += CRGVolatileStep*CRGOneStepTimePosition;
					BufferPrint[cBufferPrintLineRead].cLeft -= BufferPrint[cBufferPrintLineRead].cLeft%CRGOneStepTimePosition;			// adjust to th 16*n
					BufferPrint[cBufferPrintLineRead].cRight += CRGVolatileStep*CRGOneStepTimePosition;
					BufferPrint[cBufferPrintLineRead].cRight += CRGOneStepTimePosition-BufferPrint[cBufferPrintLineRead].cRight%CRGOneStepTimePosition; // adjust to th 16*n
					if(BufferPrint[cBufferPrintLineRead].cRight > TimePositionAll)
						BufferPrint[cBufferPrintLineRead].cRight = TimePositionAll;
			*/ // 计算左右非0数据位置
			
					// 无需打印，直接跳转到出口处理，必须跳转及时处理
					if((BufferPrint[cBufferPrintLineRead].cLeft == 0) && (BufferPrint[cBufferPrintLineRead].cRight == 0))
					{
						PrintingState == sWaitingHeadPrinting;
						goto WaitingHeadPrinting; // 必须跳转及时处理
					}
					// 正常有数据需要打印
					else if(BufferPrint[cBufferPrintLineRead].cLeft <= BufferPrint[cBufferPrintLineRead].cRight)
					{
						PrintingState = sStartCRGMotor;
						goto StartCRGMotor; // 必须跳转及时处理
					}
					// 无需打印，直接跳转到出口处理，必须跳转及时处理
					else
					{
						BufferPrint[cBufferPrintLineRead].cLeft = 0;
						BufferPrint[cBufferPrintLineRead].cRight = 0;
						PrintingState == sWaitingHeadPrinting;
						goto WaitingHeadPrinting;
					}
				}
			}
			else if(PrintingState == sStartCRGMotor)
			{
				StartCRGMotor:
				{
					if(CRGNowTimePosition <= BufferPrint[cBufferPrintLineRead].cLeft)
					{
						CRGStopTimePosition = BufferPrint[cBufferPrintLineRead].cRight;
						CRGRightSet
						FlagSystemPrintSet
						CRGCurrent(2);
						PrintingState = sWaitingHeadPrinting; // 直接启动打印
					}
					else if(CRGNowTimePosition >= BufferPrint[cBufferPrintLineRead].cRight)
					{
						CRGStopTimePosition = BufferPrint[cBufferPrintLineRead].cLeft;
						CRGLeftSet
						CRGCurrent(2);
						if(TMPOneDir || CMDOneDir)
						{
							FlagSystemPrintClr
							PrintingState = sWaitingCRGMotor; // 需要预先到达准备位置
						}
						else
						{
							FlagSystemPrintSet
							PrintingState = sWaitingHeadPrinting; // 直接启动打印
						}
					}
					else
					{
						if(TMPOneDir || CMDOneDir)
						{
							CRGStopTimePosition = BufferPrint[cBufferPrintLineRead].cLeft;
							CRGLeftSet
						}
						else
						{
							if((CRGNowTimePosition-BufferPrint[cBufferPrintLineRead].cLeft) < (BufferPrint[cBufferPrintLineRead].cRight-CRGNowTimePosition))
							{
								CRGStopTimePosition = BufferPrint[cBufferPrintLineRead].cLeft;
								CRGLeftSet
							}
							else
							{
								CRGStopTimePosition = BufferPrint[cBufferPrintLineRead].cRight;
								CRGRightSet
							}
						}
						FlagSystemPrintClr
						CRGCurrent(2);
						PrintingState = sWaitingCRGMotor; // 需要预先到达准备位置
					}
				}
			}
			else if(PrintingState == sWaitingCRGMotor)
			{
				WaitingCRGMotor:
				if(!CRGRuning)
				{
					PrintingState = sStartCRGMotor;
					goto StartCRGMotor; // 必须跳转及时处理
				}
			}
			else if(PrintingState == sWaitingHeadPrinting)
			{
				WaitingHeadPrinting:
				if(!CRGRuning)
				{
					PrintingState = sINIT;

					if(cBufferPrintLineRead <(BufferPrintLineNum-1))
						cBufferPrintLineRead++;
					else
						cBufferPrintLineRead = 0;
				}
			}
	}
	else if((CRGNowTimePosition%CRGOneStepTimePosition) == 0)
	{
		CRGPhase();
		if(CRGNowTimePosition >= CRGStopTimePosition)
		{
			Count = (CRGNowTimePosition - CRGStopTimePosition)/CRGOneStepTimePosition;
			// CRGNowTimePosition--
		}
		else
		{
			Count = (CRGStopTimePosition - CRGNowTimePosition)/CRGOneStepTimePosition;
			// CRGNowTimePosition++;
		}
		if(Count > (cCRGSpeed+1))
		{
			if(cCRGSpeed < CRGVolatileStep)
			{
				cCRGSpeed++;
			}
		}
		else // if(Count <= (cCRGSpeed+1))
		{
			if(cCRGSpeed)
			{
				cCRGSpeed--;
			}
		}

		// 左边为0点，传感器在左边，以后进纸方向视角
		if(CRGState != 0x55)							// 0x55: finished INIT
		{
			CRGState <<= 1;
			if(HOME)
				CRGState |= 0x01;
			// 只用进HOME完成初始化过程
			if(CRGState == 0x01)					// to left
			{
				CRGNowTimePosition = TimePositionInHome-1*CRGOneStepTimePosition;
				CRGState = 0x55;					// Finished INIT
			}
			/*else if(CRGState == 0xfe)				// to right
			{
				CRGNowTimePosition = TimePositionInHome+1*CRGOneStepTimePosition;
				CRGState = 0x55;					// Finished INIT
			}*/
		}
		if(CMDOneDir||TMPOneDir) // one direction print
		{
			if(CRGRightDir)
			{
				if(Count > (cCRGSpeed+1))
				{
					c32time = *(TABLECRGSpeedAccOneDirPrint+cCRGSpeed);// acc
				}
				else
				{
					c32time = *(TABLECRGSpeedDecOneDirPrint+cCRGSpeed);// dec
				}
			}
			else
			{
				if(Count > (cCRGSpeed+1))
				{
					c32time = *(TABLECRGSpeedAccDecOneDirNoprint+cCRGSpeed);// acc
				}
				else
				{
					c32time = *(TABLECRGSpeedAccDecOneDirNoprint+cCRGSpeed);// dec
				}
			}
		}
		else // double direction print
		{
			if(CRGLeftDir)
			{
				if(Count > (cCRGSpeed+1))
				{
					c32time = *(TABLECRGSpeedLeftAcc+cCRGSpeed);
				}
				else
				{
					c32time = *(TABLECRGSpeedLeftDec+cCRGSpeed);
				}
			}
			else
			{
				if(Count > (cCRGSpeed+1))
				{
					c32time = *(TABLECRGSpeedRightAcc+cCRGSpeed);
				}
				else
				{
					c32time = *(TABLECRGSpeedRightDec+cCRGSpeed);
				}
			}
		}
		
		TimPos_CRG_end:
		if(CRGLeftDir)
		{
			if(CRGNowTimePosition)
				CRGNowTimePosition--;
		}
		else
		{
			if(CRGNowTimePosition < (TimePositionAll-1))
				CRGNowTimePosition++;
		}
	}

	return c32time/CRGOneStepTimePosition;
}




/**************************************************************************************************************
	McuInt.c	McuInt.h
**************************************************************************************************************/

