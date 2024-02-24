
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



// �˴���������ݱ�������ţ�����ʱ�����
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

	// ��Ҫ�������ұ߽磬��ֹ����ʱָ�볬����������Χ
	// �������Ҵ�ӡ���˴����������뼴����߽磬�����߽翼�Ǹ����ͺ���ʱ�ֱ���
	if(CRGRightDir)
	{
		if((CRGNowTimePosition < CRGVolatileStep*CRGOneStepTimePosition) || (CRGNowTimePosition > (CRGVolatileStep*CRGOneStepTimePosition+TimePositionPrintSection+15*CRGOneStepTimePosition)))
		{
			FlagHeadIC = 0x0000;
			goto HeadWork3F16End;
		}
	}
	// ��Ҫ�������ұ߽磬��ֹ����ʱָ�볬����������Χ
	// ���������ӡ���˴����������뼴����߽磬�����߽翼�Ǹ����ͺ���ʱ�ֱ���
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

	// ��Ҫ�������ұ߽磬��ֹ����ʱָ�볬����������Χ
	// �������Ҵ�ӡ���˴����������뼴����߽磬�����߽翼�Ǹ����ͺ���ʱ�ֱ���
	if(CRGRightDir)
		if(CRGNowTimePosition < CRGVolatileStep*CRGOneStepTimePosition)
			goto HeadWork3F16End;

	// ��Ҫ�������ұ߽磬��ֹ����ʱָ�볬����������Χ
	// ���������ӡ���˴����������뼴����߽磬�����߽翼�Ǹ����ͺ���ʱ�ֱ���
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

	// ��Ҫ�������ұ߽磬��ֹ����ʱָ�볬����������Χ
	// �������Ҵ�ӡ���˴����������뼴����߽磬�����߽翼�Ǹ����ͺ���ʱ�ֱ���
	if(CRGRightDir)
	{
		if(CRGNowTimePosition < CRGVolatileStep*CRGOneStepTimePosition)
		{
			FlagHeadIC = 0x0000;
			goto HeadWork4F16End;
		}
	}

	// ��Ҫ�������ұ߽磬��ֹ����ʱָ�볬����������Χ
	// ���������ӡ���˴����������뼴����߽磬�����߽翼�Ǹ����ͺ���ʱ�ֱ���
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

	// ��Ҫ�������ұ߽磬��ֹ����ʱָ�볬����������Χ
	// �������Ҵ�ӡ���˴����������뼴����߽磬�����߽翼�Ǹ����ͺ���ʱ�ֱ���
	if(CRGRightDir)
	{
		if(CRGNowTimePosition < CRGVolatileStep*CRGOneStepTimePosition)
		{
			FlagHeadIC = 0x0000;
			goto HeadWork5F16End;
		}
	}

	// ��Ҫ�������ұ߽磬��ֹ����ʱָ�볬����������Χ
	// ���������ӡ���˴����������뼴����߽磬�����߽翼�Ǹ����ͺ���ʱ�ֱ���
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
// ϵͳ��ʱ������Ҫÿ20����ִ��һ�� static ___interrupt TSEC void timer_isr2(void)
// ���Բ��ٷŵ���ʱ���У�����û����ѭ�������ڴ�ѭ����ִ��
// ����ѭ����ʽ����ɨ����������Դ˼��ٸ��ж�ִ��ʱ��
void Tim_Sys_Handler(void)
{
	static volatile unsigned int c16ForData = 0;	// 0~4: 4������ͨ���л�
//	static volatile unsigned char c8ForScan = 0;	// 0~4: 5��һ��ѭ��������ģ������ɨ��ִ��
//	static volatile unsigned char c8ForKeyAdc = 5;	// KEYADC����ɨ��
	static volatile unsigned char c8ForPsPaper = 0;	// ֽ�Ŵ�����ɨ��
	static volatile unsigned char c8ForPower = 0;	// KEYPOWER���ػ�����
//	static volatile unsigned int c16ForCRG = 0;
	static volatile unsigned int c16ForPAP = 0;
	static volatile unsigned int c16ForLedLine = 0;
	static volatile unsigned int c16ForLedErr = 0;
//	volatile unsigned int c16c1 = 0;

//PowerState=10;

	// Power on/off function
	switch(PowerState)
	{
	case 0:	// �ϵ�ȷ��״̬
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
	case 1: // �ϵ��Ѿ�ȷ�ϣ��ȴ������ɿ�
		if(KeyPower)
		{
			LedLineClose
			LedErrClose
			PowerState = 10;
		}
		break;
	case 10: // �ϵ������ɣ�������������״̬
		if(KeyPower)
			c8ForPower = 0;
		if(c8ForPower > 4*5)
		{
			PowerState = 20;
		}
		break;
	case 20: // �ػ����̣�1s���ɿ�����û��Ӱ��
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
	case 21: // 2s��˸led�ȴ��˹��ػ�ȷ��
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


	// ϵͳ��ʱ������
	if(DelayTimeCount20mS)
		DelayTimeCount20mS--;

	// 4������ͨ���л�ɨ��
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
				if(c16ForData<50*30) // ����60���ٴ�ɨ������ͨ��
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
	else //if(DataFromChannel==0) // �������ݽ�������ʱ�޷�ȷ������ͨ��
	{
		c16ForData = 0;
	}

	// ���ȷ�ϴ�ӡ������
	if((cBufferPrintLineRead != cBufferPrintLineWrite) && (BufferPrint[cBufferPrintLineRead].cLeft == BufferPrint[cBufferPrintLineRead].cRight) && (BufferPrint[cBufferPrintLineRead].Step == 0))
	{
//printf("R��=W s==0 -----\r\n");
		if(cBufferPrintLineRead >= (BufferPrintLineNum-1))
			cBufferPrintLineRead = 0;
		else
			cBufferPrintLineRead++;
	}


	// HeadTempAdc
/*	ADCTemp = adc_get_value(AdcChTemp);
	if(ADCTemp < AdcTempLever) // ��ֵ��С�����¶ȹ���
		TMPOneDirSet
	else if(ADCTemp > 900) // ��ֵ�ӽ�����1023������������Ͽ�
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
		if(ADCTemp < AdcTempLever) // ��ֵ��С�����¶ȹ���
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
	// WIFI���ù��̵Ĳ�ͬ�׶Σ���ʱ��ʹ��
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
	if(PowerState == 10) // ֻ�п��ػ���ɺ󣬲Ż���������״̬��LED������Դ����״̬������������״̬
	{
		if(WifiState)
		{
			if(WifiState == 5)		// ���ӳɹ�������
				LedLineOpen
			else if(WifiState == 6)	// ����ʧ�ܣ�������˸
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
			else					// ����������Ҫ�ٴγ������ӣ�������˸
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
		// �˴�ÿ���ӻش�һ������
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
// ��ֽ��ʱ��ɨ��ʱ�����Ҫ�����ֳ�ɨ��ʱ�䣺�ֳ�timer��������ֽ����Ҫ��֤��ֽtimer��ʱ����
// @20230616������Ĳ��Խ��е��������ǵ����ܻ�ʹ��0a��������ֽ����������Ҫ�����ֳ���������������ֽ�������Ǵ�ӡ������ֽ�����������ӡ������ֽ
// ֻ��  0D  0A��ֽ�����󣬲Ż������ӡ������ָ��
// �����Ż��Ļ������Ը�������timer�ķ�ʽ����: @20231010������ʹ�ã����ֹͣʱ���һ��ʱ��ɨɨ�贫����״̬��ֻ����һ��timer�����ֽ������
#define TimePAPScan	10000//2000	// 10msҲ�պÿ��������Ķ�λ��ɲ��ʱ��
#define TimeCRGScan	10000//2000	//
unsigned long Tim_PAP_Handler(void)
{
	static unsigned char FlagPAPRead = 0;
	unsigned long c32time = TimePAPScan;
	static unsigned int c16ForPAP; // ��ֽ��ﳬʱ������
	static unsigned int c16ForPaper; // ֽ�Ŵ�������ʱ������

	// ����ֽ�Ŵ�����״̬����װֽ�߼�
	#if DebugPaper
		PaperState = sPaperBackNormal;
		PAPCurrent(0);
		return c32time;
	#endif

	// ����ȴ��ֳ��������ɳ�ʼ���������������״̬�������PrintingState�ǹ��õ�
	if(CRGState!=0x55)
	{
		PAPCurrent(0);
		return c32time;
	}

	// �ö�ʱ����ʼ�ռ�⴫����״̬�������Ϊֹͣ״̬ʱ��ɨ��ʱ��ΪTimePAPScan���Ա�������ֽ���
	// ������˶�ʱ������ÿ����⴫����״̬
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
		// 1������ֽ����ʼ��
		if(PAPState==0xff) // �Ⱥ���
		{
			PAPState = 0x0a;
			PAPBackToFrontSet
			PAPMoveStep = 4; // 4 ��λƥ����
		}
		else if(PAPState==0x0a)
		{
			if(PAPMoveStep == 0)
			{
				PAPState = 0xaa;
				PAPFrontToBackSet
				PAPMoveStep = 4; // 4 ��λƥ����
			}
		}
		else if(PAPState==0xaa) // ��ǰ��
		{
			if(PAPMoveStep == 0)
				PAPState = 0x55;
		}
		// 2����ֽ�Ŵ�����״̬���ֽ�ų�ʼ��
		else if(PAPState==0x55)
		{
			switch(PaperState)
			{
			default:
			case sPaperNo:
				// ��̬��⴫����״̬���Ա�ȷ��ֽ��״̬
				if((SensorRecorderFrontOut == 0xff)&&(SensorRecorderFrontIn == 0xff))
				{
					if((SensorRecorderBackOut == 0xff)&&(SensorRecorderBackIn == 0xff))
					{
//printf("BN-");
						PaperCount = StepBackToPrint; //StepToCutPos	// û��ҳ�������Ϊ�ڴ�ӡ��ʼλ������ҳ�����Ӧ����˺ֽλ��
						PaperState = sPaperBackNormal; 	// ǰ����ֽ�����ֽģʽ: ���ֽΪ����ֽ����ʱû��ҳ������
					}
					else
					{
//printf("0");//printf("FE-");
						PaperState = sPaperFrontEnd; 	// ��ǰ����ֽ����Ҫ��ֽ��ȥ��װֽ
					}
				}
				else if((SensorRecorderBackOut == 0xff)&&(SensorRecorderBackIn == 0xff))
				{
//printf("BI-");
					PaperState = sPaperBackInit; 		// ������ֽ�����ֽģʽ����Ҫ��λֽ�ű߽�
				}
				else									// ǰ����ֽ����Ҫװֽȷ��״̬�����ǵ�ǰ״̬
				{
					if((SensorRecorderFrontOut == 0xff)&&(SensorRecorderFrontIn == 0x00))
					{
						if(c16ForPaper < 50*2)
							c16ForPaper++;
						else
						{
//printf("FI-");
							c16ForPaper = 0;
							PaperState = sPaperFrontInit; // ǰ��ֽ
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
					if(adc_get_value(AdcChFrontIn) < ADC4F8) // ��ֽ
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

			case sPaperFrontEnd: // �������������ʼǰֽ���ޣ�����ֽ������ӡ������ǰ�޺�ֽ�����˳���ҳ��
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
							PaperState = sPaperNo; 							// ��ֽ�����󣬼���Ϊ��ֽ״̬
							PAPMoveStep = StepFrontToPrint*2;				// ��ʼǰֽ���ޣ�����ֽ����
					}
						else
						{
///printf("SPL");//printf("FE4");
							PaperState = sPaperNo;
							PAPMoveStep = StepPageLenth+StepFrontToPrint*2;	// ��ӡ������ǰ�޺�ֽ�����˳���ҳ��
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
						PaperState = sPaperNo; // ȫ����ֽ״̬�Żָ���ֽ����״̬
					}
				}
				else
				{
					c16ForPaper = 0;
				}
				break;
			}
			//ֹͣ��ֽʱ����ʱɨ��ֽ�Ŵ�����״̬�������������
		}

		// 3���ݴ�ӡ������������״̬��ǣ���д10ҳ�Ĵ�ӡ������
		// 3�����������д�Ĵ�ӡ��������������������ֱ��дPAPMoveStep����������˶�����ӡ��������ֽ������Ҫ�ڴ˳���2                					 ����???
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
				PAPMoveStep = BufferPrint[cBufferPrintLineRead].Step*2; // ����ȴ���ֽ��������д0����ֹ�ֳ�������ǰ����
				//BufferPrint[cBufferPrintLineRead].Step = 0; //����ʱ���жϸ���ֵȷ��β��ֽ����󣬲�д0��ǧ������ǰд0
				PrintingState = sStartPAPMotor;
			}
			else if((FlagMotorIC&0x30) == 0x00) // ���ȫ���������л���С����
			{
				c16ForPAP = 0;
				PAPCurrent(1);
			}
			else if((FlagMotorIC&0x30) != 0x30) // ���û�йرյ�������ʱ�رյ���
			{
				if(c16ForPAP < 100)  // TimePAPScan*100=1000=1s
					c16ForPAP++;
				else
					PAPCurrent(0);
			}
			else
				c16ForPAP = 0;
		}

		// 4ֻ������ֽ���������Ƿ��������
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
			// ��Ҫ���ǳ�ʼ����ҳĩ�������̬���¸�״̬
			if(PaperState==sPaperFrontNormal)													// ����״̬���ܹ�������ӡ
			{
				//if(BufferPrint[cBufferPrintLineRead].Dir == sPaperDirFrontToBack)
				if(PAPFrontToBackDir)
				{
//printf("P+");
					PaperCount++;
					// ��ֽ״̬��,��ͬ�����²�����ֽ����
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
					else if(PaperCount>=(StepPageLenth+StepFrontToPrint-32))						// ֽβ�ѹ���ӡλ�ã�������ֽ״̬
					{
						if(PAPMoveStep > PAPVolatileStep)
							PAPMoveStep = PAPVolatileStep;
						PaperState = sPaperFrontEnd;												// �л���ֽ�Ž���״̬���ȴ����ֹͣ������ֽ����
						//FlagSystemPrintClr // ��ֽ�󣬲���ӡ����ֹ��ֽʱ��ӡ
					}
//printf("3");
				}
				else // ��������ֽ
				{
//printf("P-");
					if(StepPageLenth)	StepPageLenth--;
					if(PaperCount)
						PaperCount--;	//if(SensorRecorderFrontIn==0xfe)	StepPageLenth = PaperCount;
					else if(SensorRecorderFrontIn==0xfe)
					{
						if(PAPMoveStep>StepFrontToPrint)
							PAPMoveStep = StepFrontToPrint;
						PaperState = sPaperNo; //ֹͣ�󣬵ȴ���ʱ��Ϊ��ֽ״̬
						//FlagSystemPrintClr
					}
					else if(SensorRecorderFrontIn==0x00)
						PaperState = sPaperNo;
//printf("3-----");
				}
			}
			else if(PaperState==sPaperBackNormal)												// ����״̬���ܹ�������ӡ
			{
				//PaperSensorScan();
				PaperCount++;
				// ��ֽ״̬��,��ͬ�����²�����ֽ����
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
				else if(PaperCount>=(StepPageLenth+StepBackToPrint-32))							// ֽβ�ѹ���ӡλ�ã�������ֽ״̬
				{
					PAPMoveStep += StepBackToPrint*2;
					PaperState = sPaperNo;														// ������ǰ��ֽ�߳�����������ֽ����״̬
				}
			}
			else if(PaperState==sPaperFrontInit)										// ǰ��ֽ��ʼ��״̬
			{
//printf("9");
				PaperCount++;
				if(SensorRecorderFrontIn)
					StepPageLenth = PaperCount;
				else
					StepPageLenth = 0;
				if((SensorRecorderFrontOut==0xff)&&(SensorRecorderFrontIn==0xff))		// 2������ֽ״̬
				{
//printf("b");		
					// @Alke 20231022 ���뵽��ֽͷλ�òſ���ǰ��ֽģʽ
					if(PaperCount >= StepFrontToPrint)
						PaperState = sPaperFrontNormal;
				}
				else if((SensorRecorderFrontOut==0xff)&&(SensorRecorderFrontIn==0x7f))	// 1��⵽ҳ�ף�������������ҳ��
				{
//printf("a");
					//PaperCount = 1;
					//StepPageLenth = PaperCount;
					PAPMoveStep = StepFrontToPrint+PAPVolatileStep;
				}
				else if(PAPMoveStep<PAPVolatileStep)									// 0��ֽ������������û���ҵ�ҳͷ������ֽ�Ŵ���״̬�������˹���Ԥ
				{
					PaperState = sPaperErr;
				}
				/*else if((SensorRecorderFrontOut==0x00)&&(SensorRecorderFrontIn==0x00))	// 3��ֽ���س�ʼ״̬
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
				if((SensorRecorderBackOut==0xff)&&(SensorRecorderBackIn==0xff))			// 2������ֽ״̬
				{
					PaperState = sPaperBackNormal;
				}
				else if((SensorRecorderBackOut==0xff)&&(SensorRecorderBackIn==0x01))	// 1��⵽ҳ�ף�������������ҳ��
				{
					PaperCount = 1;
					PAPMoveStep = StepBackToPrint;
				}
				else if(PAPMoveStep<PAPVolatileStep)									// 0��ֽ������������û���ҵ�ҳͷ������ֽ�Ŵ���״̬�������˹���Ԥ
				{
					PaperState = sPaperErr;
				}
				/*else if((SensorRecorderBackOut==0x00)&&(SensorRecorderBackIn==0x00))	// 3��ֽ���س�ʼ״̬
				{
					if(PAPMoveStep > PAPVolatileStep)
						PAPMoveStep = PAPVolatileStep;
					PaperState = sPaperNo;
				}*/
			}
			else if(PaperState==sPaperFrontEnd)
			{
//printf("PageEnd");
				//PaperState = sPaperNo; // ��Գ�ʼ��ʱ����ֽ���������ֹͣ��ָ���ʼ״̬�������������ֽ�ŵ�ҳβʱ����Ҫ��ֽ��ȥ
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
			// �ֳ����ڽ����������ӡ��������־��������ֽ��������ʱ�������ر�־���Ա��ܹ�����ֽ�����в�����ֽ����ʹ����ֽ���᲻�ٴ�
			// ֻ����ֽ�������д0����ֹ�ֳ�������ǰ����
			// @Alke 20231023 ����ʱ��ֽ���������ֽ������ģʽ(��װֽ����)���ܵ���ָ�룬Ҳ�����Ǵ�ӡ����
			// ���������������ֽ����£����ܵ���ָ��
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
			// @20231022 �����ʱ��ǰ��ֽģʽ�ĵ���˺ֽ�ڵ���ֽ������ֹͣ����Ҫ���ص���ֽ״̬���������Ϊ�������⴫������ֽ��ĩβ���߼�ֽ��״̬
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
	// ����ʹ��static��ÿ16�βŻ����һ��ʱ�䣬����15�ζ���Ҫʹ���ϴε�ʱ��
	static unsigned long c32time = TimeCRGScan;//*CRGOneStepTimePosition; // �˴�ʱ��Ҫ16������
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

			if(HOME)	// �ܹ�˿������
			{
				HomeRecorder = 0xff;
				CRGNowTimePosition = 0;
				CRGStopTimePosition = TimePositionAll;
			}
			else		// ��������
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
			// ���ֽ�ų�ʼ������
			if((PaperState!=sPaperFrontNormal)&&(PaperState!=sPaperBackNormal)&&(CRGStopTimePosition != TimePositionAll/2))
			{
				FlagSystemPrintClr
				CRGStopTimePosition = TimePositionAll/2;
			}
		}

//printf("0-%d-%d", CRGNowTimePosition, CRGStopTimePosition);
		// ��ﲻ�˶��Ļ��Ž�������
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
			else if((FlagMotorIC&0x03) == 0x00) // ���ȫ���������л���С����
			{
				c16ForCRG = 0;
				CRGCurrent(1);
			}
			else if((FlagMotorIC&0x03) != 0x03) // ���û�йرյ�������ʱ�رյ���
			{
				if(c16ForCRG < 100)  // TimeCRGScan*100=1000=1s
					c16ForCRG++;
				else
					CRGCurrent(0);
			}
			else
				c16ForCRG = 0;
		}

		// ֻ�����ֳ�λ�þ����Ƿ��������
		if(CRGNowTimePosition != CRGStopTimePosition)
		{
//printf("Cs");
			PrintingState = sStartCRGMotor;
		}

		c32time = TimeCRGScan; // û�д�ӡ����ʱ������೤ʱ��ɨ���ӡ������
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
		/** ��������˶�ʱ���������ʵ�ʱ����г��������ÿ��ʱ��Ƭλ�ö���Ҫ���㿼�ǳ��� **/
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
			// ��������Ҫÿ16���жϲ�ִ��һ��
			//if(	(((CRGNowTimePosition%CRGOneStepTimePosition) == (CRGOneStepTimePosition-1))&&(CRGRightDir)) || (((CRGNowTimePosition%CRGOneStepTimePosition) == 0)&&(CRGLeftDir)) )
			if((CRGNowTimePosition%CRGOneStepTimePosition) == 0)
			{
				CRGPhase();
/*if(LedLine)
	LedLineOpen
else
	LedLineClose*/
				// ���ֽ�����ӽǣ����Ϊ0�㣬����������ߣ��ֳ�����ʼ������
				if(CRGState != 0x55)							// 0x55: finished INIT
				{
					HomeRecorder <<= 1;	if(HOME)	HomeRecorder |= 0x01;
					// ֻ�ý�HOME��ɳ�ʼ������
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

				// ����ʱ��Ƭλ����Ϣ�����¼����ж�ʱ��
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

				// ����Ӽ���ʱ��
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
				// ����Ϊnʱ��ʱ��Ƭλ�þ�Ϊn+1
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
		// ����˶����� ����Ҫ�ж��Ƿ�16�ε�����ѭ��������λ����ȵĻ���һ����16������ѭ��
		else //if((CRGNowTimePosition%CRGOneStepTimePosition) == 0)
		{
//printf("4-%d-%d-%d-%d-%d", BufferPrint[cBufferPrintLineRead].cLeft, BufferPrint[cBufferPrintLineRead].cRight, CRGNowTimePosition, CRGStopTimePosition, BufferPrint[cBufferPrintLineRead].Step);
//printf("Ce");
			// ���ݴ�ӡ��־��ȷ�������ǿ���λ�ö�λ����������ӡ����ӡ�����ָ�룬δ��ӡ�����ٴδ�����
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

