
//#define Driver 0
/**************************************************************************************************************
	Driver.c	Driver.h
**************************************************************************************************************/

#include "Driver.h"
#include "McuInt.h"
#include "UserInterface.h"



struct wifi_stored_sta_infoAlke
{
    u8 pwd[64];
    u8 ssid[33];
}	wifi_setup_sta_info;



// @20230601 �����µĳ���ܹ����˺���Ϊ�յ����ݺ�Ĵ����¼�
// �˺�������һ���ֽ�һ���ֽڽ��д����˹����в�������ѭ����ֻ��һ�ι����������ñ�־λ

// @20230713 Alke USB�ش��ڴ�Ӧ�����Ѿ�����
extern Device_Rev *USB_RevC;
unsigned char UsbDataBack[64] = {"Usb to PC!\r\n"};
unsigned char cUsbDataBack = 0;

// @20230713 Alke Wifi�ش��ڴ�Ӧ������Ȼ���У�����Wifi���ǿ��Ե�
extern int tcp_send_data_sang(const void *buf, u32 len);//

// @20230713 Alke  Spp�ش����ݣ�
// @20230713 Alke  BLE�кܶ�ͨ��������
//BT_RevC->DataOut(&data,1);
extern Device_Rev *BT_RevC;



// ���Դ�����
void Cmd0dPrint(void) // ���ֽ�����û�к�������
{
	Command.Type = eCmdInit;
	Command.State = 0;
	Command.Size = 0;
#if SendCmd
	printf("Cmd0dPrint\n");
//	USB_RevC->DataOut("USB: Cmd0dPrint \r\n", 64);
//	tcp_send_data_sang("Wifi: Cmd0dPrint \r\n",17);
//	BT_RevC->DataOut("Spp: Cmd0dPrint \r\n",6);
#endif
}

// �������ֽ�Ϊ1b��������
void Cmd1bClass(unsigned char data)
{
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
#if SendCmd
		printf("Cmd1b0a\n");
//		USB_RevC->DataOut("USB: Cmd1b0a \r\n", 64);
//		tcp_send_data_sang("Wifi: Cmd1b0a \r\n", 14);
//		BT_RevC->DataOut("Spp: Cmd1b0a \r\n", 13);
#endif
		break;

	default:
		Command.Type = eCmdInit;
		Command.State = 0;
		break;
	}
}
void Cmd1b0b(unsigned char data)
{
	u8 c8c1;
	unsigned char ssid_stored_cnt;
	struct wifi_stored_sta_infoAlke
	{
    	u8 pwd[64];
	    u8 ssid[33];
	}	wifi_stored_sta_info[1]; // Alke 20230722


	switch(Command.State)
	{
	case 0:
		if(data == 0x0b)
		{
//USB_RevC->DataOUT(wifi_setup_sta_info.ssid,Command.Size);
			c8c1 = 0;
			UsbDataBack[c8c1++] = 'S';
			UsbDataBack[c8c1++] = 'S';
			UsbDataBack[c8c1++] = 'I';
			UsbDataBack[c8c1++] = 'D';
			UsbDataBack[c8c1++] = ':';
			for(; c8c1 < Command.Size; c8c1++)
			{
				UsbDataBack[c8c1] = wifi_setup_sta_info.ssid[c8c1];
			}
			UsbDataBack[c8c1++] = '\r';
			UsbDataBack[c8c1++] = '\n';
			while(c8c1++ < 64)
				UsbDataBack[c8c1] = 0;
			USB_RevC->DataOut(UsbDataBack, 64);


			// û��ռ��ʱ�����һλ�ַ������һ��0��Ϊ�������
			while(Command.Size < 33) // wifi_stored_sta_info.ssid[33]
			{
				wifi_setup_sta_info.ssid[Command.Size] = 0;
				Command.Size++;
			}

			Command.State = 1; // ������Ч
			Command.Size = 0;
#if DebugMsg
			printf("SSID: %s\r\n",wifi_setup_sta_info.ssid);// @20230711 Alke
			USB_RevC->DataOut("SSID: %s \r\n", wifi_setup_sta_info.ssid);//, 64);
			//tcp_send_data_sang("Wifi: Cmd1d56CutPaper \r\n",18+2);
			//BT_RevC->DataOut("Spp: Cmd1d56CutPaper \r\n",17+3);
#endif
		}
		else
		{
			//if(Command.Size == 0)
			//	printf("Cmd1b0b--Start------------------\n");
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
//USB_RevC->DataOUT(wifi_setup_sta_info.ssid,Command.Size);
			c8c1 = 0;
			UsbDataBack[c8c1++] = 'P';
			UsbDataBack[c8c1++] = 'W';
			UsbDataBack[c8c1++] = 'D';
			UsbDataBack[c8c1++] = ':';
			for(; c8c1 < Command.Size; c8c1++)
			{
				UsbDataBack[c8c1] = wifi_setup_sta_info.ssid[c8c1];
			}
			UsbDataBack[c8c1++] = '\r';
			UsbDataBack[c8c1++] = '\n';
			while(c8c1++ < 64)
				UsbDataBack[c8c1] = 0;
			USB_RevC->DataOut(UsbDataBack, 64);


			// û��ռ��ʱ�����һλ�ַ������һ��0��Ϊ�������
			while(Command.Size < 64) // wifi_stored_sta_info.pwd[64]
			{
				wifi_setup_sta_info.pwd[Command.Size] = 0;
				Command.Size++;
			}
#if DebugMsg
			printf("PWD:  %s\r\n", wifi_setup_sta_info.pwd);// @20230711 Alke
			USB_RevC->DataOut("PWD: %s\r\n", 64);//wifi_setup_sta_info.pwd, 64);
			//tcp_send_data_sang("Wifi: Cmd1d56CutPaper \r\n",26);
			//BT_RevC->DataOut("Spp: Cmd1d56CutPaper \r\n",25);
#endif

			ssid_stored_cnt = wifi_get_stored_sta_info(wifi_stored_sta_info);
    		for (int i = 0; i < ssid_stored_cnt; i++)
			{
	        	//printf("1:wifi_get_stored_sta_info[%d]= %s \r\n", i, wifi_stored_sta_info[i].ssid);
				wifi_del_stored_sta_info(wifi_stored_sta_info[i].ssid);// @ Alke 20230711 ɾ���洢����
	    	}
			os_time_dly(100);
			wifi_store_mode_info(STA_MODE, wifi_setup_sta_info.ssid, wifi_setup_sta_info.pwd);

			Command.Type = eCmdInit;
			Command.State = 0;
			Command.Size = 0;
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
#if 1
void Cmd1b2aDots(unsigned char data)
{
	volatile unsigned int c16c1;
	volatile unsigned char c8c1;

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
			goto PrintDataDeal;
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
			goto PrintDataDeal;
		}
		break;

	case 30: // ͼ�����������㴦��
		PrintDataDeal:
//printf("30-\n");
		//ԭʼ���ݿ�ʼ����
		// @20230707 Alke ���ǰ��ֽ��Ҫ�������ҡ����µߵ�һ��
		// @@@@@@@@@@@@@@@@@@@@@@@@@@
		if(PaperState == sPaperFrontNormal) // ǰ��ֽģʽ
		{
			// ��ӡЧ���ϵ��������ҵ��������Ǵ�ӡ������ǰ�������ͬʱA B���������ݵ��������������к�ż�������ݵ���
			for(cBufferPrintDataWrite = 0; cBufferPrintDataWrite < (BufferPRTSize/2); cBufferPrintDataWrite++)
			{
				c8c1 = BufferPrint[cBufferPrintLineWrite].BufferA[cBufferPrintDataWrite];
				BufferPrint[cBufferPrintLineWrite].BufferA[cBufferPrintDataWrite] = BufferPrint[cBufferPrintLineWrite].BufferB[BufferPRTSize-1-cBufferPrintDataWrite];
				BufferPrint[cBufferPrintLineWrite].BufferB[BufferPRTSize-1-cBufferPrintDataWrite] = c8c1;

				c8c1 = BufferPrint[cBufferPrintLineWrite].BufferB[cBufferPrintDataWrite];
				BufferPrint[cBufferPrintLineWrite].BufferB[cBufferPrintDataWrite] = BufferPrint[cBufferPrintLineWrite].BufferA[BufferPRTSize-1-cBufferPrintDataWrite];
				BufferPrint[cBufferPrintLineWrite].BufferA[BufferPRTSize-1-cBufferPrintDataWrite] = c8c1;
			}

			// ��ӡЧ���ϵ��������µߵ������Ǵ�ӡ���������ݸߵ�λ����
			for(cBufferPrintDataWrite = 0; cBufferPrintDataWrite < BufferPRTSize; cBufferPrintDataWrite++)
			{
				BufferPrint[cBufferPrintLineWrite].BufferA[cBufferPrintDataWrite] = ByteBitChang(BufferPrint[cBufferPrintLineWrite].BufferA[cBufferPrintDataWrite]);
				BufferPrint[cBufferPrintLineWrite].BufferB[cBufferPrintDataWrite] = ByteBitChang(BufferPrint[cBufferPrintLineWrite].BufferB[cBufferPrintDataWrite]);
			}
		}
		// @@@@@@@@@@@@@@@@@@@@@@@@@@

		//���������ݿ�ʼ���� VirtualHalfDotsDataDeal:
		cBufferPrintDataWrite = 0;
		while(cBufferPrintDataWrite < (BufferPRTSize-1)) // (BufferPRTSize-2)Command.Size
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

//printf("N%d-S%d-P%d\n", CRGNowTimePosition, CRGStopTimePosition, PrintingState);
//printf("BW%d-BR%d-\n", cBufferPrintLineWrite, cBufferPrintLineRead);
//if(!CRGRunning)
//	printf("LF%d-RT%d-\n", BufferPrint[cBufferPrintLineWrite].cLeft, BufferPrint[cBufferPrintLineWrite].cRight);

//@20230705 Alke
//BufferPrint[cBufferPrintLineWrite].cLeft = 0;
//BufferPrint[cBufferPrintLineWrite].cRight = TimePositionAll;
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
#if SendCmd
		printf("Cmd1b2aDots\n");
		USB_RevC->DataOut("USB: Cmd1b2aDots \r\n", 64);
		tcp_send_data_sang("Wifi: Cmd1b2aDots \r\n",22);
		BT_RevC->DataOut("Spp: Cmd1b2aDots \r\n",21);
#endif
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

#else
void Cmd1b2aDots(unsigned char data)
{
	volatile unsigned int c16c1;
	volatile unsigned char c8c1;

	switch(Command.State)
	{
	case 0:
printf("Dots-00\n");
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
		// ǰ��ֽģʽ��ǰһ��������Ҫд��ż���д�ӡ����������һ��������Ҫд�������д�ӡ���������������������б�����ֽ���ϵ�������Ҫǰ���������ֽ����ԭ��ÿ��������Ҫ��λ�ߵͻ���
		if(PaperState == sPaperFrontNormal)
		{
			;//
		}
		else
		{
			BufferPrint[cBufferPrintLineWrite].BufferA[cBufferPrintDataWrite] = data;
			cBufferPrintDataWrite++;
			BufferPrint[cBufferPrintLineWrite].BufferA[cBufferPrintDataWrite] = 0x00;
			cBufferPrintDataWrite++;

			if(cBufferPrintDataWrite >= Command.Size)
			{
				//ʣ��д0
				for(c16c1 = cBufferPrintDataWrite; c16c1 < BufferPRTSize; c16c1++)
					BufferPrint[cBufferPrintLineWrite].BufferA[c16c1] = 0;
				
				Command.State++;
			}
		}
		break;
	case 13: // ����ͼ�����ݿ�ʼ��ż��������
		if(PaperState == sPaperFrontNormal) // ǰ��ֽģʽ
		{
			;//
		}
		else
		{
			BufferPrint[cBufferPrintLineWrite].BufferB[cBufferPrintDataWrite-Command.Size/2] = data; // Alke @20230704 -Command.Size/2
			cBufferPrintDataWrite++;
			cBufferPrintDataWrite++;
			BufferPrint[cBufferPrintLineWrite].BufferB[cBufferPrintDataWrite-Command.Size/2] = 0x00; // Alke @20230704 -Command.Size/2
			cBufferPrintDataWrite++;
			cBufferPrintDataWrite++;
			
			if(cBufferPrintDataWrite >= (Command.Size*2))
			{
				//ʣ��д0
				for(c16c1 = cBufferPrintDataWrite; c16c1 < BufferPRTSize*2; c16c1++)
					BufferPrint[cBufferPrintLineWrite].BufferB[c16c1-Command.Size/2] = data;
				Command.State = 30;
				goto PrintDataDeal;
			}
		}
		break;

	case 20: // ���ͼ�����ݣ�Size��8λ
printf("Cmd1b2aDots-20\n");
		Command.Size = data;
		Command.State++;
		break;
	case 21: // ���ͼ�����ݣ�Size��8λ
printf("Cmd1b2aDots-21-%d\n", Command.Size);
		c16c1 = data;
		c16c1 <<= 8;
		Command.Size += c16c1;
		cBufferPrintDataWrite = 0;
		Command.State++;
		break;
	case 22: // ���ͼ�����ݿ�ʼ������������
printf("22-%d\n", cBufferPrintDataWrite);
if(0)//		if(PaperState == sPaperFrontNormal) // ǰ��ֽģʽ
		{
			BufferPrint[cBufferPrintLineWrite].BufferB[BufferPRTSize-1-cBufferPrintDataWrite] = ByteBitChang(data);
			cBufferPrintDataWrite++;
			// ȷ������һ���㹻���������¸�״̬
			if(cBufferPrintDataWrite >= (Command.Size/2))
			{
				//ʣ��д0
				for(c16c1 = 0; c16c1 < (BufferPRTSize-1-cBufferPrintDataWrite); c16c1++)
					BufferPrint[cBufferPrintLineWrite].BufferB[c16c1] = 0;
				Command.State++;
			}
		}
		else
		{
			BufferPrint[cBufferPrintLineWrite].BufferA[cBufferPrintDataWrite] = data;
			cBufferPrintDataWrite++;
			// ȷ������һ���㹻���������¸�״̬
			if(cBufferPrintDataWrite >= (Command.Size/2))
			{
				//ʣ��д0
				for(c16c1 = cBufferPrintDataWrite; c16c1 < BufferPRTSize; c16c1++)
					BufferPrint[cBufferPrintLineWrite].BufferA[c16c1] = 0;
				Command.State++;
			}
		}
		break;
	case 23: // ���ͼ�����ݿ�ʼ��ż��������
printf("23-%d\n", cBufferPrintDataWrite);
if(0)//		if(PaperState == sPaperFrontNormal) // ǰ��ֽģʽ
		{
			BufferPrint[cBufferPrintLineWrite].BufferA[BufferPRTSize-1-(cBufferPrintDataWrite-Command.Size/2)] = ByteBitChang(data);
			cBufferPrintDataWrite++;
			// ȷ������һ���㹻���������¸�״̬
			if(cBufferPrintDataWrite >= Command.Size)
			{
				//ʣ��д0
				for(c16c1 = 0; c16c1 < (BufferPRTSize-1-(cBufferPrintDataWrite-Command.Size/2)); c16c1++)
					BufferPrint[cBufferPrintLineWrite].BufferA[c16c1] = 0;
				Command.State = 30;
				goto PrintDataDeal;
			}
		}
		else
		{
			BufferPrint[cBufferPrintLineWrite].BufferB[cBufferPrintDataWrite-Command.Size/2] = data; // Alke @20230704 -Command.Size/2
			cBufferPrintDataWrite++;
			// ȷ������һ���㹻���������¸�״̬
			if(cBufferPrintDataWrite >= (Command.Size))
			{
				//ʣ��д0
				for(c16c1 = cBufferPrintDataWrite; c16c1 < BufferPRTSize*2; c16c1++)
					BufferPrint[cBufferPrintLineWrite].BufferB[c16c1-Command.Size/2] = data;
				Command.State = 30;
				goto PrintDataDeal;
			}
		}
		break;

	case 30: // ͼ�����������㴦��
		PrintDataDeal:
printf("30-\n");
//printf("Buff-A\n");
//put_buf(BufferPrint[cBufferPrintLineWrite].BufferA, BufferPRTSize);
//put_buf(BufferPrint[cBufferPrintLineWrite].BufferA[BufferPRTSize-1-32], 32);
/*for(cBufferPrintDataWrite = 0; cBufferPrintDataWrite < 32; cBufferPrintDataWrite++) //BufferPRTSize
{
	c8c1 = (BufferPrint[cBufferPrintLineWrite].BufferA[cBufferPrintDataWrite]);
	printf(c8c1);
}*/
//printf("Buff-B\n");
//put_buf(BufferPrint[cBufferPrintLineWrite].BufferB, BufferPRTSize);
//put_buf(BufferPrint[cBufferPrintLineWrite].BufferB[BufferPRTSize-1-32], 32);
/*for(cBufferPrintDataWrite = 0; cBufferPrintDataWrite < 32; cBufferPrintDataWrite++)
{
	c8c1 = (BufferPrint[cBufferPrintLineWrite].BufferB[cBufferPrintDataWrite]);
	printf(c8c1);
}*/
		//ԭʼ���ݿ�ʼ����
		// @20230707 Alke ���ǰ��ֽ��Ҫ�������ҡ����µߵ�һ��
		// @@@@@@@@@@@@@@@@@@@@@@@@@@
	#if 1
		if(PaperState == sPaperFrontNormal) // ǰ��ֽģʽ
		{
			// ��ӡЧ���ϵ��������ҵ��������Ǵ�ӡ������ǰ�������ͬʱA B���������ݵ��������������к�ż�������ݵ���
			for(cBufferPrintDataWrite = 0; cBufferPrintDataWrite < (BufferPRTSize/2); cBufferPrintDataWrite++)
			{
				c8c1 = BufferPrint[cBufferPrintLineWrite].BufferA[cBufferPrintDataWrite];
				BufferPrint[cBufferPrintLineWrite].BufferA[cBufferPrintDataWrite] = BufferPrint[cBufferPrintLineWrite].BufferB[BufferPRTSize-1-cBufferPrintDataWrite];
				BufferPrint[cBufferPrintLineWrite].BufferB[BufferPRTSize-1-cBufferPrintDataWrite] = c8c1;

				c8c1 = BufferPrint[cBufferPrintLineWrite].BufferB[cBufferPrintDataWrite];
				BufferPrint[cBufferPrintLineWrite].BufferB[cBufferPrintDataWrite] = BufferPrint[cBufferPrintLineWrite].BufferA[BufferPRTSize-1-cBufferPrintDataWrite];
				BufferPrint[cBufferPrintLineWrite].BufferA[BufferPRTSize-1-cBufferPrintDataWrite] = c8c1;
			}

			// ��ӡЧ���ϵ��������µߵ������Ǵ�ӡ���������ݸߵ�λ����
			for(cBufferPrintDataWrite = 0; cBufferPrintDataWrite < BufferPRTSize; cBufferPrintDataWrite++)
			{
				BufferPrint[cBufferPrintLineWrite].BufferA[cBufferPrintDataWrite] = ByteBitChang(BufferPrint[cBufferPrintLineWrite].BufferA[cBufferPrintDataWrite]);
				BufferPrint[cBufferPrintLineWrite].BufferB[cBufferPrintDataWrite] = ByteBitChang(BufferPrint[cBufferPrintLineWrite].BufferB[cBufferPrintDataWrite]);
			}
		}
	#endif
		// @@@@@@@@@@@@@@@@@@@@@@@@@@

		

//printf("Buff-A-result\n");
//put_buf(BufferPrint[cBufferPrintLineWrite].BufferA, BufferPRTSize);
//put_buf(BufferPrint[cBufferPrintLineWrite].BufferA[BufferPRTSize-1-32], 32);
/*for(cBufferPrintDataWrite = 0; cBufferPrintDataWrite < 32; cBufferPrintDataWrite++)
{
	c8c1 = (BufferPrint[cBufferPrintLineWrite].BufferA[cBufferPrintDataWrite]);
	printf(c8c1);
}*/
//printf("Buff-B-result\n");
//put_buf(BufferPrint[cBufferPrintLineWrite].BufferB, BufferPRTSize);
//put_buf(BufferPrint[cBufferPrintLineWrite].BufferB[BufferPRTSize-1-32], 32);
/*for(cBufferPrintDataWrite = 0; cBufferPrintDataWrite < 32; cBufferPrintDataWrite++)
{
	c8c1 = (BufferPrint[cBufferPrintLineWrite].BufferB[cBufferPrintDataWrite]);
	printf(c8c1);
}*/

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

		//���������ݿ�ʼ���� VirtualHalfDotsDataDeal:
		cBufferPrintDataWrite = BufferPrint[cBufferPrintLineWrite].cLeft;
		while(cBufferPrintDataWrite < (BufferPrint[cBufferPrintLineWrite].cRight-1)) // (BufferPRTSize-2)Command.Size
		{
			BufferPrint[cBufferPrintLineWrite].BufferA[cBufferPrintDataWrite+1] &= ~BufferPrint[cBufferPrintLineWrite].BufferA[cBufferPrintDataWrite];
			BufferPrint[cBufferPrintLineWrite].BufferB[cBufferPrintDataWrite+1] &= ~BufferPrint[cBufferPrintLineWrite].BufferB[cBufferPrintDataWrite];
			cBufferPrintDataWrite++;
		}

		BufferPrint[cBufferPrintLineWrite].cLeft *= BufferOneDataTimePosition;//CRGOneStepTimePosition; // ����Ӧ��Ҫ��0��������м��ٹ���
		if(BufferPrint[cBufferPrintLineWrite].cLeft >= BufferPrint[cBufferPrintLineWrite].cLeft%CRGOneStepTimePosition)
			BufferPrint[cBufferPrintLineWrite].cLeft -= BufferPrint[cBufferPrintLineWrite].cLeft%CRGOneStepTimePosition;			// adjust to th 16*n

		BufferPrint[cBufferPrintLineWrite].cRight *= BufferOneDataTimePosition;//CRGOneStepTimePosition; // �ұ߽�����λ����Ҫ���Ӽ��ٺͼ��ٲ���
		BufferPrint[cBufferPrintLineWrite].cRight += CRGVolatileStep*CRGOneStepTimePosition*2; // �ұ߽�����λ����Ҫ���Ӽ��ٺͼ��ٲ���
		BufferPrint[cBufferPrintLineWrite].cRight += CRGOneStepTimePosition-BufferPrint[cBufferPrintLineWrite].cRight%CRGOneStepTimePosition; // adjust to th 16*n
		if(BufferPrint[cBufferPrintLineWrite].cRight > TimePositionAll)
			BufferPrint[cBufferPrintLineWrite].cRight = TimePositionAll;

//printf("N%d-S%d-P%d\n", CRGNowTimePosition, CRGStopTimePosition, PrintingState);
//printf("BW%d-BR%d-\n", cBufferPrintLineWrite, cBufferPrintLineRead);
//if(!CRGRunning)
//	printf("LF%d-RT%d-\n", BufferPrint[cBufferPrintLineWrite].cLeft, BufferPrint[cBufferPrintLineWrite].cRight);

//@20230705 Alke
//BufferPrint[cBufferPrintLineWrite].cLeft = 0;
//BufferPrint[cBufferPrintLineWrite].cRight = TimePositionAll;
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
#if SendCmd
		printf("Cmd1b2aDots\n");
//		USB_RevC->DataOut("USB: Cmd1b2aDots \r\n", 64);
//		tcp_send_data_sang("Wifi: Cmd1b2aDots \r\n",22);
//		BT_RevC->DataOut("Spp: Cmd1b2aDots \r\n",21);
#endif
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
#endif
void Cmd1b4aPAP(unsigned char data)
{

	for(cBufferPrintDataWrite = (BufferPRTSize-1); cBufferPrintDataWrite; cBufferPrintDataWrite--)
	{
		BufferPrint[cBufferPrintLineWrite].BufferA[cBufferPrintDataWrite] = 0x00;
		BufferPrint[cBufferPrintLineWrite].BufferB[cBufferPrintDataWrite] = 0x00;
	}
	//cBufferPrintDataWrite = 0; // ��ȥʱ�Ѿ��պ�Ϊ0

	// ͨ��Ĭ��Ϊ16����û��0D�����л���һ��ʱ�����β������ӣ�ʹ����ֽ����
	if(PaperState == sPaperFrontNormal)
	{
		BufferPrint[cBufferPrintLineWrite].Dir = sPaperDirFrontToBack;
		PAPBackSet
	}
	else if(PaperState == sPaperBackNormal)
	{
		BufferPrint[cBufferPrintLineWrite].Dir = sPaperDirBackToFront;
		PAPFeedSet
	}
	BufferPrint[cBufferPrintLineWrite].Step += data;
	if(cBufferPrintLineWrite >= (BufferPrintLineNum-1))
		cBufferPrintLineWrite = 0;
	else
		cBufferPrintLineWrite++;

	Command.Type = eCmdInit;
	Command.State = 0;
	Command.Size = 0;
#if SendCmd
	printf("Cmd1b4aPAP\n");
//	USB_RevC->DataOut("USB: Cmd1b4aPAP \r\n", 64);
//	tcp_send_data_sang("Wifi: Cmd1b4aPAP \r\n",21);
//	BT_RevC->DataOut("Spp: Cmd1b4aPAP \r\n",20);
//printf("N%d-S%d-P%d\n", CRGNowTimePosition, CRGStopTimePosition, PrintingState);
#endif
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
	if(PaperState == sPaperFrontNormal)
	{
		BufferPrint[cBufferPrintLineWrite].Dir = sPaperDirBackToFront;
		PAPFeedSet
	}
	else if(PaperState == sPaperBackNormal)
	{
		BufferPrint[cBufferPrintLineWrite].Dir = sPaperDirFrontToBack;
		PAPBackSet
	}		
	BufferPrint[cBufferPrintLineWrite].Step += data;
	if(cBufferPrintLineWrite >= (BufferPrintLineNum-1))
		cBufferPrintLineWrite = 0;
	else
		cBufferPrintLineWrite++;

	Command.Type = eCmdInit;
	Command.State = 0;
	Command.Size = 0;
#if SendCmd
	printf("Cmd1b4bPAP\n");
//	USB_RevC->DataOut("USB: Cmd1b4bPAP \r\n", 64);
//	tcp_send_data_sang("Wifi: Cmd1b4bPAP \r\n",21);
//	BT_RevC->DataOut("Spp: Cmd1b4bPAP \r\n",20);
//printf("N%d-S%d-P%d\n", CRGNowTimePosition, CRGStopTimePosition, PrintingState);
#endif
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
#if SendCmd
	printf("Cmd1b55PrtDir\n");
//	USB_RevC->DataOut("USB: Cmd1b55PrtDir \r\n", 64);
//	tcp_send_data_sang("Wifi: Cmd1b55PrtDir \r\n",24);
//	BT_RevC->DataOut("Spp: Cmd1b55PrtDir \r\n",23);
#endif
}
void Cmd1b7a(unsigned char data)
{
	u8 c;
	int ret;
	u8 c8d1 = 0x55;

#if 0
	for(c = 0; c < BufferPrintLineNum; c++)
	{
		printf("cL%d-cR%d-Step%d\n", BufferPrint[c].cLeft, BufferPrint[c].cRight, BufferPrint[c].Step);
	}
	printf("BufferLineWrite%d-BufferLineRead%d-FlagMotorIC%d\n", cBufferPrintLineWrite, cBufferPrintLineRead,FlagMotorIC);
	printf("PAPMoveStep%d\n", PAPMoveStep);
	printf("NowTimPos%d-StopTimpos%d-PrintingState%d\n", CRGNowTimePosition, CRGStopTimePosition, PrintingState);
#endif

#if DebugMsg
	printf("SSID:  %s\r\n", wifi_setup_sta_info.ssid);// @20230711 Alke
	printf("PWD:  %s\r\n", wifi_setup_sta_info.pwd);// @20230711 Alke
//	printf("SangIP[%d.%d.%d.%d] \r\n\r\n",netif_infoA.ip&0xff, (netif_infoA.ip>>8)&0xff,(netif_infoA.ip>>16)&0xff,(netif_infoA.ip>>24)&0xff);
#endif

#if 0
	 	ret = syscfg_read(CFG_USER_DEFINE_BEGIN, &syscfg_test_info, sizeof(syscfg_test_info));
		
		//if(data != syscfg_test_info.buf[0])
		if(HeadDelayCount != data)
		{
			HeadDelayCount = data;
			syscfg_test_info.buf[16] = data;
				
       		syscfg_test_info.buf[0] = data++;
       		syscfg_test_info.buf[1] = data++;
       		syscfg_test_info.buf[2] = data++;
       		syscfg_test_info.buf[3] = data++;
       		syscfg_test_info.buf[4] = data++;

	        syscfg_write(CFG_USER_DEFINE_BEGIN, &syscfg_test_info, sizeof(syscfg_test_info));
		}
		else
		{
			printf("syscfg_read succ, syscfg_test_info.cnt = 0x%x\r\n", syscfg_test_info.cnt);
			put_buf(syscfg_test_info.buf, sizeof(syscfg_test_info.buf));
		}
			
		
		

#endif

	Command.Type = eCmdInit;
	Command.State = 0;
	Command.Size = 0;
	
#if SendCmd
	printf("Cmd1b7a\n");
//	USB_RevC->DataOut("USB: Cmd1b7a \r\n", 64);
//	tcp_send_data_sang("Wifi: Cmd1b7a \r\n",18);
//	BT_RevC->DataOut("Spp: Cmd1b7a \r\n",17);
#endif
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
#if SendCmd
		printf("Cmd1b89\n");
//		USB_RevC->DataOut("USB: Cmd1b89 \r\n", 64);
//		tcp_send_data_sang("Wifi: Cmd1b89 \r\n",18);
//		BT_RevC->DataOut("Spp: Cmd1b89 \r\n",17);
#endif
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
#if 1	
	BufferPrint[cBufferPrintLineWrite].Step = PaperCount+StepFrontToPrint; // ����ֽStepFrontToPrint��ȷ��ֽ���˳�ȥ
	BufferPrint[cBufferPrintLineWrite].Dir = sPaperDirBackToFront;
				
	if(cBufferPrintLineWrite >= (BufferPrintLineNum-1))
		cBufferPrintLineWrite = 0;
	else
		cBufferPrintLineWrite++;
	PaperState = sPaperFrontEnd;
#endif
#if SendCmd
	printf("Cmd1b9c\n");
//	USB_RevC->DataOut("USB: Cmd1b9c \r\n", 64);
//	tcp_send_data_sang("Wifi: Cmd1b9c \r\n",18);
//	BT_RevC->DataOut("Spp: Cmd1b9c \r\n",17);
#endif
}
void Cmd1ba0(unsigned char data)
{
	Command.Type = eCmdInit;
	Command.State = 0;
	Command.Size = 0;
#if SendCmd
	printf("Cmd1ba0\n");
//	USB_RevC->DataOut("USB: Cmd1ba0 \r\n", 64);
//	tcp_send_data_sang("Wifi: Cmd1ba0 \r\n",18);
//	BT_RevC->DataOut("Spp: Cmd1ba0 \r\n",17);
#endif
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
#if 0	
	BufferPrint[cBufferPrintLineWrite].Step = StepPageLenth+StepFrontToPrint*2; // ����ֽStepFrontToPrint��ȷ��ֽ���˳�ȥ
	BufferPrint[cBufferPrintLineWrite].Dir = sPaperDirBackToFront;
				
	if(cBufferPrintLineWrite >= (BufferPrintLineNum-1))
		cBufferPrintLineWrite = 0;
	else
		cBufferPrintLineWrite++;
	PaperState = sPaperFrontEnd;
#endif
#if SendCmd
	printf("Cmd1d56CutPaper\n");
//	USB_RevC->DataOut("USB: Cmd1d56CutPaper \r\n", 64);
//	tcp_send_data_sang("Wifi: Cmd1d56CutPaper \r\n",26);
//	BT_RevC->DataOut("Spp: Cmd1d56CutPaper \r\n",25);
#endif
}

// �������ֽڽ����������
//void InstructionParser(volatile unsigned char data)//�������
// �˺���Ϊ���յ�һ���ֽ����ݽ��н�������ǰ����Ԥ���жϴ�ӡ�������пհ׿���д�룬�������USB/WIFI/BL����æֹͣ��������
void DealData(unsigned char data) //�������
{
#if 0
printf("%c",data);
//printf(data);
//printf("DealData-%d\n", data);
return;
#endif
	switch(Command.Type)
	{
	case eCmd0dPrint:
		OneByteCommand0d:
		Cmd0dPrint();
		break;

	case eCmd1bClass:
		Cmd1bClass(data);
		break;

	case eCmd1b0a:
		Cmd1b0a(data);
		break;

	case eCmd1b0b:
		Cmd1b0b(data);
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

