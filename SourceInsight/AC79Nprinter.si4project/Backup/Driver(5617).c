
//#define Driver 0
/**************************************************************************************************************
	Driver.c	Driver.h
**************************************************************************************************************/

#include "Driver.h"
#include "McuInt.h"
#include "UserInterface.h"



// @20230601 按照新的程序架构，此函数为收到数据后的触发事件
// 此函数按照一个字节一个字节进行处理，此过程中不能有死循环，只能一次过，可以设置标志位

// 忽略此命令
void CmdPrint0d(void) // 但字节命令没有后续数据
{
	Command.Type = eCmdInit;
	Command.State = 0;
	Command.Size = 0;
	printf("CmdPrint0d\n");
}

// 命令首字节为1b的命令类
void Cmd1bClass(unsigned char data)
{
//	printf("Cmd1bClass--\n");

	switch(data)
	{
	case 0x0a:
		Command.Type = eCmd1b0a;
		break;

	case 0x0b:
		Command.Type = eCmd1b0b;
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
			Command.State = 1; // 命令有效
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
void Cmd1b0b(unsigned char data)
{
	struct wifi_stored_sta_info wifi_setup_sta_info;
	u8 ssid_stored_cnt;
	struct wifi_stored_sta_info wifi_stored_sta_info[32];

	switch(Command.State)
	{
	case 0:
		if(data == 0x0b)
		{
			Command.State = 1; // 命令有效
			Command.Size = 0;

			// 没有占满时，最后一位字符后面跟一个0作为结束标记
			if(Command.Size < 33) // wifi_stored_sta_info.ssid[33]
			{
				wifi_setup_sta_info.ssid[Command.Size] = 0;
				Command.Size++;
			}
		}
		else
		{
			if(Command.Size < 33)
			{
				wifi_setup_sta_info.ssid[Command.Size] = data;
				Command.Size++;
			}
		}
		break;

	case 1:
		if(data == 0x0a)
		{
			Command.Type = eCmdInit;
			Command.State = 0;
			Command.Size = 0;

   			ssid_stored_cnt = wifi_get_stored_sta_info(wifi_stored_sta_info);
    		for (int i = 0; i < ssid_stored_cnt; i++)
			{
	        	//printf("1:wifi_get_stored_sta_info[%d]= %s \r\n", i, wifi_stored_sta_info[i].ssid);
				wifi_del_stored_sta_info(wifi_stored_sta_info[i].ssid);// @ Alke 20230711 删除存储网络
	    	}

			wifi_store_mode_info(STA_MODE, wifi_setup_sta_info.ssid, wifi_setup_sta_info.pwd);
			// 没有占满时，最后一位字符后面跟一个0作为结束标记
			if(Command.Size < 64) // wifi_stored_sta_info.pwd[64]
			{
				wifi_setup_sta_info.pwd[Command.Size] = 0;
				Command.Size++;
			}
		}
		else
		{
			if(Command.Size < 64)
			{
				wifi_setup_sta_info.pwd[Command.Size] = data;
				Command.Size++;
			}
		}
		break;

	default:
		Command.Type = eCmdInit;
		Command.State = 0;
		printf("Cmd1b0a\n");
		break;
	}
}

unsigned char ByteBitChang(unsigned char dd)
{
	unsigned char d8 = 0;

	if(dd&0x80)
		d8 |= 0x01;
	if(dd&0x40)
		d8 |= 0x02;
	if(dd&0x20)
		d8 |= 0x04;
	if(dd&0x10)
		d8 |= 0x08;
	if(dd&0x08)
		d8 |= 0x10;
	if(dd&0x04)
		d8 |= 0x20;
	if(dd&0x02)
		d8 |= 0x40;
	if(dd&0x01)
		d8 |= 0x80;

	return d8;
}
void Cmd1b2aDots(unsigned char data)
{
	volatile unsigned int c16c1;
	volatile unsigned char c8c1;

	switch(Command.State)
	{
	case 0:
//printf("Dots-00\n");
			if(data == 0x00)
			Command.State = 10; // 整点图形数据
		else if(data == 0x01)
			Command.State = 20; // 半点图形数据
		break;

	case 10: // 整点图形数据，Size低8位
		Command.Size = data;
		Command.State++;
		break;
	case 11: // 整点图形数据，Size高8位
		c16c1 = data;
		c16c1 <<= 8;
		Command.Size += c16c1;
		cBufferPrintDataWrite = 0;
		Command.State++;
		break;
	case 12: // 整点图形数据开始，奇数行数据
		BufferPrint[cBufferPrintLineWrite].BufferA[cBufferPrintDataWrite] = data;
		cBufferPrintDataWrite++;
		BufferPrint[cBufferPrintLineWrite].BufferA[cBufferPrintDataWrite] = 0x00;
		cBufferPrintDataWrite++;
		if(cBufferPrintDataWrite >= Command.Size)
		{
			Command.State++;
		}
		break;
	case 13: // 整点图形数据开始，偶数行数据
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
			goto PrintDataDeal;
		}
		break;

	case 20: // 半点图形数据，Size低8位
//printf("Cmd1b2aDots-20\n");
		Command.Size = data;
		Command.State++;
		break;
	case 21: // 整点图形数据，Size高8位
//printf("Cmd1b2aDots-21-%d\n", Command.Size);
		c16c1 = data;
		c16c1 <<= 8;
		Command.Size += c16c1;
		cBufferPrintDataWrite = 0;
		Command.State++;
		break;
	case 22: // 整点图形数据开始，奇数行数据
//printf("22-%d\n", cBufferPrintDataWrite);
		BufferPrint[cBufferPrintLineWrite].BufferA[cBufferPrintDataWrite] = data;
		cBufferPrintDataWrite++;
		// 确保数据一旦足够，立马换到下个状态
		if(cBufferPrintDataWrite >= (Command.Size/2))
		{
			Command.State++;
		}
		break;
	case 23: // 整点图形数据开始，偶数行数据
//printf("23-%d\n", cBufferPrintDataWrite);
		BufferPrint[cBufferPrintLineWrite].BufferB[cBufferPrintDataWrite-Command.Size/2] = data; // Alke @20230704 -Command.Size/2
		cBufferPrintDataWrite++;
		// 确保数据一旦足够，立马换到下个状态
		if(cBufferPrintDataWrite >= (Command.Size))
		{
			//cBufferPrintDataWrite = 0;
			Command.State = 30;
			goto PrintDataDeal;
		}
		break;

	case 30: // 图形数据虚拟半点处理
		PrintDataDeal:
//printf("30-\n");
		//原始数据开始处理
		// @20230707 Alke 如果前进纸需要数据左右、上下颠倒一下
		// @@@@@@@@@@@@@@@@@@@@@@@@@@
		if(PaperState == 0xf0) // 前进纸模式
		{
			// 打印效果上的数据左右调换，就是打印缓冲区前后调换；同时A B缓冲区数据调换，就是齐数行和偶数行数据调换
			for(cBufferPrintDataWrite = 0; cBufferPrintDataWrite < (BufferPRTSize/2); cBufferPrintDataWrite++)
			{
				c8c1 = BufferPrint[cBufferPrintLineWrite].BufferA[cBufferPrintDataWrite];
				BufferPrint[cBufferPrintLineWrite].BufferA[cBufferPrintDataWrite] = BufferPrint[cBufferPrintLineWrite].BufferB[BufferPRTSize-1-cBufferPrintDataWrite];
				BufferPrint[cBufferPrintLineWrite].BufferB[BufferPRTSize-1-cBufferPrintDataWrite] = c8c1;

				c8c1 = BufferPrint[cBufferPrintLineWrite].BufferB[cBufferPrintDataWrite];
				BufferPrint[cBufferPrintLineWrite].BufferB[cBufferPrintDataWrite] = BufferPrint[cBufferPrintLineWrite].BufferA[BufferPRTSize-1-cBufferPrintDataWrite];
				BufferPrint[cBufferPrintLineWrite].BufferA[BufferPRTSize-1-cBufferPrintDataWrite] = c8c1;
			}

			// 打印效果上的数据上下颠倒，就是打印缓冲区数据高低位互换
			for(cBufferPrintDataWrite = 0; cBufferPrintDataWrite < BufferPRTSize; cBufferPrintDataWrite++)
			{
				BufferPrint[cBufferPrintLineWrite].BufferA[cBufferPrintDataWrite] = ByteBitChang(BufferPrint[cBufferPrintLineWrite].BufferA[cBufferPrintDataWrite]);
				BufferPrint[cBufferPrintLineWrite].BufferB[cBufferPrintDataWrite] = ByteBitChang(BufferPrint[cBufferPrintLineWrite].BufferB[cBufferPrintDataWrite]);
			}
		}
		// @@@@@@@@@@@@@@@@@@@@@@@@@@

		//虚拟半点数据开始处理 VirtualHalfDotsDataDeal:
		cBufferPrintDataWrite = 0;
		while(cBufferPrintDataWrite < (BufferPRTSize-1)) // (BufferPRTSize-2)Command.Size
		{
			BufferPrint[cBufferPrintLineWrite].BufferA[cBufferPrintDataWrite+1] &= ~BufferPrint[cBufferPrintLineWrite].BufferA[cBufferPrintDataWrite];
			BufferPrint[cBufferPrintLineWrite].BufferB[cBufferPrintDataWrite+1] &= ~BufferPrint[cBufferPrintLineWrite].BufferB[cBufferPrintDataWrite];
			cBufferPrintDataWrite++;
		}

		// 计算左右非0数据位置
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

		BufferPrint[cBufferPrintLineWrite].cLeft *= BufferOneDataTimePosition;//CRGOneStepTimePosition; // 左起应该要从0对齐才能有加速过程
		if(BufferPrint[cBufferPrintLineWrite].cLeft >= BufferPrint[cBufferPrintLineWrite].cLeft%CRGOneStepTimePosition)
			BufferPrint[cBufferPrintLineWrite].cLeft -= BufferPrint[cBufferPrintLineWrite].cLeft%CRGOneStepTimePosition;			// adjust to th 16*n

		BufferPrint[cBufferPrintLineWrite].cRight *= BufferOneDataTimePosition;//CRGOneStepTimePosition; // 右边结束的位置需要增加加速和减速步数
		BufferPrint[cBufferPrintLineWrite].cRight += CRGVolatileStep*CRGOneStepTimePosition*2; // 右边结束的位置需要增加加速和减速步数
		BufferPrint[cBufferPrintLineWrite].cRight += CRGOneStepTimePosition-BufferPrint[cBufferPrintLineWrite].cRight%CRGOneStepTimePosition; // adjust to th 16*n
		if(BufferPrint[cBufferPrintLineWrite].cRight > TimePositionAll)
			BufferPrint[cBufferPrintLineWrite].cRight = TimePositionAll;

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

		// 填写打印缓冲区并启动打印，但不走纸；忽略0D/0A等命令
		/*if(	((cBufferPrintLineWrite+1) == cBufferPrintLineRead)	||
			((cBufferPrintLineWrite == (BufferPrintLineNum-1))&&(cBufferPrintLineRead == 0))	)
		{
			//printf("Error! BufferPrint[] overflowl! \n");
			;//打印缓冲阵列已经满了，此时发生溢出错误，理论上不能出现这个情况
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
	//cBufferPrintDataWrite = 0; // 出去时已经刚好为0

	// 通常默认为16步，没有0D命令切换下一行时，本次步数增加，使得走纸连贯
	if(PAPState == 0xf0)
		BufferPrint[cBufferPrintLineWrite].Dir = 1;
	else
		BufferPrint[cBufferPrintLineWrite].Dir = 0;
//	if(BufferPrint[cBufferPrintLineWrite].Dir)
		PAPBackSet
//	else
//		PAPFeedSet
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
	//cBufferPrintDataWrite = 0; // 出去时已经刚好为0

	// 通常默认为16步，没有0D命令切换下一行时，本次步数增加，使得走纸连贯
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

// 切纸命令
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

// 根据首字节进行命令分类
//void InstructionParser(volatile unsigned char data)//命令解析
// 此函数为接收到一个字节数据进行解析处理，前提是预先判断打印缓冲区有空白可以写入，否则会在USB/WIFI/BL端置忙停止接收数据
void DealData(unsigned char data) //命令解析
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

	// 这是第1个字节的判断，然后才能进行命令的分类处理
	default://eCmdInit
		if(data == 0x0d) // 单字节命令直接处理，没有后续数据
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

