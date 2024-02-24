


//#define Driver 0
/**************************************************************************************************************
	Driver.c	Driver.h
**************************************************************************************************************/
#include "Driver.h"
#include "UserInterface.H"



// @20230601 按照新的程序架构，此函数为收到数据后的触发事件
// 此函数按照一个字节一个字节进行处理，此过程中不能有死循环，只能一次过，可以设置标志位

// 忽略此命令
/*void CmdPrint0d(void) // 但字节命令没有后续数据
{
	if(	((cBufferPrintLineWrite+1) == cBufferPrintLineRead)	||
		((cBufferPrintLineWrite == (BufferPrintLineNum-1))&&(cBufferPrintLineRead == 0))	)
	{
		printf("Error! BufferPrint[] overflowl! \n");
		;//打印缓冲阵列已经满了，此时发生溢出错误，理论上不能出现这个情况
	}
	else
	{
		if(cBufferPrintLineWrite == (BufferPrintLineNum-1))
			cBufferPrintLineWrite = 0;
		else
			cBufferPrintLineWrite++;
	}
}*/

// 命令首字节为1b的命令类
void Cmd1bClass(volatile unsigned char data)
{
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
void Cmd1b0a(volatile unsigned char data)
{
	switch(Command.State)
	{
	case 0:
		if(data == 1)
			Command.State = 1; // 命令有效
		break;

	case 1:
		if(Command.State == 1)
		{
			if(data == 0x01)
				FlagPrinter = FlagPRTStand;
			else if(data == 0x02)
				FlagPrinter = FlagPRTDeep;
			else if(data == 0x03)
				FlagPrinter = FlagPRTStand;
			else
				FlagPrinter = FlagPRTFast;
		}
		Command.Type = eCmdInit;
		Command.State = 0;
		break;

	default:
		Command.Type = eCmdInit;
		break;
	}
}
void Cmd1b2aDots(volatile unsigned char data)
{
	volatile unsigned int c16c1;

	switch(Command.State)
	{
	case 0:
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
		if(cBufferPrintDataWrite < Command.Size/2)
		{
			BufferPrint[cBufferPrintLineWrite].BufferA[cBufferPrintDataWrite] = data;
			cBufferPrintDataWrite++;
			BufferPrint[cBufferPrintLineWrite].BufferA[cBufferPrintDataWrite] = 0x00;
			cBufferPrintDataWrite++;
		}
		else
		{
			Command.State++;
		}
		break;
	case 13: // 整点图形数据开始，偶数行数据
		if(cBufferPrintDataWrite < (BufferPRTSize-1))
		{
			BufferPrint[cBufferPrintLineWrite].BufferB[cBufferPrintDataWrite] = data;
			cBufferPrintDataWrite++;
			BufferPrint[cBufferPrintLineWrite].BufferB[cBufferPrintDataWrite] = 0x00;
			cBufferPrintDataWrite++;
		}
		else
		{
			cBufferPrintDataWrite = 0;
			Command.State = 30;
			goto VirtualHalfDotsDataDeal;
		}
		break;

	case 20: // 半点图形数据，Size低8位
		Command.Size = data;
		Command.State++;
		break;
	case 21: // 整点图形数据，Size高8位
		c16c1 = data;
		c16c1 <<= 8;
		Command.Size += c16c1;
		cBufferPrintDataWrite = 0;
		Command.State++;
		break;
	case 22: // 整点图形数据开始，奇数行数据
		if(cBufferPrintDataWrite < Command.Size/2)
		{
			BufferPrint[cBufferPrintLineWrite].BufferA[cBufferPrintDataWrite] = data;
			cBufferPrintDataWrite++;
		}
		else
		{
			Command.State++;
		}
		break;
	case 23: // 整点图形数据开始，偶数行数据
		if(cBufferPrintDataWrite < (BufferPRTSize-1))
		{
			BufferPrint[cBufferPrintLineWrite].BufferB[cBufferPrintDataWrite] = data;
			cBufferPrintDataWrite++;
		}
		else
		{
			cBufferPrintDataWrite = 0;
			Command.State = 30;
			goto VirtualHalfDotsDataDeal;
		}
		break;

	case 30: // 图形数据虚拟半点处理
		VirtualHalfDotsDataDeal:
		while(cBufferPrintDataWrite < (BufferPRTSize-2))
		{
			BufferPrint[cBufferPrintLineWrite].BufferA[cBufferPrintDataWrite+1] &= ~BufferPrint[cBufferPrintLineWrite].BufferA[cBufferPrintDataWrite];
			BufferPrint[cBufferPrintLineWrite].BufferB[cBufferPrintDataWrite+1] &= ~BufferPrint[cBufferPrintLineWrite].BufferB[cBufferPrintDataWrite];
			cBufferPrintDataWrite++;
		}
		BufferPrint[cBufferPrintLineWrite].Step = 0;
		cBufferPrintDataWrite = 0;
		Command.Type = eCmdInit;
		Command.State = 0;
		Command.Size = 0;

		if(	((cBufferPrintLineWrite+1) == cBufferPrintLineRead)	||
			((cBufferPrintLineWrite == (BufferPrintLineNum-1))&&(cBufferPrintLineRead == 0))	)
		{
			//printf("Error! BufferPrint[] overflowl! \n");
			;//打印缓冲阵列已经满了，此时发生溢出错误，理论上不能出现这个情况
		}
		else
		{
			if(cBufferPrintLineWrite == (BufferPrintLineNum-1))
				cBufferPrintLineWrite = 0;
			else
				cBufferPrintLineWrite++;
		}
		break;

	default:
		cBufferPrintDataWrite = 0;
		Command.Type = eCmdInit;
		Command.State = 0;
		Command.Size = 0;
		break;
	}
}
void Cmd1b4aPAP(volatile unsigned char data)
{
	for(cBufferPrintDataWrite = (BufferPRTSize-1); cBufferPrintDataWrite; cBufferPrintDataWrite--)
	{
		BufferPrint[cBufferPrintLineWrite].BufferA[cBufferPrintDataWrite] = 0x00;
		BufferPrint[cBufferPrintLineWrite].BufferB[cBufferPrintDataWrite] = 0x00;
	}
	//cBufferPrintDataWrite = 0; // 出去时已经刚好为0

	// 通常默认为16步，没有0D命令切换下一行时，本次步数增加，使得走纸连贯
	BufferPrint[cBufferPrintLineWrite].Step += data;
}
void Cmd1b55PrtDir(volatile unsigned char data)
{
	if(data)
		CMDOneDirSet
	else
		CMDTwoDirSet
}
void Cmd1b7a(volatile unsigned char data)
{
	;
}
void Cmd1b89(volatile unsigned char data)
{
	;
}
void Cmd1b9c(volatile unsigned char data)
{
	;
}
void Cmd1ba0(volatile unsigned char data)
{
	;
}

// 切纸命令
void Cmd1dClass(volatile unsigned char data)
{
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
void Cmd1d56CutPaper(volatile unsigned char data)
{
	;
}

// 根据首字节进行命令分类
//void InstructionParser(volatile unsigned char data)//命令解析
// 此函数为接收到一个字节数据进行解析处理，前提是预先判断打印缓冲区有空白可以写入，否则会在USB/WIFI/BL端置忙停止接收数据
void DealData(unsigned char data) //命令解析
{
	switch(Command.Type)
	{
	case eCmd0dPrint:
		OneByteCommand0d:
		//CmdPrint0d();/////
		break;

	case eCmd1bClass:
		Cmd1bClass(data);
		break;

	case eCmd1b0a:
		Command.State = 0;
		Cmd1b0a(data);
		break;

	case eCmd1b2aDots:
		Command.State = 0;
		Cmd1b2aDots(data);
		break;

	case eCmd1b4aPAP:
		Cmd1b4aPAP(data);
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
			Command.Type = eCmd1bClass;
		else if(data == 0x1d)
			Command.Type = eCmd1dClass;
		else
			{Command.Type = eCmdInit; Command.State = 0; Command.Size = 0;}
		break;
	}
}


#if 0 // 老方案命令处理方式，可以借鉴
void PrinterDealdata(void)
{
	static unsigned int c16c1 = 0;
	static unsigned int c16c2 = 0;


//	if(!KeyPower)
//		printf("KeyPower down\n", c16c1, c16c2);

//	c16c1 = adc_get_value(9);//(AD_CH_PA07);
//	c16c2 = adc_get_voltage(9);//(AD_CH_PA07);
//	if(c16c1 > 250)
//		printf("adc_val = %d  >>>  %d mv\n", c16c1, c16c2);

	// CMD1 & CMD2 save the commond data, clear data only used
	// **********************************	one byte commond
	// 回车
	if( (CMD1 == 0x00) && (CMD2 == 0x0d) )
	{
		if(FlagDataWait == 0)
		{
			SendCmd();

			c16c1 = 0;
			while((*(BufferPRTA+c16c1) == 0) && (*(BufferPRTB+c16c1) == 0))
			{
				if(c16c1 < (BufferPRTSize-1))
					c16c1++;
				else
					break;
			}
			c16c2 = BufferPRTSize-1;
			while((*(BufferPRTA+c16c2) == 0) && (*(BufferPRTB+c16c2) == 0))
			{
				if(c16c2)
					c16c2--;
				else
					break;
			}
			c16c1 += CRGVolatileStep*CRGOneStepTimePosition;
			c16c1 -= c16c1%CRGOneStepTimePosition;			// adjust to th 16*n
			c16c2 += CRGVolatileStep*CRGOneStepTimePosition;
			c16c2 += CRGOneStepTimePosition-c16c2%CRGOneStepTimePosition; // adjust to th 16*n
			if(c16c2 > TimePositionAll)
				c16c2 = TimePositionAll;

			if(c16c1 <= c16c2)
			{
				FlagDataWait++;
			}
			else
			{
				FlagDataWait =0;
				CMD2 = 0;
			}
		}
		if(FlagDataWait == 1)
		{
			if(CRGNowTimePosition <= c16c1)
			{
				CRGStopTimePosition = c16c2;
				CRGRightSet
				FlagSystemPrintSet
				CRGCurrent(2);

				FlagDataWait += 2;
			}
			else if(CRGNowTimePosition >= c16c2)
			{
				CRGStopTimePosition = c16c1;
				CRGLeftSet
				FlagSystemPrintSet
				CRGCurrent(2);

				FlagDataWait += 2;
			}
			else
			{
				if((CRGNowTimePosition-c16c1) < (c16c2-CRGNowTimePosition))
				{
					CRGStopTimePosition = c16c1;
					CRGLeftSet
				}
				else
				{
					CRGStopTimePosition = c16c2;
					CRGRightSet
				}
				FlagSystemPrintClr
				CRGCurrent(2);
				FlagDataWait++;
			}
		}
		if(FlagDataWait == 2)
		{
			if(CRGRuning)
				return;

			FlagDataWait--;
		}
		if(FlagDataWait == 3)
		{
			if(CRGRuning)
				return;

			for(c16c2 = 0; c16c2 < BufferPRTSize; c16c2++)
			{
				*(BufferPRTA+c16c2) = 0x00;
				*(BufferPRTB+c16c2) = 0x00;
			}

			FlagDataWait = 0;
			CMD2 = 0;
		}
	}
	//
	else if( (CMD1 == 0x00) && (CMD2 >= 0x20) && (CMD2 <= 0x7e) )
	{
		SendCmd();

		WritePRTBuffer(0, CMD2, 0, 0);
		CMD2 = 0;
	}
	// **********************************	double byte commond DLE EOT
	// **********************************	双字节指令DLE EOT
	// 没有读取到两字节时，返回继续读取下一字节
	// 设置打印速度力度等
	else if( (CMD1 == 0x1b) && (CMD2 == 0x0a) )
	{
		if(FlagDataWait == 0)
		{
			SendCmd();

			if(ReceiveBufferSize()<2)
				return;

			c8c1 = ReceiveBufferData();
			c8c2 = ReceiveBufferData();

			FlagDataWait++;
		}
		if(FlagDataWait == 1)
		{
			if(c8c1 == 0x01)
			{
				if(c8c2 == 0x01)
					FlagPrinter = FlagPRTStand;
				else if(c8c2 == 0x02)
					c8c2 = FlagPRTDeep;
				else if(c8c2 == 0x03)
					FlagPrinter = FlagPRTStand;
				else
					FlagPrinter = FlagPRTFast; // default
			}

			FlagDataWait = 0;
			CMD1 = 0;
			CMD2 = 0;
		}
	}
	// -----bitmap commond------------
	// 1B 2A 和 0D 打印命令，会在2、3状态间来回切换，应该是CRGNowTimePosition CRGStopTimePosition左右运动加减方向错误的原因
	else if( (CMD1 == 0x1b) && (CMD2 == 0x2a) )
	{
		if(FlagDataWait == 0) // waiit for parameter
		{
			SendCmd();
//Alke	user_put_u8hex(FlagDataWait);

			if(ReceiveBufferSize()<3)
				return;

			c8c1 = ReceiveBufferData();
			c8c2 = ReceiveBufferData();
			c8c3 = ReceiveBufferData();
			c16c1 = c8c3;
			c16c1 <<= 8;
			c16c1 += c8c2;

			for(c16c2 = 0; c16c2 < BufferPRTSize; c16c2++)
			{
				*(BufferPRTA+c16c2) = 0x00;
				*(BufferPRTB+c16c2) = 0x00;
			}

			FlagDataWait++;
		}
		if(FlagDataWait == 1) // wait for the data
		{
//Alke    user_put_u8hex(FlagDataWait);
			if(ReceiveBufferSize()<c16c1)
				return;
			if(CRGRuning)
				return;

			if(c8c1 == 0x00) // full dot mode
			{
				c16c2 = c16c1/2; // 10-5
				cBufferPRTWrite = 0;
				while(c16c1>c16c2) // 10 9 8 7 6
				{
					c16c1--;
					c8c2 = ReceiveBufferData();
					if(cBufferPRTWrite < (BufferPRTSize-1))
					{
						*(BufferPRTA+cBufferPRTWrite) = c8c2;
						cBufferPRTWrite++;
						*(BufferPRTA+cBufferPRTWrite) = 0x00;
						cBufferPRTWrite++;
					}
				}
				cBufferPRTWrite = 0;
				while(c16c1)
				{
					c16c1--;
				//	printf("Data: GetNextData\r\n");
					c8c2 = ReceiveBufferData();
					if(cBufferPRTWrite < (BufferPRTSize-1))
					{
					//user_put_u8hex(c8c2);
						*(BufferPRTB+cBufferPRTWrite) = c8c2;
						cBufferPRTWrite++;
						*(BufferPRTB+cBufferPRTWrite) = 0x00;
						cBufferPRTWrite++;
					}
				}
			}
			else if(c8c1 == 0x01) // half dot mode
			{
				c16c2 = c16c1/2; // 10-5
				cBufferPRTWrite = 0;
				while(c16c1>c16c2) // 10 9 8 7 6
				{
					c16c1--;
					c8c2 = ReceiveBufferData();
					if(cBufferPRTWrite < (BufferPRTSize-1))
					{
						*(BufferPRTA+cBufferPRTWrite) = c8c2;
						cBufferPRTWrite++;
					}
				}
				cBufferPRTWrite = 0;
				while(c16c1)
				{
					c16c1--;
					c8c2 = ReceiveBufferData();
					if(cBufferPRTWrite < (BufferPRTSize-1))
					{
						*(BufferPRTB+cBufferPRTWrite) = c8c2;
						cBufferPRTWrite++;
					}
				}
			}
			for(c16c1 = 1; c16c1 < BufferPRTSize; c16c1++) // start 1			//@@@@@
			{
				*(BufferPRTA+c16c1) &= ~*(BufferPRTA+c16c1-1);
				*(BufferPRTB+c16c1) &= ~*(BufferPRTB+c16c1-1);
			}
			FlagDataWait++;
		}
		if(FlagDataWait == 2) // go to the position to ready for print
		{
//Alke    user_put_u8hex(FlagDataWait);
			c16c1 = 0;
			while((*(BufferPRTA+c16c1) == 0) && (*(BufferPRTB+c16c1) == 0))
			{
				if(c16c1 < (BufferPRTSize-1))
					c16c1++;
				else
					break;
			}
			c16c2 = BufferPRTSize-1;
			while((*(BufferPRTA+c16c2) == 0) && (*(BufferPRTB+c16c2) == 0))
			{
				if(c16c2)
					c16c2--;
				else
					break;
			}
			c16c1 += CRGVolatileStep*CRGOneStepTimePosition;
			c16c1 -= c16c1%CRGOneStepTimePosition;			// adjust to th 16*n
			c16c2 += CRGVolatileStep*CRGOneStepTimePosition;
			c16c2 += CRGOneStepTimePosition-c16c2%CRGOneStepTimePosition; // adjust to th 16*n
			if(c16c2 > TimePositionAll)
				c16c2 = TimePositionAll;

			if(c16c1 <= c16c2)
			{
				FlagDataWait++;
			}
			else
			{
				FlagDataWait =0;
				CMD1 = 0;
				CMD2 = 0;
			}
		}
		if(FlagDataWait = 3)
		{
//Alke    user_put_u8hex(FlagDataWait);
			// 必须要有等号：预先位置完成后就是相等的
			if((CRGNowTimePosition <= c16c1)) // do not care the one print direction
			{
				CRGStopTimePosition = c16c2;
				CRGRightSet
				FlagSystemPrintSet
				CRGCurrent(2);

				FlagDataWait += 2;// 跳过等待到达预先位置的步骤
			}
			else if(CRGNowTimePosition >= c16c2)
			{
				if(TMPOneDir || CMDOneDir)
				{
					CRGStopTimePosition = c16c1;
					CRGLeftSet
					FlagSystemPrintClr
					CRGCurrent(2);
					FlagDataWait++;// 需要预先到达准备位置
//Alke    user_put_u8hex(0x31);
				}
				else
				{
					CRGStopTimePosition = c16c1;
					CRGLeftSet
					FlagSystemPrintSet
					CRGCurrent(2);

					FlagDataWait += 2;
				}
			}
			else
			{
				if(TMPOneDir || CMDOneDir)
				{
					CRGStopTimePosition = c16c1;
					CRGLeftSet
					FlagSystemPrintClr
					CRGCurrent(2);
					FlagDataWait++;// 需要预先到达准备位置
//Alke    user_put_u8hex(0x32);
				}
				else
				{
					if((CRGNowTimePosition-c16c1) < (c16c2-CRGNowTimePosition))
					{
						CRGStopTimePosition = c16c1;
						CRGLeftSet
					}
					else
					{
						CRGStopTimePosition = c16c2;
						CRGRightSet
					}
					FlagSystemPrintClr
					CRGCurrent(2);
					FlagDataWait++;
//Alke    user_put_u8hex(0x33);
				}
			}
		}
		if(FlagDataWait == 4) // waiting for CRG arrived the position
		{
//Alke    user_put_u8hex(FlagDataWait);
			if(CRGRuning)
				return;

			FlagDataWait--;
		}
		if(FlagDataWait == 5) // waiting for the printing finished
		{
//Alke    user_put_u8hex(FlagDataWait);
			if(CRGRuning)
				return;

			/*for(c16c2 = 0; c16c2 < BufferPRTSize; c16c2++)
			{
				*(BufferPRTA+c16c2) = 0x00;
				*(BufferPRTB+c16c2) = 0x00;
			}*/

			FlagDataWait = 0;
			CMD1 = 0;
			CMD2 = 0;
		}
	}
	// 执行n/180英寸顺方向走纸		SM880: n/144
	else if( (CMD1 == 0x1b) && (CMD2 == 0x4a) )
	{
		// 等待数据
		if(FlagDataWait == 0)
		{
			SendCmd();

			//printf("DataCMD: 0x1b 0x4a->0 \r\n");
			if(ReceiveBufferSize()<1)
				return;

			PAPMoveStep = ReceiveBufferData();//GetBufferReceiveData();
			PAPMoveStep *= 2;
			// 需要检查页头是否接近胶辊位置，以此来确定是否需要打印头左右移动将纸张展平，降低进入胶辊时卡纸风险
			// 此处所述胶辊，前进纸时指的是后面胶辊，后进纸时指的前面胶辊
			// 如果是前进纸缺纸，需要停止打印，并且将纸张退出
			FlagDataWait = 10;
		}
	#if 1
		// 根据页长计数器、页长、需要走纸步数等，判断是否需要字车展平、退纸等操作，其他状态操作后，可能再次进入该状态进行判断，
		if(FlagDataWait == 10)
		{
			// 前进纸模式，纸张页头接近后胶辊；或者后进纸模式，纸张页头接近前胶辊；都需要字车左右运动展平纸张
			if(PaperCount < (StepFrontToPrint+StepBackToPrint-StepHeadWide)&&((PaperCount+PAPMoveStep) > (StepFrontToPrint+StepBackToPrint)))
			{
				c16c1 = (PaperCount+PAPMoveStep) - (StepFrontToPrint+StepBackToPrint-StepHeadWide);
				c16c2 = c16c1;
				PAPMoveStep = (StepFrontToPrint+StepBackToPrint-StepHeadWide)-PaperCount;

				if(PaperState&0xf0 == 0x10)
				{
					PAPBackSet
					PAPCurrent(2);

					FlagDataWait = 20;
				}
				else if(PaperState&0xf0 == 0x20)
				{
					PAPFeedSet
					PAPCurrent(2);

					FlagDataWait = 20;
				}
				else
					FlagDataWait = 11;

			}
			// 前进纸模式，纸张页尾越过打印位置，本页打印结束，直接退纸；后进纸模式正常送纸即可
			// 页长在马达相位切换函数中，会根据前里后后里传感器确实无纸时自动调整页长；重新装纸后回复默认页长
			else if(((PaperCount+PAPMoveStep) > (PaperPageLenth+StepFrontToPrint)) && (PaperState&0xf0 == 0x10))
			{
				c16c1 = PaperPageLenth;
				c16c2 = c16c1;
				PAPMoveStep = (PaperPageLenth+StepFrontToPrint)-PaperCount;

				PAPBackSet
				PAPCurrent(2);

				FlagDataWait = 30;
			}
			else
			{
				if(PaperState&0xf0 == 0x10)
					PAPBackSet
				else if(PaperState&0xf0 == 0x20)
					PAPFeedSet
				PAPCurrent(2);

				FlagDataWait = 11;
			}
		}
	#else
		// 根据页长计数器、页长、需要走纸步数等，判断是否需要字车展平、退纸等操作，其他状态操作后，可能再次进入该状态进行判断，
		if(FlagDataWait == 10)
		{
			if(PaperState&0xf0 == 0x10)
			{
				// 前进纸模式，纸张页头接近后胶辊，需要字车左右运动展平纸张
				if(PaperCount < (StepFrontToPrint+StepBackToPrint-StepHeadWide)&&((PaperCount+PAPMoveStep) > (StepFrontToPrint+StepBackToPrint-StepHeadWide)))
				{
					c16c1 = (PaperCount+PAPMoveStep) - (StepFrontToPrint+StepBackToPrint-StepHeadWide);
					c16c2 = c16c1;
					PAPMoveStep = (StepFrontToPrint+StepBackToPrint-StepHeadWide)-PaperCount;

					PAPBackSet
					PAPCurrent(2);

					FlagDataWait = 20;
				}
				// 前进纸模式，纸张页尾越过打印位置，本页打印结束，直接退纸
				// 页长在马达相位切换函数中，会根据前里传感器自动调整
				else if((PaperCount+PAPMoveStep) > (PaperPageLenth+StepFrontToPrint))
				{
					c16c1 = PaperPageLenth;
					c16c2 = c16c1;
					PAPMoveStep = (PaperPageLenth+StepFrontToPrint)-PaperCount;

					PAPFeedSet
					PAPCurrent(2);

					FlagDataWait = 30;
				}
				else
				{
					PAPBackSet
					PAPCurrent(2);

					FlagDataWait = 11;

				}
			}
			else if(PaperState&0xf0 == 0x20)
			{
				// 后进纸模式，纸张页头接近后胶辊，需要字车左右运动展平纸张
				if(PaperCount < (StepFrontToPrint+StepBackToPrint-StepHeadWide)&&((PaperCount+PAPMoveStep) > (StepFrontToPrint+StepBackToPrint-StepHeadWide)))if
				{
					c16c1 = (PaperCount+PAPMoveStep) - (StepFrontToPrint+StepBackToPrint-StepHeadWide);
					c16c2 = c16c1;
					PAPMoveStep = (StepFrontToPrint+StepBackToPrint-StepHeadWide)-PaperCount;

					PAPFeedSet
					PAPCurrent(2);

					FlagDataWait = 20;
				}
				// 后进纸纸张也是要从前边出来，无需退纸
				// 后进纸模式，纸张页尾(需要知道默认页长)越过打印位置，本页打印结束；如果此时后里没有纸张，说明是最后一张，继续向前送出全部纸张
				// 马达相位切换函数中，如果j检测到后外和后里都无纸，说明是最后一张，会根据实际设定页长
				/*else if((PaperCount+PAPMoveStep) > (PaperPageLenth+StepFrontToPrint))
				{
					c16c1 = PaperPageLenth;
					c16c2 = c16c1;
					PAPMoveStep = (PaperPageLenth+StepFrontToPrint)-PaperCount;

					PAPFeedSet
					PAPCurrent(2);

					FlagDataWait = 30;
				}*/
				else
				{
					PAPFeedSet
					PAPCurrent(2);

					FlagDataWait = 11;
				}
			}
			else
			{
				FlagDataWait = 11;
			}
		}
	#endif
		// 正常走纸
		if(FlagDataWait == 11)
		{
			if(PAPRuning)
				return;

			FlagDataWait = 0;
			CMD1 = 0;
			CMD2 = 0;
		}

		// 等待页头走到胶辊处，需要字车配合展平纸张
		if(FlagDataWait == 20)
		{
			if(PAPRuning)
				return;

			CRGStopTimePosition = TimePositionAll;
			CRGRightSet
			CRGCurrent(2);

			FlagDataWait++;
		}
		if(FlagDataWait == 21)
		{
			if(CRGRuning)
				return;

			CRGStopTimePosition = 0;
			CRGRightSet
			CRGCurrent(2);

			FlagDataWait++;
		}
		if(FlagDataWait == 22)
		{
			if(CRGRuning)
				return;

			CRGStopTimePosition = TimePositionMiddleForPaperFeed;
			CRGRightSet
			CRGCurrent(2);

			FlagDataWait++;
		}
		if(FlagDataWait == 23)
		{
			if(CRGRuning)
				return;

			PAPMoveStep = c16c1;
			// 运动方向与上次相同
		/*	if(PaperState&0xf0 == 0x10)
				PAPBackSet
			else if(PaperState&0xf0 == 0x20)
				PAPFeedSet	*/
			PAPCurrent(2);

			FlagDataWait = 11;
		}

		// 前进纸模式，页尾已经越过打印位置，需要退纸出去
		// 后进纸模式，几乎不需要退纸操作(仅在撕纸位置时启动打印需要退到打印位置)
		// 等待页尾退回到胶辊处，需要字车配合展平纸张
		if(FlagDataWait == 30)
		{
			if(PAPRuning)
				return;

			PAPMoveStep = StepFrontToPrint-StepHeadWide;
			PAPFeedSet
			PAPCurrent(2);

			FlagDataWait++;
		}
		if(FlagDataWait == 31)
		{
			if(PAPRuning)
				return;

			CRGStopTimePosition = TimePositionAll;
			CRGRightSet
			CRGCurrent(2);

			FlagDataWait++;
		}
		if(FlagDataWait == 32)
		{
			if(CRGRuning)
				return;

			CRGStopTimePosition = 0;
			CRGRightSet
			CRGCurrent(2);

			FlagDataWait++;
		}
		if(FlagDataWait == 33)
		{
			if(CRGRuning)
				return;

			CRGStopTimePosition = TimePositionMiddleForPaperFeed;
			CRGRightSet
			CRGCurrent(2);

			FlagDataWait++;
		}
		if(FlagDataWait == 34)
		{
			if(CRGRuning)
				return;

			PAPMoveStep = PaperPageLenth+StepFrontToPrint;
			// 运动方向与上次相同
			PAPCurrent(2);

			FlagDataWait = 11;
		}
	}
	// 设定单/双向打印
	else if( (CMD1 == 0x1b) && (CMD2 ==  0x55) )
	{
		if(FlagDataWait == 0)
		{
			SendCmd();

			if(ReceiveBufferSize()<1)
				return;

			c8c1 = ReceiveBufferData();//GetBufferReceiveData();
			if(c8c1&0x01)
			{
				CMDOneDirSet
			}
			else
			{
				CMDTwoDirSet
			}

			FlagDataWait++;
		}
		if(FlagDataWait == 1)
		{
			FlagDataWait = 0;
			CMD1 = 0;
			CMD2 = 0;
		}
	}
	else if( (CMD1 == 0x1b) && (CMD2 == 0x6a) )
	{
		// 等待数据
		if(FlagDataWait == 0)
		{
			SendCmd();

			//printf("DataCMD: 0x1b 0x4a->0 \r\n");
			if(ReceiveBufferSize()<1)
				return;

			PAPMoveStep = ReceiveBufferData();// GetBufferReceiveData();
			MotorEnable
			PAPBackSet
			PAPCurrent(2);

			FlagDataWait++;
		}
		else if(FlagDataWait == 1)
		{
			if(PAPRuning)
				return;

			FlagDataWait= 0;
			CMD1 = 0;
			CMD2 = 0;
		}
	}
	/*设置马达速度*/ // 已经确定速度，不再区分高低速
	else if( (CMD1 == 0x1b) && (CMD2 ==  0x7a) )
	{
		SendCmd();
		if(ReceiveBufferSize()<1)
			return;

		c8c1 = ReceiveBufferData();//GetBufferReceiveData();
		/*		if(c8c1 == 0)
				{
					CRGSpeedNormalSet
					PAPSpeedNormalSet
				}
				else if(c1 == 1)
				{
					CRGSpeedHighSet
					PAPSpeedHighSet
				}*/

		CMD1 = 0;
		CMD2 = 0;
	}
#if 0
	//设置productid
	else if( (CMD1 == 0x1b) && (CMD2 ==  0x80) )
	{
		if(NumBufferReceive()<2)
			return;

		c8c1 = ReceiveBufferData();//GetBufferReceiveData();
		c16c1 = ReceiveBufferData();//GetBufferReceiveData();
		c16c1 <<= 8;
		c16c1 |= c8c1;
	//	ProductID = c16c1; // @@@@@
		SendString("ProductID设置为:");
		OutputByteData((c16c1/10000)%10+'0');
		OutputByteData((c16c1/1000)%10+'0');
		OutputByteData((c16c1/100)%10+'0');
		OutputByteData((c16c1/10)%10+'0');
		OutputByteData(c16c1%10+'0');
		SendString("\r\n");
	//	WritePID(ProductID);
		while(1)
			MainPowerOff

		CMD1 = 0;
		CMD2 = 0;
	}
	/*测量电压*/
	else if( (CMD1 == 0x1b) && (CMD2 ==  0x84) )
	{
	//	TestSensorBack(); // @@@@@
		CMD1 = 0;
		CMD2 = 0;
	}
	/*向前走纸一直将纸走完*/
	else if( (CMD1 == 0x1b) && (CMD2 ==  0x88) )
	{
		if(FlagDataWait == 0)
		{
			PAPMoveStep = 72*22;
			PAPFeedSet
			PAPCurrent(2);

			FlagDataWait++;
		}
		if(FlagDataWait == 1)
		{
			if(PAPRuning)
				return;

			if(0)//@@@@@Alke//if(CheckPaperON(1))
				FlagDataWait = 0; // feed again
			else
				FlagDataWait++; // go to next step
		}

		if(FlagDataWait == 2)
		{
			PAPMoveStep = 72*10;
			PAPFeedSet
			PAPCurrent(2);

			FlagDataWait++;
		}
		if(FlagDataWait == 3)
		{
			if(PAPRuning)
				return;

			FlagDataWait = 0;
			CMD1 = 0;
			CMD2 = 0;
		}
	}
#endif
	// 逻辑定位到打印起始位，非物理定位，并且记录到达胶辊步数，用于在临近胶辊时启动左右压纸动作
	// 设置页长
	else if( (CMD1 == 0x1b) && (CMD2 ==  0x89) )
	{
		SendCmd();

		if(ReceiveBufferSize()<2)
			return;

		c8c1 = ReceiveBufferData();//GetBufferReceiveData();
		c8c2 = ReceiveBufferData();//GetBufferReceiveData();
		StepPageLenth = c8c1;
		StepPageLenth <<= 8;
		StepPageLenth |= c8c2;

		FlagDataWait = 0;
		CMD1 = 0;
		CMD2 = 0;
	}
#if 0
	/*发送成列度信息纸张电压信息*/
	else if( (CMD1 == 0x1b) && (CMD2 ==  0x90) )
	{
	;//	SendNeedleResister(); // @@@@@ Alke
	}
	/*设置打印机铭牌*/
	else if( (CMD1 == 0x1b) && (CMD2 ==  0x91) )
	{
		if(FlagDataWait == 0)
		{
			if(ReceiveBufferSize()<2)
				return;

			c8c1 = ReceiveBufferData();//GetBufferReceiveData();
			c8c2 = ReceiveBufferData();//GetBufferReceiveData();

			FlagDataWait++;
		}
		if(FlagDataWait == 1)
		{
			if(c8c1==0x55&&c8c2==0xbb)
			{
				if(NumBufferReceive()<100)
					return;

				for(c8c3=0; c8c3<100; c8c3++)
				{
				;//	bufferLogo[c8c3]=0;
				}
				/*铭牌总计100个字符，不足的补充空格*/
				for(c8c3 = 0; c8c3 < 100; c8c3++)//图形指令数据接收
				{
					c8c4 = ReceiveBufferData();//GetBufferReceiveData();//*(bufferLogo+c8c3) = GetBufferReceiveData();
				}
				/*写入字库里面*/
			//	WriteLogo();

			//	BuzzerHead(1,5);
			}

			FlagDataWait = 0;
			CMD1=0;
			CMD2=0;
		}
	}
	/*设置打印机寻边传感器电压值*/
	else if( (CMD1 == 0x1b) && (CMD2 ==  0x92) )
	{
		if(FlagDataWait == 0)
		{
			if(ReceiveBufferSize() < 4)
			{
				return;

				c8c1 = ReceiveBufferData();//GetBufferReceiveData();
				c8c2 = ReceiveBufferData();//GetBufferReceiveData();
				c8c3 = ReceiveBufferData();//GetBufferReceiveData();
				c8c4 = ReceiveBufferData();//GetBufferReceiveData();

				FlagDataWait++;
			}
		}
		if(FlagDataWait == 1)
		{
			c16c1 = c8c1;
			c16c1 <<= 8;
			c16c1 |= c8c2;
			c16c2 = c8c3;
			c16c2 <<= 8;
			c16c2 |= c8c4;
		//	LeftVoltagePaper=c16c1;
		//	RightVoltagePaper=c16c2;

		#if 0
			/*转换成为ADC的值*/
			if(LeftVoltagePaper>200||LeftVoltagePaper<100)
			{
				SendString("left设置的电压超出范围设置失败\r\n");
				LeftVoltagePaper=130;
				LeftVoltagePaper=LeftVoltagePaper*4096/330;
			}
			else
			{
				SendString("left设置成功\r\n");
				SendString("电压值:");
				OutputByteData((LeftVoltagePaper/1000)%10+'0');
				OutputByteData((LeftVoltagePaper/100)%10+'0');
				OutputByteData((LeftVoltagePaper/10)%10+'0');
				OutputByteData((LeftVoltagePaper/1)%10+'0');
				SendString("\r\n");
				LeftVoltagePaper=LeftVoltagePaper*4096/330;
				SendString("leftADC电压值:");
				OutputByteData((LeftVoltagePaper/1000)%10+'0');
				OutputByteData((LeftVoltagePaper/100)%10+'0');
				OutputByteData((LeftVoltagePaper/10)%10+'0');
				OutputByteData((LeftVoltagePaper/1)%10+'0');
				SendString("\r\n");
			}

			if(RightVoltagePaper>200||RightVoltagePaper<100)
			{
				SendString("right设置的电压超出范围设置失败\r\n");
				RightVoltagePaper=130;
				RightVoltagePaper=RightVoltagePaper*4096/330;
			}
			else
			{
				SendString("right设置成功\r\n");
				SendString("电压值:");
				OutputByteData((RightVoltagePaper/1000)%10+'0');
				OutputByteData((RightVoltagePaper/100)%10+'0');
				OutputByteData((RightVoltagePaper/10)%10+'0');
				OutputByteData((RightVoltagePaper/1)%10+'0');
				SendString("\r\n");
				RightVoltagePaper=RightVoltagePaper*4096/330;
				SendString("rightADC电压值:");
				OutputByteData((RightVoltagePaper/1000)%10+'0');
				OutputByteData((RightVoltagePaper/100)%10+'0');
				OutputByteData((RightVoltagePaper/10)%10+'0');
				OutputByteData((RightVoltagePaper/1)%10+'0');
				SendString("\r\n");
			}
			/*存入flash*/
			bufferVoltage[0]=(LeftVoltagePaper&0xff00)>>8;
			bufferVoltage[1]=LeftVoltagePaper&0xff;
			bufferVoltage[2]=(RightVoltagePaper&0xff00)>>8;
			bufferVoltage[3]=RightVoltagePaper&0xff;

			TPS=~LeftVoltagePaper;
			SendString("TPS1:");
			OutputByteData((TPS/1000)%10+'0');
			OutputByteData((TPS/100)%10+'0');
			OutputByteData((TPS/10)%10+'0');
			OutputByteData((TPS/1)%10+'0');
			SendString("\r\n");

			bufferVoltage[4]=(TPS&0xff00)>>8;
			bufferVoltage[5]=TPS&0xff;

			TPS=~RightVoltagePaper;
			bufferVoltage[6]=(TPS&0xff00)>>8;
			bufferVoltage[7]=TPS&0xff;

			TPS=~LeftVoltagePaper;
			SendString("TPS2:");
			OutputByteData((TPS/1000)%10+'0');
			OutputByteData((TPS/100)%10+'0');
			OutputByteData((TPS/10)%10+'0');
			OutputByteData((TPS/1)%10+'0');
			SendString("\r\n");

			//GtDataWrite(13000, bufferVoltage, 8);

			SendString("读出存入数据\r\n");
			/*将数据读出*/
			//GtDataRead(13000, bufferVoltage, 4);
			LeftVoltagePaper=(((unsigned int)bufferVoltage[0])<<8)|bufferVoltage[1];
			RightVoltagePaper=(((unsigned int)bufferVoltage[2])<<8)|bufferVoltage[3];
			/*转换成为电压值*/
			Temp=330*LeftVoltagePaper/4096;
			if(Temp>200||Temp<100)
			{
				SendString("left设置的电压超出范围设置失败\r\n");
				/*如果超出范围默认值1.3V*/
				LeftVoltagePaper=130*4096/330;
			}
			else
			{
				SendString("left:\r\n");
				SendString("电压值:");
				OutputByteData((Temp/1000)%10+'0');
				OutputByteData((Temp/100)%10+'0');
				OutputByteData((Temp/10)%10+'0');
				OutputByteData((Temp/1)%10+'0');
				SendString("\r\n");
			}

			Temp=330*RightVoltagePaper/4096;
			if(Temp>200||Temp<100)
			{
				SendString("right设置的电压超出范围设置失败\r\n");
				/*如果超出范围默认值1.3V*/
				RightVoltagePaper=130*4096/330;
			}
			else
			{
				SendString("Right:\r\n");
				SendString("电压值:");
				OutputByteData((Temp/1000)%10+'0');
				OutputByteData((Temp/100)%10+'0');
				OutputByteData((Temp/10)%10+'0');
				OutputByteData((Temp/1)%10+'0');
				SendString("\r\n");
			}
			AllDataInit();
			SendNeedleResister();
		#endif

			FlagDataWait = 0;
			CMD1 = 0;
			CMD2 = 0;
		}
	}
	/*寻边开关*/
	else if( (CMD1 == 0x1b) && (CMD2 ==  0x93) )
	{
		if(ReceiveBufferSize()<1)
			return;

		c8c1 = ReceiveBufferData();//GetBufferReceiveData();
	#if 0
				SendString("RightEdge:");
				OutputByteData((RightEdge/1000)%10+'0');
				OutputByteData((RightEdge/100)%10+'0');
				OutputByteData((RightEdge/10)%10+'0');
				OutputByteData((RightEdge/1)%10+'0');
				SendString("\r\n");

				SendString("LeftEdge:");
				OutputByteData((LeftEdge/1000)%10+'0');
				OutputByteData((LeftEdge/100)%10+'0');
				OutputByteData((LeftEdge/10)%10+'0');
				OutputByteData((LeftEdge/1)%10+'0');
				SendString("\r\n");
	#endif
		CMD1 = 0;
		CMD2 = 0;
	}
	/*USB自动设置功能*/
	else if( (CMD1 == 0x1b) && (CMD2 ==  0x94) )
	{
		if(NumBufferReceive()<1)
			return;

		c8c1 = ReceiveBufferData();//GetBufferReceiveData();
	#if 0
		if(c8c1)
		{
			USBHandData=18;
			SendString("USB 握手功能开启");
		}
		else
		{
			USBHandData=14;
			SendString("USB 握手功能关闭");
		}
		WriteUSBS();
		//GtDataWrite(25000, &USBHandData, 1);
		//NVIC_SystemReset();
	#endif
		CMD1 = 0;
		CMD2 = 0;
	}
	/*进入U盘*/
	else if( (CMD1 == 0x1b) && (CMD2 ==  0x95) )
	{
	#if 0
		SendString("进入USB U盘模式\r\n");
	//	Passwordf=1;
		//ForProducnt();
		for(c8c1=0;c8c1<12;c8c1++)
		{
			OutputByteData((TemDeviceID[c8c1]/1000)%10+'0');
			OutputByteData((TemDeviceID[c8c1]/100)%10+'0');
			OutputByteData((TemDeviceID[c8c1]/10)%10+'0');
			OutputByteData((TemDeviceID[c8c1]/1)%10+'0');
			SendString("\r\n");
		}
		for(c8c1=0;c8c1<12;c8c1++)
		{
			OutputByteData((ID[c8c1]/1000)%10+'0');
			OutputByteData((ID[c8c1]/100)%10+'0');
			OutputByteData((ID[c8c1]/10)%10+'0');
			OutputByteData((ID[c8c1]/1)%10+'0');
			SendString("\r\n");
		}
	#endif
		CMD1 = 0;
		CMD2 = 0;
	}
	/*重新寻边*/
	else if( (CMD1 == 0x1b) && (CMD2 ==  0x96) )
	{
		/*退纸*/
		if(FlagDataWait == 0)
		{
			PAPMoveStep = 72*22;
			PAPBackSet
			PAPCurrent(2);

			FlagDataWait++;
		}
		if(FlagDataWait == 1)
		{
			FlagDataWait = 0;
			CMD1 = 0;
			CMD2 = 0;
		}
	}
	else if( (CMD1 == 0x1b) && (CMD2 ==  0x97) )
	{
		if(NumBufferReceive()<2)
			return;

		c8c1 = ReceiveBufferData();//GetBufferReceiveData();
		c8c2 = ReceiveBufferData();//GetBufferReceiveData();
		if(c8c1==0x55&&c8c2==0xbb)
		{
		;//	oldageing(); // @@@@@
		}

		CMD1 = 0;
		CMD2 = 0;
	}
	/*设置为正常速度*/
	else if( (CMD1 == 0x1b) && (CMD2 ==  0x98) ) // 1B98不能用，有J乱码
	{
		CMD1 = 0;
		CMD2 = 0;
	}
	/*设置为慢速*/
	else if( (CMD1 == 0x1b) && (CMD2 ==  0x99) )
	{
		CMD1 = 0;
		CMD2 = 0;
	}
	/*清除加密区*/
	else if( (CMD1 == 0x1b) && (CMD2 ==  0x9a))
	{
		if(NumBufferReceive()<2)
			return;

		c8c1 = ReceiveBufferData();//GetBufferReceiveData();
		c8c2 = ReceiveBufferData();//GetBufferReceiveData();
		if(c8c1==0x55&&c8c2==0xbb)
		{
//			USBPause;
		//	BuzzerHead(1,5);
		//	WriteClearData();
			MainPowerOff//NVIC_SystemReset();
		}
		CMD1 = 0;
		CMD2 = 0;
	}
	/*撕纸位置进纸--------------------------------------------------------------*/
	else if( (CMD1 == 0x1b) && (CMD2 ==  0x9c) )
	{
		if(NumBufferReceive()<2)
			return;

		c8c1 = ReceiveBufferData();//GetBufferReceiveData();
		SendString("走到撕纸位置\r\n");
	#if 0
	//	PositionFlag=0;
		/*不需要走到撕纸位置*/
	//	if(DriverHead==0)
		{
			SendString("不执行走到撕纸位置命令----\r\n");
	//		return;
		}
	//	DriverHead=0;

		/*判断后面是否还有数据进来，如果有则不走到撕纸位置，理论前面已经处理掉了*/
		//后进纸执行
		if(PrinttingMode==PrintBack)
		{
					SendString("走到撕纸位置\r\n");

					/*走纸到撕纸位置*/
					/*如果当前位置在打印起始位置*/
					//if(PaperPositionMode==PrintPrintP)
					{
						/*补偿打印起始位置*/
						if(PaperPrintFirstLine>PrintPrintConst)
						{
							c2=PaperPrintFirstLine-PrintPrintConst;
							PAPMoveStep = PrintCutConst+CMD1-c2;
						}
						else
						{
							c2=PrintPrintConst-PaperPrintFirstLine;
							PAPMoveStep = PrintCutConst+CMD1+c2;
						}
						PaperCutPs=PrintCutConst+CMD1;
						if(PrinttingMode==PrintBack)
						{
							/*动态记录当前打印页面撕纸位置*/
							PAPFeedStart(0);
							while(PAPCRT == 0x00);
						}
						//记录当前撕纸位置
						SendString("撕纸位置走纸\r\n");
						OutputByteData((PaperCutPs/1000)%10+'0');
						OutputByteData((PaperCutPs/100)%10+'0');
						OutputByteData((PaperCutPs/10)%10+'0');
						OutputByteData((PaperCutPs/1)%10+'0');
						SendString("\r\n");
						PaperPositionMode=PrintCut;

						/*判断这张纸是否已经走过后胶辊*/
						if(0==CheckPaperON(4))
						{
							DelayTime2mS(5);// 5	// @20220516 Alke 开始结束加快纸张进退速度
							if(0==CheckPaperON(4))
							{
								SendString("走到撕纸位置命令将纸走出后胶辊\r\n");
								/*将纸张全部退出*/
								SendString("将纸张全部退出\r\n");
								PAPMoveStep = 72*10;
								PAPFeedStart(0);
								while(PAPCRT == 0x00);
								while(CheckPaperON(4)||CheckPaperON(1))
								{
									PAPMoveStep = 72*10;
									PAPFeedStart(0);
								}
								backwardmotorcount=0;
								SangYi2=1;
								/*对位退纸功能取消*/
								PositionFlag=0;
								DriverHead=0;
								/*打印起始位置对位打开*/
								PrintDataContinu=0;

							}
						}
						else
						{
							SendString("走到撕纸位置命令，纸张还在后胶辊\r\n");
							Position=1;
							backwardmotorcount=0;
							SangYi2=0;
							/*对位退纸功能取消*/
							PositionFlag=0;
							DriverHead=0;
							/*打印起始位置对位打开*/
							PrintDataContinu=0;
						}
					}
		}
	#endif

		CMD1=0;
		CMD2=0;
	}

#endif
#if 0
	/*撕纸位置退纸*/
	else if( (CMD1 == 0x1b) && (CMD2 ==  0x9e) )
	{
		if(NumBufferReceive()<1)
			return;

		c8c1 = ReceiveBufferData();//GetBufferReceiveData();
		#if 0
				SendString("走到撕纸位置1\r\n");
				PositionFlag=0;
				WaitForData(1);

				/*连续打印时候*/
				if(DriverHead==0)
				{
					SendString("不执行走到撕纸位置命令----\r\n");
					return;
				}
				DriverHead=0;

				//后进纸执行
				if(PrinttingMode==PrintBack)
				{
					/*走纸到撕纸位置*/

					/*如果当前位置在打印起始位置*/
					//if(PaperPositionMode==PrintPrintP)
					{
						/*理论上是成立的*/
						if(PrintCutConst>CMD1)
						{
							PAPMoveStep=PrintCutConst-CMD1;
							if(PrinttingMode==PrintBack)
							{
								PaperCutPs=PAPMoveStep;
								PAPFeedStart(0);
								while(PAPCRT == 0x00);
							}

							PaperPositionMode=PrintCut;
							SendString("撕纸位置:");
							OutputByteData((PaperCutPs/1000)%10+'0');
							OutputByteData((PaperCutPs/100)%10+'0');
							OutputByteData((PaperCutPs/10)%10+'0');
							OutputByteData((PaperCutPs/1)%10+'0');
							SendString("\r\n");
						}
						/*判断这张纸是否已经走过后胶辊*/
						if(0==CheckPaperON(4))
						{
							DelayTime2mS(5);// 10	// @20220516 Alke 开始结束加快纸张进退速度
							if(0==CheckPaperON(4))
							{
								SendString("走到撕纸位置命令将纸走出后胶辊\r\n");
								/*将纸张全部退出*/
								SendString("将纸张全部退出\r\n");
								PAPMoveStep = 72*10;
								PAPFeedStart(0);
								while(PAPCRT == 0x00);
								while(CheckPaperON(4)||CheckPaperON(1))
								{
									PAPMoveStep = 72*10;
									PAPFeedStart(0);
								}
								while(1)
								{
									if((CheckPaperON(1)==0)&&(CheckPaperON(4)==0))
									{
										DelayTime2mS(100);
										if((CheckPaperON(1)==0)&&(CheckPaperON(4)==0))
										{
											DelayTime2mS(100);
											if((CheckPaperON(1)==0)&&(CheckPaperON(4)==0))
											{
												break;
											}
										}
									}
								}
								SendString("纸张已经退出\r\n");
								backwardmotorcount=0;
								SangYi2=1;
								/*对位退纸功能取消*/
								PositionFlag=0;
								DriverHead=0;
								/*打印起始位置对位打开*/
								PrintDataContinu=0;

							}
						}
						else
						{
							SendString("走到撕纸位置命令，纸张还在后胶辊\r\n");
							Position=1;

							backwardmotorcount=0;
							SangYi2=0;
							/*对位退纸功能取消*/
							PositionFlag=0;
							DriverHead=0;
							/*打印起始位置对位打开*/
							PrintDataContinu=0;
						}
					}
				}
		#endif
		CMD1=0;
		CMD2=0;
	}
#endif
	/*打印起始位置调整+	如果是前进纸，在到达逻辑打印起始位置后，再用参数C1去修正该位置*/
	else if( (CMD1 == 0x1b) && (CMD2 ==  0xa0) )
	{
		if(FlagDataWait == 0)
		{
			SendCmd();

			if(ReceiveBufferSize()<1)
				return;

			c8c1 = ReceiveBufferData();//GetBufferReceiveData();
			FlagDataWait++;
		}
		if(FlagDataWait == 1)
		{
			if((PaperState&0xf0) == 0x10)
			{
				if(PaperCount <= StepFrontToPrint)
				{
					PAPMoveStep = c8c1+StepFrontToPrint-PaperCount;
					PAPBackSet
					PAPCurrent(2);

					FlagDataWait++;
				}
				else
				{
					PAPMoveStep = PaperCount+StepHeadWide-StepFrontToPrint;
					PAPFeedSet
					PAPCurrent(2);

					FlagDataWait = 10;
				}
			}
			else if((PaperState&0xf0) == 0x20)
			{
				if(PaperCount <= StepFrontToPrint)
				{
					PAPMoveStep = c8c1+StepFrontToPrint-PaperCount;
					PAPFeedSet
					PAPCurrent(2);

					FlagDataWait++;
				}
				else
				{
					PAPMoveStep = PaperCount+StepHeadWide-StepFrontToPrint;
					PAPBackSet
					PAPCurrent(2);

					FlagDataWait = 10;
				}
			}
			else
				FlagDataWait++;
		}
		if(FlagDataWait == 2)
		{
			if(PAPRuning)
				return;

			FlagDataWait = 0;
			CMD1=0;
			CMD2=0;
		}

		if(FlagDataWait == 10)
		{
			PAPMoveStep = c8c1+StepHeadWide;
			if((PaperState&0xf0) == 0x10)
				PAPBackSet
			else
				PAPFeedSet
			PAPCurrent(2);

			FlagDataWait++;
		}
		if(FlagDataWait == 11)
		{
			FlagDataWait = 0;
			CMD1=0;
			CMD2=0;
		}
	#if 0
				SendString("起始打印位置---1\r\n");
				DriverHead=1;//
				PositionFlag=1;

				if(PrintDataContinu==0)
				{
					SendString("起始打印位置---1\r\n");
					//后进纸执行
					//if(PrinttingMode==PrintBack)
					{
						/*走纸到撕纸位置*/
						/*如果当前位置在撕纸位置，理论情况在撕纸位置，开机状态在打印起始位置*/
						PaperPrintFirstLine=PrintPrintConst+CMD1;
						//if(PaperPositionMode==PrintCut)
						if(Position)//如果撕纸位置
						{

								/*退纸打印*/
								if(PrinttingMode==PrintBack)
								{
									Dirc=0;
									REStep=CMD1;

									if(Position)
									{
										PAPMoveStep = PaperCutPs-CMD1;
										PostionStepRe=PAPMoveStep;//记录退纸步数
										PAPFeedStart(1);
										while(PAPCRT == 0x00);
									}
									Position=0;
									SendString("修正打印机机械间隙\r\n");

									PAPMoveStep = 36;
									PAPFeedStart(1);
									while(PAPCRT == 0x00);

									PAPMoveStep = 36;
									PAPFeedStart(0);
									while(PAPCRT == 0x00);

									SendString("起始位置退纸-----------\r\n");

									SendString("PostionStepRe:");
									OutputByteData((PostionStepRe/1000)%10+'0');
									OutputByteData((PostionStepRe/100)%10+'0');
									OutputByteData((PostionStepRe/10)%10+'0');
									OutputByteData((PostionStepRe/1)%10+'0');
									SendString("\r\n");
								}

								//DelayTime2mS(u32 CountFor2mS);

								PaperPositionMode=PrintPrintP;
								SendString("打印起始位置\r\n");
								OutputByteData(((PaperPrintFirstLine)/1000)%10+'0');
								OutputByteData(((PaperPrintFirstLine)/100)%10+'0');
								OutputByteData(((PaperPrintFirstLine)/10)%10+'0');
								OutputByteData(((PaperPrintFirstLine)/1)%10+'0');
								SendString("\r\n");
						}
					}
					/*前进纸*/
					 if(PrinttingMode==PrintForward)
					{
						/*如果是前面进纸则执行进纸操作*/
						if(CMD1>0)
						{
							PAPMoveStep = CMD1;
							PAPFeedStart(0);
							while(PAPCRT == 0x00);
							SendString("指令修订打印起始位置\r\n");
						}

						PostionStepRe = PaperCutPs-CMD1;
					}
				}
				PrintDataContinu=0;
	#endif
	}
#if 0
	/*打印起始位置-*/
	else if( (CMD1 == 0x1b) && (CMD2 ==  0xa1) )
	{
		if(NumBufferReceive()<1)
			return;

		c8c1 = ReceiveBufferData();//GetBufferReceiveData();
	#if 0
				SendString("起始打印位置---\r\n");
				DriverHead=1;//
				PositionFlag=1;

				if(PrintDataContinu==0)
				{
					//后进纸执行
					//if(PrinttingMode==PrintBack)
					{

						/*走纸到打印起始位置*/

						/*如果当前位置在撕纸位置，理论情况在撕纸位置，开机状态在打印起始位置*/
						PaperPrintFirstLine=PrintPrintConst-CMD1;
							Dirc=1;
									REStep=CMD1;
						//if(PaperPositionMode==PrintCut)
						if(Position)
						{
							/*退纸打印*/

							if(PrinttingMode==PrintBack)
							{
								if(Position)
								{
									PAPMoveStep = PaperCutPs+CMD1;
									PostionStepRe=PAPMoveStep;//记录退纸步数
									PAPFeedStart(1);
									while(PAPCRT == 0x00);
									SendString("起始位置退纸-----------\r\n");
								}
								Position=0;
								SendString("修正打印机机械间隙\r\n");
								PAPMoveStep = 36;
								PAPFeedStart(1);
								while(PAPCRT == 0x00);
								PAPMoveStep = 36;
								PAPFeedStart(0);
								while(PAPCRT == 0x00);

								SendString("PostionStepRe:");
								OutputByteData((PostionStepRe/1000)%10+'0');
								OutputByteData((PostionStepRe/100)%10+'0');
								OutputByteData((PostionStepRe/10)%10+'0');
								OutputByteData((PostionStepRe/1)%10+'0');
								SendString("\r\n");
							}
							PaperPositionMode=PrintPrintP;
							SendString("打印起始位置\r\n");
							OutputByteData(((PaperPrintFirstLine)/1000)%10+'0');
							OutputByteData(((PaperPrintFirstLine)/100)%10+'0');
							OutputByteData(((PaperPrintFirstLine)/10)%10+'0');
							OutputByteData(((PaperPrintFirstLine)/1)%10+'0');
							SendString("\r\n");
						}
					}
					/*前进纸*/
					 if(PrinttingMode==PrintForward)
					{
						WaitForData(1);
						CMD1 = *(BufferReceive+cBufferReceiveRead);
						cBufferReceiveReadADD();
						/*如果是前面进纸则执行进纸操作*/
						if(CMD1>0)
						{
							PAPMoveStep = CMD1;
							PAPFeedStart(1);
							while(PAPCRT == 0x00);
							SendString("指令修订打印起始位置\r\n");
						}
					}
				}
				PrintDataContinu=0;
	#endif
		CMD1=0;
		CMD2=0;
	}
	/*设置延时时间*/
	else if( (CMD1 == 0x1b) && (CMD2 ==  0xa2) )
	{
		if(NumBufferReceive()<1)
			return;

		c8c1 = ReceiveBufferData();//GetBufferReceiveData();
	#if 0

				if(c8c1==0)
				{
					TimeDelayP=200;
				}
				else if(c8c1==1)
				{
					TimeDelayP=400;
				}
				else if(c8c1==2)
				{
					TimeDelayP=1000;
				}
				else if(c8c1==3)
				{
					TimeDelayP=2000;
				}
				else if(c8c1==4)
				{
					TimeDelayP=4000;
				}
				else if(c8c1==5)
				{
					TimeDelayP=20000;
				}
	#endif
		CMD1=0;
		CMD2=0;
	}
/*	else if( (CMD1 == 0x1b) && (CMD2 == 0xaa) )
	{
		if(ReceiveBufferSize()<2)
			return;

		FlagHeadIC = 0x0000;
		c8c2 = ReceiveBufferData();//2468
		c8c1 = ReceiveBufferData();//1357
		if(c8c1&0x80) FlagHeadIC |= Needle01;
		if(c8c2&0x80) FlagHeadIC |= Needle02;
		if(c8c1&0x40) FlagHeadIC |= Needle03;
		if(c8c2&0x40) FlagHeadIC |= Needle04;
		if(c8c1&0x20) FlagHeadIC |= Needle05;
		if(c8c2&0x20) FlagHeadIC |= Needle06;
		if(c8c1&0x10) FlagHeadIC |= Needle07;
		if(c8c2&0x10) FlagHeadIC |= Needle08;
		if(c8c1&0x08) FlagHeadIC |= Needle09;
		if(c8c2&0x08) FlagHeadIC |= Needle10;
		if(c8c1&0x04) FlagHeadIC |= Needle11;
		if(c8c2&0x04) FlagHeadIC |= Needle12;
		if(c8c1&0x02) FlagHeadIC |= Needle13;
		if(c8c2&0x02) FlagHeadIC |= Needle14;
		if(c8c1&0x01) FlagHeadIC |= Needle15;
		if(c8c2&0x01) FlagHeadIC |= Needle16;
		HeadIC();

		CMD1 = 0;
		CMD2 = 0;
	}*/
	/*孔位置计算开关*/
	else if( (CMD1 == 0x1b) && (CMD2 ==  0xf3) )
	{
		if(NumBufferReceive()<1)
			return;

		c8c1 = ReceiveBufferData();//GetBufferReceiveData();
	#if 0
				HoleSizeCalculate=CMD1;
				SendString("HoleSizeCalculate:");
				OutputByteData((HoleSizeCalculate/1000)%10+'0');
				OutputByteData((HoleSizeCalculate/100)%10+'0');
				OutputByteData((HoleSizeCalculate/10)%10+'0');
				OutputByteData((HoleSizeCalculate/1)%10+'0');
				SendString("\r\n");
	#endif
		CMD1 = 0;
		CMD2 = 0;
	}
	else if( (CMD1 == 0x1b) && (CMD2 ==  0xf4) )
	{
		if(NumBufferReceive()<2)
			return;

		c8c1 = ReceiveBufferData();//GetBufferReceiveData();
		c8c2 = ReceiveBufferData();//GetBufferReceiveData();
	#if 0
				LeftEdgeMove=((unsigned int)c8c1<<8)|c8c2;
				if(LeftEdgeMove>10000)
				{
					if((LeftEdgeMove-10000)>80)
					{
						LeftEdgeMove=10000+80;
					}
				}
				else
				{
					if((10000-LeftEdgeMove)>80)
					{
						LeftEdgeMove=10000-80;
					}
				}

				SendString("LeftEdgeMove:");
				OutputByteData((LeftEdgeMove/1000)%10+'0');
				OutputByteData((LeftEdgeMove/100)%10+'0');
				OutputByteData((LeftEdgeMove/10)%10+'0');
				OutputByteData((LeftEdgeMove/1)%10+'0');
				SendString("\r\n");
				CMD1 = 0;
				CMD2 = 0;
		}
	#endif
	}
	else if( (CMD1 == 0x1b) && (CMD2 ==  0xf8) )
	{
		if(NumBufferReceive()<3)
			return;

		c8c1 = ReceiveBufferData();//GetBufferReceiveData();
		c8c2 = ReceiveBufferData();//GetBufferReceiveData();
		c8c3 = ReceiveBufferData();//GetBufferReceiveData();
	#if 0
				if((c8c1==0xaa)&&(c8c2==0))
				{
					HeadDelayCount=c8c3;
					if(HeadDelayCount>60)
					{
						SendString("别乱来\r\n");
						HeadDelayCount=60;
					}
					SendString("打打而已别认真\r\n");
					SendString("HeadDelayCount:");
					OutputByteData((HeadDelayCount/1000)%10+'0');
					OutputByteData((HeadDelayCount/100)%10+'0');
					OutputByteData((HeadDelayCount/10)%10+'0');
					OutputByteData((HeadDelayCount/1)%10+'0');
					SendString("\r\n");
				}
				else if((c8c1==0xaa)&&c8c2)
				{
					HeadDelayCount=c8c3;
					SendString("HeadDelayCount:");
					OutputByteData((HeadDelayCount/1000)%10+'0');
					OutputByteData((HeadDelayCount/100)%10+'0');
					OutputByteData((HeadDelayCount/10)%10+'0');
					OutputByteData((HeadDelayCount/1)%10+'0');
					SendString("\r\n");

					if(HeadDelayCount>60)
					{
						SendString("别乱来\r\n");
						HeadDelayCount=60;
					}
					WriteDelay(HeadDelayCount);
					SendString("HeadDelayCount:");
					OutputByteData((HeadDelayCount/1000)%10+'0');
					OutputByteData((HeadDelayCount/100)%10+'0');
					OutputByteData((HeadDelayCount/10)%10+'0');
					OutputByteData((HeadDelayCount/1)%10+'0');
				    SendString("\r\n");
				}
	#endif
		CMD1=0;
		CMD2=0;
	}

	// 设定单行倍宽
	else if( (CMD1 == 0x1c) && (CMD2 ==  0x0e) )
	{
		CMD1 = 0;
		CMD2 = 0;
	}
	// 设定半角汉字
	else if( (CMD1 == 0x1c) && (CMD2 ==  0x0f) )
	{
		CMD1 = 0;
		CMD2 = 0;
	}
	// 解除半角汉字
	else if( (CMD1 == 0x1c) && (CMD2 ==  0x12) )
	{
		CMD1 = 0;
		CMD2 = 0;
	}
	// 解除单行倍宽
	else if( (CMD1 == 0x1c) && (CMD2 ==  0x14) )
	{
		CMD1 = 0;
		CMD2 = 0;
	}
	// 汉字打印模式组合
	else if( (CMD1 == 0x1c) && (CMD2 ==  0x21) )
	{
		if(NumBufferReceive()<1)
			return;

		c8c1 = ReceiveBufferData();//GetBufferReceiveData();
		if(c8c1&0x04)
			FontChineseDWSet;
		else
			FontChineseDWClr;
		if(c8c1&0x08)
			FontChineseDHSet;
		else
			FontChineseDHClr;
		if(c8c1&0x80)
			FontChineseLBSet;
		else
			FontChineseLBClr;

		CMD1 = 0;
		CMD2 = 0;
	}
	// 设定汉字模式
	else if( (CMD1 == 0x1c) && (CMD2 ==  0x26) )
	{
		CMD1 = 0;
		CMD2 = 0;
	}
	//
	else if( (CMD1 == 0x1c) && (CMD2 ==  0x28) )
	{
		if(NumBufferReceive()<5)
			return;

		c8c1 = ReceiveBufferData();//GetBufferReceiveData();
		c8c2 = ReceiveBufferData();//GetBufferReceiveData();
		c8c3 = ReceiveBufferData();//GetBufferReceiveData();
		c8c4 = ReceiveBufferData();//GetBufferReceiveData();
		c8c5 = ReceiveBufferData();//GetBufferReceiveData();

		CMD1 = 0;
		CMD2 = 0;
	}
	// 设定/解除汉字下划线
	else if( (CMD1 == 0x1c) && (CMD2 ==  0x2d) )
	{
		if(NumBufferReceive()<1)
			return;

		c8c1 = ReceiveBufferData();//GetBufferReceiveData();
		if(c8c1&0x01)
			FontChineseLBSet;
		else
			FontChineseLBClr;

		CMD1 = 0;
		CMD2 = 0;
	}
	// 解除汉字模式
	else if( (CMD1 == 0x1c) && (CMD2 ==  0x2e) )
	{
		CMD1 = 0;
		CMD2 = 0;
	}
	// 用户自定义中文字符
	else if( (CMD1 == 0x1c) && (CMD2 ==  0x32) )
	{
		if(NumBufferReceive()<34)
			return;

		for(c8c1=0; c8c1<34;c8c1++)
			c8c2 = ReceiveBufferData();//GetBufferReceiveData();

		CMD1 = 0;
		CMD2 = 0;
	}
	// 取消自定义汉字
	else if( (CMD1 == 0x1c) && (CMD2 ==  0x3f) )
	{
		if(NumBufferReceive()<2)
			return;

		c8c1 = ReceiveBufferData();//GetBufferReceiveData();
		c8c2 = ReceiveBufferData();//GetBufferReceiveData();

		CMD1 = 0;
		CMD2 = 0;
	}
	// 纵向半角两字符并列打印
	else if( (CMD1 == 0x1c) && (CMD2 ==  0x44) )
	{
		CMD1 = 0;
		CMD2 = 0;
	}
	// 设定纵向打印
	else if( (CMD1 == 0x1c) && (CMD2 ==  0x4a) )
	{
		CMD1 = 0;
		CMD2 = 0;
	}
	// 设定横向打印
	else if( (CMD1 == 0x1c) && (CMD2 ==  0x4b) )
	{
		CMD1 = 0;
		CMD2 = 0;
	}
	// 左旋
	else if( (CMD1 == 0x1c) && (CMD2 ==  0x4c) )
	{
		CMD1 = 0;
		CMD2 = 0;
	}
	// 取消旋转
	else if( (CMD1 == 0x1c) && (CMD2 ==  0x4e) )
	{
		CMD1 = 0;
		CMD2 = 0;
	}
	// 右旋
	else if( (CMD1 == 0x1c) && (CMD2 ==  0x52) )
	{
		CMD1 = 0;
		CMD2 = 0;
	}
	// 设定汉字左右间距
	else if( (CMD1 == 0x1c) && (CMD2 ==  0x53) )
	{
		if(NumBufferReceive()<2)
			return;

		c8c1 = ReceiveBufferData();//GetBufferReceiveData();
		c8c2 = ReceiveBufferData();//GetBufferReceiveData();

		CMD1 = 0;
		CMD2 = 0;
	}
	// 设定半角字符字间距
	else if( (CMD1 == 0x1c) && (CMD2 ==  0x54) )
	{
		CMD1 = 0;
		CMD2 = 0;
	}
	// 设定半角字符间距调整
	else if( (CMD1 == 0x1c) && (CMD2 ==  0x55) )
	{
		if(FlagDataWait = 0)
		{
			if(NumBufferReceive()<1)
				return;

			c8c1 = ReceiveBufferData();//GetBufferReceiveData();
			SendString("设置打印机驱动名称\r\n");
			SendString("驱动名称长度\r\n");
			SendString("len:");
			OutputByteData((c8c1/100)%10+'0'); // <= 255
			OutputByteData((c8c1/10)%10+'0');
			OutputByteData((c8c1/1)%10+'0');
			SendString("\r\n");

			FlagDataWait++;
		}
		if(FlagDataWait == 1)
		{
			if(NumBufferReceive()<c8c1)
				return;

			c8c2 = 0;
			while(c8c2<c8c1)
			{
				c8c3=ReceiveBufferData();//GetBufferReceiveData();//DriverName[c8c2++]=GetBufferReceiveData();
			}
			SendString("打印机名称:");
		//	SendString(DriverName);
			SendString("\r\n");
			if(c8c1&&(c8c1<20))
			{
				for(c8c2=c8c1;c8c2<20;c8c2++)
				{
					;//DriverName[c8c2]=0;
				}
				;//WriteUSBName();
				MainPowerOff//NVIC_SystemReset();
			}

			FlagDataWait = 0;
			CMD1 = 0;
			CMD2 = 0;
		}
	}
	// 取消半角字符间距调整
	else if( (CMD1 == 0x1c) && (CMD2 ==  0x56) )
	{
		if(NumBufferReceive()<2)
			return;

		c8c1 = ReceiveBufferData();//GetBufferReceiveData();

		CMD1 = 0;
		CMD2 = 0;
	}
	// 设定/解除四倍角打印
	else if( (CMD1 == 0x1c) && (CMD2 ==  0x57) )
	{
		if(NumBufferReceive()<1)
				return;

		c8c1 = ReceiveBufferData();//GetBufferReceiveData();
		if(c8c1&0x01)
		{
			FontChineseDHSet;
			FontChineseDWSet;
		}
		else
		{
			FontChineseDHClr;
			FontChineseDWClr;
		}

		CMD1 = 0;
		CMD2 = 0;
	}
	// 选择汉字点阵和汉字高度
	else if( (CMD1 == 0x1c) && (CMD2 ==  0x59) )
	{
		CMD1 = 0;
		CMD2 = 0;
	}
	// 设定汉字基准线
	else if( (CMD1 == 0x1c) && (CMD2 ==  0x62) )
	{
		CMD1 = 0;
		CMD2 = 0;
	}
	// 设定汉字水平移动距离
	else if( (CMD1 == 0x1c) && (CMD2 ==  0x63) )
	{
		CMD1 = 0;
		CMD2 = 0;
	}
	// 选择汉字字体
	else if( (CMD1 == 0x1c) && (CMD2 ==  0x6b) )
	{
		CMD1 = 0;
		CMD2 = 0;
	}
	// 设定汉字上/下标打印
	else if( (CMD1 == 0x1c) && (CMD2 ==  0x72) )
	{
		CMD1 = 0;
		CMD2 = 0;
	}
	// 设定/解除表格符连线
	else if( (CMD1 == 0x1c) && (CMD2 ==  0x76) )
	{
		CMD1 = 0;
		CMD2 = 0;
	}
	// 设定解除高速打印
	else if( (CMD1 == 0x1c) && (CMD2 ==  0x78) )
	{
		CMD1 = 0;
		CMD2 = 0;
	}
	// **********************************	双字节指令GS *
	// 逻辑定位到打印起始位置
	else if( (CMD1 == 0x1d) && (CMD2 ==  0x0c) )
	{
		SendString("StepPageCount:");
		OutputByteData((StepPageCount/1000)%10+'0');
		OutputByteData((StepPageCount/100)%10+'0');
		OutputByteData((StepPageCount/10)%10+'0');
		OutputByteData((StepPageCount/1)%10+'0');
		SendString("\r\n");
		//sangzijinm=1;

		CMD1 = 0;
		CMD2 = 0;
	}
	// 衍生多条命令
	else if( (CMD1 == 0x1d) && (CMD2 ==  0x28) )
	{
		if(FlagDataWait == 0)
		{
			if(NumBufferReceive()<1)
				return;

			c8c1 = ReceiveBufferData();//GetBufferReceiveData();

			if(c8c1 == 0x41) // 测试命令
				FlagDataWait = 10;
			else if(c8c1 == 0x46) // 设置黑标定位偏移量
				FlagDataWait = 20;
			else
			{
				FlagDataWait++;
			}
		}
		if(FlagDataWait == 10)
		{
			if(NumBufferReceive()<4)
				return;

			c8c1 = ReceiveBufferData();//GetBufferReceiveData();
			c8c2 = ReceiveBufferData();//GetBufferReceiveData();
			c8c3 = ReceiveBufferData();//GetBufferReceiveData();
			c8c4 = ReceiveBufferData();//GetBufferReceiveData();

			if( (c8c1 == 0x02) && (c8c2 ==  0x00) && (c8c3 == 0x01) && (c8c4 == 0x01) )
				FlagManager = FlagManagerHexPrint;

			FlagDataWait = 1;
		}
		if(FlagDataWait == 20)	// 1D 28 46 pL pH a m nL nH
		{
			if(NumBufferReceive()<6)
				return;

			c8c1 = ReceiveBufferData();//GetBufferReceiveData();
			c8c2 = ReceiveBufferData();//GetBufferReceiveData();
			c8c3 = ReceiveBufferData();//GetBufferReceiveData();
			c8c4 = ReceiveBufferData();//GetBufferReceiveData();
			c8c5 = ReceiveBufferData();//GetBufferReceiveData();
			c8c6 = ReceiveBufferData();//GetBufferReceiveData();
			if( (c8c1 == 0x04) && (c8c2 == 0x00) )
			{
				if(c8c3 == 1)		// PaperPositionFirstPrint
				{
					PaperPositionFirstPrint = c8c6;
					PaperPositionFirstPrint <<= 8;
					PaperPositionFirstPrint += c8c5;
					if(PaperPositionFirstPrint > 1700)
					{
						PaperPositionFirstPrint = 1700;
					}
				}
				else if(c8c3 == 2)	// PaperPositionCut
				{
					PaperPositionCut = c8c6;
					PaperPositionCut <<= 8;
					PaperPositionCut += c8c5;
					if(PaperPositionFirstPrint > 1700)
					{
						PaperPositionFirstPrint = 1700;
					}
				}
				else if(c8c3 == 3)	// PaperPositionOut
				{
					PaperPositionOut = c8c6;
					PaperPositionOut <<= 8;
					PaperPositionOut += c8c5;
				}
			}

			FlagDataWait = 1;
		}
		if(FlagDataWait == 1)
		{
			FlagDataWait = 0;
			CMD1 = 0;
			CMD2 = 0;
		}
	}
	// 反白打印模式
	else if( (CMD1 == 0x1d) && (CMD2 ==  0x42) )
	{
		CMD1 = 0;
		CMD2 = 0;
	}
#endif
	// 逻辑定位到撕切纸位置
	else if( (CMD1 == 0x1d) && (CMD2 ==  0x56) )
	{
		if(FlagDataWait == 0)
		{
			SendCmd();

			if(ReceiveBufferSize()<1)
				return;
			if(PAPRuning)
				return;

			c8c1 = ReceiveBufferData();//GetBufferReceiveData();
			SendString("1d5653\r\n");

			FlagDataWait++;
		}
		#if 0 // @@@@@Alke
				if(PrinttingMode==PrintForward)
				{
					/*如果是前进纸，撕纸位置命令不执行*/
					DriverHead=0;
					SangShu=1;
					// 0x00,0x30:当前位置全切纸		0x01,0x31:当前位置半切纸		0x01,0x31
					// 0x41+n:偏移切纸位置n步全切纸	0x41+n:偏移切纸位置n步半切纸
					if(c8c1 == 0x53)
					{
						SendString("页结束命令退纸........................................................................\r\n");
						if(PrinttingMode==PrintForward)
						{
							PAPCutPosition();
						}
					}
					//
					SendString("不是最后一张纸\r\n");
					if(NumBufferReceive()<100)
					{
						 cBufferReceiveRead 	= 0;
						cBufferReceiveWrite	= 0;
					}
					backwardmotorcount=0;
					SangYi2=1;
					PositionFlag=0;
					DriverHead=0;
					PrintDataContinu=0;
				}
				//后进纸执行
				else
				{
					/*判断纸张是否已经走出*/

					PrintDataContinu=0;
					/*判断是否是最后一张纸*/
					if(NumBufferReceive()>10)
					{
						/*如果不是最后一张纸*/
						SendString("不是最后一张纸\r\n");
						goto tiaochuqu;
					}
					else
					{
						SendString("不是最后一张纸2\r\n");
						DelayTime2mS(5);// 100	// @20220516 Alke 开始结束加快纸张进退速度
						if(NumBufferReceive()>10)
						{
							goto tiaochuqu;
						}

						SendString("不是最后一张纸3\r\n");
						DelayTime2mS(5);// 100	// @20220516 Alke 开始结束加快纸张进退速度
						if(NumBufferReceive()>10)
						{
							goto tiaochuqu;
						}
						SendString("不是最后一张纸4\r\n");
						DelayTime2mS(5);// 100	// @20220516 Alke 开始结束加快纸张进退速度
						if(NumBufferReceive()>10)
						{
							goto tiaochuqu;
						}
						SendString("不是最后一张纸5\r\n");
						DelayTime2mS(5);// 100	// @20220516 Alke 开始结束加快纸张进退速度
						if(NumBufferReceive()>10)
						{
							goto tiaochuqu;
						}

					}

					SendString("最后一张纸\r\n");
					/*如果后面传感器无纸*/
					DelayTime2mS(5);// 10	// @20220516 Alke 开始结束加快纸张进退速度
					/*判断纸张是否走出, 如果前面传感器已经没有检测到纸张*/

						if(CheckPaperON(4)==0)
						{
							/*这种情况是后进纸，纸张走到最后一张*/
							/*将纸张全部走出*/
							PAPMoveStep =72*10;
							PAPFeedStart(0);
							while(PAPCRT == 0x00);
							PrinttingMode=PrintStandby;
							while(1)
							{
								if((CheckPaperON(1)==0)&&(CheckPaperON(4)==0))
								{
									DelayTime2mS(5);// 20	// @20220516 Alke 开始结束加快纸张进退速度
									if((CheckPaperON(1)==0)&&(CheckPaperON(4)==0))
									{
										SendString("前后纸张都已经退出\r\n");
										break;
									}
								}
							}
							while(1)
							{
								if((CheckPaperON(1)==0)&&(CheckPaperON(4)==0))
								{
									DelayTime2mS(10);// 100	// @20220516 Alke 开始结束加快纸张进退速度
									if((CheckPaperON(1)==0)&&(CheckPaperON(4)==0))
									{
										DelayTime2mS(10);// 100	// @20220516 Alke 开始结束加快纸张进退速度
										if((CheckPaperON(1)==0)&&(CheckPaperON(4)==0))
										{
											break;
										}
									}
								}
							}

							/*判断前面传感器纸张是否取出*/
							SendString("后进纸最后一页1d5653\r\n");
							LastPaperBack=1;//驱动 连续打印

							/*如果后面传感器无纸*/
							if(CheckPaperON(4)==0)
							{
								SendString("后进纸将纸张全部走出----\r\n");
								PAPMoveStep = 72*10;
								PAPFeedStart(0);
								while(PAPCRT == 0x00);
								backwardmotorcount=0;
								SangYi2=1;
								PositionFlag=0;
								PrinttingMode=PrintStandby;

							}

							/*这种情况已经是最后一页，后面会跟走到撕纸位置，由于纸张已经走出，取命令扔掉*/
							WaitForData(1);
							CMD1 = *(BufferReceive+cBufferReceiveRead);
							cBufferReceiveReadADD();

							WaitForData(1);
							CMD1 = *(BufferReceive+cBufferReceiveRead);
							cBufferReceiveReadADD();
							WaitForData(1);
							CMD1 = *(BufferReceive+cBufferReceiveRead);
							cBufferReceiveReadADD();
							SendString("打印到最后一页将走到撕纸位置指令取出\r\n");

							backwardmotorcount=0;
							SangYi2=1;
							/*对位回纸取消*/
							PositionFlag=0;
							SendString("定位到打印起始位置功能打开\r\n");
							PrintDataContinu=0;//这种情况需要定位打印起始位置
							/*将撕纸位置指令取出*/

							/*检测纸张*/
							PaperTest();
							PrintDataContinu=0;//这种情况需要定位打印起始位置
							SPFlag=1;//还没有处理走到撕纸位置指令
						}
						else
						{
							/*最后一页后面有纸*/
							/*走到撕纸位置打开*/
							/*后面会跟走到撕纸位置命名，在最后处理*/
							DriverHead=1;
						}
						goto ennd;
		tiaochuqu:
					SendString("不是最后一张纸\r\n");
					/*判断是否纸张已经走出*/
					if(CheckPaperON(4)==0)
					{
						DelayTime2mS(5);
						if(CheckPaperON(4)==0)
						{
							SendString("纸张已经走过后胶辊\r\n");
							{
								SendString("后进纸将纸张全部走出----\r\n");
								PAPMoveStep = 72*10;
								PAPFeedStart(0);
								while(PAPCRT == 0x00);
								PrinttingMode=PrintStandby;
							}
							while(1)
							{
								if((CheckPaperON(1)==0)&&(CheckPaperON(4)==0))
								{
									DelayTime2mS(5);// 100	// @20220516 Alke 开始结束加快纸张进退速度
									if((CheckPaperON(1)==0)&&(CheckPaperON(4)==0))
									{
										DelayTime2mS(5);// 100	// @20220516 Alke 开始结束加快纸张进退速度
										if((CheckPaperON(1)==0)&&(CheckPaperON(4)==0))
										{
											break;
										}
									}
								}
							}
							backwardmotorcount=0;
							//无纸状态
							SangYi2=1;
							// 对好位后不需要退纸
							PositionFlag=0;
							SendString("定位到打印起始位置功能打开\r\n");
							// 打印起始位置对位打开
							PrintDataContinu=0;
							// 走到撕纸位置指令还没有处理，这种情况撕纸位置指令不处理
							// 这种情况如果是连续打印，则没有走到撕纸位置指令，否则进纸后处理的第一条指令是撕纸位置指令
							DriverHead=0;
						}
					}
					else
					{
						/*纸张在后胶辊*/
						SendString("纸张在后胶辊\r\n");
						backwardmotorcount=0;
						//有纸状态
						SangYi2=0;
						/*这种情况如果连续打印，后面会有走到撕纸位置命令*/
						/*无论哪种情况，走到撕纸位置命令都不处理*/
						DriverHead=0;
						/*如果是文件打印，后面会跟走到打印起始位置，这种情况不处理*/
						/*如果是连续打印，不会有走到起始位置*/
						PrintDataContinu=1;
					}
		ennd:
					SangShu=0;
				}
				StopRun=0;
		#endif

		CMD1 = 0;
		CMD2 = 0;
	}
#if 0
	// 传送状态
	else if( (CMD1 == 0x1d) && (CMD2 ==  0x72) )
	{
		CMD1 = 0;
		CMD2 = 0;
	}
	// 设置在线恢复等待时间
	else if( (CMD1 == 0x1d) && (CMD2 ==  0x7a) )
	{
		CMD1 = 0;
		CMD2 = 0;
	}
	// 汉字打印2 byte-1
	else if(	(CMD1 >= 0xa1) && (CMD1 <= 0xa9) && 	(CMD2 >= 0xa1) && (CMD2 <= 0xfe) 	)
	{
		Chinese2ByteAddressCalculate:
		WritePRTBuffer(CMD1, CMD2, 0, 0);

		CMD1 = 0;
		CMD2 = 0;
	}
	// 汉字打印2 byte-2
	else if( (CMD1 >= 0xa8) && (CMD1 <= 0xa9) && (CMD2 >= 0x40) && (CMD2 <= 0xa0) )
	{
		goto  Chinese2ByteAddressCalculate;
	}
	// 汉字打印2 byte-3
	else if( (CMD1 >= 0xb0) && (CMD1 <= 0xf7) && (CMD2 >= 0xa1) && (CMD2 <= 0xfe) )
	{
		goto  Chinese2ByteAddressCalculate;
	}
	// 汉字打印2 byte-4
	else if( (CMD1 >= 0x81) && (CMD1 <= 0xa0) && (CMD2 >= 0x40) && (CMD2 <= 0xfe) )
	{
		goto  Chinese2ByteAddressCalculate;
	}
	// 汉字打印2 byte-5
	else if( (CMD1 >= 0xaa) && (CMD1 <= 0xfe) && (CMD2 >= 0x40) && (CMD2 <= 0xa0) )
	{
		goto  Chinese2ByteAddressCalculate;
	}
	// 汉字打印4 byte-81-1/2
	else if( (CMD1 == 0x81) && (CMD2 ==  0x39) )
	{
		Chinese4ByteWaitFor2Byte:
		if(NumBufferReceive()<2)
			return;

		c8c1 = CMD1;
		c8c2 = CMD2;
		c8c3 = ReceiveBufferData();//GetBufferReceiveData();
		c8c4 = ReceiveBufferData();//GetBufferReceiveData();
		// 汉字打印4 byte-81-1
		if( (c8c1 == 0x81) && (c8c2 == 0x39) && (c8c3 == 0xee) && (c8c4 == 0x39) )
		{
			Chinese4ByteAddressCalculate:
			WritePRTBuffer(c8c1, c8c2, c8c3, c8c4);
		}
		// 汉字打印4 byte-81-2
		else if( (c8c1 == 0x81) && (c8c2 ==  0x39) && (c8c3 >= 0xef) && (c8c3 <= 0xfe) && (c8c4 >= 0x30) && (c8c4 <= 0x39) )
			goto Chinese4ByteAddressCalculate;
		// 汉字打印4 byte-82-1
		else if( (c8c1 == 0x82) && (c8c2 >= 0x30) && (c8c2 <= 0x34) && (c8c3 >= 0x81) && (c8c3 <= 0xfe) && (c8c4 >= 0x30) && (c8c4 <= 0x39) )
			goto Chinese4ByteAddressCalculate;
		// 汉字打印4 byte-82-2
		else if( (c8c1 == 0x82) && (c8c2 ==  0x35) && (c8c3 >= 0x81) && (c8c3 <= 0x86) && (c8c4 >= 0x30) && (c8c4 <= 0x39) )
			goto Chinese4ByteAddressCalculate;
		// 汉字打印4 byte-82-3
		else if( (c8c1 == 0x82) && (c8c2 ==  0x35) && (c8c3 == 0x87) && (c8c4 >= 0x30) && (c8c4 <= 0x38) )
			goto Chinese4ByteAddressCalculate;

		CMD1 = 0;
		CMD2 = 0;
	}
	// 汉字打印4 byte-82-1/2/3
	else if( (CMD1 == 0x82) && (CMD2 >= 0x30) && (CMD2 <= 0x35) )
	{
		goto  Chinese4ByteWaitFor2Byte;
	}
#endif
	// not all the commond, get next byte to fit commond
	else
	{
		GetNextDataCMD:
		if(ReceiveBufferSize())
		{
		//printf("GetNextDataCMD: <<<from USB---");//\r\n
			CMD1 = CMD2;
			CMD2 = ReceiveBufferData();
		}
		#if(!DebugWifi)
		if(wifi_is_on())
		{
			if(ReceiveBufferSizeWifi())
			{
			printf("GetNextDataCMD: <<<from WIFI---");//\r\n
				CMD1 = CMD2;
				CMD2 = ReceiveBufferDataWifi();
			//user_put_u8hex(CMD2);
			}
		}
		#endif
	}
}
#endif

/**************************************************************************************************************
	Driver.c	Driver.h
**************************************************************************************************************/

