
//#define Driver 0
/**************************************************************************************************************
	Driver.c	Driver.h
**************************************************************************************************************/

#include "Driver.h"
#include "McuInt.h"
#include "UserInterface.h"



// @20230601 �����µĳ���ܹ����˺���Ϊ�յ����ݺ�Ĵ����¼�
// �˺�������һ���ֽ�һ���ֽڽ��д����˹����в�������ѭ����ֻ��һ�ι����������ñ�־λ

// ���Դ�����
void CmdPrint0d(void) // ���ֽ�����û�к�������
{
	Command.Type = eCmdInit;
	Command.State = 0;
	Command.Size = 0;
	printf("CmdPrint0d\n");
}

// �������ֽ�Ϊ1b��������
void Cmd1bClass(unsigned char data)
{
//	printf("Cmd1bClass--\n");

	switch(data)
	{
	case 0x0a:
		Command.Type = eCmd1b0a;
		break;

	case 0x2a:
		Command.Type = eCmd1b2aDots;
		break;

	case 0x4a:
		Command.Type = eCmd1b4aPAP;
		break;

	case 0x4b:
		Command.Type = eCmd1b4bPAP;
		break;

	case 0x55:
		Command.Type = eCmd1b55PrtDir;
		break;

	case 0x7a:
		Command.Type = eCmd1b7a;
		break;

	case 0x89:
		Command.Type = eCmd1b89;
		break;

	case 0x9c:
		Command.Type = eCmd1b9c;
		break;

	case 0xa0:
		Command.Type = eCmd1ba0;
		break;

	default:
		Command.Type = eCmdInit;
		break;
	}
}
void Cmd1b0a(unsigned char data)
{
	switch(Command.State)
	{
	case 0:
		//if(data == 1)
			Command.State = 1; // ������Ч
		break;

	case 1:
		if(data == 0x01)
			FlagPrinter = eFlagPRTStand;
		else if(data == 0x02)
			FlagPrinter = eFlagPRTDeep;
		else if(data == 0x03)
			FlagPrinter = eFlagPRTStand;
		else
			FlagPrinter = eFlagPRTFast;

		Command.Type = eCmdInit;
		Command.State = 0;
		break;

	default:
		Command.Type = eCmdInit;
		Command.State = 0;
		printf("Cmd1b0a\n");
		break;
	}
}
void Cmd1b2aDots(unsigned char data)
{
	volatile unsigned int c16c1;

	switch(Command.State)
	{
	case 0:
//printf("Dots-00\n");
			if(data == 0x00)
			Command.State = 10; // ����ͼ������
		else if(data == 0x01)
			Command.State = 20; // ���ͼ������
		break;

	case 10: // ����ͼ�����ݣ�Size��8λ
		Command.Size = data;
		Command.State++;
		break;
	case 11: // ����ͼ�����ݣ�Size��8λ
		c16c1 = data;
		c16c1 <<= 8;
		Command.Size += c16c1;
		cBufferPrintDataWrite = 0;
		Command.State++;
		break;
	case 12: // ����ͼ�����ݿ�ʼ������������
		BufferPrint[cBufferPrintLineWrite].BufferA[cBufferPrintDataWrite] = data;
		cBufferPrintDataWrite++;
		BufferPrint[cBufferPrintLineWrite].BufferA[cBufferPrintDataWrite] = 0x00;
		cBufferPrintDataWrite++;
		if(cBufferPrintDataWrite >= Command.Size)
		{
			Command.State++;
		}
		break;
	case 13: // ����ͼ�����ݿ�ʼ��ż��������
		BufferPrint[cBufferPrintLineWrite].BufferB[cBufferPrintDataWrite-Command.Size/2] = data; // Alke @20230704 -Command.Size/2
		cBufferPrintDataWrite++;
		cBufferPrintDataWrite++;
		BufferPrint[cBufferPrintLineWrite].BufferB[cBufferPrintDataWrite-Command.Size/2] = 0x00; // Alke @20230704 -Command.Size/2
		cBufferPrintDataWrite++;
		cBufferPrintDataWrite++;
		if(cBufferPrintDataWrite >= (Command.Size*2))
		{
			//cBufferPrintDataWrite = 0;
			Command.State = 30;
			goto VirtualHalfDotsDataDeal;
		}
		break;

	case 20: // ���ͼ�����ݣ�Size��8λ
//printf("Cmd1b2aDots-20\n");
		Command.Size = data;
		Command.State++;
		break;
	case 21: // ����ͼ�����ݣ�Size��8λ
//printf("Cmd1b2aDots-21-%d\n", Command.Size);
		c16c1 = data;
		c16c1 <<= 8;
		Command.Size += c16c1;
		cBufferPrintDataWrite = 0;
		Command.State++;
		break;
	case 22: // ����ͼ�����ݿ�ʼ������������
//printf("22-%d\n", cBufferPrintDataWrite);
		BufferPrint[cBufferPrintLineWrite].BufferA[cBufferPrintDataWrite] = data;
		cBufferPrintDataWrite++;
		// ȷ������һ���㹻���������¸�״̬
		if(cBufferPrintDataWrite >= (Command.Size/2))
		{
			Command.State++;
		}
		break;
	case 23: // ����ͼ�����ݿ�ʼ��ż��������
//printf("23-%d\n", cBufferPrintDataWrite);
		BufferPrint[cBufferPrintLineWrite].BufferB[cBufferPrintDataWrite-Command.Size/2] = data; // Alke @20230704 -Command.Size/2
		cBufferPrintDataWrite++;
		// ȷ������һ���㹻���������¸�״̬
		if(cBufferPrintDataWrite >= (Command.Size))
		{
			//cBufferPrintDataWrite = 0;
			Command.State = 30;
			goto VirtualHalfDotsDataDeal;
		}
		break;

	case 30: // ͼ�����������㴦��
//printf("30-\n");
		VirtualHalfDotsDataDeal:
		cBufferPrintDataWrite = 0;
		while(cBufferPrintDataWrite < (BufferPRTSize-2)) // (BufferPRTSize-2)Command.Size
		{
			BufferPrint[cBufferPrintLineWrite].BufferA[cBufferPrintDataWrite+1] &= ~BufferPrint[cBufferPrintLineWrite].BufferA[cBufferPrintDataWrite];
			BufferPrint[cBufferPrintLineWrite].BufferB[cBufferPrintDataWrite+1] &= ~BufferPrint[cBufferPrintLineWrite].BufferB[cBufferPrintDataWrite];
			cBufferPrintDataWrite++;
		}

		// �������ҷ�0����λ��
		cBufferPrintDataWrite = 0;
		while((BufferPrint[cBufferPrintLineWrite].BufferA[cBufferPrintDataWrite] == 0) && (BufferPrint[cBufferPrintLineWrite].BufferB[cBufferPrintDataWrite] == 0))
		{
			if(cBufferPrintDataWrite < (BufferPRTSize-1))
				cBufferPrintDataWrite++;
			else
				break;
		}
		BufferPrint[cBufferPrintLineWrite].cLeft = cBufferPrintDataWrite;
		cBufferPrintDataWrite = BufferPRTSize-1;
		while((BufferPrint[cBufferPrintLineWrite].BufferA[cBufferPrintDataWrite] == 0) && (BufferPrint[cBufferPrintLineWrite].BufferB[cBufferPrintDataWrite] == 0))
		{
			if(cBufferPrintDataWrite)
				cBufferPrintDataWrite--;
			else
				break;
		}
		BufferPrint[cBufferPrintLineWrite].cRight = cBufferPrintDataWrite;

		BufferPrint[cBufferPrintLineWrite].cLeft *= BufferOneDataTimePosition;//CRGOneStepTimePosition; // ����Ӧ��Ҫ��0��������м��ٹ���
		if(BufferPrint[cBufferPrintLineWrite].cLeft >= BufferPrint[cBufferPrintLineWrite].cLeft%CRGOneStepTimePosition)
			BufferPrint[cBufferPrintLineWrite].cLeft -= BufferPrint[cBufferPrintLineWrite].cLeft%CRGOneStepTimePosition;			// adjust to th 16*n

		BufferPrint[cBufferPrintLineWrite].cRight *= BufferOneDataTimePosition;//CRGOneStepTimePosition; // �ұ߽�����λ����Ҫ���Ӽ��ٺͼ��ٲ���
		BufferPrint[cBufferPrintLineWrite].cRight += CRGVolatileStep*CRGOneStepTimePosition*2; // �ұ߽�����λ����Ҫ���Ӽ��ٺͼ��ٲ���
		BufferPrint[cBufferPrintLineWrite].cRight += CRGOneStepTimePosition-BufferPrint[cBufferPrintLineWrite].cRight%CRGOneStepTimePosition; // adjust to th 16*n
		if(BufferPrint[cBufferPrintLineWrite].cRight > TimePositionAll)
			BufferPrint[cBufferPrintLineWrite].cRight = TimePositionAll;

		// @20230707 Alke ���ǰ��ֽ��Ҫ�������ҡ����µߵ�һ��
//		@@@@@@@@@@@@@@@@@@@@@@@@@@

//printf("N%d-S%d-P%d\n", CRGNowTimePosition, CRGStopTimePosition, PrintingState);
//printf("BW%d-BR%d-\n", cBufferPrintLineWrite, cBufferPrintLineRead);
if(!CRGRunning)
	printf("LF%d-RT%d-\n", BufferPrint[cBufferPrintLineWrite].cLeft, BufferPrint[cBufferPrintLineWrite].cRight);

//@20230705 Alke
BufferPrint[cBufferPrintLineWrite].cLeft = 0;
BufferPrint[cBufferPrintLineWrite].cRight = TimePositionAll;
#if 0
BufferPrint[cBufferPrintLineWrite].cLeft = 0;
BufferPrint[cBufferPrintLineWrite].cRight = TimePositionAll;
if(CRGNowTimePosition)
{
	CRGStopTimePosition = 0;//BufferPrint[cBufferPrintLineWrite].cLeft;
	CRGLeftSet
	FlagSystemPrintClr
	CRGCurrent(2);
}
else
{
	CRGStopTimePosition = TimePositionAll;//BufferPrint[cBufferPrintLineWrite].cRight;
	CRGRightSet
	FlagSystemPrintClr
	CRGCurrent(2);
}
#endif
		BufferPrint[cBufferPrintLineWrite].Step = 0;
		cBufferPrintDataWrite = 0;
		Command.Type = eCmdInit;
		Command.State = 0;
		Command.Size = 0;
		printf("Cmd1b2aDots\n");

		// ��д��ӡ��������������ӡ��������ֽ������0D/0A������
		/*if(	((cBufferPrintLineWrite+1) == cBufferPrintLineRead)	||
			((cBufferPrintLineWrite == (BufferPrintLineNum-1))&&(cBufferPrintLineRead == 0))	)
		{
			//printf("Error! BufferPrint[] overflowl! \n");
			;//��ӡ���������Ѿ����ˣ���ʱ����������������ϲ��ܳ���������
		}
		else*/
		{
			if(cBufferPrintLineWrite >= (BufferPrintLineNum-1))
				cBufferPrintLineWrite = 0;
			else
				cBufferPrintLineWrite++;
		}
		break;

	default:
//printf("df--\n");
		cBufferPrintDataWrite = 0;
		Command.Type = eCmdInit;
		Command.State = 0;
		Command.Size = 0;
		break;
	}
}
void Cmd1b4aPAP(unsigned char data)
{

	for(cBufferPrintDataWrite = (BufferPRTSize-1); cBufferPrintDataWrite; cBufferPrintDataWrite--)
	{
		BufferPrint[cBufferPrintLineWrite].BufferA[cBufferPrintDataWrite] = 0x00;
		BufferPrint[cBufferPrintLineWrite].BufferB[cBufferPrintDataWrite] = 0x00;
	}
	//cBufferPrintDataWrite = 0; // ��ȥʱ�Ѿ��պ�Ϊ0

	// ͨ��Ĭ��Ϊ16����û��0D�����л���һ��ʱ�����β������ӣ�ʹ����ֽ����
	if(PAPState == 0xf0)
		BufferPrint[cBufferPrintLineWrite].Dir = 1;
	else
		BufferPrint[cBufferPrintLineWrite].Dir = 0;
	if(BufferPrint[cBufferPrintLineWrite].Dir)
		PAPBackSet
	else
		PAPFeedSet
	BufferPrint[cBufferPrintLineWrite].Step += data;
	if(cBufferPrintLineWrite >= (BufferPrintLineNum-1))
		cBufferPrintLineWrite = 0;
	else
		cBufferPrintLineWrite++;

	Command.Type = eCmdInit;
	Command.State = 0;
	Command.Size = 0;
	printf("Cmd1b4aPAP\n");
//printf("N%d-S%d-P%d\n", CRGNowTimePosition, CRGStopTimePosition, PrintingState);
}
void Cmd1b4bPAP(unsigned char data)
{
	for(cBufferPrintDataWrite = (BufferPRTSize-1); cBufferPrintDataWrite; cBufferPrintDataWrite--)
	{
		BufferPrint[cBufferPrintLineWrite].BufferA[cBufferPrintDataWrite] = 0x00;
		BufferPrint[cBufferPrintLineWrite].BufferB[cBufferPrintDataWrite] = 0x00;
	}
	//cBufferPrintDataWrite = 0; // ��ȥʱ�Ѿ��պ�Ϊ0

	// ͨ��Ĭ��Ϊ16����û��0D�����л���һ��ʱ�����β������ӣ�ʹ����ֽ����
	if(PAPState == 0xf0)
		BufferPrint[cBufferPrintLineWrite].Dir = 0;
	else
		BufferPrint[cBufferPrintLineWrite].Dir = 1;
	if(BufferPrint[cBufferPrintLineWrite].Dir)
		PAPBackSet
	else
		PAPFeedSet
	BufferPrint[cBufferPrintLineWrite].Step += data;
	if(cBufferPrintLineWrite >= (BufferPrintLineNum-1))
		cBufferPrintLineWrite = 0;
	else
		cBufferPrintLineWrite++;

	Command.Type = eCmdInit;
	Command.State = 0;
	Command.Size = 0;
	printf("Cmd1b4bPAP\n");
//printf("N%d-S%d-P%d\n", CRGNowTimePosition, CRGStopTimePosition, PrintingState);
}

void Cmd1b55PrtDir(unsigned char data)
{

	if(data)
		CMDOneDirSet
	else
		CMDTwoDirSet

	Command.Type = eCmdInit;
	Command.State = 0;
	Command.Size = 0;
	printf("Cmd1b55PrtDir\n");
}
void Cmd1b7a(unsigned char data)
{
	u8 c;

	for(c = 0; c < BufferPrintLineNum; c++)
	{
		printf("cL%d-cR%d-Step%d\n", BufferPrint[c].cLeft, BufferPrint[c].cRight, BufferPrint[c].Step);
	}
	printf("BufferLineWrite%d-BufferLineRead%d-FlagMotorIC%d\n", cBufferPrintLineWrite, cBufferPrintLineRead,FlagMotorIC);
	printf("PAPMoveStep%d\n", PAPMoveStep);
	printf("NowTimPos%d-StopTimpos%d-PrintingState%d\n", CRGNowTimePosition, CRGStopTimePosition, PrintingState);

	Command.Type = eCmdInit;
	Command.State = 0;
	Command.Size = 0;
	printf("Cmd1b7a\n");
}
void Cmd1b89(unsigned char data)
{

	switch(Command.State)
	{
	case 0:
		Command.State = 1;
		break;

	case 1:
		Command.Type = eCmdInit;
		Command.State = 0;
		printf("Cmd1b89\n");
		break;

	default:
		Command.Type = eCmdInit;
		Command.State = 0;
		Command.Size = 0;
		break;
	}
}
void Cmd1b9c(unsigned char data)
{
	Command.Type = eCmdInit;
	Command.State = 0;
	Command.Size = 0;
	printf("Cmd1b9c\n");
}
void Cmd1ba0(unsigned char data)
{
	Command.Type = eCmdInit;
	Command.State = 0;
	Command.Size = 0;
	printf("Cmd1ba0\n");
}

// ��ֽ����
void Cmd1dClass(unsigned char data)
{
//	printf("Cmd1dClass--\n");

	switch(data)
	{
	case 0x56:
		Command.Type = eCmd1d56CutPaper;
		break;

	default:
		Command.Type = eCmdInit;
		break;
	}
}
void Cmd1d56CutPaper(unsigned char data)
{
	Command.Type = eCmdInit;
	Command.State = 0;
	Command.Size = 0;
	printf("Cmd1d56CutPaper\n");
}

// �������ֽڽ����������
//void InstructionParser(volatile unsigned char data)//�������
// �˺���Ϊ���յ�һ���ֽ����ݽ��н�������ǰ����Ԥ���жϴ�ӡ�������пհ׿���д�룬�������USB/WIFI/BL����æֹͣ��������
void DealData(unsigned char data) //�������
{
//static u8 cc = 0;;
//printf("%d,%c,%d\n", cc, cBufferPrintLineRead, cBufferPrintLineWrite);
#if 0
printf("DealData-%d\n", data);
return;
#endif
	switch(Command.Type)
	{
	case eCmd0dPrint:
		OneByteCommand0d:
		CmdPrint0d();
		break;

	case eCmd1bClass:
		Cmd1bClass(data);
		break;

	case eCmd1b0a:
		Cmd1b0a(data);
		break;

	case eCmd1b2aDots:
		Cmd1b2aDots(data);
		break;

	case eCmd1b4aPAP:
		Cmd1b4aPAP(data);
		break;

	case eCmd1b4bPAP:
		Cmd1b4bPAP(data);
		break;

	case eCmd1b55PrtDir:
		Cmd1b55PrtDir(data);
		break;

	case eCmd1b7a:
		Cmd1b7a(data);
		break;

	case eCmd1b89:
		Cmd1b89(data);
		break;

	case eCmd1b9c:
		Cmd1b9c(data);
		break;

	case eCmd1ba0:
		Cmd1ba0(data);
		break;

	case eCmd1dClass:
		Cmd1dClass(data);
		break;

	case eCmd1d56CutPaper:
		Cmd1d56CutPaper(data);
		break;

	// ���ǵ�1���ֽڵ��жϣ�Ȼ����ܽ�������ķ��ദ��
	default://eCmdInit
		if(data == 0x0d) // ���ֽ�����ֱ�Ӵ���û�к�������
			{Command.Type = eCmd0dPrint; goto OneByteCommand0d;}
		else if(data == 0x1b)
		//if(data == 0x1b)
			Command.Type = eCmd1bClass;
		else if(data == 0x1d)
			Command.Type = eCmd1dClass;
		else
			{Command.Type = eCmdInit; Command.State = 0; Command.Size = 0;}
		break;
	}
}



/**************************************************************************************************************
	Driver.c	Driver.h
**************************************************************************************************************/

