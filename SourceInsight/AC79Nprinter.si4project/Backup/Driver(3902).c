


//#define Driver 0
/**************************************************************************************************************
	Driver.c	Driver.h
**************************************************************************************************************/
#include "Driver.h"
#include "UserInterface.H"



// @20230601 �����µĳ���ܹ����˺���Ϊ�յ����ݺ�Ĵ����¼�
// �˺�������һ���ֽ�һ���ֽڽ��д����˹����в�������ѭ����ֻ��һ�ι����������ñ�־λ

// ���Դ�����
/*void CmdPrint0d(void) // ���ֽ�����û�к�������
{
	if(	((cBufferPrintLineWrite+1) == cBufferPrintLineRead)	||
		((cBufferPrintLineWrite == (BufferPrintLineNum-1))&&(cBufferPrintLineRead == 0))	)
	{
		printf("Error! BufferPrint[] overflowl! \n");
		;//��ӡ���������Ѿ����ˣ���ʱ����������������ϲ��ܳ���������
	}
	else
	{
		if(cBufferPrintLineWrite == (BufferPrintLineNum-1))
			cBufferPrintLineWrite = 0;
		else
			cBufferPrintLineWrite++;
	}
}*/

// �������ֽ�Ϊ1b��������
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
			Command.State = 1; // ������Ч
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
	case 13: // ����ͼ�����ݿ�ʼ��ż��������
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

	case 20: // ���ͼ�����ݣ�Size��8λ
		Command.Size = data;
		Command.State++;
		break;
	case 21: // ����ͼ�����ݣ�Size��8λ
		c16c1 = data;
		c16c1 <<= 8;
		Command.Size += c16c1;
		cBufferPrintDataWrite = 0;
		Command.State++;
		break;
	case 22: // ����ͼ�����ݿ�ʼ������������
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
	case 23: // ����ͼ�����ݿ�ʼ��ż��������
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

	case 30: // ͼ�����������㴦��
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
			;//��ӡ���������Ѿ����ˣ���ʱ����������������ϲ��ܳ���������
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
	//cBufferPrintDataWrite = 0; // ��ȥʱ�Ѿ��պ�Ϊ0

	// ͨ��Ĭ��Ϊ16����û��0D�����л���һ��ʱ�����β������ӣ�ʹ����ֽ����
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

// ��ֽ����
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

// �������ֽڽ����������
//void InstructionParser(volatile unsigned char data)//�������
// �˺���Ϊ���յ�һ���ֽ����ݽ��н�������ǰ����Ԥ���жϴ�ӡ�������пհ׿���д�룬�������USB/WIFI/BL����æֹͣ��������
void DealData(unsigned char data) //�������
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

	// ���ǵ�1���ֽڵ��жϣ�Ȼ����ܽ�������ķ��ദ��
	default://eCmdInit
		if(data == 0x0d) // ���ֽ�����ֱ�Ӵ���û�к�������
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


#if 0 // �Ϸ��������ʽ�����Խ��
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
	// �س�
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
	// **********************************	˫�ֽ�ָ��DLE EOT
	// û�ж�ȡ�����ֽ�ʱ�����ؼ�����ȡ��һ�ֽ�
	// ���ô�ӡ�ٶ����ȵ�
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
	// 1B 2A �� 0D ��ӡ�������2��3״̬�������л���Ӧ����CRGNowTimePosition CRGStopTimePosition�����˶��Ӽ���������ԭ��
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
			// ����Ҫ�еȺţ�Ԥ��λ����ɺ������ȵ�
			if((CRGNowTimePosition <= c16c1)) // do not care the one print direction
			{
				CRGStopTimePosition = c16c2;
				CRGRightSet
				FlagSystemPrintSet
				CRGCurrent(2);

				FlagDataWait += 2;// �����ȴ�����Ԥ��λ�õĲ���
			}
			else if(CRGNowTimePosition >= c16c2)
			{
				if(TMPOneDir || CMDOneDir)
				{
					CRGStopTimePosition = c16c1;
					CRGLeftSet
					FlagSystemPrintClr
					CRGCurrent(2);
					FlagDataWait++;// ��ҪԤ�ȵ���׼��λ��
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
					FlagDataWait++;// ��ҪԤ�ȵ���׼��λ��
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
	// ִ��n/180Ӣ��˳������ֽ		SM880: n/144
	else if( (CMD1 == 0x1b) && (CMD2 == 0x4a) )
	{
		// �ȴ�����
		if(FlagDataWait == 0)
		{
			SendCmd();

			//printf("DataCMD: 0x1b 0x4a->0 \r\n");
			if(ReceiveBufferSize()<1)
				return;

			PAPMoveStep = ReceiveBufferData();//GetBufferReceiveData();
			PAPMoveStep *= 2;
			// ��Ҫ���ҳͷ�Ƿ�ӽ�����λ�ã��Դ���ȷ���Ƿ���Ҫ��ӡͷ�����ƶ���ֽ��չƽ�����ͽ��뽺��ʱ��ֽ����
			// �˴�����������ǰ��ֽʱָ���Ǻ��潺�������ֽʱָ��ǰ�潺��
			// �����ǰ��ֽȱֽ����Ҫֹͣ��ӡ�����ҽ�ֽ���˳�
			FlagDataWait = 10;
		}
	#if 1
		// ����ҳ����������ҳ������Ҫ��ֽ�����ȣ��ж��Ƿ���Ҫ�ֳ�չƽ����ֽ�Ȳ���������״̬�����󣬿����ٴν����״̬�����жϣ�
		if(FlagDataWait == 10)
		{
			// ǰ��ֽģʽ��ֽ��ҳͷ�ӽ��󽺹������ߺ��ֽģʽ��ֽ��ҳͷ�ӽ�ǰ����������Ҫ�ֳ������˶�չƽֽ��
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
			// ǰ��ֽģʽ��ֽ��ҳβԽ����ӡλ�ã���ҳ��ӡ������ֱ����ֽ�����ֽģʽ������ֽ����
			// ҳ���������λ�л������У������ǰ�����ﴫ����ȷʵ��ֽʱ�Զ�����ҳ��������װֽ��ظ�Ĭ��ҳ��
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
		// ����ҳ����������ҳ������Ҫ��ֽ�����ȣ��ж��Ƿ���Ҫ�ֳ�չƽ����ֽ�Ȳ���������״̬�����󣬿����ٴν����״̬�����жϣ�
		if(FlagDataWait == 10)
		{
			if(PaperState&0xf0 == 0x10)
			{
				// ǰ��ֽģʽ��ֽ��ҳͷ�ӽ��󽺹�����Ҫ�ֳ������˶�չƽֽ��
				if(PaperCount < (StepFrontToPrint+StepBackToPrint-StepHeadWide)&&((PaperCount+PAPMoveStep) > (StepFrontToPrint+StepBackToPrint-StepHeadWide)))
				{
					c16c1 = (PaperCount+PAPMoveStep) - (StepFrontToPrint+StepBackToPrint-StepHeadWide);
					c16c2 = c16c1;
					PAPMoveStep = (StepFrontToPrint+StepBackToPrint-StepHeadWide)-PaperCount;

					PAPBackSet
					PAPCurrent(2);

					FlagDataWait = 20;
				}
				// ǰ��ֽģʽ��ֽ��ҳβԽ����ӡλ�ã���ҳ��ӡ������ֱ����ֽ
				// ҳ���������λ�л������У������ǰ�ﴫ�����Զ�����
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
				// ���ֽģʽ��ֽ��ҳͷ�ӽ��󽺹�����Ҫ�ֳ������˶�չƽֽ��
				if(PaperCount < (StepFrontToPrint+StepBackToPrint-StepHeadWide)&&((PaperCount+PAPMoveStep) > (StepFrontToPrint+StepBackToPrint-StepHeadWide)))if
				{
					c16c1 = (PaperCount+PAPMoveStep) - (StepFrontToPrint+StepBackToPrint-StepHeadWide);
					c16c2 = c16c1;
					PAPMoveStep = (StepFrontToPrint+StepBackToPrint-StepHeadWide)-PaperCount;

					PAPFeedSet
					PAPCurrent(2);

					FlagDataWait = 20;
				}
				// ���ֽֽ��Ҳ��Ҫ��ǰ�߳�����������ֽ
				// ���ֽģʽ��ֽ��ҳβ(��Ҫ֪��Ĭ��ҳ��)Խ����ӡλ�ã���ҳ��ӡ�����������ʱ����û��ֽ�ţ�˵�������һ�ţ�������ǰ�ͳ�ȫ��ֽ��
				// �����λ�л������У����j��⵽����ͺ��ﶼ��ֽ��˵�������һ�ţ������ʵ���趨ҳ��
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
		// ������ֽ
		if(FlagDataWait == 11)
		{
			if(PAPRuning)
				return;

			FlagDataWait = 0;
			CMD1 = 0;
			CMD2 = 0;
		}

		// �ȴ�ҳͷ�ߵ�����������Ҫ�ֳ����չƽֽ��
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
			// �˶��������ϴ���ͬ
		/*	if(PaperState&0xf0 == 0x10)
				PAPBackSet
			else if(PaperState&0xf0 == 0x20)
				PAPFeedSet	*/
			PAPCurrent(2);

			FlagDataWait = 11;
		}

		// ǰ��ֽģʽ��ҳβ�Ѿ�Խ����ӡλ�ã���Ҫ��ֽ��ȥ
		// ���ֽģʽ����������Ҫ��ֽ����(����˺ֽλ��ʱ������ӡ��Ҫ�˵���ӡλ��)
		// �ȴ�ҳβ�˻ص�����������Ҫ�ֳ����չƽֽ��
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
			// �˶��������ϴ���ͬ
			PAPCurrent(2);

			FlagDataWait = 11;
		}
	}
	// �趨��/˫���ӡ
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
		// �ȴ�����
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
	/*��������ٶ�*/ // �Ѿ�ȷ���ٶȣ��������ָߵ���
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
	//����productid
	else if( (CMD1 == 0x1b) && (CMD2 ==  0x80) )
	{
		if(NumBufferReceive()<2)
			return;

		c8c1 = ReceiveBufferData();//GetBufferReceiveData();
		c16c1 = ReceiveBufferData();//GetBufferReceiveData();
		c16c1 <<= 8;
		c16c1 |= c8c1;
	//	ProductID = c16c1; // @@@@@
		SendString("ProductID����Ϊ:");
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
	/*������ѹ*/
	else if( (CMD1 == 0x1b) && (CMD2 ==  0x84) )
	{
	//	TestSensorBack(); // @@@@@
		CMD1 = 0;
		CMD2 = 0;
	}
	/*��ǰ��ֽһֱ��ֽ����*/
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
	// �߼���λ����ӡ��ʼλ��������λ�����Ҽ�¼���ｺ���������������ٽ�����ʱ��������ѹֽ����
	// ����ҳ��
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
	/*���ͳ��ж���Ϣֽ�ŵ�ѹ��Ϣ*/
	else if( (CMD1 == 0x1b) && (CMD2 ==  0x90) )
	{
	;//	SendNeedleResister(); // @@@@@ Alke
	}
	/*���ô�ӡ������*/
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
				/*�����ܼ�100���ַ�������Ĳ���ո�*/
				for(c8c3 = 0; c8c3 < 100; c8c3++)//ͼ��ָ�����ݽ���
				{
					c8c4 = ReceiveBufferData();//GetBufferReceiveData();//*(bufferLogo+c8c3) = GetBufferReceiveData();
				}
				/*д���ֿ�����*/
			//	WriteLogo();

			//	BuzzerHead(1,5);
			}

			FlagDataWait = 0;
			CMD1=0;
			CMD2=0;
		}
	}
	/*���ô�ӡ��Ѱ�ߴ�������ѹֵ*/
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
			/*ת����ΪADC��ֵ*/
			if(LeftVoltagePaper>200||LeftVoltagePaper<100)
			{
				SendString("left���õĵ�ѹ������Χ����ʧ��\r\n");
				LeftVoltagePaper=130;
				LeftVoltagePaper=LeftVoltagePaper*4096/330;
			}
			else
			{
				SendString("left���óɹ�\r\n");
				SendString("��ѹֵ:");
				OutputByteData((LeftVoltagePaper/1000)%10+'0');
				OutputByteData((LeftVoltagePaper/100)%10+'0');
				OutputByteData((LeftVoltagePaper/10)%10+'0');
				OutputByteData((LeftVoltagePaper/1)%10+'0');
				SendString("\r\n");
				LeftVoltagePaper=LeftVoltagePaper*4096/330;
				SendString("leftADC��ѹֵ:");
				OutputByteData((LeftVoltagePaper/1000)%10+'0');
				OutputByteData((LeftVoltagePaper/100)%10+'0');
				OutputByteData((LeftVoltagePaper/10)%10+'0');
				OutputByteData((LeftVoltagePaper/1)%10+'0');
				SendString("\r\n");
			}

			if(RightVoltagePaper>200||RightVoltagePaper<100)
			{
				SendString("right���õĵ�ѹ������Χ����ʧ��\r\n");
				RightVoltagePaper=130;
				RightVoltagePaper=RightVoltagePaper*4096/330;
			}
			else
			{
				SendString("right���óɹ�\r\n");
				SendString("��ѹֵ:");
				OutputByteData((RightVoltagePaper/1000)%10+'0');
				OutputByteData((RightVoltagePaper/100)%10+'0');
				OutputByteData((RightVoltagePaper/10)%10+'0');
				OutputByteData((RightVoltagePaper/1)%10+'0');
				SendString("\r\n");
				RightVoltagePaper=RightVoltagePaper*4096/330;
				SendString("rightADC��ѹֵ:");
				OutputByteData((RightVoltagePaper/1000)%10+'0');
				OutputByteData((RightVoltagePaper/100)%10+'0');
				OutputByteData((RightVoltagePaper/10)%10+'0');
				OutputByteData((RightVoltagePaper/1)%10+'0');
				SendString("\r\n");
			}
			/*����flash*/
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

			SendString("������������\r\n");
			/*�����ݶ���*/
			//GtDataRead(13000, bufferVoltage, 4);
			LeftVoltagePaper=(((unsigned int)bufferVoltage[0])<<8)|bufferVoltage[1];
			RightVoltagePaper=(((unsigned int)bufferVoltage[2])<<8)|bufferVoltage[3];
			/*ת����Ϊ��ѹֵ*/
			Temp=330*LeftVoltagePaper/4096;
			if(Temp>200||Temp<100)
			{
				SendString("left���õĵ�ѹ������Χ����ʧ��\r\n");
				/*���������ΧĬ��ֵ1.3V*/
				LeftVoltagePaper=130*4096/330;
			}
			else
			{
				SendString("left:\r\n");
				SendString("��ѹֵ:");
				OutputByteData((Temp/1000)%10+'0');
				OutputByteData((Temp/100)%10+'0');
				OutputByteData((Temp/10)%10+'0');
				OutputByteData((Temp/1)%10+'0');
				SendString("\r\n");
			}

			Temp=330*RightVoltagePaper/4096;
			if(Temp>200||Temp<100)
			{
				SendString("right���õĵ�ѹ������Χ����ʧ��\r\n");
				/*���������ΧĬ��ֵ1.3V*/
				RightVoltagePaper=130*4096/330;
			}
			else
			{
				SendString("Right:\r\n");
				SendString("��ѹֵ:");
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
	/*Ѱ�߿���*/
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
	/*USB�Զ����ù���*/
	else if( (CMD1 == 0x1b) && (CMD2 ==  0x94) )
	{
		if(NumBufferReceive()<1)
			return;

		c8c1 = ReceiveBufferData();//GetBufferReceiveData();
	#if 0
		if(c8c1)
		{
			USBHandData=18;
			SendString("USB ���ֹ��ܿ���");
		}
		else
		{
			USBHandData=14;
			SendString("USB ���ֹ��ܹر�");
		}
		WriteUSBS();
		//GtDataWrite(25000, &USBHandData, 1);
		//NVIC_SystemReset();
	#endif
		CMD1 = 0;
		CMD2 = 0;
	}
	/*����U��*/
	else if( (CMD1 == 0x1b) && (CMD2 ==  0x95) )
	{
	#if 0
		SendString("����USB U��ģʽ\r\n");
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
	/*����Ѱ��*/
	else if( (CMD1 == 0x1b) && (CMD2 ==  0x96) )
	{
		/*��ֽ*/
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
	/*����Ϊ�����ٶ�*/
	else if( (CMD1 == 0x1b) && (CMD2 ==  0x98) ) // 1B98�����ã���J����
	{
		CMD1 = 0;
		CMD2 = 0;
	}
	/*����Ϊ����*/
	else if( (CMD1 == 0x1b) && (CMD2 ==  0x99) )
	{
		CMD1 = 0;
		CMD2 = 0;
	}
	/*���������*/
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
	/*˺ֽλ�ý�ֽ--------------------------------------------------------------*/
	else if( (CMD1 == 0x1b) && (CMD2 ==  0x9c) )
	{
		if(NumBufferReceive()<2)
			return;

		c8c1 = ReceiveBufferData();//GetBufferReceiveData();
		SendString("�ߵ�˺ֽλ��\r\n");
	#if 0
	//	PositionFlag=0;
		/*����Ҫ�ߵ�˺ֽλ��*/
	//	if(DriverHead==0)
		{
			SendString("��ִ���ߵ�˺ֽλ������----\r\n");
	//		return;
		}
	//	DriverHead=0;

		/*�жϺ����Ƿ������ݽ�������������ߵ�˺ֽλ�ã�����ǰ���Ѿ��������*/
		//���ִֽ��
		if(PrinttingMode==PrintBack)
		{
					SendString("�ߵ�˺ֽλ��\r\n");

					/*��ֽ��˺ֽλ��*/
					/*�����ǰλ���ڴ�ӡ��ʼλ��*/
					//if(PaperPositionMode==PrintPrintP)
					{
						/*������ӡ��ʼλ��*/
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
							/*��̬��¼��ǰ��ӡҳ��˺ֽλ��*/
							PAPFeedStart(0);
							while(PAPCRT == 0x00);
						}
						//��¼��ǰ˺ֽλ��
						SendString("˺ֽλ����ֽ\r\n");
						OutputByteData((PaperCutPs/1000)%10+'0');
						OutputByteData((PaperCutPs/100)%10+'0');
						OutputByteData((PaperCutPs/10)%10+'0');
						OutputByteData((PaperCutPs/1)%10+'0');
						SendString("\r\n");
						PaperPositionMode=PrintCut;

						/*�ж�����ֽ�Ƿ��Ѿ��߹��󽺹�*/
						if(0==CheckPaperON(4))
						{
							DelayTime2mS(5);// 5	// @20220516 Alke ��ʼ�����ӿ�ֽ�Ž����ٶ�
							if(0==CheckPaperON(4))
							{
								SendString("�ߵ�˺ֽλ�����ֽ�߳��󽺹�\r\n");
								/*��ֽ��ȫ���˳�*/
								SendString("��ֽ��ȫ���˳�\r\n");
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
								/*��λ��ֽ����ȡ��*/
								PositionFlag=0;
								DriverHead=0;
								/*��ӡ��ʼλ�ö�λ��*/
								PrintDataContinu=0;

							}
						}
						else
						{
							SendString("�ߵ�˺ֽλ�����ֽ�Ż��ں󽺹�\r\n");
							Position=1;
							backwardmotorcount=0;
							SangYi2=0;
							/*��λ��ֽ����ȡ��*/
							PositionFlag=0;
							DriverHead=0;
							/*��ӡ��ʼλ�ö�λ��*/
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
	/*˺ֽλ����ֽ*/
	else if( (CMD1 == 0x1b) && (CMD2 ==  0x9e) )
	{
		if(NumBufferReceive()<1)
			return;

		c8c1 = ReceiveBufferData();//GetBufferReceiveData();
		#if 0
				SendString("�ߵ�˺ֽλ��1\r\n");
				PositionFlag=0;
				WaitForData(1);

				/*������ӡʱ��*/
				if(DriverHead==0)
				{
					SendString("��ִ���ߵ�˺ֽλ������----\r\n");
					return;
				}
				DriverHead=0;

				//���ִֽ��
				if(PrinttingMode==PrintBack)
				{
					/*��ֽ��˺ֽλ��*/

					/*�����ǰλ���ڴ�ӡ��ʼλ��*/
					//if(PaperPositionMode==PrintPrintP)
					{
						/*�������ǳ�����*/
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
							SendString("˺ֽλ��:");
							OutputByteData((PaperCutPs/1000)%10+'0');
							OutputByteData((PaperCutPs/100)%10+'0');
							OutputByteData((PaperCutPs/10)%10+'0');
							OutputByteData((PaperCutPs/1)%10+'0');
							SendString("\r\n");
						}
						/*�ж�����ֽ�Ƿ��Ѿ��߹��󽺹�*/
						if(0==CheckPaperON(4))
						{
							DelayTime2mS(5);// 10	// @20220516 Alke ��ʼ�����ӿ�ֽ�Ž����ٶ�
							if(0==CheckPaperON(4))
							{
								SendString("�ߵ�˺ֽλ�����ֽ�߳��󽺹�\r\n");
								/*��ֽ��ȫ���˳�*/
								SendString("��ֽ��ȫ���˳�\r\n");
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
								SendString("ֽ���Ѿ��˳�\r\n");
								backwardmotorcount=0;
								SangYi2=1;
								/*��λ��ֽ����ȡ��*/
								PositionFlag=0;
								DriverHead=0;
								/*��ӡ��ʼλ�ö�λ��*/
								PrintDataContinu=0;

							}
						}
						else
						{
							SendString("�ߵ�˺ֽλ�����ֽ�Ż��ں󽺹�\r\n");
							Position=1;

							backwardmotorcount=0;
							SangYi2=0;
							/*��λ��ֽ����ȡ��*/
							PositionFlag=0;
							DriverHead=0;
							/*��ӡ��ʼλ�ö�λ��*/
							PrintDataContinu=0;
						}
					}
				}
		#endif
		CMD1=0;
		CMD2=0;
	}
#endif
	/*��ӡ��ʼλ�õ���+	�����ǰ��ֽ���ڵ����߼���ӡ��ʼλ�ú����ò���C1ȥ������λ��*/
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
				SendString("��ʼ��ӡλ��---1\r\n");
				DriverHead=1;//
				PositionFlag=1;

				if(PrintDataContinu==0)
				{
					SendString("��ʼ��ӡλ��---1\r\n");
					//���ִֽ��
					//if(PrinttingMode==PrintBack)
					{
						/*��ֽ��˺ֽλ��*/
						/*�����ǰλ����˺ֽλ�ã����������˺ֽλ�ã�����״̬�ڴ�ӡ��ʼλ��*/
						PaperPrintFirstLine=PrintPrintConst+CMD1;
						//if(PaperPositionMode==PrintCut)
						if(Position)//���˺ֽλ��
						{

								/*��ֽ��ӡ*/
								if(PrinttingMode==PrintBack)
								{
									Dirc=0;
									REStep=CMD1;

									if(Position)
									{
										PAPMoveStep = PaperCutPs-CMD1;
										PostionStepRe=PAPMoveStep;//��¼��ֽ����
										PAPFeedStart(1);
										while(PAPCRT == 0x00);
									}
									Position=0;
									SendString("������ӡ����е��϶\r\n");

									PAPMoveStep = 36;
									PAPFeedStart(1);
									while(PAPCRT == 0x00);

									PAPMoveStep = 36;
									PAPFeedStart(0);
									while(PAPCRT == 0x00);

									SendString("��ʼλ����ֽ-----------\r\n");

									SendString("PostionStepRe:");
									OutputByteData((PostionStepRe/1000)%10+'0');
									OutputByteData((PostionStepRe/100)%10+'0');
									OutputByteData((PostionStepRe/10)%10+'0');
									OutputByteData((PostionStepRe/1)%10+'0');
									SendString("\r\n");
								}

								//DelayTime2mS(u32 CountFor2mS);

								PaperPositionMode=PrintPrintP;
								SendString("��ӡ��ʼλ��\r\n");
								OutputByteData(((PaperPrintFirstLine)/1000)%10+'0');
								OutputByteData(((PaperPrintFirstLine)/100)%10+'0');
								OutputByteData(((PaperPrintFirstLine)/10)%10+'0');
								OutputByteData(((PaperPrintFirstLine)/1)%10+'0');
								SendString("\r\n");
						}
					}
					/*ǰ��ֽ*/
					 if(PrinttingMode==PrintForward)
					{
						/*�����ǰ���ֽ��ִ�н�ֽ����*/
						if(CMD1>0)
						{
							PAPMoveStep = CMD1;
							PAPFeedStart(0);
							while(PAPCRT == 0x00);
							SendString("ָ���޶���ӡ��ʼλ��\r\n");
						}

						PostionStepRe = PaperCutPs-CMD1;
					}
				}
				PrintDataContinu=0;
	#endif
	}
#if 0
	/*��ӡ��ʼλ��-*/
	else if( (CMD1 == 0x1b) && (CMD2 ==  0xa1) )
	{
		if(NumBufferReceive()<1)
			return;

		c8c1 = ReceiveBufferData();//GetBufferReceiveData();
	#if 0
				SendString("��ʼ��ӡλ��---\r\n");
				DriverHead=1;//
				PositionFlag=1;

				if(PrintDataContinu==0)
				{
					//���ִֽ��
					//if(PrinttingMode==PrintBack)
					{

						/*��ֽ����ӡ��ʼλ��*/

						/*�����ǰλ����˺ֽλ�ã����������˺ֽλ�ã�����״̬�ڴ�ӡ��ʼλ��*/
						PaperPrintFirstLine=PrintPrintConst-CMD1;
							Dirc=1;
									REStep=CMD1;
						//if(PaperPositionMode==PrintCut)
						if(Position)
						{
							/*��ֽ��ӡ*/

							if(PrinttingMode==PrintBack)
							{
								if(Position)
								{
									PAPMoveStep = PaperCutPs+CMD1;
									PostionStepRe=PAPMoveStep;//��¼��ֽ����
									PAPFeedStart(1);
									while(PAPCRT == 0x00);
									SendString("��ʼλ����ֽ-----------\r\n");
								}
								Position=0;
								SendString("������ӡ����е��϶\r\n");
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
							SendString("��ӡ��ʼλ��\r\n");
							OutputByteData(((PaperPrintFirstLine)/1000)%10+'0');
							OutputByteData(((PaperPrintFirstLine)/100)%10+'0');
							OutputByteData(((PaperPrintFirstLine)/10)%10+'0');
							OutputByteData(((PaperPrintFirstLine)/1)%10+'0');
							SendString("\r\n");
						}
					}
					/*ǰ��ֽ*/
					 if(PrinttingMode==PrintForward)
					{
						WaitForData(1);
						CMD1 = *(BufferReceive+cBufferReceiveRead);
						cBufferReceiveReadADD();
						/*�����ǰ���ֽ��ִ�н�ֽ����*/
						if(CMD1>0)
						{
							PAPMoveStep = CMD1;
							PAPFeedStart(1);
							while(PAPCRT == 0x00);
							SendString("ָ���޶���ӡ��ʼλ��\r\n");
						}
					}
				}
				PrintDataContinu=0;
	#endif
		CMD1=0;
		CMD2=0;
	}
	/*������ʱʱ��*/
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
	/*��λ�ü��㿪��*/
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
						SendString("������\r\n");
						HeadDelayCount=60;
					}
					SendString("�����ѱ�����\r\n");
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
						SendString("������\r\n");
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

	// �趨���б���
	else if( (CMD1 == 0x1c) && (CMD2 ==  0x0e) )
	{
		CMD1 = 0;
		CMD2 = 0;
	}
	// �趨��Ǻ���
	else if( (CMD1 == 0x1c) && (CMD2 ==  0x0f) )
	{
		CMD1 = 0;
		CMD2 = 0;
	}
	// �����Ǻ���
	else if( (CMD1 == 0x1c) && (CMD2 ==  0x12) )
	{
		CMD1 = 0;
		CMD2 = 0;
	}
	// ������б���
	else if( (CMD1 == 0x1c) && (CMD2 ==  0x14) )
	{
		CMD1 = 0;
		CMD2 = 0;
	}
	// ���ִ�ӡģʽ���
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
	// �趨����ģʽ
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
	// �趨/��������»���
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
	// �������ģʽ
	else if( (CMD1 == 0x1c) && (CMD2 ==  0x2e) )
	{
		CMD1 = 0;
		CMD2 = 0;
	}
	// �û��Զ��������ַ�
	else if( (CMD1 == 0x1c) && (CMD2 ==  0x32) )
	{
		if(NumBufferReceive()<34)
			return;

		for(c8c1=0; c8c1<34;c8c1++)
			c8c2 = ReceiveBufferData();//GetBufferReceiveData();

		CMD1 = 0;
		CMD2 = 0;
	}
	// ȡ���Զ��庺��
	else if( (CMD1 == 0x1c) && (CMD2 ==  0x3f) )
	{
		if(NumBufferReceive()<2)
			return;

		c8c1 = ReceiveBufferData();//GetBufferReceiveData();
		c8c2 = ReceiveBufferData();//GetBufferReceiveData();

		CMD1 = 0;
		CMD2 = 0;
	}
	// ���������ַ����д�ӡ
	else if( (CMD1 == 0x1c) && (CMD2 ==  0x44) )
	{
		CMD1 = 0;
		CMD2 = 0;
	}
	// �趨�����ӡ
	else if( (CMD1 == 0x1c) && (CMD2 ==  0x4a) )
	{
		CMD1 = 0;
		CMD2 = 0;
	}
	// �趨�����ӡ
	else if( (CMD1 == 0x1c) && (CMD2 ==  0x4b) )
	{
		CMD1 = 0;
		CMD2 = 0;
	}
	// ����
	else if( (CMD1 == 0x1c) && (CMD2 ==  0x4c) )
	{
		CMD1 = 0;
		CMD2 = 0;
	}
	// ȡ����ת
	else if( (CMD1 == 0x1c) && (CMD2 ==  0x4e) )
	{
		CMD1 = 0;
		CMD2 = 0;
	}
	// ����
	else if( (CMD1 == 0x1c) && (CMD2 ==  0x52) )
	{
		CMD1 = 0;
		CMD2 = 0;
	}
	// �趨�������Ҽ��
	else if( (CMD1 == 0x1c) && (CMD2 ==  0x53) )
	{
		if(NumBufferReceive()<2)
			return;

		c8c1 = ReceiveBufferData();//GetBufferReceiveData();
		c8c2 = ReceiveBufferData();//GetBufferReceiveData();

		CMD1 = 0;
		CMD2 = 0;
	}
	// �趨����ַ��ּ��
	else if( (CMD1 == 0x1c) && (CMD2 ==  0x54) )
	{
		CMD1 = 0;
		CMD2 = 0;
	}
	// �趨����ַ�������
	else if( (CMD1 == 0x1c) && (CMD2 ==  0x55) )
	{
		if(FlagDataWait = 0)
		{
			if(NumBufferReceive()<1)
				return;

			c8c1 = ReceiveBufferData();//GetBufferReceiveData();
			SendString("���ô�ӡ����������\r\n");
			SendString("�������Ƴ���\r\n");
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
			SendString("��ӡ������:");
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
	// ȡ������ַ�������
	else if( (CMD1 == 0x1c) && (CMD2 ==  0x56) )
	{
		if(NumBufferReceive()<2)
			return;

		c8c1 = ReceiveBufferData();//GetBufferReceiveData();

		CMD1 = 0;
		CMD2 = 0;
	}
	// �趨/����ı��Ǵ�ӡ
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
	// ѡ���ֵ���ͺ��ָ߶�
	else if( (CMD1 == 0x1c) && (CMD2 ==  0x59) )
	{
		CMD1 = 0;
		CMD2 = 0;
	}
	// �趨���ֻ�׼��
	else if( (CMD1 == 0x1c) && (CMD2 ==  0x62) )
	{
		CMD1 = 0;
		CMD2 = 0;
	}
	// �趨����ˮƽ�ƶ�����
	else if( (CMD1 == 0x1c) && (CMD2 ==  0x63) )
	{
		CMD1 = 0;
		CMD2 = 0;
	}
	// ѡ��������
	else if( (CMD1 == 0x1c) && (CMD2 ==  0x6b) )
	{
		CMD1 = 0;
		CMD2 = 0;
	}
	// �趨������/�±��ӡ
	else if( (CMD1 == 0x1c) && (CMD2 ==  0x72) )
	{
		CMD1 = 0;
		CMD2 = 0;
	}
	// �趨/�����������
	else if( (CMD1 == 0x1c) && (CMD2 ==  0x76) )
	{
		CMD1 = 0;
		CMD2 = 0;
	}
	// �趨������ٴ�ӡ
	else if( (CMD1 == 0x1c) && (CMD2 ==  0x78) )
	{
		CMD1 = 0;
		CMD2 = 0;
	}
	// **********************************	˫�ֽ�ָ��GS *
	// �߼���λ����ӡ��ʼλ��
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
	// ������������
	else if( (CMD1 == 0x1d) && (CMD2 ==  0x28) )
	{
		if(FlagDataWait == 0)
		{
			if(NumBufferReceive()<1)
				return;

			c8c1 = ReceiveBufferData();//GetBufferReceiveData();

			if(c8c1 == 0x41) // ��������
				FlagDataWait = 10;
			else if(c8c1 == 0x46) // ���úڱ궨λƫ����
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
	// ���״�ӡģʽ
	else if( (CMD1 == 0x1d) && (CMD2 ==  0x42) )
	{
		CMD1 = 0;
		CMD2 = 0;
	}
#endif
	// �߼���λ��˺��ֽλ��
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
					/*�����ǰ��ֽ��˺ֽλ�����ִ��*/
					DriverHead=0;
					SangShu=1;
					// 0x00,0x30:��ǰλ��ȫ��ֽ		0x01,0x31:��ǰλ�ð���ֽ		0x01,0x31
					// 0x41+n:ƫ����ֽλ��n��ȫ��ֽ	0x41+n:ƫ����ֽλ��n������ֽ
					if(c8c1 == 0x53)
					{
						SendString("ҳ����������ֽ........................................................................\r\n");
						if(PrinttingMode==PrintForward)
						{
							PAPCutPosition();
						}
					}
					//
					SendString("�������һ��ֽ\r\n");
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
				//���ִֽ��
				else
				{
					/*�ж�ֽ���Ƿ��Ѿ��߳�*/

					PrintDataContinu=0;
					/*�ж��Ƿ������һ��ֽ*/
					if(NumBufferReceive()>10)
					{
						/*����������һ��ֽ*/
						SendString("�������һ��ֽ\r\n");
						goto tiaochuqu;
					}
					else
					{
						SendString("�������һ��ֽ2\r\n");
						DelayTime2mS(5);// 100	// @20220516 Alke ��ʼ�����ӿ�ֽ�Ž����ٶ�
						if(NumBufferReceive()>10)
						{
							goto tiaochuqu;
						}

						SendString("�������һ��ֽ3\r\n");
						DelayTime2mS(5);// 100	// @20220516 Alke ��ʼ�����ӿ�ֽ�Ž����ٶ�
						if(NumBufferReceive()>10)
						{
							goto tiaochuqu;
						}
						SendString("�������һ��ֽ4\r\n");
						DelayTime2mS(5);// 100	// @20220516 Alke ��ʼ�����ӿ�ֽ�Ž����ٶ�
						if(NumBufferReceive()>10)
						{
							goto tiaochuqu;
						}
						SendString("�������һ��ֽ5\r\n");
						DelayTime2mS(5);// 100	// @20220516 Alke ��ʼ�����ӿ�ֽ�Ž����ٶ�
						if(NumBufferReceive()>10)
						{
							goto tiaochuqu;
						}

					}

					SendString("���һ��ֽ\r\n");
					/*������洫������ֽ*/
					DelayTime2mS(5);// 10	// @20220516 Alke ��ʼ�����ӿ�ֽ�Ž����ٶ�
					/*�ж�ֽ���Ƿ��߳�, ���ǰ�洫�����Ѿ�û�м�⵽ֽ��*/

						if(CheckPaperON(4)==0)
						{
							/*��������Ǻ��ֽ��ֽ���ߵ����һ��*/
							/*��ֽ��ȫ���߳�*/
							PAPMoveStep =72*10;
							PAPFeedStart(0);
							while(PAPCRT == 0x00);
							PrinttingMode=PrintStandby;
							while(1)
							{
								if((CheckPaperON(1)==0)&&(CheckPaperON(4)==0))
								{
									DelayTime2mS(5);// 20	// @20220516 Alke ��ʼ�����ӿ�ֽ�Ž����ٶ�
									if((CheckPaperON(1)==0)&&(CheckPaperON(4)==0))
									{
										SendString("ǰ��ֽ�Ŷ��Ѿ��˳�\r\n");
										break;
									}
								}
							}
							while(1)
							{
								if((CheckPaperON(1)==0)&&(CheckPaperON(4)==0))
								{
									DelayTime2mS(10);// 100	// @20220516 Alke ��ʼ�����ӿ�ֽ�Ž����ٶ�
									if((CheckPaperON(1)==0)&&(CheckPaperON(4)==0))
									{
										DelayTime2mS(10);// 100	// @20220516 Alke ��ʼ�����ӿ�ֽ�Ž����ٶ�
										if((CheckPaperON(1)==0)&&(CheckPaperON(4)==0))
										{
											break;
										}
									}
								}
							}

							/*�ж�ǰ�洫����ֽ���Ƿ�ȡ��*/
							SendString("���ֽ���һҳ1d5653\r\n");
							LastPaperBack=1;//���� ������ӡ

							/*������洫������ֽ*/
							if(CheckPaperON(4)==0)
							{
								SendString("���ֽ��ֽ��ȫ���߳�----\r\n");
								PAPMoveStep = 72*10;
								PAPFeedStart(0);
								while(PAPCRT == 0x00);
								backwardmotorcount=0;
								SangYi2=1;
								PositionFlag=0;
								PrinttingMode=PrintStandby;

							}

							/*��������Ѿ������һҳ���������ߵ�˺ֽλ�ã�����ֽ���Ѿ��߳���ȡ�����ӵ�*/
							WaitForData(1);
							CMD1 = *(BufferReceive+cBufferReceiveRead);
							cBufferReceiveReadADD();

							WaitForData(1);
							CMD1 = *(BufferReceive+cBufferReceiveRead);
							cBufferReceiveReadADD();
							WaitForData(1);
							CMD1 = *(BufferReceive+cBufferReceiveRead);
							cBufferReceiveReadADD();
							SendString("��ӡ�����һҳ���ߵ�˺ֽλ��ָ��ȡ��\r\n");

							backwardmotorcount=0;
							SangYi2=1;
							/*��λ��ֽȡ��*/
							PositionFlag=0;
							SendString("��λ����ӡ��ʼλ�ù��ܴ�\r\n");
							PrintDataContinu=0;//���������Ҫ��λ��ӡ��ʼλ��
							/*��˺ֽλ��ָ��ȡ��*/

							/*���ֽ��*/
							PaperTest();
							PrintDataContinu=0;//���������Ҫ��λ��ӡ��ʼλ��
							SPFlag=1;//��û�д����ߵ�˺ֽλ��ָ��
						}
						else
						{
							/*���һҳ������ֽ*/
							/*�ߵ�˺ֽλ�ô�*/
							/*�������ߵ�˺ֽλ���������������*/
							DriverHead=1;
						}
						goto ennd;
		tiaochuqu:
					SendString("�������һ��ֽ\r\n");
					/*�ж��Ƿ�ֽ���Ѿ��߳�*/
					if(CheckPaperON(4)==0)
					{
						DelayTime2mS(5);
						if(CheckPaperON(4)==0)
						{
							SendString("ֽ���Ѿ��߹��󽺹�\r\n");
							{
								SendString("���ֽ��ֽ��ȫ���߳�----\r\n");
								PAPMoveStep = 72*10;
								PAPFeedStart(0);
								while(PAPCRT == 0x00);
								PrinttingMode=PrintStandby;
							}
							while(1)
							{
								if((CheckPaperON(1)==0)&&(CheckPaperON(4)==0))
								{
									DelayTime2mS(5);// 100	// @20220516 Alke ��ʼ�����ӿ�ֽ�Ž����ٶ�
									if((CheckPaperON(1)==0)&&(CheckPaperON(4)==0))
									{
										DelayTime2mS(5);// 100	// @20220516 Alke ��ʼ�����ӿ�ֽ�Ž����ٶ�
										if((CheckPaperON(1)==0)&&(CheckPaperON(4)==0))
										{
											break;
										}
									}
								}
							}
							backwardmotorcount=0;
							//��ֽ״̬
							SangYi2=1;
							// �Ժ�λ����Ҫ��ֽ
							PositionFlag=0;
							SendString("��λ����ӡ��ʼλ�ù��ܴ�\r\n");
							// ��ӡ��ʼλ�ö�λ��
							PrintDataContinu=0;
							// �ߵ�˺ֽλ��ָ�û�д����������˺ֽλ��ָ�����
							// ������������������ӡ����û���ߵ�˺ֽλ��ָ������ֽ����ĵ�һ��ָ����˺ֽλ��ָ��
							DriverHead=0;
						}
					}
					else
					{
						/*ֽ���ں󽺹�*/
						SendString("ֽ���ں󽺹�\r\n");
						backwardmotorcount=0;
						//��ֽ״̬
						SangYi2=0;
						/*����������������ӡ����������ߵ�˺ֽλ������*/
						/*��������������ߵ�˺ֽλ�����������*/
						DriverHead=0;
						/*������ļ���ӡ���������ߵ���ӡ��ʼλ�ã��������������*/
						/*�����������ӡ���������ߵ���ʼλ��*/
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
	// ����״̬
	else if( (CMD1 == 0x1d) && (CMD2 ==  0x72) )
	{
		CMD1 = 0;
		CMD2 = 0;
	}
	// �������߻ָ��ȴ�ʱ��
	else if( (CMD1 == 0x1d) && (CMD2 ==  0x7a) )
	{
		CMD1 = 0;
		CMD2 = 0;
	}
	// ���ִ�ӡ2 byte-1
	else if(	(CMD1 >= 0xa1) && (CMD1 <= 0xa9) && 	(CMD2 >= 0xa1) && (CMD2 <= 0xfe) 	)
	{
		Chinese2ByteAddressCalculate:
		WritePRTBuffer(CMD1, CMD2, 0, 0);

		CMD1 = 0;
		CMD2 = 0;
	}
	// ���ִ�ӡ2 byte-2
	else if( (CMD1 >= 0xa8) && (CMD1 <= 0xa9) && (CMD2 >= 0x40) && (CMD2 <= 0xa0) )
	{
		goto  Chinese2ByteAddressCalculate;
	}
	// ���ִ�ӡ2 byte-3
	else if( (CMD1 >= 0xb0) && (CMD1 <= 0xf7) && (CMD2 >= 0xa1) && (CMD2 <= 0xfe) )
	{
		goto  Chinese2ByteAddressCalculate;
	}
	// ���ִ�ӡ2 byte-4
	else if( (CMD1 >= 0x81) && (CMD1 <= 0xa0) && (CMD2 >= 0x40) && (CMD2 <= 0xfe) )
	{
		goto  Chinese2ByteAddressCalculate;
	}
	// ���ִ�ӡ2 byte-5
	else if( (CMD1 >= 0xaa) && (CMD1 <= 0xfe) && (CMD2 >= 0x40) && (CMD2 <= 0xa0) )
	{
		goto  Chinese2ByteAddressCalculate;
	}
	// ���ִ�ӡ4 byte-81-1/2
	else if( (CMD1 == 0x81) && (CMD2 ==  0x39) )
	{
		Chinese4ByteWaitFor2Byte:
		if(NumBufferReceive()<2)
			return;

		c8c1 = CMD1;
		c8c2 = CMD2;
		c8c3 = ReceiveBufferData();//GetBufferReceiveData();
		c8c4 = ReceiveBufferData();//GetBufferReceiveData();
		// ���ִ�ӡ4 byte-81-1
		if( (c8c1 == 0x81) && (c8c2 == 0x39) && (c8c3 == 0xee) && (c8c4 == 0x39) )
		{
			Chinese4ByteAddressCalculate:
			WritePRTBuffer(c8c1, c8c2, c8c3, c8c4);
		}
		// ���ִ�ӡ4 byte-81-2
		else if( (c8c1 == 0x81) && (c8c2 ==  0x39) && (c8c3 >= 0xef) && (c8c3 <= 0xfe) && (c8c4 >= 0x30) && (c8c4 <= 0x39) )
			goto Chinese4ByteAddressCalculate;
		// ���ִ�ӡ4 byte-82-1
		else if( (c8c1 == 0x82) && (c8c2 >= 0x30) && (c8c2 <= 0x34) && (c8c3 >= 0x81) && (c8c3 <= 0xfe) && (c8c4 >= 0x30) && (c8c4 <= 0x39) )
			goto Chinese4ByteAddressCalculate;
		// ���ִ�ӡ4 byte-82-2
		else if( (c8c1 == 0x82) && (c8c2 ==  0x35) && (c8c3 >= 0x81) && (c8c3 <= 0x86) && (c8c4 >= 0x30) && (c8c4 <= 0x39) )
			goto Chinese4ByteAddressCalculate;
		// ���ִ�ӡ4 byte-82-3
		else if( (c8c1 == 0x82) && (c8c2 ==  0x35) && (c8c3 == 0x87) && (c8c4 >= 0x30) && (c8c4 <= 0x38) )
			goto Chinese4ByteAddressCalculate;

		CMD1 = 0;
		CMD2 = 0;
	}
	// ���ִ�ӡ4 byte-82-1/2/3
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

