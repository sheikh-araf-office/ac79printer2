
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
		if(CRGNowTimePosition < CRGVolatileStep*CRGOneStepTimePosition)
			goto HeadWork4F16End;

	// ��Ҫ�������ұ߽磬��ֹ����ʱָ�볬����������Χ
	// ���������ӡ���˴����������뼴����߽磬�����߽翼�Ǹ����ͺ���ʱ�ֱ���
	if(CRGLeftDir)
		if(CRGNowTimePosition >= (CRGVolatileStep*CRGOneStepTimePosition+TimePositionPrintSection))
			goto HeadWork4F16End;

	c32c1 = CRGNowTimePosition/BufferOneDataTimePosition-CRGVolatileStep*CRGOneStepTimePosition/BufferOneDataTimePosition;
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
		if(CRGNowTimePosition < CRGVolatileStep*CRGOneStepTimePosition)
			goto HeadWork5F16End;

	// ��Ҫ�������ұ߽磬��ֹ����ʱָ�볬����������Χ
	// ���������ӡ���˴����������뼴����߽磬�����߽翼�Ǹ����ͺ���ʱ�ֱ���
	if(CRGLeftDir)
		if(CRGNowTimePosition >= (CRGVolatileStep*CRGOneStepTimePosition+TimePositionPrintSection))
			goto HeadWork5F16End;

	c32c1 = CRGNowTimePosition/BufferOneDataTimePosition-CRGVolatileStep*CRGOneStepTimePosition/BufferOneDataTimePosition;
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
// ϵͳ��ʱ������Ҫÿ20����ִ��һ�� static ___interrupt TSEC void timer_isr2(void)
// ���Բ��ٷŵ���ʱ���У�����û����ѭ�������ڴ�ѭ����ִ��
void Tim_Sys_Handler(void)
{
	static volatile unsigned char c8ForKeyAdc = 5;	// KEYADC����ɨ��
	static volatile unsigned char c8ForPsPaper = 0;	// ֽ�Ŵ�����ɨ��
	static volatile unsigned char c8ForPower = 0;	// KEYPOWER���ػ�����
	static volatile unsigned int c16ForCRG = 0;
	static volatile unsigned int c16ForPAP = 0;
	volatile unsigned int c16c1 = 0;

	
	// Power on/off function
	switch(PowerState)
	{
	case 0:	// �ϵ�ȷ��״̬
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
		if(c8ForPower > 20)
		{
			PowerState = 20;
		}
		break;
	case 20: // 1s���ɿ�����û��Ӱ��
		if(KeyPower)
		{
			PowerState = 10;
		}
		if(c8ForPower > 20)// 500*2ms=1000ms
		{
			c8ForPower = 0;
			PowerState = 21;
		}
		break;
	default:
	case 21: // 2s��˸led�ȴ��˹��ػ�ȷ��
		MainPowerOff

		if(c8ForPower%20 == 0)
		{
			if(LedErr)
				LedErrOpen
			else
				LedErrClose
		}
		if(c8ForPower >= 60)
			c8ForPower = 0;
		break;
	}
	if(c8ForPower < 0xff)
		c8ForPower++;

	// ϵͳ��ʱ������
	if(DelayTimeCount2mS)
		DelayTimeCount2mS--;

#if 0
if(c16ForPAP < 200)
	c16ForPAP++;
else
	c16ForPAP = 0;
if(c16ForPAP == 0)
{
	LedLineOpen
	LedErrClose

	HdClrHigh;
	HdDiHigh;
	HdGoutHigh;
	HdRckHigh;
	HdSckHigh;

	MtClrHigh;
	MtDiHigh;
	MtGoutHigh;
	MtRckHigh;
	MtSckHigh;
}
else if(c16ForPAP == 100)
{
	LedLineClose
	LedErrOpen
	
	HdClrLow;
	HdDiLow;
	HdGoutLow;
	HdRckLow;
	HdSckLow;

	MtClrLow;
	MtDiLow;
	MtGoutLow;
	MtRckLow;
	MtSckLow;
}
return;
#endif
	
	// ���ȷ�ϴ�ӡ������
	if((cBufferPrintLineRead != cBufferPrintLineWrite) && (BufferPrint[cBufferPrintLineRead].cLeft == BufferPrint[cBufferPrintLineRead].cRight) && (BufferPrint[cBufferPrintLineRead].Step == 0))
	{
		if(cBufferPrintLineRead >= (BufferPrintLineNum-1))
			cBufferPrintLineRead = 0;
		else
			cBufferPrintLineRead++;
	}
#if 0
	// ��ֽ���С��������
	if((FlagMotorIC&0x30) == 0x10)
	{
		if(c16ForPAP < 50*2) // 20ms*50*2 = 2s @@@@@
			c16ForPAP++;
		else
			PAPCurrent(0);
	}
	else
		c16ForPAP = 0;

	// �ֳ����С��������
	if((FlagMotorIC&0x03) == 0x01)
	{
		if(c16ForCRG < 50*2) // 20ms*50*2 = 2s @@@@@
			c16ForCRG++;
		else
			CRGCurrent(0);
	}
	else
		c16ForCRG = 0;
#endif

	
	/*if(DebugCRGHome)
		CRGState = 0x55;
	if(DebugPaper)
		PAPState = 0x55;*/
	// �����ֳ�����ʼ��
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
	// ������ֽ����ʼ�����ȴ��ֳ�����ʼ����ɣ�������д��ӡ�������ĳ�ͻ����
	else if(PAPState != 0x55)
	{
		PAPMoveAction:
		if(BufferPrintLineWrited() < BufferPrintLineFull)
		{
			BufferPrint[cBufferPrintLineWrite].Step = 4;
			BufferPrint[cBufferPrintLineWrite].Dir = 1;
				
			if(cBufferPrintLineWrite >= (BufferPrintLineNum-1))
				cBufferPrintLineWrite = 0;
			else
				cBufferPrintLineWrite++;

			BufferPrint[cBufferPrintLineWrite].Step = 4;
			BufferPrint[cBufferPrintLineWrite].Dir = 0;
				
			if(cBufferPrintLineWrite >= (BufferPrintLineNum-1))
				cBufferPrintLineWrite = 0;
			else
				cBufferPrintLineWrite++;
		}
	}

	// HeadTempAdc
	ADCTemp = adc_get_value(AdcChTemp);
	if(ADCTemp < AdcTempLever)
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

	// PaperSensor scan
	SensorRecorderFrontOut <<= 1;
	if(adc_get_value(AdcChFrontOut) < ADC4F8)
		SensorRecorderFrontOut |= 0x01;
	SensorRecorderFrontIn <<= 1;
	if(adc_get_value(AdcChFrontIn) < ADC4F8)
		SensorRecorderFrontIn |= 0x01;
	SensorRecorderBackIn <<= 1;
	if(adc_get_value(AdcChBackIn) < ADC4F8)
		SensorRecorderBackIn |= 0x01;
	SensorRecorderBackOut <<= 1;
	if(adc_get_value(AdcChBackOut) < ADC4F8)
		SensorRecorderBackOut |= 0x01;
	// ����ֽ�Ŵ�����״̬����װֽ�߼�
	switch(PaperState)
	{
	default:
	case sPaperNo:
		// ��̬��⴫����״̬���Ա�ȷ��ֽ��״̬
		if((SensorRecorderFrontOut == 0xff)&&(SensorRecorderFrontIn == 0xff))
		{
			if((SensorRecorderBackOut == 0xff)&&(SensorRecorderBackIn == 0xff))
			{
				PaperState = sPaperBackNormal; // ǰ����ֽ�����ֽģʽ
			}
			else
			{
				if(BufferPrintLineWrited() < BufferPrintLineFull) // ���β��ܸ���ָ�룬��Ҫ��ϳ�ʼ����̬������ֽ����
				{
					BufferPrint[cBufferPrintLineWrite].Step = StepPageLenth+StepFrontToPrint;
					BufferPrint[cBufferPrintLineWrite].Dir = sPaperDirBackToFront;
				
					if(cBufferPrintLineWrite >= (BufferPrintLineNum-1))
						cBufferPrintLineWrite = 0;
					else
						cBufferPrintLineWrite++;
					
					PaperState = sPaperFrontEnd; // ǰ��ֽ����Ҫ��ֽ��ȥ��װֽ
				}
			}
		}
		else
		{
			if((SensorRecorderBackOut == 0xff)&&(SensorRecorderBackIn == 0xff))
			{
				PaperState = sPaperBackNormal; // ������ֽ�����ֽģʽ
			}
			else
			{
				PaperState = sPaperNo; // ǰ����ֽ����ǰ״̬
			}
		}
		// ��̬���ǰ��ֽװֽ����
		if((SensorRecorderFrontOut == 0x3f)&&(SensorRecorderFrontIn == 0x00))
		{
			if(BufferPrintLineWrited() < BufferPrintLineFull) // ���β��ܸ���ָ�룬��Ҫ��ϳ�ʼ����̬������ֽ����
			{
				BufferPrint[cBufferPrintLineWrite].Step = StepPageLenth;
				BufferPrint[cBufferPrintLineWrite].Dir = sPaperDirFrontToBack;

				if(cBufferPrintLineRead == cBufferPrintLineWrite)
				{
					if(cBufferPrintLineWrite >= (BufferPrintLineNum-1))
						cBufferPrintLineWrite = 0;
				else
					cBufferPrintLineWrite++;
				}

				PaperState = sPaperFrontInit; // ǰ��ֽ
			}
		}
		break;
		
	case sPaperFrontInit:
		if(PAPRunning)
		{
			if(SensorRecorderFrontIn == 0x7f)
			{
				if(PAPMoveStep > PAPVolatileStep)
					PAPMoveStep = StepFrontToPrint;
			}
		}
		else
		{
			if((SensorRecorderFrontOut == 0xff) && (SensorRecorderFrontIn == 0xff))
			{
				PaperCount = 0;
				StepPageCount = StepPageLenthFront;
				PaperState = sPaperFrontNormal;
			}
			else
				PaperState = sPaperNo;
		}
		break;
		
	case sPaperFrontNormal:
		//if(!PAPRunning)
		if(PaperCount > StepPageLenth)
		{
			if(BufferPrintLineWrited() < BufferPrintLineFull)
			{
				BufferPrint[cBufferPrintLineWrite].Step = StepPageLenth+StepFrontToPrint*2; // ����ֽStepFrontToPrint��ȷ��ֽ���˳�ȥ
				BufferPrint[cBufferPrintLineWrite].Dir = sPaperDirBackToFront;
				
				if(cBufferPrintLineWrite >= (BufferPrintLineNum-1))
					cBufferPrintLineWrite = 0;
				else
					cBufferPrintLineWrite++;
				PaperState = sPaperFrontEnd;
			}
		}
		else if((SensorRecorderFrontOut == 0x00)&&(SensorRecorderFrontIn == 0x00))
		{
			if(PaperCount < (StepPageLenth-StepFrontToPrint))
				PaperState = sPaperNo;
		}
		break;
		
	case sPaperFrontEnd:
	#if 1
		if(PAPRunning)
		{
			if((SensorRecorderFrontOut == 0x00) && (SensorRecorderFrontIn == 0x00))
			{
			//	if(BufferPrint[cBufferPrintLineWrite].Step > PAPVolatileStep)
					BufferPrint[cBufferPrintLineWrite].Step = PAPVolatileStep;
				BufferPrint[cBufferPrintLineWrite].Dir = sPaperDirBackToFront;
				
				if(cBufferPrintLineWrite >= (BufferPrintLineNum-1))
					cBufferPrintLineWrite = 0;
				else
					cBufferPrintLineWrite++;

				PaperState = sPaperNo;
			}
		}
		else
		{
			if((BufferPrintLineWrited() < BufferPrintLineFull) && (!PAPRunning))
			{
				BufferPrint[cBufferPrintLineWrite].Step = StepPageLenth+StepFrontToPrint;
				BufferPrint[cBufferPrintLineWrite].Dir = sPaperDirBackToFront;

				/*if(cBufferPrintLineWrite >= (BufferPrintLineNum-1))
				cBufferPrintLineWrite = 0;
				else
					cBufferPrintLineWrite++;*/
			}
		}
	#else
		if((BufferPrintLineWrited() < BufferPrintLineFull) && (!PAPRunning))
		{
			BufferPrint[cBufferPrintLineWrite].Step = StepPageLenth+StepFrontToPrint;
			BufferPrint[cBufferPrintLineWrite].Dir = sPaperDirBackToFront;
				
			/*if(cBufferPrintLineWrite >= (BufferPrintLineNum-1))
				cBufferPrintLineWrite = 0;
			else
				cBufferPrintLineWrite++;*/
					
			//PaperState = sPaperFrontEnd; // ǰ��ֽ����Ҫ��ֽ��ȥ��װֽ
		}
		if((SensorRecorderFrontOut == 0x00) && (SensorRecorderFrontIn == 0x00))
		{
			BufferPrint[cBufferPrintLineWrite].Step = PAPVolatileStep;
			BufferPrint[cBufferPrintLineWrite].Dir = sPaperDirBackToFront;
				
			if(cBufferPrintLineWrite >= (BufferPrintLineNum-1))
				cBufferPrintLineWrite = 0;
			else
				cBufferPrintLineWrite++;

			PaperState = sPaperNo;
		}
	#endif
		break;
		
	case sPaperBackInit:
		
		break;
		
	case sPaperBackNormal:
		
		break;
		
	case sPaperBackEnd:
		
		break;
	}

#if 0
printf("PaperState:  %d  PaperCount: %d \r\n", PaperState, PaperCount);	
#endif
#if 0
	if(c8ForPsPaper < 50)
		c8ForPsPaper++;
	else
	{
		c8ForPsPaper = 0;
		
		ADCKEY = adc_get_value(AdcChKey);
		c16c1 = adc_get_voltage(AdcChKey);
		printf("ADCKEY = %d  >>>  %d mv    ", ADCKEY, c16c1);
		ADCUsb = adc_get_value(AdcChUsb);
		c16c1 = adc_get_voltage(AdcChUsb);
		printf("ADCUsb = %d  >>>  %d mv    ", ADCUsb, c16c1);
		if(USBin)
			printf("UsbIn = High  ");
		else
			printf("UsbIn = Low  ");
		ADCTemp = adc_get_value(AdcChTemp);
		c16c1 = adc_get_voltage(AdcChTemp);
		printf("ADCTemp = %d  >>>  %d mv\n", ADCTemp, c16c1);
		
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

#if 0
	HeadState
#endif
	
#if 1
	//LedDisplay
	if(PowerState == 10) // ֻ�п��ػ���ɺ󣬲Ż���������״̬��LED������Դ����״̬������������״̬
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

}
// ��ֽ��ʱ��ɨ��ʱ�����Ҫ�����ֳ�ɨ��ʱ�䣺�ֳ�timer��������ֽ����Ҫ��֤��ֽtimer��ʱ����
// @20230616������Ĳ��Խ��е��������ǵ����ܻ�ʹ��0a��������ֽ����������Ҫ�����ֳ���������������ֽ�������Ǵ�ӡ������ֽ�����������ӡ������ֽ
// ֻ��  0D  0A��ֽ�����󣬲Ż������ӡ������ָ��
// �����Ż��Ļ������Ը�������timer�ķ�ʽ����
#define TimePAPScan	2000	//
#define TimeCRGScan	2000//20000	//
unsigned long Tim_PAP_Handler(void)
{
	unsigned long c32time = TimePAPScan;
	static unsigned int c16ForPAP;

	switch (PrintingState)
	{
	case sINIT:
		if((cBufferPrintLineRead != cBufferPrintLineWrite) && (BufferPrint[cBufferPrintLineRead].Step) && (!CRGRunning))
			PrintingState = sStartPAPMotor;
		else // �������С������������ʱ�ر����
		{
			if((FlagMotorIC&0x30) == 0x10)
			{
				if(c16ForPAP < 1000)
					c16ForPAP++;
				else
					PAPCurrent(0);
			}
			else
				c16ForPAP = 0;
		}
		c32time = TimePAPScan; // û����ֽ����ʱ������೤ʱ��ɨ���ӡ������
		break;

	case sStartPAPMotor:
		PAPMoveStep = (BufferPrint[cBufferPrintLineRead].Step)*2;
		if(BufferPrint[cBufferPrintLineRead].Dir)
			PAPBackSet
		else
			PAPFeedSet
		cPAPSpeed = 0;
		PAPCurrent(2);
//@@@@@		�ֳ�����жϺ��� ��ֽ����жϺ���������SWITCH��д�󣬾�Ȼ���ϵ粻��

		// �ֳ����ڽ����������ӡ��������־��������ֽ��������ʱ�������ر�־���Ա��ܹ�����ֽ�����в�����ֽ����ʹ����ֽ���᲻�ٴ�
		BufferPrint[cBufferPrintLineRead].Step = 0;
		if(cBufferPrintLineRead >= (BufferPrintLineNum-1))
			cBufferPrintLineRead = 0;
		else
			cBufferPrintLineRead++;

		PrintingState = sWaitingPAPMotor;
		c32time = *TABLEPAPSpeed; // Hold Time
		break;

	case sWaitingPAPMotor:
		if(PAPMoveStep)
		{
			// ��ֽ״̬��,��ͬ�����²�����ֽ����
			if((cBufferPrintLineRead != cBufferPrintLineWrite) && (BufferPrint[cBufferPrintLineRead].Step))
			{
				if(	(PAPFeedDir && BufferPrint[cBufferPrintLineRead].Dir == 0)	||
					(PAPBackDir && BufferPrint[cBufferPrintLineRead].Dir)	)
				{
					PAPMoveStep += (BufferPrint[cBufferPrintLineRead].Step)*2;

					BufferPrint[cBufferPrintLineRead].Step = 0;
			
					if(cBufferPrintLineRead >= (BufferPrintLineNum-1))
						cBufferPrintLineRead = 0;
					else
						cBufferPrintLineRead++;
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
			/*else
			{
				if(PAPState == 0xf0)
				{
					if(PaperCount >= StepPageLenthFront)
						PAPState = 0x00;
					PaperCount = 0;
				}
			}*/

			c32time = *(TABLEPAPSpeed+cPAPSpeed);
		}
		else
		{
			// �ֳ����ڽ����������ӡ��������־��������ֽ��������ʱ�������ر�־���Ա��ܹ�����ֽ�����в�����ֽ����ʹ����ֽ���᲻�ٴ�
					
			// �˴������������С����
			// ���׹ر���������Ҫ��ϵͳ��ʱ�������
			PAPCurrent(1);
			PrintingState = sINIT;
			c32time = *TABLEPAPSpeed;
		}
		break;

		// @20230702 PAP��CRG��״̬����ֻ�и��Ե������������Ĭ�ϵķ�֧�жϣ����߹�����ͬ��״̬����
	/*default:
		PAPCurrent(0);
		PrintingState = sINIT;
		c32time = TimePAPScan;
		break;*/
	}
	
	return c32time;
}

unsigned long TimPos_CRG_Handler(void)
{
	// ����ʹ��static��ÿ16�βŻ����һ��ʱ�䣬����15�ζ���Ҫʹ���ϴε�ʱ��
	static unsigned long c32time = TimeCRGScan*CRGOneStepTimePosition; // �˴�ʱ��Ҫ16������
	unsigned short Count;
	static unsigned int c16ForCRG;


	switch (PrintingState)
	{
	case sINIT:
//	case sStartPAPMotor:
//	case sWaitingPAPMotor:
		if((cBufferPrintLineRead != cBufferPrintLineWrite) && ((BufferPrint[cBufferPrintLineRead].cLeft < BufferPrint[cBufferPrintLineRead].cRight)) && (!PAPRunning))
			PrintingState = sStartCRGMotor;
		else // �������С������������ʱ�ر����
		{
			if((FlagMotorIC&0x03) == 0x01)
			{
				if(c16ForCRG < 1000)
					c16ForCRG++;
				else
					CRGCurrent(0);
			}
			else
				c16ForCRG = 0;
		}
		
		c32time = TimeCRGScan; // û�д�ӡ����ʱ������೤ʱ��ɨ���ӡ������
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
FlagPrinter = eFlagPRTFast; // @@@@@
		/** ��������˶�ʱ���������ʵ�ʱ����г��������ÿ��ʱ��Ƭλ�ö���Ҫ���㿼�ǳ��� **/
		if(FlagSystemPrint&&(CRGState == 0x55)&&((PaperState == sPaperFrontNormal)||(PaperState == sPaperBackNormal))) // if(FlagSystemPrint && CRGRunning)
		{
			if(FlagPrinter == eFlagPRTFast)
				TimPos_HeadWork3F16_Handler();
			else if(FlagPrinter == eFlagPRTStand)
				TimPos_HeadWork4F16_Handler();
			else if(FlagPrinter == eFlagPRTDeep)
				TimPos_HeadWork5F16_Handler();
		}

		// ��������Ҫÿ16���жϲ�ִ��һ��
		if((CRGNowTimePosition%CRGOneStepTimePosition) == 0)
		{
			// ����˶�����
			/*if(CRGNowTimePosition == CRGStopTimePosition)
			{
				// ���ݴ�ӡ��־��ȷ�������ǿ���λ�ö�λ����������ӡ
				if(FlagSystemPrint)
				{
					BufferPrint[cBufferPrintLineRead].cLeft = 0;
					BufferPrint[cBufferPrintLineRead].cRight = 0;

					if(cBufferPrintLineRead >= (BufferPrintLineNum-1))
						cBufferPrintLineRead = 0;
					else
						cBufferPrintLineRead++;
					
					// �˴������������С����
					// ���׹ر���������Ҫ��ϵͳ��ʱ�������
					CRGCurrent(1);
					PrintingState = sINIT;
				}
				else
				{
					// �ո�û�д�ӡ���������������ʼ��λ�������ٴ���������ӡ
					PrintingState = sStartCRGMotor;
				}
				c32time = *TABLECRGSpeedRightAcc; //Lash Time
			}
			// ����˶���
			else*/
			if(CRGNowTimePosition != CRGStopTimePosition)
			{
				CRGPhase();
	
				// ���Ϊ0�㣬����������ߣ����ֽ�����ӽǣ��ֳ�����ʼ������
				if(CRGState != 0x55)							// 0x55: finished INIT
				{
					CRGState <<= 1;
					if(HOME)
						CRGState |= 0x01;
					// ֻ�ý�HOME��ɳ�ʼ������
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

				// ����ʱ��Ƭλ����Ϣ�����¼����ж�ʱ��
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
				
				// ����Ӽ���ʱ��
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
			// ����Ϊnʱ��ʱ��Ƭλ�þ�Ϊn+1
			if(CRGNowTimePosition < (TimePositionAll))
				//if(CRGNowTimePosition != CRGStopTimePosition)
					CRGNowTimePosition++;
		}

		// ����˶�����
		if(CRGNowTimePosition == CRGStopTimePosition)
		{
			// ���ݴ�ӡ��־��ȷ�������ǿ���λ�ö�λ����������ӡ
			if(FlagSystemPrint)
			{
				BufferPrint[cBufferPrintLineRead].cLeft = 0;
				BufferPrint[cBufferPrintLineRead].cRight = 0;

				if(cBufferPrintLineRead >= (BufferPrintLineNum-1))
					cBufferPrintLineRead = 0;
				else
					cBufferPrintLineRead++;
					
				// �˴������������С����
				// ���׹ر���������Ҫ��ϵͳ��ʱ�������
				CRGCurrent(1);
				PrintingState = sINIT;
			}
			else
			{
				// �ո�û�д�ӡ���������������ʼ��λ�������ٴ���������ӡ
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

