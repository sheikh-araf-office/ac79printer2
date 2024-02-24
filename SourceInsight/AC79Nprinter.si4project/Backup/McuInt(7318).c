
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
	if(CRGRightDir)
		c32c1 -= HeadDelayCount;
	c32c2 = CRGNowTimePosition%BufferOneDataTimePosition;

	if((c32c2 == 0)||(c32c2 == 1)||(c32c2 == 2))
	{
		if((c32c1 >= DR0810) && (c32c1 < (BufferPRTSize+DR0810)))
			if(*(BufferPrint[cBufferPrintLineRead].BufferA+c32c1-DR0810)&0x10)		FlagHeadIC |= Needle08;
		if((c32c1 >= DR0115) && (c32c1 < (BufferPRTSize+DR0115)))
			if(*(BufferPrint[cBufferPrintLineRead].BufferB+c32c1-DR0115)&0x80)		FlagHeadIC |= Needle01;
	}
	if((c32c2 == 1)||(c32c2 == 2)||(c32c2 == 3))
	{
		if((c32c1 >= DR0810) && (c32c1 < (BufferPRTSize+DR0810)))
			if(*(BufferPrint[cBufferPrintLineRead].BufferA+c32c1-DR0810)&0x08)		FlagHeadIC |= Needle10;
		if((c32c1 >= DR0115) && (c32c1 < (BufferPRTSize+DR0115)))
			if(*(BufferPrint[cBufferPrintLineRead].BufferB+c32c1-DR0115)&0x01)		FlagHeadIC |= Needle15;
	}
	if((c32c2 == 2)||(c32c2 == 3)||(c32c2 == 4))
	{
		if((c32c1 >= DR0612) && (c32c1 < (BufferPRTSize+DR0612)))
			if(*(BufferPrint[cBufferPrintLineRead].BufferA+c32c1-DR0612)&0x20)		FlagHeadIC |= Needle06;
		if((c32c1 >= DR0313) && (c32c1 < (BufferPRTSize+DR0313)))
			if(*(BufferPrint[cBufferPrintLineRead].BufferB+c32c1-DR0313)&0x40)		FlagHeadIC |= Needle03;
	}
	if((c32c2 == 3)||(c32c2 == 4)||(c32c2 == 5))
	{
		if((c32c1 >= DR0612) && (c32c1 < (BufferPRTSize+DR0612)))
			if(*(BufferPrint[cBufferPrintLineRead].BufferA+c32c1-DR0612)&0x04)		FlagHeadIC |= Needle12;
		if((c32c1 >= DR0313) && (c32c1 < (BufferPRTSize+DR0313)))
			if(*(BufferPrint[cBufferPrintLineRead].BufferB+c32c1-DR0313)&0x02)		FlagHeadIC |= Needle13;
	}
	if((c32c2 == 4)||(c32c2 == 5)||(c32c2 == 6))
	{
		if((c32c1 >= DR0414) && (c32c1 < (BufferPRTSize+DR0414)))
			if(*(BufferPrint[cBufferPrintLineRead].BufferA+c32c1-DR0414)&0x40)		FlagHeadIC |= Needle04;
		if((c32c1 >= DR0511) && (c32c1 < (BufferPRTSize+DR0511)))
			if(*(BufferPrint[cBufferPrintLineRead].BufferB+c32c1-DR0511)&0x20)		FlagHeadIC |= Needle05;
	}
	if((c32c2 == 5)||(c32c2 == 6)||(c32c2 == 7))
	{
		if((c32c1 >= DR0414) && (c32c1 < (BufferPRTSize+DR0414)))
			if(*(BufferPrint[cBufferPrintLineRead].BufferA+c32c1-DR0414)&0x02)		FlagHeadIC |= Needle14;
		if((c32c1 >= DR0511) && (c32c1 < (BufferPRTSize+DR0511)))
			if(*(BufferPrint[cBufferPrintLineRead].BufferB+c32c1-DR0511)&0x04)		FlagHeadIC |= Needle11;
	}
	
	if((c32c2 == 6)||(c32c2 == 7))
	{
		if((c32c1 >= DR0216) && (c32c1 < (BufferPRTSize+DR0216)))
			if(*(BufferPrint[cBufferPrintLineRead].BufferA+c32c1-DR0216)&0x80)		FlagHeadIC |= Needle02;
		if((c32c1 >= DR0709) && (c32c1 < (BufferPRTSize+DR0709)))
			if(*(BufferPrint[cBufferPrintLineRead].BufferB+c32c1-DR0709)&0x10)		FlagHeadIC |= Needle07;
	}
	if(c32c2 == 0)
	{
		if((c32c1 >= (DR0216+1)) && (c32c1 < (BufferPRTSize+(DR0216+1))))
			if(*(BufferPrint[cBufferPrintLineRead].BufferA+c32c1-(DR0216+1))&0x80)	FlagHeadIC |= Needle02;
		if((c32c1 >= (DR0709+1)) && (c32c1 < (BufferPRTSize+(DR0709+1))))
			if(*(BufferPrint[cBufferPrintLineRead].BufferB+c32c1-(DR0709+1))&0x10)	FlagHeadIC |= Needle07;
	}
	
	if(c32c2 == 7)
	{
		if((c32c1 >= DR0216) && (c32c1 < (BufferPRTSize+DR0216)))
			if(*(BufferPrint[cBufferPrintLineRead].BufferA+c32c1-DR0216)&0x01)		FlagHeadIC |= Needle16;
		if((c32c1 >= DR0709) && (c32c1 < (BufferPRTSize+DR0709)))
			if(*(BufferPrint[cBufferPrintLineRead].BufferB+c32c1-DR0709)&0x08)		FlagHeadIC |= Needle09;
	}
	if((c32c2 == 0)||(c32c2 == 1))
	{
		if((c32c1 >= (DR0216+1)) && (c32c1 < (BufferPRTSize+(DR0216+1))))
			if(*(BufferPrint[cBufferPrintLineRead].BufferA+c32c1-(DR0216+1))&0x01)	FlagHeadIC |= Needle16;
		if((c32c1 >= (DR0709+1)) && (c32c1 < (BufferPRTSize+(DR0709+1))))
			if(*(BufferPrint[cBufferPrintLineRead].BufferB+c32c1-(DR0709+1))&0x08)	FlagHeadIC |= Needle09;
	}
	
	HeadWork3F16End:
	HeadIC();
}

/*void TimPos_HeadWork3F16_Handler(void)
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
			if(*(BufferPrint[cBufferPrintLineRead].BufferA+c32c1-DR0810)&0x10)		FlagHeadIC |= Needle08;
	if((c32c2 == 1)||(c32c2 == 2)||(c32c2 == 3))
		if((c32c1 >= DR0810) && (c32c1 < (BufferPRTSize+DR0810)))
			if(*(BufferPrint[cBufferPrintLineRead].BufferA+c32c1-DR0810)&0x08)		FlagHeadIC |= Needle10;
	if((c32c2 == 2)||(c32c2 == 3)||(c32c2 == 4))
		if((c32c1 >= DR0612) && (c32c1 < (BufferPRTSize+DR0612)))
			if(*(BufferPrint[cBufferPrintLineRead].BufferA+c32c1-DR0612)&0x20)		FlagHeadIC |= Needle06;
	if((c32c2 == 3)||(c32c2 == 4)||(c32c2 == 5))
		if((c32c1 >= DR0612) && (c32c1 < (BufferPRTSize+DR0612)))
			if(*(BufferPrint[cBufferPrintLineRead].BufferA+c32c1-DR0612)&0x04)		FlagHeadIC |= Needle12;
	if((c32c2 == 4)||(c32c2 == 5)||(c32c2 == 6))
		if((c32c1 >= DR0414) && (c32c1 < (BufferPRTSize+DR0414)))
			if(*(BufferPrint[cBufferPrintLineRead].BufferA+c32c1-DR0414)&0x40)		FlagHeadIC |= Needle04;
	if((c32c2 == 5)||(c32c2 == 6)||(c32c2 == 7))
		if((c32c1 >= DR0414) && (c32c1 < (BufferPRTSize+DR0414)))
			if(*(BufferPrint[cBufferPrintLineRead].BufferA+c32c1-DR0414)&0x02)		FlagHeadIC |= Needle14;

	if((c32c2 == 6)||(c32c2 == 7))
		if((c32c1 >= DR0216) && (c32c1 < (BufferPRTSize+DR0216)))
			if(*(BufferPrint[cBufferPrintLineRead].BufferA+c32c1-DR0216)&0x80)		FlagHeadIC |= Needle02;
	if(c32c2 == 0)
		if((c32c1 >= (DR0216+1)) && (c32c1 < (BufferPRTSize+(DR0216+1))))
			if(*(BufferPrint[cBufferPrintLineRead].BufferA+c32c1-(DR0216+1))&0x80)	FlagHeadIC |= Needle02;

	if(c32c2 == 7)
		if((c32c1 >= DR0216) && (c32c1 < (BufferPRTSize+DR0216)))
			if(*(BufferPrint[cBufferPrintLineRead].BufferA+c32c1-DR0216)&0x01)		FlagHeadIC |= Needle16;
	if((c32c2 == 0)||(c32c2 == 1))
		if((c32c1 >= (DR0216+1)) && (c32c1 < (BufferPRTSize+(DR0216+1))))
			if(*(BufferPrint[cBufferPrintLineRead].BufferA+c32c1-(DR0216+1))&0x01)	FlagHeadIC |= Needle16;

	if((c32c2 == 0)||(c32c2 == 1)||(c32c2 == 2))
		if((c32c1 >= DR0115) && (c32c1 < (BufferPRTSize+DR0115)))
			if(*(BufferPrint[cBufferPrintLineRead].BufferB+c32c1-DR0115)&0x80)		FlagHeadIC |= Needle01;
	if((c32c2 == 1)||(c32c2 == 2)||(c32c2 == 3))
		if((c32c1 >= DR0115) && (c32c1 < (BufferPRTSize+DR0115)))
			if(*(BufferPrint[cBufferPrintLineRead].BufferB+c32c1-DR0115)&0x01)		FlagHeadIC |= Needle15;
	if((c32c2 == 2)||(c32c2 == 3)||(c32c2 == 4))
		if((c32c1 >= DR0313) && (c32c1 < (BufferPRTSize+DR0313)))
			if(*(BufferPrint[cBufferPrintLineRead].BufferB+c32c1-DR0313)&0x40)		FlagHeadIC |= Needle03;
	if((c32c2 == 3)||(c32c2 == 4)||(c32c2 == 5))
		if((c32c1 >= DR0313) && (c32c1 < (BufferPRTSize+DR0313)))
			if(*(BufferPrint[cBufferPrintLineRead].BufferB+c32c1-DR0313)&0x02)		FlagHeadIC |= Needle13;
	if((c32c2 == 4)||(c32c2 == 5)||(c32c2 == 6))
		if((c32c1 >= DR0511) && (c32c1 < (BufferPRTSize+DR0511)))
			if(*(BufferPrint[cBufferPrintLineRead].BufferB+c32c1-DR0511)&0x20)		FlagHeadIC |= Needle05;
	if((c32c2 == 5)||(c32c2 == 6)||(c32c2 == 7))
		if((c32c1 >= DR0511) && (c32c1 < (BufferPRTSize+DR0511)))
			if(*(BufferPrint[cBufferPrintLineRead].BufferB+c32c1-DR0511)&0x04)		FlagHeadIC |= Needle11;

	if((c32c2 == 6)||(c32c2 == 7))
		if((c32c1 >= DR0709) && (c32c1 < (BufferPRTSize+DR0709)))
			if(*(BufferPrint[cBufferPrintLineRead].BufferB+c32c1-DR0709)&0x10)		FlagHeadIC |= Needle07;
	if(c32c2 == 0)
		if((c32c1 >= (DR0709+1)) && (c32c1 < (BufferPRTSize+(DR0709+1))))
			if(*(BufferPrint[cBufferPrintLineRead].BufferB+c32c1-(DR0709+1))&0x10)	FlagHeadIC |= Needle07;

	if(c32c2 == 7)
		if((c32c1 >= DR0709) && (c32c1 < (BufferPRTSize+DR0709)))
			if(*(BufferPrint[cBufferPrintLineRead].BufferB+c32c1-DR0709)&0x08)		FlagHeadIC |= Needle09;
	if((c32c2 == 0)||(c32c2 == 1))
		if((c32c1 >= (DR0709+1)) && (c32c1 < (BufferPRTSize+(DR0709+1))))
			if(*(BufferPrint[cBufferPrintLineRead].BufferB+c32c1-(DR0709+1))&0x08)	FlagHeadIC |= Needle09;

	HeadWork3F16End:
	HeadIC();
}
*/
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
	if(CRGRightDir)
		c32c1 -= HeadDelayCount;
	c32c2 = CRGNowTimePosition%BufferOneDataTimePosition;

	if((c32c2 == 0)||(c32c2 == 1)||(c32c2 == 2)||(c32c2 == 3))
		if((c32c1 >= DR0810) && (c32c1 < (BufferPRTSize+DR0810)))
			if(*(BufferPrint[cBufferPrintLineRead].BufferA+c32c1-DR0810)&0x10)		FlagHeadIC |= Needle08;
	if((c32c2 == 1)||(c32c2 == 2)||(c32c2 == 3)||(c32c2 == 4))
		if((c32c1 >= DR0810) && (c32c1 < (BufferPRTSize+DR0810)))
			if(*(BufferPrint[cBufferPrintLineRead].BufferA+c32c1-DR0810)&0x08)		FlagHeadIC |= Needle10;
	if((c32c2 == 2)||(c32c2 == 3)||(c32c2 == 4)||(c32c2 == 5))
		if((c32c1 >= DR0612) && (c32c1 < (BufferPRTSize+DR0612)))
			if(*(BufferPrint[cBufferPrintLineRead].BufferA+c32c1-DR0612)&0x20)		FlagHeadIC |= Needle06;
	if((c32c2 == 3)||(c32c2 == 4)||(c32c2 == 5)||(c32c2 == 6))
		if((c32c1 >= DR0612) && (c32c1 < (BufferPRTSize+DR0612)))
			if(*(BufferPrint[cBufferPrintLineRead].BufferA+c32c1-DR0612)&0x04)		FlagHeadIC |= Needle12;
	if((c32c2 == 4)||(c32c2 == 5)||(c32c2 == 6)||(c32c2 == 7))
		if((c32c1 >= DR0414) && (c32c1 < (BufferPRTSize+DR0414)))
			if(*(BufferPrint[cBufferPrintLineRead].BufferA+c32c1-DR0414)&0x40)		FlagHeadIC |= Needle04;

	if((c32c2 == 5)||(c32c2 == 6)||(c32c2 == 7))
		if((c32c1 >= DR0414) && (c32c1 < (BufferPRTSize+DR0414)))
			if(*(BufferPrint[cBufferPrintLineRead].BufferA+c32c1-DR0414)&0x02)		FlagHeadIC |= Needle14;
	if(c32c2 == 0)
		if((c32c1 >= (DR0414+1)) && (c32c1 < (BufferPRTSize+(DR0414+1))))
			if(*(BufferPrint[cBufferPrintLineRead].BufferA+c32c1-(DR0414+1))&0x02)	FlagHeadIC |= Needle14;

	if((c32c2 == 6)||(c32c2 == 7))
		if((c32c1 >= DR0216) && (c32c1 < (BufferPRTSize+DR0216)))
			if(*(BufferPrint[cBufferPrintLineRead].BufferA+c32c1-DR0216)&0x80)		FlagHeadIC |= Needle02;
	if((c32c2 == 0)||(c32c2 == 1))
		if((c32c1 >= (DR0216+1)) && (c32c1 < (BufferPRTSize+(DR0216+1))))
			if(*(BufferPrint[cBufferPrintLineRead].BufferA+c32c1-(DR0216+1))&0x80)	FlagHeadIC |= Needle02;

	if(c32c2 == 7)
		if((c32c1 >= DR0216) && (c32c1 < (BufferPRTSize+DR0216)))
			if(*(BufferPrint[cBufferPrintLineRead].BufferA+c32c1-DR0216)&0x01)		FlagHeadIC |= Needle16;
	if((c32c2 == 0)||(c32c2 == 1)||(c32c2 == 2))
		if((c32c1 >= (DR0216+1)) && (c32c1 < (BufferPRTSize+(DR0216+1))))
			if(*(BufferPrint[cBufferPrintLineRead].BufferA+c32c1-(DR0216+1))&0x01)	FlagHeadIC |= Needle16;

	if((c32c2 == 0)||(c32c2 == 1)||(c32c2 == 2)||(c32c2 == 3))
		if((c32c1 >= DR0115) && (c32c1 < (BufferPRTSize+DR0115)))
			if(*(BufferPrint[cBufferPrintLineRead].BufferB+c32c1-DR0115)&0x80)		FlagHeadIC |= Needle01;
	if((c32c2 == 1)||(c32c2 == 2)||(c32c2 == 3)||(c32c2 == 4))
		if((c32c1 >= DR0115) && (c32c1 < (BufferPRTSize+DR0115)))
			if(*(BufferPrint[cBufferPrintLineRead].BufferB+c32c1-DR0115)&0x01)		FlagHeadIC |= Needle15;
	if((c32c2 == 2)||(c32c2 == 3)||(c32c2 == 4)||(c32c2 == 5))
		if((c32c1 >= DR0313) && (c32c1 < (BufferPRTSize+DR0313)))
			if(*(BufferPrint[cBufferPrintLineRead].BufferB+c32c1-DR0313)&0x40)		FlagHeadIC |= Needle03;
	if((c32c2 == 3)||(c32c2 == 4)||(c32c2 == 5)||(c32c2 == 6))
		if((c32c1 >= DR0313) && (c32c1 < (BufferPRTSize+DR0313)))
			if(*(BufferPrint[cBufferPrintLineRead].BufferB+c32c1-DR0313)&0x02)		FlagHeadIC |= Needle13;
	if((c32c2 == 4)||(c32c2 == 5)||(c32c2 == 6)||(c32c2 == 7))
		if((c32c1 >= DR0511) && (c32c1 < (BufferPRTSize+DR0511)))
			if(*(BufferPrint[cBufferPrintLineRead].BufferB+c32c1-DR0511)&0x20)		FlagHeadIC |= Needle05;

	if((c32c2 == 5)||(c32c2 == 6)||(c32c2 == 7))
		if((c32c1 >= DR0511) && (c32c1 < (BufferPRTSize+DR0511)))
			if(*(BufferPrint[cBufferPrintLineRead].BufferB+c32c1-DR0511)&0x04)		FlagHeadIC |= Needle11;
	if(c32c2 == 0)
		if((c32c1 >= (DR0511+1)) && (c32c1 < (BufferPRTSize+(DR0511+1))))
			if(*(BufferPrint[cBufferPrintLineRead].BufferB+c32c1-(DR0511+1))&0x04)	FlagHeadIC |= Needle11;

	if((c32c2 == 6)||(c32c2 == 7))
		if((c32c1 >= DR0709) && (c32c1 < (BufferPRTSize+DR0709)))
			if(*(BufferPrint[cBufferPrintLineRead].BufferB+c32c1-DR0709)&0x10)		FlagHeadIC |= Needle07;
	if((c32c2 == 0)||(c32c2 == 1))
		if((c32c1 >= (DR0709+1)) && (c32c1 < (BufferPRTSize+(DR0709+1))))
			if(*(BufferPrint[cBufferPrintLineRead].BufferB+c32c1-(DR0709+1))&0x10)	FlagHeadIC |= Needle07;

	if(c32c2 == 7)
		if((c32c1 >= DR0709) && (c32c1 < (BufferPRTSize+DR0709)))
			if(*(BufferPrint[cBufferPrintLineRead].BufferB+c32c1-DR0709)&0x08)		FlagHeadIC |= Needle09;
	if((c32c2 == 0)||(c32c2 == 1)||(c32c2 == 2))
		if((c32c1 >= (DR0709+1)) && (c32c1 < (BufferPRTSize+(DR0709+1))))
			if(*(BufferPrint[cBufferPrintLineRead].BufferB+c32c1-(DR0709+1))&0x08)	FlagHeadIC |= Needle09;

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
	if(CRGRightDir)
		c32c1 -= HeadDelayCount;
	c32c2 = CRGNowTimePosition%BufferOneDataTimePosition;

	if((c32c2 == 0)||(c32c2 == 1)||(c32c2 == 2)||(c32c2 == 3)||(c32c2 == 4))
		if((c32c1 >= DR0810) && (c32c1 < (BufferPRTSize+DR0810)))
			if(*(BufferPrint[cBufferPrintLineRead].BufferA+c32c1-DR0810)&0x10)		FlagHeadIC |= Needle08;
	if((c32c2 == 1)||(c32c2 == 2)||(c32c2 == 3)||(c32c2 == 4)||(c32c2 == 5))
		if((c32c1 >= DR0810) && (c32c1 < (BufferPRTSize+DR0810)))
			if(*(BufferPrint[cBufferPrintLineRead].BufferA+c32c1-DR0810)&0x08)		FlagHeadIC |= Needle10;
	if((c32c2 == 2)||(c32c2 == 3)||(c32c2 == 4)||(c32c2 == 5)||(c32c2 == 6))
		if((c32c1 >= DR0612) && (c32c1 < (BufferPRTSize+DR0612)))
			if(*(BufferPrint[cBufferPrintLineRead].BufferA+c32c1-DR0612)&0x20)		FlagHeadIC |= Needle06;
	if((c32c2 == 3)||(c32c2 == 4)||(c32c2 == 5)||(c32c2 == 6)||(c32c2 == 7))
		if((c32c1 >= DR0612) && (c32c1 < (BufferPRTSize+DR0612)))
			if(*(BufferPrint[cBufferPrintLineRead].BufferA+c32c1-DR0612)&0x04)		FlagHeadIC |= Needle12;

	if((c32c2 == 4)||(c32c2 == 5)||(c32c2 == 6)||(c32c2 == 7))
		if((c32c1 >= DR0414) && (c32c1 < (BufferPRTSize+DR0414)))
			if(*(BufferPrint[cBufferPrintLineRead].BufferA+c32c1-DR0414)&0x40)		FlagHeadIC |= Needle04;
	if(c32c2 == 0)
		if((c32c1 >= (DR0414+1)) && (c32c1 < (BufferPRTSize+(DR0414+1))))
			if(*(BufferPrint[cBufferPrintLineRead].BufferA+c32c1-(DR0414+1))&0x40)	FlagHeadIC |= Needle04;

	if((c32c2 == 5)||(c32c2 == 6)||(c32c2 == 7))
		if((c32c1 >= DR0414) && (c32c1 < (BufferPRTSize+DR0414)))
			if(*(BufferPrint[cBufferPrintLineRead].BufferA+c32c1-DR0414)&0x02)		FlagHeadIC |= Needle14;
	if((c32c2 == 0)||(c32c2 == 1))
		if((c32c1 >= (DR0414+1)) && (c32c1 < (BufferPRTSize+(DR0414+1))))
			if(*(BufferPrint[cBufferPrintLineRead].BufferA+c32c1-(DR0414+1))&0x02)	FlagHeadIC |= Needle14;

	if((c32c2 == 6)||(c32c2 == 7))
		if((c32c1 >= DR0216) && (c32c1 < (BufferPRTSize+DR0216)))
			if(*(BufferPrint[cBufferPrintLineRead].BufferA+c32c1-DR0216)&0x80)		FlagHeadIC |= Needle02;
	if((c32c2 == 0)||(c32c2 == 1)||(c32c2 == 2))
		if((c32c1 >= (DR0216+1)) && (c32c1 < (BufferPRTSize+(DR0216+1))))
			if(*(BufferPrint[cBufferPrintLineRead].BufferA+c32c1-(DR0216+1))&0x80)	FlagHeadIC |= Needle02;

	if(c32c2 == 7)
		if((c32c1 >= DR0216) && (c32c1 < (BufferPRTSize+DR0216)))
			if(*(BufferPrint[cBufferPrintLineRead].BufferA+c32c1-DR0216)&0x01)		FlagHeadIC |= Needle16;
	if((c32c2 == 0)||(c32c2 == 1)||(c32c2 == 2)||(c32c2 == 3))
		if((c32c1 >= (DR0216+1)) && (c32c1 < (BufferPRTSize+(DR0216+1))))
			if(*(BufferPrint[cBufferPrintLineRead].BufferA+c32c1-(DR0216+1))&0x01)	FlagHeadIC |= Needle16;

	if((c32c2 == 0)||(c32c2 == 1)||(c32c2 == 2)||(c32c2 == 3)||(c32c2 == 4))
		if((c32c1 >= DR0115) && (c32c1 < (BufferPRTSize+DR0115)))
			if(*(BufferPrint[cBufferPrintLineRead].BufferB+c32c1-DR0115)&0x80)		FlagHeadIC |= Needle01;
	if((c32c2 == 1)||(c32c2 == 2)||(c32c2 == 3)||(c32c2 == 4)||(c32c2 == 5))
		if((c32c1 >= DR0115) && (c32c1 < (BufferPRTSize+DR0115)))
			if(*(BufferPrint[cBufferPrintLineRead].BufferB+c32c1-DR0115)&0x01)		FlagHeadIC |= Needle15;
	if((c32c2 == 2)||(c32c2 == 3)||(c32c2 == 4)||(c32c2 == 5)||(c32c2 == 6))
		if((c32c1 >= DR0313) && (c32c1 < (BufferPRTSize+DR0313)))
			if(*(BufferPrint[cBufferPrintLineRead].BufferB+c32c1-DR0313)&0x40)		FlagHeadIC |= Needle03;
	if((c32c2 == 3)||(c32c2 == 4)||(c32c2 == 5)||(c32c2 == 6)||(c32c2 == 7))
		if((c32c1 >= DR0313) && (c32c1 < (BufferPRTSize+DR0313)))
			if(*(BufferPrint[cBufferPrintLineRead].BufferB+c32c1-DR0313)&0x02)		FlagHeadIC |= Needle13;

	if((c32c2 == 4)||(c32c2 == 5)||(c32c2 == 6)||(c32c2 == 7))
		if((c32c1 >= DR0511) && (c32c1 < (BufferPRTSize+DR0511)))
			if(*(BufferPrint[cBufferPrintLineRead].BufferB+c32c1-DR0511)&0x20)		FlagHeadIC |= Needle05;
	if(c32c2 == 0)
		if((c32c1 >= (DR0511+1)) && (c32c1 < (BufferPRTSize+(DR0511+1))))
			if(*(BufferPrint[cBufferPrintLineRead].BufferB+c32c1-(DR0511+1))&0x20)	FlagHeadIC |= Needle05;

	if((c32c2 == 5)||(c32c2 == 6)||(c32c2 == 7))
		if((c32c1 >= DR0511) && (c32c1 < (BufferPRTSize+DR0511)))
			if(*(BufferPrint[cBufferPrintLineRead].BufferB+c32c1-DR0511)&0x04)		FlagHeadIC |= Needle11;
	if((c32c2 == 0)||(c32c2 == 1))
		if((c32c1 >= (DR0511+1)) && (c32c1 < (BufferPRTSize+(DR0511+1))))
			if(*(BufferPrint[cBufferPrintLineRead].BufferB+c32c1-(DR0511+1))&0x04)	FlagHeadIC |= Needle11;

	if((c32c2 == 6)||(c32c2 == 7))
		if((c32c1 >= DR0709) && (c32c1 < (BufferPRTSize+DR0709)))
			if(*(BufferPrint[cBufferPrintLineRead].BufferB+c32c1-DR0709)&0x10)		FlagHeadIC |= Needle07;
	if((c32c2 == 0)||(c32c2 == 1)||(c32c2 == 2))
		if((c32c1 >= (DR0709+1)) && (c32c1 < (BufferPRTSize+(DR0709+1))))
			if(*(BufferPrint[cBufferPrintLineRead].BufferB+c32c1-(DR0709+1))&0x10)	FlagHeadIC |= Needle07;

	if(c32c2 == 7)
		if((c32c1 >= DR0709) && (c32c1 < (BufferPRTSize+DR0709)))
			if(*(BufferPrint[cBufferPrintLineRead].BufferB+c32c1-DR0709)&0x08)		FlagHeadIC |= Needle09;
	if((c32c2 == 0)||(c32c2 == 1)||(c32c2 == 2)||(c32c2 == 3))
		if((c32c1 >= (DR0709+1)) && (c32c1 < (BufferPRTSize+(DR0709+1))))
			if(*(BufferPrint[cBufferPrintLineRead].BufferB+c32c1-(DR0709+1))&0x08)	FlagHeadIC |= Needle09;

	HeadWork5F16End:
	HeadIC();
}


/**************************************************************************************************************
	McuInt.c	McuInt.h
**************************************************************************************************************/
// 系统定时器，需要每20毫秒执行一次 static ___interrupt TSEC void timer_isr2(void)
// 可以不再放到定时器中，由于没有死循环，可在大循环中执行
// 采用循环方式依次扫描各个任务，以此减少该中断执行时间
void Tim_Sys_Handler(void)
{
//	static volatile unsigned char c8ForScan = 0;	// 0~4: 5次一个循环，各个模块依次扫描执行
//	static volatile unsigned char c8ForKeyAdc = 5;	// KEYADC按键扫描
	static volatile unsigned char c8ForPsPaper = 0;	// 纸张传感器扫描
	static volatile unsigned char c8ForPower = 0;	// KEYPOWER开关机处理
//	static volatile unsigned int c16ForCRG = 0;
	static volatile unsigned int c16ForPAP = 0;
//	volatile unsigned int c16c1 = 0;

//PowerState=21;

	// Power on/off function
	switch(PowerState)
	{
	case 0:	// 上电确认状态
		if(KeyPower)
			c8ForPower = 0;
		if(c8ForPower > 1) //
		{
			LedLineOpen
			LedErrOpen
			MainPowerOn
			PowerState = 1;
		}
		break;
	case 1: // 上电已经确认，等待按键松开
		if(KeyPower)
		{
			LedLineClose
			LedErrClose
			PowerState = 10;
		}
		break;
	case 10: // 上电过程完成，正常开机保持状态
		if(KeyPower)
			c8ForPower = 0;
		if(c8ForPower > 4*5)
		{
			PowerState = 20;
		}
		break;
	case 20: // 1s内松开按键没有影响
		if(KeyPower)
		{
			PowerState = 10;
		}
		if(c8ForPower > 4*5)// 500*2ms=1000ms
		{
			c8ForPower = 0;
			PowerState = 21;
		}
		break;
	default:
	case 21: // 2s闪烁led等待人工关机确认
		MainPowerOff

		if(c8ForPower%(4*5) == 0)
		{
			if(LedErr)
				LedErrOpen
			else
				LedErrClose
		}
		if(c8ForPower >= 15*4)
			c8ForPower = 0;
		break;
	}
	if(c8ForPower < 0xff)
		c8ForPower++;

	// 系统延时计数器
	if(DelayTimeCount20mS)
		DelayTimeCount20mS--;

	// 检查确认打印缓冲区
	if((cBufferPrintLineRead != cBufferPrintLineWrite) && (BufferPrint[cBufferPrintLineRead].cLeft == BufferPrint[cBufferPrintLineRead].cRight) && (BufferPrint[cBufferPrintLineRead].Step == 0))
	{
		if(cBufferPrintLineRead >= (BufferPrintLineNum-1))
			cBufferPrintLineRead = 0;
		else
			cBufferPrintLineRead++;
	}

	// 启动字车马达初始化
	if(CRGState != 0x55)
	{
		if(BufferPrintLineWrited() < BufferPrintLineFull)
		{
			BufferPrint[cBufferPrintLineWrite].cLeft = 0;
			BufferPrint[cBufferPrintLineWrite].cRight = TimePositionAll;
			BufferPrint[cBufferPrintLineWrite].Step = 0;
				
			if(cBufferPrintLineWrite >= (BufferPrintLineNum-1))
				cBufferPrintLineWrite = 0;
			else
				cBufferPrintLineWrite++;

			BufferPrint[cBufferPrintLineWrite].cLeft = 0;
			BufferPrint[cBufferPrintLineWrite].cRight = TimePositionAll;
			BufferPrint[cBufferPrintLineWrite].Step = 0;
					
			if(cBufferPrintLineWrite >= (BufferPrintLineNum-1))
				cBufferPrintLineWrite = 0;
			else
				cBufferPrintLineWrite++;

			BufferPrint[cBufferPrintLineWrite].cLeft = 0;
			BufferPrint[cBufferPrintLineWrite].cRight = TimePositionMiddleForPaperFeed;
			BufferPrint[cBufferPrintLineWrite].Step = 0;
					
			if(cBufferPrintLineWrite >= (BufferPrintLineNum-1))
				cBufferPrintLineWrite = 0;
			else
				cBufferPrintLineWrite++;
		}
	}
	


	// HeadTempAdc
	ADCTemp = adc_get_value(AdcChTemp);
	if(ADCTemp < AdcTempLever) // 数值过小表明温度过高
		TMPOneDirSet
	else if(ADCTemp > 900) // 数值接近满副1023表明热敏电阻断开
		TMPOneDirSet
	else
		TMPTwoDirSet
	
	// UsbInAdc
	//ADCUsb = adc_get_value(AdcChUsb);
	
	// KeyAdc scan
	ADCKEY = adc_get_value(AdcChKey);
	KeyLineRecorder <<= 1;
	KeyPaperRecorder <<= 1;
	if(ADCKEY > ADC6F8)
	{
		KeyLineRecorder |= 0x01;
		if(KeyLineRecorder == 0xff)
			KeyLineDownTime++;
	}
	else if(ADCKEY > ADC2F8)
	{
		KeyPaperRecorder |= 0x01;
		if(KeyPaperRecorder == 0xff)
			KeyPaperDownTime++;
	}
	else
	{
		KeyLineDownTime = 0;
		KeyPaperDownTime = 0;
	}
#if 0
	if(FlagKeyPadNo == 0x0000)
	{
		if(KeyPaperRecorder == 0xf0)
		{
			if(BufferPrintLineWrited() < BufferPrintLineFull)
			{
				BufferPrint[cBufferPrintLineWrite].Step = 32;
				BufferPrint[cBufferPrintLineWrite].Dir = sPaperDirFrontToBack;

				if(cBufferPrintLineWrite >= (BufferPrintLineNum-1))
					cBufferPrintLineWrite = 0;
				else
					cBufferPrintLineWrite++;
			}
		}
		if(KeyLineRecorder == 0xf0)
		{
			if(BufferPrintLineWrited() < BufferPrintLineFull)
			{
				BufferPrint[cBufferPrintLineWrite].Step = 32;
				BufferPrint[cBufferPrintLineWrite].Dir = sPaperDirBackToFront;

				if(cBufferPrintLineWrite >= (BufferPrintLineNum-1))
					cBufferPrintLineWrite = 0;
				else
					cBufferPrintLineWrite++;
			}
		}

		if((KeyPaperRecorder == 0xff) && (KeyPaperDownTime > 50))
		{
			if(BufferPrintLineWrited() < BufferPrintLineFull)
			{
				BufferPrint[cBufferPrintLineWrite].Step = 32;
				BufferPrint[cBufferPrintLineWrite].Dir = sPaperDirFrontToBack;

				/*if(cBufferPrintLineWrite >= (BufferPrintLineNum-1))
					cBufferPrintLineWrite = 0;
				else
					cBufferPrintLineWrite++;*/
			}
		}
		else if((KeyLineRecorder == 0xff) && (KeyLineDownTime > 50))
		{
			if(BufferPrintLineWrited() < BufferPrintLineFull)
			{
				BufferPrint[cBufferPrintLineWrite].Step = 32;
				BufferPrint[cBufferPrintLineWrite].Dir = sPaperDirBackToFront;

				/*if(cBufferPrintLineWrite >= (BufferPrintLineNum-1))
					cBufferPrintLineWrite = 0;
				else
					cBufferPrintLineWrite++;*/
			}
		}
		else
		{
			if(BufferPrint[cBufferPrintLineWrite].Step > 32)
				BufferPrint[cBufferPrintLineWrite].Step = 32;
			if(cBufferPrintLineWrite >= (BufferPrintLineNum-1))
				cBufferPrintLineWrite = 0;
			else
				cBufferPrintLineWrite++;
		}
	}
#endif

	
#if 0
printf("PaperState:  %d  PaperCount: %d \r\n", PaperState, PaperCount);	

	if(c8ForPsPaper < 250)
		c8ForPsPaper++;
	else
	{
		c8ForPsPaper = 0;
	#if DebugMsg
		if(ADCTemp < AdcTempLever) // 数值过小表明温度过高
			printf("OverTemp!\n");
	#endif
	/*	ADCKEY = adc_get_value(AdcChKey);
		c16c1 = adc_get_voltage(AdcChKey);
		printf("ADCKEY = %d  >>>  %d mv    ", ADCKEY, c16c1);
		ADCUsb = adc_get_value(AdcChUsb);
		c16c1 = adc_get_voltage(AdcChUsb);
		printf("ADCUsb = %d  >>>  %d mv    ", ADCUsb, c16c1);
		if(USBin)
			printf("UsbIn = High  ");
		else
			printf("UsbIn = Low  ");*/
	/*	ADCTemp = adc_get_value(AdcChTemp);
		c16c1 = adc_get_voltage(AdcChTemp);
		printf("ADCTemp = %d  >>>  %d mv\n", ADCTemp, c16c1);*/
		
	/*	ADCKEY = adc_get_value(AdcChFrontOut);
		c16c1 = adc_get_voltage(AdcChFrontOut);
		printf("AdcChFO  = %d  >>>  %d mv    ", ADCKEY, c16c1);
		ADCKEY = adc_get_value(AdcChFrontIn);
		c16c1 = adc_get_voltage(AdcChFrontIn);
		printf("AdcChFI = %d  >>>  %d mv\n", ADCKEY, c16c1);
		
		ADCKEY = adc_get_value(AdcChBackOut);
		c16c1 = adc_get_voltage(AdcChBackOut);
		printf("AdcChBO = %d  >>>  %d mv    ", ADCKEY, c16c1);
		ADCKEY = adc_get_value(AdcChBackIn);
		c16c1 = adc_get_voltage(AdcChBackIn);
		printf("AdcChBI = %d  >>>  %d mv\n", ADCKEY, c16c1);

		//printf("PowerState:  %d  CRGState: %d \r\n", PowerState, CRGState);
		printf("PaperState:  %d  FO: %d FI: %d BI: %d BO: %d \r\n", PaperState, SensorRecorderFrontOut, SensorRecorderFrontIn, SensorRecorderBackIn, SensorRecorderBackOut);*/
	}
#endif

#if 0
	// WIFI配置过程的不同阶段，暂时不使用
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
#endif

#if 0
	HeadState
#endif

#if 1
	//LedDisplay
	if(PowerState == 10) // 只有开关机完成后，才会体现其他状态的LED，即电源正常状态才体现其他的状态
	{
		if(USBin)
			LedLineOpen
		else
			LedLineClose
			
		if((PaperState == sPaperFrontNormal)||(PaperState == sPaperBackNormal))
			LedErrClose
		else
			LedErrOpen
	}
#endif


	if(c8ForPsPaper < 50*2)
		c8ForPsPaper++;
	else
	{
		c8ForPsPaper = 0;

		printf("%#x-%d-%d-%#x-\r\n", PAPState, PaperState, PaperCount, FlagMotorIC);
	}
}
// 走纸定时器扫描时间必须要少于字车扫描时间：字车timer会启动走纸，需要保证走纸timer及时启动
// @20230616对上面的策略进行调整：考虑到可能会使用0a，所以走纸启动，必须要经过字车启动后，再启动走纸；可以是打印后再走纸，或者无需打印启动走纸
// 只有  0D  0A走纸结束后，才会调整打印缓冲区指针
// 后续优化的话，可以改用启动timer的方式进行: @20231010不会再使用：马达停止时间隔一定时间扫扫描传感器状态，只是用一个timer完成走纸马达操作
#define TimePAPScan	10000//2000	// 10ms也刚好可以做马达的定位和刹车时间
#define TimeCRGScan	10000//2000	//
unsigned long Tim_PAP_Handler(void)
{
	unsigned long c32time = TimePAPScan;
	static unsigned int c16ForPAP;

	// 该定时器内始终检测传感器状态，当马达为停止状态时，扫描时间为TimePAPScan，以便启动走纸马达
	// 当马达运动时，就是每步检测传感器状态
	// PaperSensor scan
	SensorRecorderFrontOut <<= 1;	if(adc_get_value(AdcChFrontOut) < ADC4F8)	SensorRecorderFrontOut |= 0x01;
	SensorRecorderFrontIn <<= 1;	if(adc_get_value(AdcChFrontIn) < ADC4F8)	SensorRecorderFrontIn |= 0x01;
	SensorRecorderBackIn <<= 1;		if(adc_get_value(AdcChBackIn) < ADC4F8)		SensorRecorderBackIn |= 0x01;
	SensorRecorderBackOut <<= 1;	if(adc_get_value(AdcChBackOut) < ADC4F8)	SensorRecorderBackOut |= 0x01;
	// 根据纸张传感器状态决定装纸逻辑
	#if DebugPaper
		PaperState = sPaperBackNormal;
	#endif
	
	switch (PrintingState)
	{
	case sINIT:
		// 1优先走纸马达初始化
		if(PAPState==0xff) // 先后走
		{
			PAPState = 0xaa;
			PAPBackToFrontSet
			PAPMoveStep = 4;
			PrintingState = sStartPAPMotor;
		}
		else if(PAPState==0xaa) // 再前走
		{
			PAPState = 0x55;
			PAPFrontToBackSet
			PAPMoveStep = 4;
			PrintingState = sStartPAPMotor;
		}
		
		// 2根据纸张传感器状态完成纸张初始化
		else if(PAPState==0x55)
		{
			switch(PaperState)
			{
			default:
			case sPaperNo:
				// 静态检测传感器状态，以便确定纸张状态
				if((SensorRecorderFrontOut == 0xff)&&(SensorRecorderFrontIn == 0xff))
				{
					if((SensorRecorderBackOut == 0xff)&&(SensorRecorderBackIn == 0xff))
					{
						PaperCount = StepBackToPrint; //StepToCutPos	// 没有页长概念，认为在打印起始位；如有页长概念，应该在撕纸位置
						PaperState = sPaperBackNormal; 	// 前后都有纸，后进纸模式: 后进纸为连续纸，暂时没有页长概念
					}
					else
						PaperState = sPaperFrontEnd; 	// 仅前边有纸，需要退纸出去再装纸
				}
				else if((SensorRecorderBackOut == 0xff)&&(SensorRecorderBackIn == 0xff))
					PaperState = sPaperBackInit; 		// 仅后部有纸，后进纸模式，需要定位纸张边界
				else									// 前后都无纸，需要装纸确定状态，就是当前状态
				{
					if((SensorRecorderFrontOut == 0xff)&&(SensorRecorderFrontIn == 0x00))
					{
						if(c16ForPAP < 50*2)
							c16ForPAP++;
						else
							PaperState = sPaperFrontInit; // 前进纸
					}
				}
				break;
	
			case sPaperFrontInit:
				PAPFrontToBackSet
				PAPMoveStep = StepFrontToPrint*2;
				break;

			//case sPaperFrontNormal:
				//break;

			case sPaperFrontEnd: // 有两种情况：初始前纸后无，仅退纸少许；打印结束，前无后纸，需退超过页长
				PAPBackToFrontSet
				if(SensorRecorderFrontOut==0xff)
					PAPMoveStep = StepFrontToPrint*2;				// 初始前纸后无，仅退纸少许；
				else
					PAPMoveStep = StepPageLenth+StepFrontToPrint*2;	// 打印结束，前无后纸，需退超过页长 
				break;

			case sPaperBackInit:
				PAPBackToFrontSet
				PAPMoveStep = StepBackToPrint*2;
				break;

			//case sPaperBackNormal:
				//break;
			
			//case sPaperBackEnd:
				//break;
			}
			//停止走纸时，定时扫描纸张传感器状态，用于启动马达
		}

		// 3根据打印缓冲区及各种状态标记，填写10页的打印缓冲区
		// 3命令解析后填写的打印缓冲区，结合其他情况，直接写PAPMoveStep，启动马达运动，打印命令中走纸步数需要在此乘以2
		else if(PAPMoveStep == 0)
		{
			if(	(CRGState==0x55) && ((PaperState==sPaperFrontNormal)||(PaperState==sPaperBackNormal)) &&
				(cBufferPrintLineRead != cBufferPrintLineWrite) && (BufferPrint[cBufferPrintLineRead].Step)	)
			{
				if(BufferPrint[cBufferPrintLineRead].Dir == sPaperDirFrontToBack)
					PAPFrontToBackSet
				else
					PAPBackToFrontSet
				PAPMoveStep = BufferPrint[cBufferPrintLineRead].Step*2;
				PrintingState = sStartPAPMotor;
			}
		}
		
		// 只根据走纸步数决定是否启动马达
		if(PAPMoveStep)
		{
			PrintingState = sStartPAPMotor;
		}
		else if((FlagMotorIC&0x30) == 0x00) // 如果全电流，则切换到小电流
		{
			c16ForPAP = 0;
			PAPCurrent(1);
		}
		else if((FlagMotorIC&0x30) != 0x30) // 如果没有关闭电流，则超时关闭电流
		{
			if(c16ForPAP < 100)  // TimePAPScan*100=1000=1s
				c16ForPAP++;
			else
				PAPCurrent(0);
		}
		else
			c16ForPAP = 0;
		break;

	case sStartPAPMotor:
		cPAPSpeed = 0;
		if((FlagMotorIC&0x30) == 0x30)
			PAPCurrent(1);
		else if((FlagMotorIC&0x30) != 0x00)
		{
			PAPCurrent(2);
			PrintingState = sWaitingPAPMotor;
		}
		c32time = *TABLEPAPSpeed; // Hold Time
		break;

	case sWaitingPAPMotor:
		if(PAPMoveStep)
		{
			// 需要考虑初始化及页末情况，动态更新该状态
			if(PaperState==sPaperFrontNormal)													// 正常状态，能够正常打印
			{
				PaperCount++;
				// 走纸状态下,相同方向下补充走纸步数
				if((cBufferPrintLineRead != cBufferPrintLineWrite) && (BufferPrint[cBufferPrintLineRead].Step))
				{
					if(BufferPrint[cBufferPrintLineRead].Dir == sPaperDirFrontToBack)
					{
						PAPMoveStep += (BufferPrint[cBufferPrintLineRead].Step)*2;
						BufferPrint[cBufferPrintLineRead].Step = 0;
			
						if(cBufferPrintLineRead >= (BufferPrintLineNum-1))
							cBufferPrintLineRead = 0;
						else
							cBufferPrintLineRead++;
					}
				}

				if(SensorRecorderFrontIn!=0x00)
					StepPageLenth = PaperCount;
				else if(PaperCount>=(StepPageLenth+StepFrontToPrint-32))						// 纸尾已过打印位置，进入无纸状态
				{
					if(PAPMoveStep > PAPVolatileStep)
						PAPMoveStep = PAPVolatileStep;
					PaperState = sPaperFrontEnd;												// 切换到纸张结束状态，等待马达停止后，再退纸出来
				}
			}
			else if(PaperState==sPaperBackNormal)												// 正常状态，能够正常打印
			{
				PaperCount++;
				// 走纸状态下,相同方向下补充走纸步数
				if((cBufferPrintLineRead != cBufferPrintLineWrite) && (BufferPrint[cBufferPrintLineRead].Step))
				{
					if(BufferPrint[cBufferPrintLineRead].Dir == sPaperDirBackToFront)
					{
						PAPMoveStep += (BufferPrint[cBufferPrintLineRead].Step)*2;
						BufferPrint[cBufferPrintLineRead].Step = 0;
			
						if(cBufferPrintLineRead >= (BufferPrintLineNum-1))
							cBufferPrintLineRead = 0;
						else
							cBufferPrintLineRead++;
					}
				}

				if(SensorRecorderBackIn!=0x00)
					StepPageLenth = PaperCount;
				else if(PaperCount>=(StepPageLenth+StepBackToPrint-32))							// 纸尾已过打印位置，进入无纸状态
				{
					PAPMoveStep += StepBackToPrint*2;
					PaperState = sPaperNo;														// 继续向前把纸走出来，进入无纸待定状态
				}
			}
			else if(PaperState==sPaperFrontInit)										// 前进纸初始化状态
			{
				PaperCount++;
				if((SensorRecorderFrontOut==0xff)&&(SensorRecorderFrontIn==0xff))		// 2进入有纸状态
				{
					PaperState = sPaperFrontNormal;
				}
				else if((SensorRecorderFrontOut==0xff)&&(SensorRecorderFrontIn==0x01))	// 1检测到页首，调整步数到达页首
				{
					PaperCount = 1;
					PAPMoveStep = StepFrontToPrint;
				}
				else if((SensorRecorderFrontOut==0x00)&&(SensorRecorderFrontIn==0x00))	// 3无纸返回初始状态
				{
					if(PAPMoveStep > PAPVolatileStep)
						PAPMoveStep = PAPVolatileStep;
					PaperState = sPaperNo;
				}
			}
			else if(PaperState==sPaperBackInit)
			{
				PaperCount++;
				if((SensorRecorderBackOut==0xff)&&(SensorRecorderBackIn==0xff))			// 2进入有纸状态
				{
					PaperState = sPaperBackNormal;
				}
				else if((SensorRecorderBackOut==0xff)&&(SensorRecorderBackIn==0x01))	// 1检测到页首，调整步数到达页首
				{
					PaperCount = 0;
					PAPMoveStep = StepBackToPrint;
				}
				else if((SensorRecorderBackOut==0x00)&&(SensorRecorderBackIn==0x00))	// 3无纸返回初始状态
				{
					if(PAPMoveStep > PAPVolatileStep)
						PAPMoveStep = PAPVolatileStep;
					PaperState = sPaperNo;
				}
			}
		
			PAPPhase();
			if(PAPMoveStep > (cPAPSpeed+1))
			{
				if(cPAPSpeed < PAPVolatileStep)
					cPAPSpeed++;
			}
			else if(PAPMoveStep < (cPAPSpeed+1)) //Alke 20230703 <= (-->)<
			{
				if(cPAPSpeed)
					cPAPSpeed--;
			}
			PAPMoveStep--;

			//PaperCount++;
			if(PAPState != 0x55)
				PAPState = 0x55;

			c32time = *(TABLEPAPSpeed+cPAPSpeed);
		}
		else
		{
			// 字车是在结束后清除打印缓冲区标志，但是走纸是在启动时就清除相关标志，以便能够在走纸过程中补充走纸步数使得走纸连贯不顿挫
			PAPCurrent(1);
			PrintingState = sINIT;
			c32time = *TABLEPAPSpeed;
		}
		break;
	}
	
	return c32time;
}

unsigned long TimPos_CRG_Handler(void)
{
	// 必须使用static，每16次才会计算一次时间，其他15次都需要使用上次的时间
	static unsigned long c32time = TimeCRGScan*CRGOneStepTimePosition; // 此处时间要16倍除的
	unsigned short Count;
	static unsigned int c16ForCRG;

	switch (PrintingState)
	{
	case sINIT:
//	case sStartPAPMotor:
//	case sWaitingPAPMotor:
		if((cBufferPrintLineRead != cBufferPrintLineWrite) && ((BufferPrint[cBufferPrintLineRead].cLeft < BufferPrint[cBufferPrintLineRead].cRight)) && (!PAPRunning))//&&((PaperState==sPaperFrontNormal)||(PaperState==sPaperFrontNormal)))
		{
			/*if(CRGState==0x55)
			{
				if((PaperState==sPaperFrontNormal)||(PaperState==sPaperBackNormal))
					PrintingState = sStartCRGMotor;
			}
			else*/
				PrintingState = sStartCRGMotor;
		}
		else // 如果还是小电流待机，则超时关闭马达
		{
			if((FlagMotorIC&0x03) == 0x01)
			{
				if(c16ForCRG < 100) // 1000
					c16ForCRG++;
				else
					CRGCurrent(0);
			}
			else
				c16ForCRG = 0;
		}
		
		c32time = TimeCRGScan; // 没有打印任务时，间隔多长时间扫描打印缓冲区
		break;
		
	case sStartCRGMotor:
		if(CRGNowTimePosition <= BufferPrint[cBufferPrintLineRead].cLeft)
		{
			CRGStopTimePosition = BufferPrint[cBufferPrintLineRead].cRight;
			CRGRightSet
			FlagSystemPrintSet
		}
		else if(CRGNowTimePosition >= BufferPrint[cBufferPrintLineRead].cRight)
		{
			CRGStopTimePosition = BufferPrint[cBufferPrintLineRead].cLeft;
			CRGLeftSet
			if(TMPOneDir || CMDOneDir)
				FlagSystemPrintClr
			else
				FlagSystemPrintSet
		}
		else
		{
			if(TMPOneDir || CMDOneDir)//if(TMPOneDir)//if(TMPOneDir || CMDOneDir)
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
		}
		cCRGSpeed = 0;
		CRGCurrent(2);
		PrintingState = sWaitingCRGMotor;
		c32time = *TABLECRGSpeedRightAcc; // Hold Time
		break;

	case sWaitingCRGMotor:
		/** 马达匀速运动时，可以在适当时间进行出针操作，每个时间片位置都需要计算考虑出针 **/
		if(FlagSystemPrint&&(CRGState == 0x55)&&((PaperState == sPaperFrontNormal)||(PaperState == sPaperBackNormal))) // if(FlagSystemPrint && CRGRunning)
		{
			if(FlagPrinter == eFlagPRTFast)
				TimPos_HeadWork3F16_Handler();
			else if(FlagPrinter == eFlagPRTStand)
				TimPos_HeadWork4F16_Handler();
			else if(FlagPrinter == eFlagPRTDeep)
				TimPos_HeadWork5F16_Handler();
		}

		// 马达操作需要每16次中断才执行一次
		if((CRGNowTimePosition%CRGOneStepTimePosition) == 0)
		{
			// 马达运动结束
			/*if(CRGNowTimePosition == CRGStopTimePosition)
			{
				// 根据打印标志来确定本次是空跑位置定位还是真正打印
				if(FlagSystemPrint)
				{
					BufferPrint[cBufferPrintLineRead].cLeft = 0;
					BufferPrint[cBufferPrintLineRead].cRight = 0;

					if(cBufferPrintLineRead >= (BufferPrintLineNum-1))
						cBufferPrintLineRead = 0;
					else
						cBufferPrintLineRead++;
					
					// 此处仅仅启动马达小电流
					// 彻底关闭马达电流需要在系统定时器中完成
					CRGCurrent(1);
					PrintingState = sINIT;
				}
				else
				{
					// 刚刚没有打印，仅仅完成了马达初始定位，现在再次启动马达打印
					PrintingState = sStartCRGMotor;
				}
				c32time = *TABLECRGSpeedRightAcc; //Lash Time
			}
			// 马达运动中
			else*/
			if(CRGNowTimePosition != CRGStopTimePosition)
			{
				CRGPhase();
	
				// 后进纸方向视角，左边为0点，传感器在左边，字车马达初始化过程
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
					else if(CRGState == 0xfe)				// to right
					{
						CRGNowTimePosition = TimePositionInHome+1*CRGOneStepTimePosition;
						CRGState = 0x55;					// Finished INIT
					}
				}

				// 计算时间片位置信息，重新计算中断时间
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
				else if(Count < (cCRGSpeed+1)) //Alke 20230703 <= (-->)<
				{
					if(cCRGSpeed)
					{
						cCRGSpeed--;
					}
				}
				
				// 计算加减速时间
				/*if(CMDOneDir||TMPOneDir) // one direction print
				{
					if(CRGRightDir)
					{
						if(Count > (cCRGSpeed+1))
							c32time = *(TABLECRGSpeedAccOneDirPrint+cCRGSpeed);// acc
						else
							c32time = *(TABLECRGSpeedDecOneDirPrint+cCRGSpeed);// dec
					}
					else
					{
						if(Count > (cCRGSpeed+1))
							c32time = *(TABLECRGSpeedAccDecOneDirNoprint+cCRGSpeed);// acc
						else
							c32time = *(TABLECRGSpeedAccDecOneDirNoprint+cCRGSpeed);// dec
					}
				}
				else // double direction print*/
				{
					if(CRGLeftDir)
					{
						if(Count > (cCRGSpeed+1))
							c32time = *(TABLECRGSpeedLeftAcc+cCRGSpeed);
						else
							c32time = *(TABLECRGSpeedLeftDec+cCRGSpeed);
					}
					else
					{
						if(Count > (cCRGSpeed+1))
							c32time = *(TABLECRGSpeedRightAcc+cCRGSpeed);
						else
							c32time = *(TABLECRGSpeedRightDec+cCRGSpeed);
					}
				}
				c32time /= CRGOneStepTimePosition;
			}
		}
			
		if(CRGLeftDir)
		{
			if(CRGNowTimePosition)//if(CRGNowTimePosition != CRGStopTimePosition)
				CRGNowTimePosition--;
		}
		else
		{
			// 步数为n时，时间片位置就为n+1
			if(CRGNowTimePosition < (TimePositionAll))
				//if(CRGNowTimePosition != CRGStopTimePosition)
					CRGNowTimePosition++;
		}

		// 马达运动结束
		if(CRGNowTimePosition == CRGStopTimePosition)
		{
			// 根据打印标志来确定本次是空跑位置定位还是真正打印
			if(FlagSystemPrint)
			{
				BufferPrint[cBufferPrintLineRead].cLeft = 0;
				BufferPrint[cBufferPrintLineRead].cRight = 0;

				if(cBufferPrintLineRead >= (BufferPrintLineNum-1))
					cBufferPrintLineRead = 0;
				else
					cBufferPrintLineRead++;
					
				// 此处仅仅启动马达小电流
				// 彻底关闭马达电流需要在系统定时器中完成
				CRGCurrent(1);
				PrintingState = sINIT;
			}
			else// if(CRGState != 0x55)
			{
				// 刚刚没有打印，仅仅完成了马达初始定位，现在再次启动马达打印
				PrintingState = sStartCRGMotor;
			}
			c32time = *TABLECRGSpeedRightAcc/CRGOneStepTimePosition; //Lash Time
		}	
		break;
	}
		
	return c32time;
}


/**************************************************************************************************************
	McuInt.c	McuInt.h
**************************************************************************************************************/

