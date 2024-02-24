
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
	{
		if((CRGNowTimePosition < CRGVolatileStep*CRGOneStepTimePosition) || (CRGNowTimePosition > (CRGVolatileStep*CRGOneStepTimePosition+TimePositionPrintSection+15*CRGOneStepTimePosition)))
		{
			FlagHeadIC = 0x0000;
			goto HeadWork3F16End;
		}
	}
	// 需要限制左右边界，防止计算时指针超出缓冲区范围
	// 从右向左打印，此处检查最早出针即右起边界，结束边界考虑各针滞后延时分别考虑
	else //if(CRGLeftDir)
	{
		if((CRGNowTimePosition < (CRGVolatileStep*CRGOneStepTimePosition-15*CRGOneStepTimePosition)) || (CRGNowTimePosition > (CRGVolatileStep*CRGOneStepTimePosition+TimePositionPrintSection)))
		{
			FlagHeadIC = 0x00;
			goto HeadWork3F16End;
		}
	}

	c32c1 = CRGNowTimePosition/BufferOneDataTimePosition-CRGVolatileStep*CRGOneStepTimePosition/BufferOneDataTimePosition;
	if(CRGRightDir)
		c32c1 -= HeadDelayCount;
	c32c2 = CRGNowTimePosition%BufferOneDataTimePosition;

/*if(c32c2 <= 2)
	FlagHeadIC=0xffff;
else
	FlagHeadIC=0x00;
goto
	HeadWork3F16End;*/

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
	{
		if(CRGNowTimePosition < CRGVolatileStep*CRGOneStepTimePosition)
		{
			FlagHeadIC = 0x0000;
			goto HeadWork4F16End;
		}
	}

	// 需要限制左右边界，防止计算时指针超出缓冲区范围
	// 从右向左打印，此处检查最早出针即右起边界，结束边界考虑各针滞后延时分别考虑
	if(CRGLeftDir)
	{
		if(CRGNowTimePosition >= (CRGVolatileStep*CRGOneStepTimePosition+TimePositionPrintSection))
		{
			FlagHeadIC = 0x00;
			goto HeadWork4F16End;
		}
	}

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
	{
		if(CRGNowTimePosition < CRGVolatileStep*CRGOneStepTimePosition)
		{
			FlagHeadIC = 0x0000;
			goto HeadWork5F16End;
		}
	}

	// 需要限制左右边界，防止计算时指针超出缓冲区范围
	// 从右向左打印，此处检查最早出针即右起边界，结束边界考虑各针滞后延时分别考虑
	if(CRGLeftDir)
	{
		if(CRGNowTimePosition >= (CRGVolatileStep*CRGOneStepTimePosition+TimePositionPrintSection))
		{
			FlagHeadIC = 0x00;
			goto HeadWork5F16End;
		}
	}

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

extern unsigned long Tlen;
extern unsigned long Tlen1;
extern Device_Rev USB_Rev;

extern struct sBufferPrint BufferPrint[];

/**************************************************************************************************************
	McuInt.c	McuInt.h
**************************************************************************************************************/
// 系统定时器，需要每20毫秒执行一次 static ___interrupt TSEC void timer_isr2(void)
// 可以不再放到定时器中，由于没有死循环，可在大循环中执行
// 采用循环方式依次扫描各个任务，以此减少该中断执行时间
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
	case 0:	// 上电确认状态
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
	case 20: // 关机过程，1s内松开按键没有影响
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
else
{
PowerState =21;
MainPowerOff
LedErrClose
LedLineClose
}


	// 系统延时计数器
	if(DelayTimeCount20mS)
		DelayTimeCount20mS--;

	// 4个数据通道切换扫描
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
				if(c16ForData<50*30) // 超过60秒再次扫描数据通道
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
	else //if(DataFromChannel==0) // 尚无数据进来，暂时无法确定数据通道
	{
		c16ForData = 0;
	}

	// 检查确认打印缓冲区
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
		if((PaperState==sPaperFrontNormal)||(PaperState==sPaperBackNormal))
		{
			if(BufferPrintLineWrited()==0)
			{
				if((KeyPaperRecorder == 0xff) && (KeyPaperDownTime > 50))
				{
					PAPMoveStep += 24;
					PAPBackToFrontSet
				}
				else if((KeyLineRecorder == 0xff) && (KeyLineDownTime > 50))
				{
					PAPMoveStep += 24;
					PAPFrontToBackSet
				}
				else if(PAPMoveStep == 0)
				{
					if(KeyPaperRecorder == 0xf0)
					{
						PAPMoveStep = 24;
						PAPBackToFrontSet
					}
					else if(KeyLineRecorder == 0xf0)
					{
						PAPMoveStep = 24;
						PAPFrontToBackSet
					}
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


#if 1
	//LedDisplay
	if(PowerState == 10) // 只有开关机完成后，才会体现其他状态的LED，即电源正常状态才体现其他的状态
	{
		if(WifiState)
		{
			if(WifiState == 5)		// 连接成功，长亮
				LedLineOpen
			else if(WifiState == 6)	// 连接失败，慢速闪烁
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
			else					// 其他错误，需要再次尝试连接，快速闪烁
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
		// 此处每秒钟回传一次数据
		c8ForPsPaper = 0;
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
	
	if(c8ForPsPaper == 0)
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
		printf("%d", c16ForData);
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
	}
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
// 走纸定时器扫描时间必须要少于字车扫描时间：字车timer会启动走纸，需要保证走纸timer及时启动
// @20230616对上面的策略进行调整：考虑到可能会使用0a，所以走纸启动，必须要经过字车启动后，再启动走纸；可以是打印后再走纸，或者无需打印启动走纸
// 只有  0D  0A走纸结束后，才会调整打印缓冲区指针
// 后续优化的话，可以改用启动timer的方式进行: @20231010不会再使用：马达停止时间隔一定时间扫扫描传感器状态，只是用一个timer完成走纸马达操作
#define TimePAPScan	10000//2000	// 10ms也刚好可以做马达的定位和刹车时间
#define TimeCRGScan	10000//2000	//
unsigned long Tim_PAP_Handler(void)
{
	static unsigned char FlagPAPRead = 0;
	unsigned long c32time = TimePAPScan;
	static unsigned int c16ForPAP; // 走纸马达超时计数器
	static unsigned int c16ForPaper; // 纸张传感器超时计数器

	// 根据纸张传感器状态决定装纸逻辑
	#if DebugPaper
		PaperState = sPaperBackNormal;
		PAPCurrent(0);
		return c32time;
	#endif

	// 必须等待字车马达先完成初始化动作，两者马达状态管理参数PrintingState是公用的
	if(CRGState!=0x55)
	{
		PAPCurrent(0);
		return c32time;
	}

	// 该定时器内始终检测传感器状态，当马达为停止状态时，扫描时间为TimePAPScan，以便启动走纸马达
	// 当马达运动时，就是每步检测传感器状态
	// PaperSensor scan
	if((PaperState==sPaperFrontNormal)||(PaperState==sPaperBackNormal))
	{
		if(FlagMotorIC&0x30)//&&(cPAPSpeed))
			PaperSensorScan();
	}
	else
		PaperSensorScan();

	switch (PrintingState)
	{
	case sINIT:
		// 1优先走纸马达初始化
		if(PAPState==0xff) // 先后走
		{
			PAPState = 0x0a;
			PAPBackToFrontSet
			PAPMoveStep = 4; // 4 相位匹配用
		}
		else if(PAPState==0x0a)
		{
			if(PAPMoveStep == 0)
			{
				PAPState = 0xaa;
				PAPFrontToBackSet
				PAPMoveStep = 4; // 4 相位匹配用
			}
		}
		else if(PAPState==0xaa) // 再前走
		{
			if(PAPMoveStep == 0)
				PAPState = 0x55;
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
//printf("BN-");
						PaperCount = StepBackToPrint; //StepToCutPos	// 没有页长概念，认为在打印起始位；如有页长概念，应该在撕纸位置
						PaperState = sPaperBackNormal; 	// 前后都有纸，后进纸模式: 后进纸为连续纸，暂时没有页长概念
					}
					else
					{
//printf("0");//printf("FE-");
						PaperState = sPaperFrontEnd; 	// 仅前边有纸，需要退纸出去再装纸
					}
				}
				else if((SensorRecorderBackOut == 0xff)&&(SensorRecorderBackIn == 0xff))
				{
//printf("BI-");
					PaperState = sPaperBackInit; 		// 仅后部有纸，后进纸模式，需要定位纸张边界
				}
				else									// 前后都无纸，需要装纸确定状态，就是当前状态
				{
					if((SensorRecorderFrontOut == 0xff)&&(SensorRecorderFrontIn == 0x00))
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
					else
					{
//printf("No-");//
						PaperCount = 0;
						c16ForPaper = 0;
					}
				}
				break;

			case sPaperFrontInit:
//printf("FI1");
				PAPFrontToBackSet
				PAPMoveStep = StepFrontToPrint*5;
				break;

			case sPaperFrontNormal:
//printf("FN-");
				if(((FlagMotorIC&0x30) == 0x30) && (PaperCount<StepFrontToPrint*2))
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
				}
				break;

			case sPaperFrontEnd: // 有两种情况：初始前纸后无，仅退纸少许；打印结束，前无后纸，需退超过页长
//printf("1");//printf("FE1");
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

			case sPaperBackInit:
//printf("BI1");
				PAPBackToFrontSet
				PAPMoveStep = StepBackToPrint*2;
				break;

			case sPaperBackNormal:
				break;

			case sPaperBackEnd:
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
		}

		// 3根据打印缓冲区及各种状态标记，填写10页的打印缓冲区
		// 3命令解析后填写的打印缓冲区，结合其他情况，直接写PAPMoveStep，启动马达运动，打印命令中走纸步数需要在此乘以2                					 存疑???
		if(PAPMoveStep == 0)
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

		// 4只根据走纸步数决定是否启动马达
		if(PAPMoveStep)
		{
//printf("1");////printf("4");
			PrintingState = sStartPAPMotor;
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
		break;

	case sWaitingPAPMotor:
		if(PAPMoveStep)
		{
//printf("-");//printf("PM-");
			// 需要考虑初始化及页末情况，动态更新该状态
			if(PaperState==sPaperFrontNormal)													// 正常状态，能够正常打印
			{
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

					if(SensorRecorderFrontIn!=0x00)
						StepPageLenth = PaperCount;
					else if(PaperCount>=(StepPageLenth+StepFrontToPrint-32))						// 纸尾已过打印位置，进入无纸状态
					{
						if(PAPMoveStep > PAPVolatileStep)
							PAPMoveStep = PAPVolatileStep;
						PaperState = sPaperFrontEnd;												// 切换到纸张结束状态，等待马达停止后，再退纸出来
						//FlagSystemPrintClr // 无纸后，不打印，防止进纸时打印
					}
//printf("3");
				}
				else // 反方向退纸
				{
//printf("P-");
					if(StepPageLenth)	StepPageLenth--;
					if(PaperCount)
						PaperCount--;	//if(SensorRecorderFrontIn==0xfe)	StepPageLenth = PaperCount;
					else if(SensorRecorderFrontIn==0xfe)
					{
						if(PAPMoveStep>StepFrontToPrint)
							PAPMoveStep = StepFrontToPrint;
						PaperState = sPaperNo; //停止后，等待超时成为无纸状态
						//FlagSystemPrintClr
					}
					else if(SensorRecorderFrontIn==0x00)
						PaperState = sPaperNo;
//printf("3-----");
				}
			}
			else if(PaperState==sPaperBackNormal)												// 正常状态，能够正常打印
			{
				//PaperSensorScan();
				PaperCount++;
				// 走纸状态下,相同方向下补充走纸步数
				/*if((cBufferPrintLineRead != cBufferPrintLineWrite) && (BufferPrint[cBufferPrintLineRead].Step))
				{
					if(BufferPrint[cBufferPrintLineRead].Dir == sPaperDirBackToFront)
					{
						PAPMoveStep += (BufferPrint[cBufferPrintLineRead].Step);
						BufferPrint[cBufferPrintLineRead].Step = 0;
						/*if(cBufferPrintLineRead >= (BufferPrintLineNum-1))
							cBufferPrintLineRead = 0;
						else
							cBufferPrintLineRead++;///////////////////////////////////////////
					}
				}*/

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
//printf("9");
				PaperCount++;
				if(SensorRecorderFrontIn)
					StepPageLenth = PaperCount;
				else
					StepPageLenth = 0;
				if((SensorRecorderFrontOut==0xff)&&(SensorRecorderFrontIn==0xff))		// 2进入有纸状态
				{
//printf("b");		
					// @Alke 20231022 必须到达纸头位置才开启前进纸模式
					if(PaperCount >= StepFrontToPrint)
						PaperState = sPaperFrontNormal;
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
			}
			else if(PaperState==sPaperBackInit)
			{
				PaperCount++;
				if(SensorRecorderBackIn!=0x00)
					StepPageLenth = PaperCount;
				else
					StepPageLenth = 0;
				if((SensorRecorderBackOut==0xff)&&(SensorRecorderBackIn==0xff))			// 2进入有纸状态
				{
					PaperState = sPaperBackNormal;
				}
				else if((SensorRecorderBackOut==0xff)&&(SensorRecorderBackIn==0x01))	// 1检测到页首，调整步数到达页首
				{
					PaperCount = 1;
					PAPMoveStep = StepBackToPrint;
				}
				else if(PAPMoveStep<PAPVolatileStep)									// 0走纸即将结束，仍没能找到页头，进入纸张错误状态，必须人工干预
				{
					PaperState = sPaperErr;
				}
				/*else if((SensorRecorderBackOut==0x00)&&(SensorRecorderBackIn==0x00))	// 3无纸返回初始状态
				{
					if(PAPMoveStep > PAPVolatileStep)
						PAPMoveStep = PAPVolatileStep;
					PaperState = sPaperNo;
				}*/
			}
			else if(PaperState==sPaperFrontEnd)
			{
//printf("PageEnd");
				//PaperState = sPaperNo; // 针对初始化时的退纸操作，马达停止后恢复初始状态；但如果是正常纸张到页尾时，需要退纸出去
			}
			else if(PaperState==sPaperBackEnd)
			{;}

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
		else
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
			if((PaperState==sPaperFrontNormal)&&(PAPBackToFrontDir))//&&(SensorRecorderFrontIn==0x00))
			{
				PaperState = sPaperNo;
			}

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
	default:
	case sINIT:
		if(CRGState == 0xff)
		{
			FlagSystemPrintClr

			if(HOME)	// 能够丝滑运行
			{
				HomeRecorder = 0xff;
				CRGNowTimePosition = 0;
				CRGStopTimePosition = TimePositionAll;
			}
			else		// 不能运行
			{
				HomeRecorder = 0x00;
				CRGNowTimePosition = TimePositionAll;
				CRGStopTimePosition = 0;
			}
			CRGState = 0x05;
		}
		else if(CRGState == 0x05)
		{
			if(CRGNowTimePosition == CRGStopTimePosition)
			{
				if(CRGNowTimePosition)
					CRGStopTimePosition = 0;
				else
					CRGStopTimePosition = TimePositionAll;
			}
		}
		else if(CRGState == 0x55)
		{
			// 配合纸张初始化动作
			if((PaperState!=sPaperFrontNormal)&&(PaperState!=sPaperBackNormal)&&(CRGStopTimePosition != TimePositionAll/2))
			{
				FlagSystemPrintClr
				CRGStopTimePosition = TimePositionAll/2;
			}
		}

//printf("0-%d-%d", CRGNowTimePosition, CRGStopTimePosition);
		// 马达不运动的话才解析命令
		if(CRGNowTimePosition == CRGStopTimePosition)
		{
			if(	(!PAPRunning)&&(!CRGRunning)&&((PaperState==sPaperFrontNormal)||(PaperState==sPaperFrontNormal)) && //(PaperCount>=StepFrontToPrint) &&
				(cBufferPrintLineRead != cBufferPrintLineWrite) && ((BufferPrint[cBufferPrintLineRead].cLeft < BufferPrint[cBufferPrintLineRead].cRight)) && (BufferPrint[cBufferPrintLineRead].Step==0)	)
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
		/** 马达匀速运动时，可以在适当时间进行出针操作，每个时间片位置都需要计算考虑出针 **/
		//if(FlagSystemPrint&&(CRGState == 0x55)&&((PaperState == sPaperFrontNormal)||(PaperState == sPaperBackNormal))) // if(FlagSystemPrint && CRGRunning)
		if(FlagSystemPrint&&((PaperState == sPaperFrontNormal)||(PaperState == sPaperBackNormal)))
		{
			if(FlagPrinter == eFlagPRTFast)
				TimPos_HeadWork3F16_Handler();
			else if(FlagPrinter == eFlagPRTStand)
				TimPos_HeadWork4F16_Handler();
			else if(FlagPrinter == eFlagPRTDeep)
				TimPos_HeadWork5F16_Handler();
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
				// 后进纸方向视角，左边为0点，传感器在左边，字车马达初始化过程
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
			// 根据打印标志来确定本次是空跑位置定位还是真正打印，打印后调整指针，未打印返回再次处理即可
			if(FlagSystemPrint&&(BufferPrint[cBufferPrintLineRead].cLeft!=BufferPrint[cBufferPrintLineRead].cRight))
			{
				if(cBufferPrintLineRead != cBufferPrintLineWrite)
				{
					BufferPrint[cBufferPrintLineRead].cLeft = 0;
					BufferPrint[cBufferPrintLineRead].cRight = 0;

					if(cBufferPrintLineRead >= (BufferPrintLineNum-1))
						cBufferPrintLineRead = 0;
					else
						cBufferPrintLineRead++;
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

