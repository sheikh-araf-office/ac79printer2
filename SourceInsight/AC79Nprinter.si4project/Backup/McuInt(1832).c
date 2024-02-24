
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
// ϵͳ��ʱ������Ҫÿ2����ִ��һ��
// ���Բ��ٷŵ���ʱ���У�����û����ѭ�������ڴ�ѭ����ִ��
void Tim_Sys_Handler(void)
{
	static volatile unsigned char c8ForKeyAdc = 5;		// KEYADC����ɨ��
	static volatile unsigned char c8ForPsPaper = 0;	// ֽ�Ŵ�����ɨ��
	static volatile unsigned int c16ForPower = 0;	// KEYPOWER���ػ�����

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

	// ϵͳ��ʱ������
	if(DelayTimeCount2mS)
		DelayTimeCount2mS--;

	// Ledline ��ͬ��˸״̬����ʾWIFI���ù��̵Ĳ�ͬ�׶�
	// ������WiFi�Ѿ�OK
	// ��˸1/2/3/4/5����ʾ���ֲ�ͬ�����ý׶�
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
	if(PowerState == 0)		// �ϵ�ȷ��״̬
	{
		if(!KeyPower)
			c16ForPower = 0;
		if(c16ForPower >= 50) // 50*2mS=100ms
		{
			PowerState = 1;
		}
	}
	else if(PowerState == 1)// �ϵ��Ѿ�ȷ�ϣ��ȴ������ɿ�
	{
		if(!KeyPower)
		{
			MainPowerOn
			PowerState = 10;
		}
	}
	else if(PowerState == 10)// �ϵ������ɣ�������������״̬
	{
		c16ForPower = 0;
		if(KeyPower)
		{
			PowerState = 21;
		}
	}
	else if(PowerState == 21)// 1s���ɿ�����û��Ӱ��
	{
		if(!KeyPower)
			PowerState = 10;
		if(c16ForPower >= 500)// 500*2ms=1000ms
		{
			c16ForPower = 0;
			PowerState = 22;
		}
	}
	else if(PowerState == 22)// 2s��˸led�ȴ��˹��ػ�ȷ��
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
		if(c16ForPower >= 100)// 100*2ms=200ms 1000->100����ػ���ȷ��
		{
			MainPowerOff
			PowerState = 23;
		}
	}
	else if(PowerState == 23)
	{
		if(c16ForPower%100 == 0) // 50->100��˸Ƶ����֮ǰ����һֱ
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
	//����а��������򲻼��ֽ�Ŵ���
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
	// ֽ�Ŵ�����״̬ɨ��
//	c8ForPsPaper
	if(PrinttingMode==PrintForward)
	{
		if(RecordPaperInstep>500&&SangYi==0)
		{
			SangYi=1;
		//	SendString("λ�� ����\r\n");
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

	// ����ɨ��
	if(FlagKeyPadNo == 0x0000)
	{
		if(c8ForKeyAdc)
		{
			c8ForKeyAdc--;
		}
		else
		{
			// ����2*5=10msɨ��һ��
			// ĳ����������ʱ����Ҫ��ʱ�ϳ�ʱ�䣬�Ա�ADC��ѹ���䵽0λ����ֹ���а���
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
	// æ�źŶ�ʱ���
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
// ��ֽ��ʱ��ɨ��ʱ�����Ҫ�����ֳ�ɨ��ʱ�䣺�ֳ�timer��������ֽ����Ҫ��֤��ֽtimer��ʱ����
// @20230616������Ĳ��Խ��е��������ǵ����ܻ�ʹ��0a��������ֽ����������Ҫ�����ֳ���������������ֽ�������Ǵ�ӡ������ֽ�����������ӡ������ֽ
// ֻ��  0D  0A��ֽ�����󣬲Ż������ӡ������ָ��
// �����Ż��Ļ������Ը�������timer�ķ�ʽ����
#define TimePAPScan	20000	//
#define TimeCRGScan	25000	//
unsigned long Tim_PAP_Handler(void)
{
	unsigned long c32time = TimePAPScan;

	if(PAPMoveStep == 0)		// stop
	{
		cPAPSpeed = 0;

		// ֹͣ״̬��������ֽ
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
		// ��ֽ״̬�²�����ֽ����
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
	static unsigned long c32time = TimeCRGScan; // 50ms �ֳ�ֹͣ�˶���ʱ���ж�ʱ��
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

	/** ��������˶�ʱ���������ʵ�ʱ����г��������ÿ��ʱ��Ƭλ�ö���Ҫ���㿼�ǳ��� **/
if(0)//	if(FlagSystemPrint)
	{
		if(FlagPrinter == FlagPRTFast)
			TimPos_HeadWork3F16_Handler();
		else if(FlagPrinter == FlagPRTStand)
			TimPos_HeadWork4F16_Handler();
		else// if(FlagPrinter == FlagPRTDeep)
			TimPos_HeadWork5F16_Handler();
	}

	// ֻ��������˶�������Ž����¸��׶εĻ
	if(CRGNowTimePosition == CRGStopTimePosition)	// Stop
	{
		cCRGSpeed = 0;

		// ��Ϊ��ӡ���ȣ��ʴ˲��ؿ�����ֽ��״̬
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
				
 			/* // �������ҷ�0����λ��
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
			*/ // �������ҷ�0����λ��
			
					// �����ӡ��ֱ����ת�����ڴ���������ת��ʱ����
					if((BufferPrint[cBufferPrintLineRead].cLeft == 0) && (BufferPrint[cBufferPrintLineRead].cRight == 0))
					{
						PrintingState == sWaitingHeadPrinting;
						goto WaitingHeadPrinting; // ������ת��ʱ����
					}
					// ������������Ҫ��ӡ
					else if(BufferPrint[cBufferPrintLineRead].cLeft <= BufferPrint[cBufferPrintLineRead].cRight)
					{
						PrintingState = sStartCRGMotor;
						goto StartCRGMotor; // ������ת��ʱ����
					}
					// �����ӡ��ֱ����ת�����ڴ���������ת��ʱ����
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
						PrintingState = sWaitingHeadPrinting; // ֱ��������ӡ
					}
					else if(CRGNowTimePosition >= BufferPrint[cBufferPrintLineRead].cRight)
					{
						CRGStopTimePosition = BufferPrint[cBufferPrintLineRead].cLeft;
						CRGLeftSet
						CRGCurrent(2);
						if(TMPOneDir || CMDOneDir)
						{
							FlagSystemPrintClr
							PrintingState = sWaitingCRGMotor; // ��ҪԤ�ȵ���׼��λ��
						}
						else
						{
							FlagSystemPrintSet
							PrintingState = sWaitingHeadPrinting; // ֱ��������ӡ
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
						PrintingState = sWaitingCRGMotor; // ��ҪԤ�ȵ���׼��λ��
					}
				}
			}
			else if(PrintingState == sWaitingCRGMotor)
			{
				WaitingCRGMotor:
				if(!CRGRuning)
				{
					PrintingState = sStartCRGMotor;
					goto StartCRGMotor; // ������ת��ʱ����
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

		// ���Ϊ0�㣬����������ߣ��Ժ��ֽ�����ӽ�
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

