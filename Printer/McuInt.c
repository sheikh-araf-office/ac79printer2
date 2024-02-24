
#define McuInt 0
/**************************************************************************************************************
	McuInt.c	McuInt.h
**************************************************************************************************************/

#include "Driver.h"
#include "McuInt.h"
#include "UserInterface.h"
#include "boot.h"
/*UserInterface.h
wifi_app_task_printer.h
wifi_app_task_print.h
wifi_app_task_printer.c
wifi_app_task_print.c*/



// The data defined here must be enclosed in parentheses, otherwise the timing will be wrong.
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

//adDelayCount = 50;

	FlagHeadIC = 0x0000;
// Need to limit the left and right boundaries to prevent the pointer from exceeding the buffer range during calculation.
// Printing from left to right, here we check the earliest needle out, which is the left boundary, and consider the ending boundary by separately considering the delayed time of each needle.
	if(CRGRightDir)
	{
		if(CRGNowTimePosition < CRGVolatileStep*CRGOneStepTimePosition)
			goto HeadWork3F16End;
	}
// Need to limit the left and right boundaries to prevent the pointer from exceeding the buffer range during calculation.
// Printing from right to left, here we check the earliest needle out, which is the right boundary, and consider the ending boundary by separately considering the delayed time of each needle.
	else //if(CRGLeftDir)
	{
		if(CRGNowTimePosition >= (CRGVolatileStep*CRGOneStepTimePosition+TimePositionPrintSection+DR0709*BufferOneDataTimePosition+HeadDelayCount))
			goto HeadWork3F16End;
	}

	if(CRGLeftDir)
	{
		c32c1 = (CRGNowTimePosition+HeadDelayCount)/BufferOneDataTimePosition-CRGVolatileStep*CRGOneStepTimePosition/BufferOneDataTimePosition;
		c32c2 = (CRGNowTimePosition+HeadDelayCount)%BufferOneDataTimePosition;
	}
	else
	{
		c32c1 = (CRGNowTimePosition-0)/BufferOneDataTimePosition-CRGVolatileStep*CRGOneStepTimePosition/BufferOneDataTimePosition;
		c32c2 = (CRGNowTimePosition-0)%BufferOneDataTimePosition;
	}

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
	{
		if(CRGNowTimePosition < CRGVolatileStep*CRGOneStepTimePosition)
			goto HeadWork4F16End;
	}

	// 需要限制左右边界，防止计算时指针超出缓冲区范围
	// 从右向左打印，此处检查最早出针即右起边界，结束边界考虑各针滞后延时分别考虑
	else //if(CRGLeftDir)
	{
		if(CRGNowTimePosition >= (CRGVolatileStep*CRGOneStepTimePosition+TimePositionPrintSection+DR0709*BufferOneDataTimePosition+HeadDelayCount))
			goto HeadWork4F16End;
	}

	if(CRGLeftDir)
	{
		c32c1 = (CRGNowTimePosition+HeadDelayCount)/BufferOneDataTimePosition-CRGVolatileStep*CRGOneStepTimePosition/BufferOneDataTimePosition;
		c32c2 = (CRGNowTimePosition+HeadDelayCount)%BufferOneDataTimePosition;
	}
	else
	{
		c32c1 = (CRGNowTimePosition-0)/BufferOneDataTimePosition-CRGVolatileStep*CRGOneStepTimePosition/BufferOneDataTimePosition;
		c32c2 = (CRGNowTimePosition-0)%BufferOneDataTimePosition;
	}

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
	{
		if(CRGNowTimePosition < CRGVolatileStep*CRGOneStepTimePosition)
			goto HeadWork5F16End;
	}

	// 需要限制左右边界，防止计算时指针超出缓冲区范围
	// 从右向左打印，此处检查最早出针即右起边界，结束边界考虑各针滞后延时分别考虑
	else //if(CRGLeftDir)
	{
		if(CRGNowTimePosition >= (CRGVolatileStep*CRGOneStepTimePosition+TimePositionPrintSection+DR0709*BufferOneDataTimePosition+HeadDelayCount))
			goto HeadWork5F16End;
	}

	if(CRGLeftDir)
	{
		c32c1 = (CRGNowTimePosition+HeadDelayCount)/BufferOneDataTimePosition-CRGVolatileStep*CRGOneStepTimePosition/BufferOneDataTimePosition;
		c32c2 = (CRGNowTimePosition+HeadDelayCount)%BufferOneDataTimePosition;
	}
	else
	{
		c32c1 = (CRGNowTimePosition-0)/BufferOneDataTimePosition-CRGVolatileStep*CRGOneStepTimePosition/BufferOneDataTimePosition;
		c32c2 = (CRGNowTimePosition-0)%BufferOneDataTimePosition;
	}

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


extern unsigned long Tlen;
extern unsigned long Tlen1;
extern Device_Rev USB_Rev;

extern struct sBufferPrint BufferPrint[];
//extern BOOT_INFO boot_info;

/**************************************************************************************************************
	McuInt.c	McuInt.h
**************************************************************************************************************/
// System timer, needs to be executed every 20 milliseconds.
//static ___interrupt TSEC void timer_isr2(void)
//{
    // This can be excluded from the timer and executed within the main loop since there's no infinite loop.
    // Using a loop to sequentially scan each task to reduce the execution time of this interrupt.
//}
void Tim_Sys_Handler(void)
{
	static volatile unsigned int c16ForData = 0;	// 0~4: 4个数据通道切换
//	static volatile unsigned char c8ForScan = 0;	// 0~4: 5次一个循环，各个模块依次扫描执行
//	static volatile unsigned char c8ForKeyAdc = 5;	// KEYADC按键扫描
	static volatile unsigned char c8ForPsPaper = 0;	// 纸张传感器扫描
	static volatile unsigned char c8ForPower = 0;	// KEYPOWER开关机处理
//	static volatile unsigned int c16ForCRG = 0;
	static volatile unsigned int c16ForPAP = 0;
	static volatile unsigned int c16ForLedLine = 0;
	static volatile unsigned int c16ForLedErr = 0;
//	volatile unsigned int c16c1 = 0;

//PowerState=10;

	// Power on/off function
	switch(PowerState)
	{
	case 0:	// Power-on confirmation status
		if(KeyPower)
			c8ForPower = 0;
		else
			c8ForPower = 10;
		if(c8ForPower > 1) //
		{
			LedLineOpen
			LedErrOpen
			MainPowerOn
			PowerState = 1;
		}
		break;
	case 1: // Power-on confirmed, waiting for key release.
		if(KeyPower)
		{
			LedLineClose
			LedErrClose
			PowerState = 10;
		}
		break;
	case 10:  // Power-on process completed, maintaining normal startup state.
		if(KeyPower)
			c8ForPower = 0;
		if(c8ForPower > 4*5)
		{
			PowerState = 20;
		}
		break;
	case 20: // During shutdown process, releasing the button within 1 second has no effect.
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
	case 21:// LED blinks for 2 seconds, waiting for manual shutdown confirmation.
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
else
{
PowerState =21;
MainPowerOff
LedErrClose
LedLineClose
}



    // System delay counter
	if(DelayTimeCount20mS)
		DelayTimeCount20mS--;


// Scanning for switching between 4 data channels
	if(DataFromChannel)
	{
		if((cBufferPrintLineRead==cBufferPrintLineWrite)&&((PaperState==sPaperFrontNormal)||(PaperState==sPaperBackNormal))&&(!CRGRunning)&&(!PAPRunning))
		{
			if(DataFromChannel==1)
			{
				if(c16ForData<50*5) // USB only 5 second, for send SSID PWD
				{
					c16ForData++;
				}
				else
				{
					c16ForData = 0;
					DataFromChannel = 0;
				}
			}
			else
			{
				if(c16ForData<50*30) // Scan data channels again if more than 60 seconds have elapsed.
				{
					c16ForData++;
				}
				else
				{
					c16ForData = 0;
					DataFromChannel = 0;
				}
			}
		}
		else
		{
			c16ForData = 0;
		}
	}
	else //if(DataFromChannel==0) // No data has come in yet, so the data channel cannot be determined temporarily.
	{
		c16ForData = 0;
	}

    // Check and confirm the print buffer.
	if((cBufferPrintLineRead != cBufferPrintLineWrite) && (BufferPrint[cBufferPrintLineRead].cLeft == BufferPrint[cBufferPrintLineRead].cRight) && (BufferPrint[cBufferPrintLineRead].Step == 0))
	{
//printf("R！=W s==0 -----\r\n");
		if(cBufferPrintLineRead >= (BufferPrintLineNum-1))
			cBufferPrintLineRead = 0;
		else
			cBufferPrintLineRead++;
	}


	// HeadTempAdc
/*	ADCTemp = adc_get_value(AdcChTemp);
	if(ADCTemp < AdcTempLever) // 数值过小表明温度过高
		TMPOneDirSet
	else if(ADCTemp > 900) // 数值接近满副1023表明热敏电阻断开
		TMPOneDirSet
	else
		TMPTwoDirSet*/


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
#if 1
	if(FlagKeyPadNo == 0x0000)
	{
		if(PaperState==sPaperFrontNormal)
		{
			if(BufferPrintLineWrited()==0)
			{
				if((KeyPaperRecorder == 0xff) && (KeyPaperDownTime > 50)) // mid key
				{
					if(SensorRecorderFrontIn==0x00)
						PaperCount = 0;
					PaperState = sPaperFrontEnd; //应该将纸完全退出去即可
				}
				else if((KeyLineRecorder == 0xff) && (KeyLineDownTime > 50))
				{
					PAPMoveStep += 24;
					PAPBackToFrontSet
				}
				else if(PAPMoveStep == 0)
				{
					if(KeyPaperRecorder == 0x0f) // down edge enable
					{
						PAPMoveStep = 24;
						PAPFrontToBackSet
					}
					else if(KeyLineRecorder == 0x0f)
					{
						PAPMoveStep = 24;
						PAPBackToFrontSet
					}
				}

				if((SensorRecorderFrontIn==0x00)&&(SensorRecorderBackIn==0x00))
				{
					PaperCount = 0;
					PAPContinueStep = 0;
					CRGFlattenPaper = 0;
					PAPState = sPaperNo;
				}
			}
			/*else //When there is data in the print buffer that needs to be printed,
                    the printing should be paused when a key is pressed at this time.
			{
				if(KeyLineDownTime == 5)
				{
					if(FlagPrintPause)
						FlagPrintPause = 0;
					else
						FlagPrintPause = 1;
				}
			}*/
		}
		else if(PaperState==sPaperBackNormal)
		{
			if(BufferPrintLineWrited()==0)
			{
				if((KeyPaperRecorder == 0xff) && (KeyPaperDownTime > 50)) // mid key
				{
					PAPMoveStep += 24;
					PAPFrontToBackSet
				}
				else if((KeyLineRecorder == 0xff) && (KeyLineDownTime > 50))
				{
					PAPMoveStep += 24;
					PAPBackToFrontSet//PAPFrontToBackSet
				}
				else if(PAPMoveStep == 0)
				{
					if(KeyPaperRecorder == 0x0f) // down edge enable
					{
						if(SensorRecorderBackIn==0x00)
							PaperCount = 0;
						PaperState = sPaperBackNormalOut; //应该将纸完全退出去即可
					}
					else if(KeyLineRecorder == 0x0f)
					{
						PAPMoveStep = 24;
						PAPBackToFrontSet//PAPFrontToBackSet
					}
				}
				if((SensorRecorderBackIn==0x00)&&(SensorRecorderFrontIn==0x00))
				{
					PaperCount = 0;
					PAPContinueStep = 0;
					CRGFlattenPaper = 0;
					PAPState = sPaperNo;
				}
			}
			else // Printing should be paused when a key is pressed while there is data in the print buffer that needs to be printed.
			{
				if(KeyLineDownTime == 5)
				{
					if(FlagPrintPause)
						FlagPrintPause = 0;
					else
						FlagPrintPause = 1;
				}
			}
		}
		else if(PaperState==sPaperNo)
		{
			if((KeyPaperRecorder == 0xff) && (KeyPaperDownTime > 50))
			{
				PAPMoveStep = 24;
				PAPBackToFrontSet
			}
			else if((KeyLineRecorder == 0xff) && (KeyLineDownTime > 50))
			{
				PAPMoveStep = 24;
				PAPFrontToBackSet
			}
			else if(PAPMoveStep==0)
			{
				if(KeyPaperRecorder == 0x0f)
				{
					PaperState = sPaperBackNoIn;
				}
				else if(KeyLineRecorder == 0x0f)
				{
					PAPMoveStep = 24;
					PAPFrontToBackSet
				}
			}
		}
	}
#endif


#if 0
//printf("PaperState:  %d  PaperCount: %d \r\n", PaperState, PaperCount);

	if(c8ForPsPaper < 250)
		c8ForPsPaper++;
	else
	{
		c8ForPsPaper = 0;
	#if DebugMsg
		if(ADCTemp < AdcTempLever) // 数值过小表明温度过高
			printf("OverTemp!\n");
	#endif
		ADCKEY = adc_get_value(AdcChKey);
		printf("ADCKEY = %d  \r\n ", ADCKEY);
	/*	c16c1 = adc_get_voltage(AdcChKey);
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

	#if 0
		ADCKEY = adc_get_value(AdcChFrontOut);
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
		printf("PaperState:  %d  FO: %d FI: %d BI: %d BO: %d \r\n", PaperState, SensorRecorderFrontOut, SensorRecorderFrontIn, SensorRecorderBackIn, SensorRecorderBackOut);
	#endif
	}
#endif

#if 1
	// Temporary non-usage of different stages in the WIFI configuration process.
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


#if 1
	//LedDisplay
	if(PowerState == 10) // Only after the power-on and power-off processes are completed will the LED reflect other states. In other words, the LED will indicate other states only when the power is in a normal state.
	{
		if(WifiState)
		{
			if(WifiState == 5)		// Connected successfully, stays lit
				LedLineOpen
			else if(WifiState == 6)	// Connection failure, slow blinking
			{
				if(c16ForLedLine<25*2*5) // 20*25*2*5=5s
					c16ForLedLine++;
				else
					c16ForLedLine = 0;

				if(c16ForLedLine<25)
					LedLineOpen
				else
					LedLineClose
			}
			else					// Other errors, require reconnecting, fast blinking
			{
				if(c16ForLedLine<25*2*5) // 20*25*2*5=5s
					c16ForLedLine++;
				else
					c16ForLedLine = 0;

				if(c16ForLedLine<25*2*2)
				{
					if(c16ForLedLine%50<25)
						LedLineOpen
					else
						LedLineClose
				}
			}
		}
		else if(USBin)
		{
			if(c16ForLedLine<25*2*5) // 20*25*2*5=5s
				c16ForLedLine++;
			else
				c16ForLedLine = 0;

			//if(c16ForLedLine<25*2*2)
			{
				if(c16ForLedLine%50<25)
					LedLineOpen
				else
					LedLineClose
			}
		}
		else
			LedLineClose

		if((PaperState == sPaperFrontNormal)||(PaperState == sPaperBackNormal))
			LedErrClose
		else if(PaperState == sPaperErr)
		{
			if(c16ForLedLine<25*2*5) // 20*25*2*5=5s
				c16ForLedLine++;
			else
				c16ForLedLine = 0;

			if(c16ForLedLine<25*2*2)
			{
				if(c16ForLedLine%50<25)
					LedLineOpen
				else
					LedLineClose
			}
		}
		else
			LedErrOpen
	}
#endif
#if 0
	if(c8ForPsPaper < 20) //80 //50*2
		c8ForPsPaper++;
	else
	{

        // Data is transmitted back once per second at this point.
		c8ForPsPaper = 0;

		//if(BufferPrintLineWrited()==0)
		if((!PAPRunning)&&(!CRGRunning))
		{
			const u8 *mac_addr;
			mac_addr = bt_get_mac_addr();
			printf("MAC:%02X%02X%02X%02X%02X%02X", mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
			//sprintf((char *)ssid, AP_SSID"%02x%02x%02x%02x%02x%02x", mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
			struct wifi_mode_info info;
			 info.mode = STA_MODE;
        	//wifi_store_mode_info(STA_MODE, info.ssid, info.pwd);
			wifi_get_mode_cur_info(&info);
			printf(info.ssid);
			printf(info.pwd);

			printf("ID:%d", boot_info.chip_id);
			//printf("PS:%d  SPL:%d PC:%d PCS:%d CFP:%d\r\n", PaperState, StepPageLenth, PaperCount, PAPContinueStep, CRGFlattenPaper);
			//printf("FO:0x%x-FI:0x%x-BO:0x%x-BI:0x%x-PMS:%d\r\n", SensorRecorderFrontOut, SensorRecorderFrontIn, SensorRecorderBackOut, SensorRecorderBackIn, PAPMoveStep);
		}
	}
/*if(MainPower)
{
MainPowerOff
LedErrClose
}
else
{
MainPowerOn
LedErrOpen
}*/

/*if(c8ForPsPaper == 0)
{
	FlagHeadIC=0x5555;
	HeadIC();
	LedLineClose
	LedErrOpen
}
else if(c8ForPsPaper == 20)
{
	FlagHeadIC=0x0000;
	HeadIC();
	LedLineOpen
	LedErrClose
}
else if(c8ForPsPaper == 40)
{
	FlagHeadIC=0xaaaa;
	HeadIC();
	LedLineClose
	LedErrOpen
}
else if(c8ForPsPaper == 60)
{
	FlagHeadIC=0x0000;
	HeadIC();
	LedLineOpen
	LedErrClose
}*/

/*	if(c8ForPsPaper == 0)
	{
		//printf("DFC:-%d-%d", DataFromChannel, c16ForData);
		if(DataFromChannel==1)
			printf("U-");
		else if(DataFromChannel==2)
			printf("W-");
		else if(DataFromChannel==3)
			printf("S-");
		else if(DataFromChannel==4)
			printf("B-");
		else
			printf("--");
		printf("%d", c16ForData);*/
//printf("PAPState:0x%x-PrintingState:%d-PaperState:%d-FlagMotorIC:0x%x-PAPMoveStep:%d-cBufferPrintLineWrite:%d-cBufferPrintLineRead:%d\r\n", PAPState, PrintingState, PaperState, FlagMotorIC, PAPMoveStep, cBufferPrintLineWrite, cBufferPrintLineRead);
/*if(cBufferPrintLineRead==cBufferPrintLineWrite)
	;//printf("cBufferPrintLineRead:%d=\r\n", cBufferPrintLineRead);
else
{
	printf("PrintingState:%d-FlagMotorIC:0x%x-cBufferPrintLineRead:%d-cBufferPrintLineWrite:%d\r\n", PrintingState, FlagMotorIC, cBufferPrintLineRead, cBufferPrintLineWrite);

	printf("PAPState:0x%x-CRGState:0x%x-PaperState:%d-PAPMoveStep:%d-CRGNowTimePosition:%d-CRGStopTimePosition:%d", PAPState, CRGState, PaperState, PAPMoveStep, CRGNowTimePosition, CRGStopTimePosition);
	printf("Buffer:[*]-%d-%d-%d-%d-%d", BufferPrint[cBufferPrintLineRead].cLeft, BufferPrint[cBufferPrintLineRead].cRight, BufferPrint[cBufferPrintLineRead].Step, BufferPrint[cBufferPrintLineRead].Dir, PaperCount);
}*/
	//	printf("MEP.Buffer: %d--%d-%d--%d-%d", BufferPrintLineWrited(), MEP.SocketPread, MEP.SocketPwrite, Tlen, Tlen1);
	//	printf("USB.Buffer: %d-%d", USB_Rev.Pread, USB_Rev.Pwrite );

	//	printf("%d-%d\n", PowerState, ADCKEY);
	//	printf("PAPState:0x%x-PrintingState:%d-PaperState:%d-FlagMotorIC:0x%x-PAPMoveStep:%d\r\n", PAPState, PrintingState, PaperState, FlagMotorIC, PAPMoveStep);
	//	printf("%d-%d-%d-%d-%d", cBufferPrintLineRead, cBufferPrintLineWrite, BufferPrint[cBufferPrintLineRead].Step, BufferPrint[cBufferPrintLineRead].cLeft, BufferPrint[cBufferPrintLineRead].cRight);
	//	printf("FO:0x%x-FI:0x%x-BO:0x%x-BI:0x%x-PMS:%d\r\n", SensorRecorderFrontOut, SensorRecorderFrontIn, SensorRecorderBackOut, SensorRecorderBackIn, PAPMoveStep);
	//	printf("PaperCount:%d-PaperPageLenth:%d-StepPageLenth:%d\r\n", PaperCount, PaperPageLenth, StepPageLenth);
	//	printf("%d", PaperState);
	//	printf("FlagMotorIC:0x%x-HomeRecorder:0x%x-PrintingState:%d-CRGState:0x%x-CRGNowTimePosition:%d-CRGStopTimePosition:%d\r\n", FlagMotorIC, HomeRecorder, PrintingState, CRGState, CRGNowTimePosition, CRGStopTimePosition);
	//}
#endif
}
void PaperSensorScan(void)
{
	if(PAPState == 0x55)
	{
		SensorRecorderFrontOut <<= 1;	if(adc_get_value(AdcChFrontOut) < ADC4F8)	SensorRecorderFrontOut |= 0x01;
		SensorRecorderFrontIn <<= 1;	if(adc_get_value(AdcChFrontIn) < ADC4F8)	SensorRecorderFrontIn |= 0x01;
		SensorRecorderBackIn <<= 1;		if(adc_get_value(AdcChBackIn) < ADC4F8)		SensorRecorderBackIn |= 0x01;
		SensorRecorderBackOut <<= 1;	if(adc_get_value(AdcChBackOut) < ADC4F8)	SensorRecorderBackOut |= 0x01;
	}
	else
	{
		if(adc_get_value(AdcChFrontOut) < ADC4F8)	SensorRecorderFrontOut = 0xff;
		if(adc_get_value(AdcChFrontIn) < ADC4F8)	SensorRecorderFrontIn = 0xff;
		if(adc_get_value(AdcChBackIn) < ADC4F8)		SensorRecorderBackIn = 0xff;
		if(adc_get_value(AdcChBackOut) < ADC4F8)	SensorRecorderBackOut = 0xff;
	}
}
// The paper feeding timer scanning time must be less than the carriage scanning time: the carriage timer will start paper feeding, so it is necessary to ensure that the paper feeding timer starts promptly after the carriage timer starts.
// Adjustment to the above strategy on 20230616: considering the possibility of using 0a, the paper feeding must be started after the carriage is started, and then the paper feeding can be started; it can be after printing or started without printing.
// Only after 0D 0A paper feeding is completed, will the printing buffer pointer be adjusted.
// For subsequent optimizations, starting the timer can be used: on 20231010, it will no longer be used: stop the motor at intervals and scan the sensor status. Use a timer to complete the paper feeding motor operation.
#define TimePAPScan	14000//2000	// 10ms is just enough for motor positioning and braking time
#define TimeCRGScan	12000//2000	//

unsigned long Tim_PAP_Handler(void)
{
	static unsigned char FlagPAPRead = 0;
	unsigned long c32time = TimePAPScan;
	static unsigned int c16ForPAP; // 走纸马达超时计数器
	static unsigned int c16ForPaper; // 纸张传感器超时计数器
	unsigned int cBufferPrintLineTemp;

	// 根据纸张传感器状态决定装纸逻辑
	#if DebugPaper
		PaperState = sPaperBackNormal;
		StepPageLenth = StepPageLenthBack*2;
		PAPCurrent(0);
		return c32time;
	#endif

	// 必须等待字车马达先完成初始化动作，两者马达状态管理参数PrintingState是公用的
	if(CRGState!=0x55)
	{
		PAPCurrent(0);
		return c32time;
	}

// This timer always checks the sensor status. When the motor is in a stopped state, the sensor status is scanned according to time, with a scanning time of TimePAPScan, in order to start the paper feeding motor.
// When the motor is in motion, the sensor status is checked with each step.

	// PaperSensor scan
	//if(PaperState) //Do not use this statement: Stop scanning and recording status during paper feeding.
	/*if((PaperState==sPaperFrontNormal)||(PaperState==sPaperBackNormal))
	{
		if(FlagMotorIC&0x30) //&&(cPAPSpeed))
			PaperSensorScan();
	}
	else*/
		PaperSensorScan();

	switch (PrintingState)
	{
	//default: //PAP CRG Shared use flag, default state cannot be enabled, will deadlock each other.
	case sINIT:
		switch (PAPState)
		{
		// 1优先走纸马达初始化
		//default:
		case 0xff: // 先后走
			PAPState = 0x0a;
			PAPBackToFrontSet
			PAPMoveStep = 4; // 4 相位匹配用
			break;

		case 0x0a:
			if(PAPMoveStep == 0)
			{
				PAPState = 0xaa;
				PAPFrontToBackSet
				PAPMoveStep = 4; // 4 相位匹配用
			}
			break;

		case 0xaa: // 再前走
			if(PAPMoveStep == 0)
				PAPState = 0x55;
			break;

		// 2根据纸张传感器状态完成纸张初始化
		case 0x55:
			switch(PaperState)
			{
			//default:
			case sPaperNo:
				InitPaperStata:
				PaperCount = 0;
				// 静态检测传感器状态，以便确定纸张状态
				if((SensorRecorderFrontOut == 0xff)&&(SensorRecorderFrontIn == 0xff))
				{
					if((SensorRecorderBackOut == 0xff)&&(SensorRecorderBackIn == 0xff))
					{
//printf("BN-");
						PaperCount = StepBackToPrint+StepFrontToPrint+StepToCutPos;; //StepToCutPos	//
// There is no concept of page length, it is considered at the beginning of printing; if there is a concept of page length, it should be at the paper tear position.
						PaperState = sPaperBackNormal; 	//
//Front and back are both paper, rear paper feed mode: Rear paper feed is continuous paper, currently there is no concept of page length.
						StepPageLenth = StepPageLenthBack*2;
					}
					else
					{
//printf("0");//printf("FE-");
						PaperState = sPaperFrontEnd; 	// 仅前边有纸，需要退纸出去再装纸
					}
				}
				else if((SensorRecorderBackOut == 0xff)&&(SensorRecorderBackIn == 0xff))
				{
//printf("BE-");
					PaperState = sPaperBackEnd; 		// 仅后部有纸，后进纸模式，需要定位纸张边界
				}
				// 动态检测是否前进纸
				else if((SensorRecorderFrontOut == 0xff)&&(SensorRecorderFrontIn == 0x00))
				{
					if(c16ForPaper < 50*2)
						c16ForPaper++;
					else
					{
//printf("FI-");
						c16ForPaper = 0;
						PaperState = sPaperFrontInit; // 前进纸
					}
				}
				// 动态检测是否后进纸
				else if((SensorRecorderBackOut == 0xff)&&(SensorRecorderBackIn == 0x00))
				{
					if(c16ForPaper < 50*2)
						c16ForPaper++;
					else
					{
//printf("BI-");
						c16ForPaper = 0;
						PaperState = sPaperBackInit; // 后进纸
					}
				}
				else
				{
//printf("No-");
					PaperCount = 0;
					c16ForPaper = 0;
				}
				break;

			case sPaperFrontInit: // 前边检测到纸张，开始启动前进纸初始化
//printf("FI1");
				PAPFrontToBackSet
				PAPMoveStep = StepFrontToPrint*5;
				break;

			case sPaperFrontNormal: // 前进纸正常模式，不到页末的逻辑检测纸张时需要检测纸张是否被抽出
//printf("FN-");
				if((SensorRecorderFrontIn == 0x00)&&(SensorRecorderBackIn == 0x00))
					PaperState = sPaperNo;
				/*if(((FlagMotorIC&0x30) == 0x30) && (PaperCount<StepFrontToPrint*2))
				{
					if(adc_get_value(AdcChFrontIn) < ADC4F8) // 有纸
						c16ForPaper = 0;
					else
					{
						if(c16ForPaper<50*2)
							c16ForPaper++;
						else
							PaperState = sPaperNo;
					}
				}*/
				break;

			case sPaperFrontEnd: // 有两种情况：初始前纸后无，仅退纸少许；打印结束，前无后纸，需退超过页长
//printf("FE1");
				//if(SensorRecorderFrontOut||SensorRecorderFrontIn||SensorRecorderBackOut||SensorRecorderBackIn)
				{
//printf("2");//printf("FE2");
				//	if(PAPMoveStep == 0)
					{
						PAPBackToFrontSet
						if(SensorRecorderFrontIn)
						{
//printf("PNo");//printf("FE3");
							PaperState = sPaperNo; 							// 退纸结束后，即成为无纸状态
							PAPMoveStep = StepFrontToPrint*2;				// 初始前纸后无，仅退纸少许；
					}
						else
						{
///printf("SPL");//printf("FE4");
							PaperState = sPaperNo;
							PAPMoveStep = StepPageLenth+StepFrontToPrint*2;	// 打印结束，前无后纸，需退超过页长
						}
					}
				}
				/*else
				{
//printf("5");//printf("FE5");
					PaperState = sPaperNo;
				}*/
				break;

			case sPaperBackNo:
				goto InitPaperStata;
				break;

			case sPaperBackNoIn: // 按键启动进入该状态
//printf("BI1");
				PAPBackToFrontSet
				if(CRGFlattenPaper==0) // 先走到整平位置然后字车启动整平
					PAPMoveStep = StepToFlattenPos*2;
				else if(CRGFlattenPaper==4) // 等待字车左右运动整平动作完成
				{
					CRGFlattenPaper = 5;
					//CRGFlattenPaper = 0;
					if(PaperCount < (StepBackToPrint+StepFrontToPrint+StepToCutPos))
						PAPMoveStep = (StepBackToPrint+StepFrontToPrint+StepToCutPos)-PaperCount;
					/*else // 使用该代码也是对的，状态是在走纸运动结束时切换的
						PaperState = sPaperBackNormal;*/
				}
				else if(CRGFlattenPaper==5)
				{
					if(!PAPRunning)
					{
						CRGFlattenPaper = 0;
						//PaperState = sPaperBackNormal;
						if(SensorRecorderBackIn==0xff)
						{
							if(SensorRecorderFrontIn==0xff)
								PaperState = sPaperBackNormal;
							else
								PaperState = sPaperBackInit;
						}
						else
							PaperState = sPaperErr;
					}
				}
				break;

			case sPaperBackInit: // 后边检测到纸张，开始启动后进纸初始化
//printf("BI1");
				PAPBackToFrontSet
				PAPMoveStep = StepBackToPrint*2+StepFrontToPrint*2;
				break;

			case sPaperBackNormal: // 后进纸正常模式，不到纸张结束的逻辑检测纸张时需要检测纸张是否被抽出
				if(SensorRecorderBackIn==0x00) //&& (SensorRecorderFrontIn==0x00))
				{
					if(PaperCount <= (StepBackToPrint+StepFrontToPrint+StepToCutPos))//if(PaperCount >= (StepPageLenth+StepBackToPrint))
					{
						PAPMoveStep = StepFrontToPrint+StepBackToPrint+StepToCutPos;
						PaperCount = 1;
						PAPContinueStep = 0;
						CRGFlattenPaper = 0;
						//PaperState = sPaperFrontEnd;
					}
				}
				if((SensorRecorderBackIn==0x00) && (SensorRecorderFrontIn==0x00))
				{
					PaperCount = 0;
					PAPContinueStep = 0;
					CRGFlattenPaper = 0;
					PaperState = sPaperNo;
				}
				/*if(PaperCount<(StepBackToPrint+StepFrontToPrint+StepToCutPos+250))
				{
					if(SensorRecorderBackIn==0x00)
						PaperState = sPaperNo;
				}*/
				/*if(((FlagMotorIC&0x30) == 0x30) && (PaperCount<(StepBackToPrint+StepFrontToPrint+StepToCutPos+50)))
				{
					if(SensorRecorderBackIn)//if(adc_get_value(AdcChBackIn) < ADC4F8) // 有纸
						c16ForPaper = 0;
					else
					{
						if(c16ForPaper<50*2)
							c16ForPaper++;
						else
							PaperState = sPaperNo;
					}
				}*/
				break;

			case sPaperBackNormalOut: // 正常情况下，将纸完全退出
				PAPMoveStep = (StepBackToPrint+StepFrontToPrint+StepToCutPos)*2; // 多点距离，走纸顺畅
				PAPFrontToBackSet
				break;

			case sPaperBackEnd: // 后进纸不在撕纸位置，需要退纸到BackIn无纸，返回到初始状态(自动再次进纸到打印位置)
//printf("BE1");
				PAPFrontToBackSet
				PAPMoveStep = StepBackToPrint+StepFrontToPrint;
				break;

			case sPaperBackHead:
//printf("BE1");
				if(SensorRecorderFrontOut&&SensorRecorderFrontIn)
				{
					if(PaperCount>StepBackToPrint)
					{
						PAPFrontToBackSet
						PAPMoveStep = PaperCount-StepBackToPrint;
					}
					else if(PaperCount<StepBackToPrint)
					{
						PAPBackToFrontSet
						PAPMoveStep = StepBackToPrint-PaperCount;
					}
				}
				else
					PaperState = sPaperBackNo;
				break;

			case sPaperErr:
				if((SensorRecorderFrontOut == 0x00)&&(SensorRecorderFrontIn == 0x00)&&(SensorRecorderBackOut == 0x00)&&(SensorRecorderBackIn == 0x00))
				{
					if(c16ForPaper < 50*2)
						c16ForPaper++;
					else
					{
						c16ForPaper = 0;
						PaperState = sPaperNo; // 全都无纸状态才恢复无纸待定状态
					}
				}
				else
				{
					c16ForPaper = 0;
				}
				break;
			}
			//停止走纸时，定时扫描纸张传感器状态，用于启动马达
			break;
		}

		// 4只根据走纸步数决定是否启动马达
		/*if(CRGFlattenPaper==4)//if((PAPContinueStep)||(CRGFlattenPaper==4))
		{
			if(PAPContinueStep)
			{
				PAPMoveStep = PAPContinueStep;
				PAPContinueStep = 0;
			}
			//else
			//	CRGFlattenPaper = 0;
		}*/
		if(PAPMoveStep)
		{
//printf("1");////printf("4");
			PrintingState = sStartPAPMotor;
		}
		// 3根据打印缓冲区及各种状态标记，填写10页的打印缓冲区
		// 3命令解析后填写的打印缓冲区，结合其他情况，直接写PAPMoveStep，启动马达运动，打印命令中走纸步数需要在此乘以2                					 存疑???
		else //if(PAPMoveStep == 0)
		{
//printf("CMP");
			if(	(!CRGRunning)&&(!PAPRunning)&&((PaperState==sPaperFrontNormal)||(PaperState==sPaperBackNormal)) && //(PaperCount>=StepFrontToPrint) &&
				(cBufferPrintLineRead != cBufferPrintLineWrite) && BufferPrint[cBufferPrintLineRead].Step && (BufferPrint[cBufferPrintLineRead].cLeft==BufferPrint[cBufferPrintLineRead].cRight)	)
			{
//printf("Pr-%d-%d-%d-%d-%d-%d", BufferPrint[cBufferPrintLineRead].cLeft, BufferPrint[cBufferPrintLineRead].cRight, BufferPrint[cBufferPrintLineRead].Dir, BufferPrint[cBufferPrintLineRead].Step, PaperState, PAPMoveStep);
//printf("Pr");
				FlagPAPRead = 1;
				if(BufferPrint[cBufferPrintLineRead].Dir == sPaperDirFrontToBack)
					PAPFrontToBackSet
				else
					PAPBackToFrontSet
				PAPMoveStep = BufferPrint[cBufferPrintLineRead].Step*2; // 必须等待走纸结束后再写0，防止字车意外提前启动
				//BufferPrint[cBufferPrintLineRead].Step = 0; //结束时，判断该数值确定尾走纸命令后，才写0。千万不能提前写0
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
		}
		break;

	case sStartPAPMotor:
//printf("7");//printf("SP-");
		cPAPSpeed = 0;
		if((FlagMotorIC&0x30) == 0x30)
		{
//printf("8");//printf("SP1");
			PAPCurrent(1);
		}
		else// if((FlagMotorIC&0x30) != 0x00)
		{
//printf("9");
//printf("Ps:r%d\r\n", cBufferPrintLineRead);
			PAPCurrent(2);
			PrintingState = sWaitingPAPMotor;
		}
		//PaperCount = 0;
		c32time = *TABLEPAPSpeed; // Hold Time
//printf("2");
		// 判断走纸位置，确定是否需要字车对纸张进行整平

		if((CRGFlattenPaper==0)&&(PaperCount<=StepToFlattenPos)&&((PaperCount+PAPMoveStep)>StepToFlattenPos))
		{
			cBufferPrintLineTemp = cBufferPrintLineRead;
			while(cBufferPrintLineTemp!=cBufferPrintLineWrite)
			{
				if(BufferPrint[cBufferPrintLineTemp].Step==0) // 必须是打印行，不能是走纸行
				{
					BufferPrint[cBufferPrintLineTemp].cLeft = 0;
					BufferPrint[cBufferPrintLineTemp].cRight = TimePositionAll;
				}
				if(cBufferPrintLineTemp < (BufferPrintLineNum-1))
					cBufferPrintLineTemp++;
				else
					cBufferPrintLineTemp = 0;
			}
			// 此处增加字车马达左右空跑操作用于将纸张整平，但是走纸会多走一行，不得解情况下，准备这种情况下，将打印缓冲区调整到左右满行即可
			/*if((PaperState==sPaperFrontNormal)&&(PAPFrontToBackDir))
			{
				PAPContinueStep = PaperCount+PAPMoveStep-StepToFlattenPos;//PaperCount+PAPMoveStep-StepToFlattenPos;
				PAPMoveStep = StepToFlattenPos-PaperCount;
				CRGFlattenPaper = 1;
			}
			else if((PaperState==sPaperBackNormal)&&(PAPBackToFrontDir)&&(SensorRecorderBackIn==0xff)&&(SensorRecorderFrontIn==0x00))
			{
				PAPContinueStep = PaperCount+PAPMoveStep-StepToFlattenPos;//PaperCount+PAPMoveStep-StepToFlattenPos;
				PAPMoveStep = StepToFlattenPos-PaperCount;
				CRGFlattenPaper = 1;
			}*/
		}

		//if(CRGFlattenPaper==4)
		//	CRGFlattenPaper = 0;
		break;

	case sWaitingPAPMotor:
		if(PAPMoveStep)
		{
//printf("-");//printf("PM-");
			switch (PaperState)
			{
			default:
			case sPaperNo:
				break;

			case sPaperFrontInit:										// 前进纸初始化状态
//printf("9");
				PaperCount++;
				/*if(SensorRecorderFrontIn)
					StepPageLenth = PaperCount;
				else
					StepPageLenth = 0;*/
				if((SensorRecorderFrontOut==0xff)&&(SensorRecorderFrontIn==0xff))		// 2进入有纸状态
				{
//printf("b");
					// @Alke 20231022 必须到达纸头位置才开启前进纸模式
					if(PaperCount >= StepFrontToPrint)
					{
						StepPageLenth = StepPageLenthFront*2;
						PaperState = sPaperFrontNormal;
					}
				}
				else if((SensorRecorderFrontOut==0xff)&&(SensorRecorderFrontIn==0x7f))	// 1检测到页首，调整步数到达页首
				{
//printf("a");
					//PaperCount = 1;
					//StepPageLenth = PaperCount;
					PAPMoveStep = StepFrontToPrint+PAPVolatileStep;
				}
				else if(PAPMoveStep<PAPVolatileStep)									// 0走纸即将结束，仍没能找到页头，进入纸张错误状态，必须人工干预
				{
					PaperState = sPaperErr;
				}
				/*else if((SensorRecorderFrontOut==0x00)&&(SensorRecorderFrontIn==0x00))	// 3无纸返回初始状态
				{
					if(PAPMoveStep > PAPVolatileStep)
						PAPMoveStep = PAPVolatileStep;
					PaperState = sPaperNo;
				}*/
				break;

			// 需要考虑初始化及页末情况，动态更新该状态
			case sPaperFrontNormal:													// 正常状态，能够正常打印
				//if(BufferPrint[cBufferPrintLineRead].Dir == sPaperDirFrontToBack)
				if(PAPFrontToBackDir)
				{
//printf("P+");
					PaperCount++;
					// 走纸状态下,相同方向下补充走纸步数
					/*if((cBufferPrintLineRead != cBufferPrintLineWrite) && (BufferPrint[cBufferPrintLineRead].Step))
					{
						//if(BufferPrint[cBufferPrintLineRead].Dir == sPaperDirFrontToBack)
						{
							PAPMoveStep += (BufferPrint[cBufferPrintLineRead].Step);
							BufferPrint[cBufferPrintLineRead].Step = 0;
							/*if(cBufferPrintLineRead >= (BufferPrintLineNum-1))
								cBufferPrintLineRead = 0;
							else
								cBufferPrintLineRead++;////////////////////////////////
						}
					}*/

					if(SensorRecorderFrontIn==0xfe)
						StepPageLenth = PaperCount;
					else if(PaperCount>=(StepPageLenth+StepFrontToPrint-32))						// 纸尾已过打印位置，进入无纸状态
					{
						if(SensorRecorderFrontIn==0x00)
						{
							if(PAPMoveStep > PAPVolatileStep)
								PAPMoveStep = PAPVolatileStep;
							PaperState = sPaperFrontEnd;												// 切换到纸张结束状态，等待马达停止后，再退纸出来
							//FlagSystemPrintClr // 无纸后，不打印，防止进纸时打印
						}
					}
//printf("3");
				}
				else // 反方向退纸
				{
//printf("P-");
					//if(StepPageLenth)	StepPageLenth--;
					if(PaperCount)
						PaperCount--;	//if(SensorRecorderFrontIn==0xfe)	StepPageLenth = PaperCount;
					/*else if(SensorRecorderFrontIn==0xfe)
					{
						if(PAPMoveStep>StepFrontToPrint)
							PAPMoveStep = StepFrontToPrint;
						PaperState = sPaperNo; //停止后，等待超时成为无纸状态
						//FlagSystemPrintClr
					}
					else if(SensorRecorderFrontIn==0x00)
						PaperState = sPaperNo;*/
//printf("3-----");
					else if(SensorRecorderFrontIn==0x00)
						PaperState = sPaperNo;
				}
				break;

			case sPaperFrontEnd:
				break;

			case sPaperBackNo:
				break;

			case sPaperBackNoIn:
				PaperCount++;
				if((SensorRecorderBackOut==0xff)&&(SensorRecorderBackIn==0x7f))	// 1检测到页首，调整步数到达撕纸位
				{
					PaperCount = 1;
					PAPMoveStep = StepToFlattenPos; //StepBackToPrint+StepFrontToPrint+StepToCutPos;
				}
				if(CRGFlattenPaper==0)
				{
					if(PAPMoveStep<PAPVolatileStep)
					{
						CRGFlattenPaper = 1;
					}
				}
				else if(CRGFlattenPaper==4)
				{
					if(SensorRecorderBackIn==0x7f)
						PAPMoveStep=StepToCutPos;
					if(PAPMoveStep==1)//if(PAPMoveStep<PAPVolatileStep)									// 0走纸即将结束，仍没能找到页头，进入纸张错误状态，必须人工干预
					{
						CRGFlattenPaper = 5;
						/*CRGFlattenPaper = 0;
						//PaperState = sPaperBackNormal;
						if(SensorRecorderBackIn==0xff)
						{
							if(SensorRecorderFrontIn==0xff)
								PaperState = sPaperBackNormal;
							else
								PaperState = sPaperBackInit;
						}
						else
							PaperState = sPaperErr;*/
					}
				}
				break;

			case sPaperBackInit:
				PaperCount++;
				/*if(SensorRecorderBackIn)
					StepPageLenth = PaperCount;
				else
					StepPageLenth = 0;*/
				/*if((SensorRecorderBackOut==0xff)&&(SensorRecorderBackIn==0xff))			// 2进入有纸状态
				{
					// @Alke 20231022 必须到达纸头位置才开启前进纸模式
					if(PaperCount >= StepBackToPrint)
					{
						PaperState = sPaperBackNormal;
						StepPageLenth = StepPageLenthBack*2;
					}
				}
				else */
				if((SensorRecorderBackOut==0xff)&&(SensorRecorderBackIn==0x7f))	// 1检测到页首，调整步数到达页首
				{
					PaperCount = 1;
					PAPMoveStep = StepBackToPrint+StepFrontToPrint+StepToCutPos;
				}
				else if(PAPMoveStep<PAPVolatileStep)									// 0走纸即将结束，仍没能找到页头，进入纸张错误状态，必须人工干预
				{
					if(SensorRecorderBackIn==0xff)
					{
						if(SensorRecorderFrontIn==0xff)
							PaperState = sPaperBackNormal;
						else
							PaperState = sPaperBackInit;
					}
					else
						PaperState = sPaperErr;
				}
				/*else if((SensorRecorderBackOut==0x00)&&(SensorRecorderBackIn==0x00))	// 3无纸返回初始状态
				{
					if(PAPMoveStep > PAPVolatileStep)
						PAPMoveStep = PAPVolatileStep;
					PaperState = sPaperNo;
				}*/
				break;

			case sPaperBackNormal:												// 正常状态，能够正常打印
				if(PAPBackToFrontDir)
				{
					PaperCount++;

					if(PaperCount>=(StepPageLenth+StepBackToPrint+StepFrontToPrint+StepToCutPos))//-5
					{
						PaperCount = StepBackToPrint+StepFrontToPrint+StepToCutPos;
						if(SensorRecorderBackIn==0x00)
						{
							PAPMoveStep = StepFrontToPrint+StepToCutPos;
							PAPContinueStep =0;
							CRGFlattenPaper = 0;
							PaperState = sPaperNo;
						}
					}
					if((SensorRecorderBackIn==0x00) && (SensorRecorderFrontIn==0x00))
					{
						PaperCount = 0;
						PAPContinueStep = 0;
						CRGFlattenPaper = 0;
						PaperState = sPaperNo;
					}

					//if(SensorRecorderBackIn!=0x00)
					//	StepPageLenth = PaperCount;
					/*else if(PaperCount>=(StepPageLenth+StepBackToPrint-32))
					{
						PAPMoveStep += StepBackToPrint*2;
						PaperState = sPaperNo;
					}*/
				}
				else // 反方向退纸
				{
					//if(StepPageLenth)	StepPageLenth--;
					if(PaperCount)	PaperCount--;

					if(SensorRecorderBackOut!=0xff) //SensorRecorderBackOut
					{
						PaperCount = 0;
						if(PAPMoveStep>PAPVolatileStep)
							PAPMoveStep = PAPVolatileStep;
						//PaperState = sPaperNo; //停止后，等待超时成为无纸状态

						if(SensorRecorderBackOut==0x00)
							PaperState = sPaperNo;
					}
				}
				break;

			case sPaperBackNormalOut:												// 正常状态，能够正常打印
				if(PaperCount)	PaperCount--;

				if(SensorRecorderBackOut==0xfe) //SensorRecorderBackOut
				{
					PaperCount = 0;
					if(PAPMoveStep!=450) //(PAPVolatileStep+200)
						PAPMoveStep = 450;
					//PaperState = sPaperNo; // After stopping, wait for timeout to become a paperless state.
				}
				if(SensorRecorderBackOut==0x00)
					PaperState = sPaperNo;
				break;

			case sPaperBackEnd:
				if(SensorRecorderBackIn!=0xff) // Retract paper to the paper head and then stop.
				{
					if(PAPMoveStep>PAPVolatileStep)	// Paper feed is about to end, but the page header has not been found.
                                                    //Enters paper error state, manual intervention is required.
					{
						PAPMoveStep = PAPVolatileStep;
						PaperState = sPaperNo;
					}
				}
				break;

			case sPaperBackHead:
				if((SensorRecorderFrontOut==0x00)&&(SensorRecorderFrontIn==0x00))
				{
					if(PAPMoveStep>PAPVolatileStep)
						PAPMoveStep = PAPVolatileStep;
					//else if(PAPMoveStep==0)
					//	PaperState = sPaperNo;
				}
				else if(PAPMoveStep<PAPVolatileStep)
				{
					PaperState = sPaperBackNormal;
					StepPageLenth = StepPageLenthBack*2;
				}

				if(PAPBackToFrontDir)
					PaperCount++;
				else
					if(PaperCount)	PaperCount--;
				break;

			case sPaperErr:
				break;
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

			c32time = *(TABLEPAPSpeed+cPAPSpeed);
		}
		else // if(PAPMoveStep==0)
		{
//printf("4");
//printf("Pe:r%d\r\n", cBufferPrintLineRead);
			// 字车是在结束后清除打印缓冲区标志，但是走纸是在启动时就清除相关标志，以便能够在走纸过程中补充走纸步数使得走纸连贯不顿挫
			// 只有走纸结束后才写0，防止字车意外提前启动
			// @Alke 20231023 必须时走纸命令，并且是纸张正常模式(非装纸过程)才能调整指针，也可能是打印命令
			// 必须是命令解析走纸情况下，才能调整指针
			if(FlagPAPRead)//if(BufferPrint[cBufferPrintLineRead].Step&&((PaperState==sPaperFrontNormal)||(PaperState==sPaperBackNormal)))
			{
//printf("PeP\r\n");
				FlagPAPRead = 0;
				if(cBufferPrintLineRead!=cBufferPrintLineWrite)
				{
					BufferPrint[cBufferPrintLineRead].Step = 0;
					if(cBufferPrintLineRead >= (BufferPrintLineNum-1))
						cBufferPrintLineRead = 0;
					else
						cBufferPrintLineRead++;
				}
			}
			// @20231022 如果此时是前进纸模式的到达撕纸口的退纸操作，停止后需要返回到无纸状态，否则会认为是无需检测传感器的纸张末尾的逻辑纸张状态
			/*if((PaperState==sPaperFrontNormal)&&(PAPBackToFrontDir))//&&(SensorRecorderFrontIn==0x00))
			{
				PaperState = sPaperNo;
			}*/

			PAPCurrent(1);
			PrintingState = sINIT;
			c16ForPAP = 0;
			c16ForPaper = 0;
			c32time = *TABLEPAPSpeed;
		}
		break;
	}

	return c32time;
}

unsigned long TimPos_CRG_Handler(void)
{
	// 必须使用static，每16次才会计算一次时间，其他15次都需要使用上次的时间
	static unsigned long c32time = TimeCRGScan;//*CRGOneStepTimePosition; // 此处时间要16倍除的
	unsigned short Count;
	static unsigned int c16ForCRG;

/*if((PaperState!=sPaperFrontNormal)&&(PaperState!=sPaperBackNormal))
	return c32time;
if(PAPRunning)
	return c32time;*/
/*if(LedErr)
	LedErrOpen
else
	LedErrClose*/
//return TimeCRGScan;
//TMPTwoDirSet
//CMDTwoDirSet

	#if DebugCRGHome
		CRGState = 0x55;
		CRGCurrent(0);
		return c32time;
	#endif

	switch (PrintingState)
	{

// default:// PAP CRG shared use flag, cannot be enabled in default state, will deadlock each other
	case sINIT:
		switch (CRGState)
		{
		case 0xff:
			FlagSystemPrintClr
			if(HOME)	// Able to run smoothly
			{
				HomeRecorder = 0xff;
				CRGNowTimePosition = 0;
				CRGStopTimePosition = TimePositionAll;
			}
			else		// Unable to run
			{
				HomeRecorder = 0x00;
				CRGNowTimePosition = TimePositionAll;
				CRGStopTimePosition = 0;
			}
			CRGState = 0x05;
			break;

		case 0x05:
			if(CRGNowTimePosition == CRGStopTimePosition)
			{
				if(CRGNowTimePosition)
					CRGStopTimePosition = 0;
				else
					CRGStopTimePosition = TimePositionAll;
			}
			break;

		case 0x55:

            // Coordinate with paper initialization action
			if((PaperState!=sPaperFrontNormal)&&(PaperState!=sPaperBackNormal)&&(CRGStopTimePosition != TimePositionAll/2))
			{
				FlagSystemPrintClr
				CRGStopTimePosition = TimePositionAll/2;
			}
			break;
		}

//printf("0-%d-%d", CRGNowTimePosition, CRGStopTimePosition);
		// 马达不运动的话才解析命令
		if(CRGNowTimePosition == CRGStopTimePosition)
		{
			if(	(!PAPRunning)&&(!CRGRunning)&&((PaperState==sPaperFrontNormal)||(PaperState==sPaperBackNormal)) && //(PaperCount>=StepFrontToPrint) &&
				(cBufferPrintLineRead != cBufferPrintLineWrite) && ((BufferPrint[cBufferPrintLineRead].cLeft < BufferPrint[cBufferPrintLineRead].cRight))	)// && (BufferPrint[cBufferPrintLineRead].Step==0)	)
			{
//printf("C-%d-%d-%d-%d-%d", BufferPrint[cBufferPrintLineRead].cLeft, BufferPrint[cBufferPrintLineRead].cRight, CRGNowTimePosition, CRGStopTimePosition, BufferPrint[cBufferPrintLineRead].Step);
//printf("Cr");

				if(CRGNowTimePosition <= BufferPrint[cBufferPrintLineRead].cLeft)
				{
					CRGStopTimePosition = BufferPrint[cBufferPrintLineRead].cRight;
					CRGRightSet
					FlagSystemPrintSet
//printf("11: %d-%d-%d-%d", BufferPrint[cBufferPrintLineRead].cLeft, BufferPrint[cBufferPrintLineRead].cRight, CRGNowTimePosition, CRGStopTimePosition);
//intf("C1");

				}
				else if(CRGNowTimePosition >= BufferPrint[cBufferPrintLineRead].cRight)
				{
					CRGStopTimePosition = BufferPrint[cBufferPrintLineRead].cLeft;
					CRGLeftSet
					if(TMPOneDir || CMDOneDir)
						FlagSystemPrintClr
					else
						FlagSystemPrintSet
//printf("12");
				}
				else
				{
					if(TMPOneDir || CMDOneDir)//if(TMPOneDir)//if(TMPOneDir || CMDOneDir)
					{
						CRGStopTimePosition = BufferPrint[cBufferPrintLineRead].cLeft;
						CRGLeftSet
//printf("13");
					}
					else
					{
						// (N-L)<(R-N) ---> N*2<(L+R)
						if((CRGNowTimePosition*2) < (BufferPrint[cBufferPrintLineRead].cRight+BufferPrint[cBufferPrintLineRead].cLeft))
						{
							CRGStopTimePosition = BufferPrint[cBufferPrintLineRead].cLeft;
							CRGLeftSet
						}
						else
						{
							CRGStopTimePosition = BufferPrint[cBufferPrintLineRead].cRight;
							CRGRightSet
						}
//printf("14");
					}
					FlagSystemPrintClr
				}
			}
			else if((FlagMotorIC&0x03) == 0x00) // 如果全电流，则切换到小电流
			{
				c16ForCRG = 0;
				CRGCurrent(1);
			}
			else if((FlagMotorIC&0x03) != 0x03) // 如果没有关闭电流，则超时关闭电流
			{
				if(c16ForCRG < 100)  // TimeCRGScan*100=1000=1s
					c16ForCRG++;
				else
					CRGCurrent(0);
			}
			else
				c16ForCRG = 0;
		}

		// 只根据字车位置决定是否启动马达
		if((CRGFlattenPaper==1)||(CRGFlattenPaper==2))
		{
			if(CRGNowTimePosition < TimePositionAll/2)
				CRGStopTimePosition = TimePositionAll;
			else
				CRGStopTimePosition = 0;
			CRGFlattenPaper++;
		}
		else if(CRGFlattenPaper==3)
		{
			CRGStopTimePosition = TimePositionAll/2;
			CRGFlattenPaper = 4;
		}
		if(CRGNowTimePosition != CRGStopTimePosition)
		{
//printf("Cs");
			PrintingState = sStartCRGMotor;
		}

		c32time = TimeCRGScan; // 没有打印任务时，间隔多长时间扫描打印缓冲区
		break;

	case sStartCRGMotor:
//printf("2-");
//printf("2-%d-%d-%d-%d-%d", BufferPrint[cBufferPrintLineRead].cLeft, BufferPrint[cBufferPrintLineRead].cRight, CRGNowTimePosition, CRGStopTimePosition, BufferPrint[cBufferPrintLineRead].Step);

		cCRGSpeed = 0;
		if((FlagMotorIC&0x03) == 0x03)
		{
/*if(LedErr)
	LedErrOpen
else
	LedErrClose*/
			CRGCurrent(1);
		}
		else// if((FlagMotorIC&0x03) != 0x00)
		{
			CRGCurrent(2);
			if(CRGNowTimePosition < CRGStopTimePosition)
			{
				CRGRightSet
/*if(LedErr)
	LedErrOpen
else
	LedErrClose*/
			}
			else
			{
				CRGLeftSet
			}
			PrintingState = sWaitingCRGMotor;
		};
		c32time = *TABLECRGSpeedRightAcc; // Hold Time
		break;

	case sWaitingCRGMotor:
	/** When the motor moves at a constant speed, needle-out operations can be performed at appropriate times. Needle-out needs to be calculated for each time slot position. **/
		//if(FlagSystemPrint&&(CRGState == 0x55)&&((PaperState == sPaperFrontNormal)||(PaperState == sPaperBackNormal))) // if(FlagSystemPrint && CRGRunning)
		if(FlagSystemPrint&&((PaperState == sPaperFrontNormal)||(PaperState == sPaperBackNormal)))
		{
			TimPos_HeadWork4F16_Handler();//TimPos_HeadWork3F16_Handler();
			/*if(FlagPrinter == eFlagPRTFast)
				TimPos_HeadWork3F16_Handler();
			else if(FlagPrinter == eFlagPRTStand)
				TimPos_HeadWork4F16_Handler();
			else if(FlagPrinter == eFlagPRTDeep)
				TimPos_HeadWork5F16_Handler();*/
		}
		else
		{
			FlagHeadIC = 0x0000;
			HeadIC();
		}

		if(CRGNowTimePosition != CRGStopTimePosition)
		{
			// 马达操作需要每16次中断才执行一次
			//if(	(((CRGNowTimePosition%CRGOneStepTimePosition) == (CRGOneStepTimePosition-1))&&(CRGRightDir)) || (((CRGNowTimePosition%CRGOneStepTimePosition) == 0)&&(CRGLeftDir)) )
			if((CRGNowTimePosition%CRGOneStepTimePosition) == 0)
			{
				CRGPhase();
/*if(LedLine)
	LedLineOpen
else
	LedLineClose*/
				// From the perspective of rear paper feeding direction, the left side is considered as the 0 point. Sensors are on the left side. This describes the initialization process of the carriage motor.
				if(CRGState != 0x55)							// 0x55: finished INIT
				{
					HomeRecorder <<= 1;	if(HOME)	HomeRecorder |= 0x01;
					// 只用进HOME完成初始化过程
					if(HomeRecorder == 0x01)					// to left
					{
						CRGNowTimePosition = TimePositionInHome-1*CRGOneStepTimePosition;
						CRGState = 0x55;					// Finished INIT
					}
					/*else if(HomeRecorder == 0xfe)				// to right
					{
						CRGNowTimePosition = TimePositionInHome+1*CRGOneStepTimePosition;
						CRGState = 0x55;					// Finished INIT
					}*/
				}

				// 计算时间片位置信息，重新计算中断时间
				if(CRGNowTimePosition > CRGStopTimePosition)
				{
					Count = (CRGNowTimePosition - CRGStopTimePosition)/CRGOneStepTimePosition;
					// CRGNowTimePosition--
				}
				else
				{
					Count = (CRGStopTimePosition - CRGNowTimePosition)/CRGOneStepTimePosition;
					// CRGNowTimePosition++;
				}
				/*if(CRGRightDir)
				{
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
				}
				else*/
				{
					if(Count > (cCRGSpeed+1))
					{
						if(cCRGSpeed < CRGVolatileStep)	cCRGSpeed++;
					}
					else if(Count < (cCRGSpeed+1)) //Alke 20230703 <= (-->)<
					{
						if(cCRGSpeed)	cCRGSpeed--;
					}
				}

				// 计算加减速时间
			/*	if(CMDOneDir||TMPOneDir) // one direction print
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
					if(CRGRightDir)
					{
						if(Count > (cCRGSpeed+1))
							c32time = *(TABLECRGSpeedRightAcc+cCRGSpeed);
						else
							c32time = *(TABLECRGSpeedRightDec+cCRGSpeed);
					}
					else
					{
						if(Count > (cCRGSpeed+1))
							c32time = *(TABLECRGSpeedLeftAcc+cCRGSpeed);
						else
							c32time = *(TABLECRGSpeedLeftDec+cCRGSpeed);
					}
				}
				c32time = *(TABLECRGSpeedRightAcc+cCRGSpeed);
				c32time /= CRGOneStepTimePosition;
			}

			if(CRGRightDir)
			{
				// 步数为n时，时间片位置就为n+1
				if(CRGNowTimePosition < (TimePositionAll))
					CRGNowTimePosition++;
			}
			else
			{
				if(CRGNowTimePosition)//if(CRGNowTimePosition != CRGStopTimePosition)
					CRGNowTimePosition--;
			}
//printf("0x%x", FlagMotorIC);
//printf("%d", c32time);
/*printf("%d", CRGNowTimePosition);
if(LedErr)
	LedErrOpen
else
	LedErrClose*/
		}
		// 马达运动结束 不需要判断是否16次的完整循环：两个位置相等的话，一定是16次完整循环
		else //if((CRGNowTimePosition%CRGOneStepTimePosition) == 0)
		{
//printf("4-%d-%d-%d-%d-%d", BufferPrint[cBufferPrintLineRead].cLeft, BufferPrint[cBufferPrintLineRead].cRight, CRGNowTimePosition, CRGStopTimePosition, BufferPrint[cBufferPrintLineRead].Step);
//printf("Ce");

// Based on the print flag, determine whether this time is for idle position positioning or actual printing. Adjust the pointer after printing. If not printed, return and process again.
			if(FlagSystemPrint&&(BufferPrint[cBufferPrintLineRead].cLeft!=BufferPrint[cBufferPrintLineRead].cRight))
			{
				if(cBufferPrintLineRead != cBufferPrintLineWrite)
				{
					for(cBufferPrintDataWrite = 0; cBufferPrintDataWrite < BufferPRTSize; cBufferPrintDataWrite++)
					{
						BufferPrint[cBufferPrintLineRead].BufferA[cBufferPrintDataWrite]=0;
						BufferPrint[cBufferPrintLineRead].BufferB[cBufferPrintDataWrite]=0;
					}
					cBufferPrintDataWrite = 0;
					BufferPrint[cBufferPrintLineRead].cLeft = 0;
					BufferPrint[cBufferPrintLineRead].cRight = 0;
					if(BufferPrint[cBufferPrintLineRead].Step == 0)
					{
						if(cBufferPrintLineRead >= (BufferPrintLineNum-1))
							cBufferPrintLineRead = 0;
						else
							cBufferPrintLineRead++;
					}
				}
//printf("4+");
			}
			else
			{
				;
			}
//printf("4E%d-%d", CRGNowTimePosition, CRGStopTimePosition);

			CRGCurrent(1);
			PrintingState = sINIT;
			c16ForCRG = 0;
			c32time = *TABLECRGSpeedRightAcc; //Lash Time
		}
		break;
	}

	return c32time;
}


/**************************************************************************************************************
	McuInt.c	McuInt.h
**************************************************************************************************************/

