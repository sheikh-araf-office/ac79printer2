#include "app_config.h"
#include "spp_user.h"
#include "bt_common.h"
#include "btstack/avctp_user.h"
#include "system/timer.h"
#include "system/sys_time.h"
#include "BTPrinter.h"

#include "Driver.h"
#include "McuInt.h"
#include "UserInterface.h"

void BTInitial(void);
void BTSaveData(unsigned long  sizebytes,unsigned char *BufferReceive);
unsigned long BTBufferGetEmptySize(void);
unsigned char BTCheckBufferEmptySize(void);
void BTDataProcesser(void);
void BTDataIn(void);
void BTRefreshRev(void);
void BTDataOUT(unsigned char *buffer,unsigned int len);



/* --------BT call back function receive data------------*/
extern u16 spp_channel;
void transport_spp_recieve_cbk(void *priv, u8 *buf, u16 len)
{
    spp_channel = (u16)priv;
	BTSaveData(len,buf);		//save BT data
	if(BT_RevC->CheckBufferEmptySize()&&BT_RevC->Waitting==false)
	{

		BT_RevC->Waitting=true;
		transport_spp_flow_enable(1);// disable data in
    #if DebugMsg
		printf("sang:BT(SPP) busy set:%d..............................\r\n",BT_RevC->BuferGetEmptySize());
    #endif
	}
    clear_sniff_cnt();
}

void BTInitial(void)
{
	BT_RevC->PBufferRev=(unsigned char*)malloc(BTMAXBufferReceiveSize);
	task_create(BTPrinterTask, NULL, "BTPrinterTask");
}
void BTSaveData(unsigned long  sizebytes,unsigned char *BufferReceive)
{
	unsigned int i;

	if((BT_RevC->Pwrite+sizebytes)<=BTMAXBufferReceiveSize)
	{
		memcpy(&BT_RevC->PBufferRev[BT_RevC->Pwrite], BufferReceive, sizebytes);
		if((BT_RevC->Pwrite+sizebytes)==BTMAXBufferReceiveSize)
			BT_RevC->Pwrite=0;
		else
			BT_RevC->Pwrite=BT_RevC->Pwrite+sizebytes;
	}
	else
	{
		memcpy(&BT_RevC->PBufferRev[BT_RevC->Pwrite],BufferReceive,BTMAXBufferReceiveSize-BT_RevC->Pwrite);
		memcpy(BT_RevC->PBufferRev,&BufferReceive[BTMAXBufferReceiveSize-BT_RevC->Pwrite], BT_RevC->Pwrite+sizebytes-BTMAXBufferReceiveSize);
		BT_RevC->Pwrite=BT_RevC->Pwrite+sizebytes-BTMAXBufferReceiveSize;

	}
}
unsigned long BTBufferGetEmptySize(void)
{
	if(BT_RevC->Pread<=BT_RevC->Pwrite)
	{
		return BTMAXBufferReceiveSize+BT_RevC->Pread-BT_RevC->Pwrite;
	}
	else
	{
        return BT_RevC->Pread-BT_RevC->Pwrite;

	}
}
void BTDataIn(void)
{
	transport_spp_flow_enable(0);// enable data in
}
void BTDataOut(unsigned char *buffer,unsigned int len)
{
#if DebugMsg
    printf("transport_spp_send_dataSang:%d\r\n",transport_spp_send_dataSang(buffer,len));
#endif
	//transport_spp_send_dataSang(buffer,len);
    clear_sniff_cnt();
}

unsigned char BTCheckBufferEmptySize(void)
{
	//if(BT_RevC->BuferGetEmptySize()<BTRevBusyLevelValue) // 桑子金原来的
	if(BTBufferGetEmptySize()<BTRevBusyLevelValue) // @Alke 改动的 @20231018
	{
		return true;
	}
	return false;
}
void BTRefreshRev(void)
{
	if(BT_RevC->Waitting)
	{
		if(BT_RevC->BuferGetEmptySize()>BTRevRefreshLevelValue)
		{

			BT_RevC->Waitting=false;
        #if DebugMsg
			printf("sang:BT(SPP) busy clear:%d..............................\r\n",BT_RevC->BuferGetEmptySize());
		#endif
            BT_RevC->DataIn();
		}
	}

}

// SPP
void PrinterInstructionSet(unsigned char data)
{
//u8 cc;
//cc=255-data;
	// Alke @20230726 此处可以有SPP回传数据，单个字节返回，效果一般
	DealData(data);
//	BT_RevC->DataOut(&"Router_RSSI=%d,Quality=%d \r\n",wifi_get_rssi(), wifi_get_cqi(), 64)
#if DebugChannelRerutn
	BT_RevC->DataOut(&data, 1);
#endif
}
void BTDataProcesser(void)
{
	unsigned int loopRev=0;

	while(1)
	{
		if(BufferPrintLineWrited() > BufferPrintLineFull) // Alke @20230629
			return;

		// R==W: 空
		if(BT_RevC->Pread==BT_RevC->Pwrite) //空的返回
			return;
		else								// 有数据
		{
			// 只有有数据进来，才会固定当前数据通道
			if((DataFromChannel!=0)&&(DataFromChannel!=3))
				return;
			DataFromChannel = 3;
		
			PrinterInstructionSet(BT_RevC->PBufferRev[BT_RevC->Pread]);
			BT_RevC->ReVRecordTotalLen++;
			if(BT_RevC->Pread==(BTMAXBufferReceiveSize-1))
				BT_RevC->Pread = 0;
			else
				BT_RevC->Pread++;
		}

		loopRev++;
		if(loopRev>5000)
			return;
	}
}


/*
------Bluetooth main task------
*/
void BTPrinterTask(void *p)
{
	int res;
	int msg[16];
static int cc;
	char rssi = 0; // 负数
	u8 cqi = 0;
	u8 SppSend[18];
	
//return; // ALke 这样会锁死MCU
	res = __os_taskq_pend(msg, ARRAY_SIZE(msg), 100);
	printf("sang:BT(SPP) PrinterTask....");
	while(1)
	{
		BT_RevC->RefreshRev();
		BT_RevC->DataProcesser();
		res = __os_taskq_pend(msg, ARRAY_SIZE(msg), 1);

	
		if(cc<50)
			cc++;
		else
		{
			cc = 0;
			rssi=wifi_get_rssi();
			cqi=wifi_get_cqi(); //侦测路由器端信号质量

			// 00:0 7f:127 80:-128 ff:-1
			//printf("%d-0x%x %d-0x%x %d-0x%x %d-0x%x \r\n", 0,0, 127,127, -1,-1, -128,-128);
    		//printf("Router_RSSI=%d,Quality=%d \r\n", wifi_get_rssi(), wifi_get_cqi());
			//printf("RSSI:%d CQI:%d \r\n", rssi, cqi);
			
			SppSend[0]='R';
			SppSend[1]='S';
			SppSend[2]='S';
			SppSend[3]='I';
			SppSend[4]=':';
			if(rssi&0x80)
			{
				rssi &= ~0x80;
				rssi = 128-rssi;
				SppSend[5]='-';
				SppSend[6]='0'+rssi/100;
				SppSend[7]='0'+(rssi%100)/10;
				SppSend[8]='0'+rssi%10;
				SppSend[9]=' ';
			}
			else
			{
				SppSend[5]='+';
				SppSend[6]='0'+rssi/100;
				SppSend[7]='0'+(rssi%100)/10;
				SppSend[8]='0'+rssi%10;
				SppSend[9]=' ';	
			}
			SppSend[10]='C';
			SppSend[11]='Q';
			SppSend[12]='I';
			SppSend[13]=':';
			SppSend[14]='0'+cqi/100;
			SppSend[15]='0'+(cqi%100)/10;
			SppSend[16]='0'+cqi%10;
			SppSend[17]=' ';
			// @20231024 始终未能实现数据返回
			//BT_RevC->DataOut(&SppSend, 1);
			//BT_RevC->DataOut(&SppSend, 18);
			//transport_spp_send_dataSang(SppSend, 18);
			//transport_spp_send_data(SppSend, 18);
		    //clear_sniff_cnt();
			//printf(&SppSend, 18); // only this can use return the wifi singal qulatity
		}
	}
}
Device_Rev BT_Rev=
{
	BTInitial,
	BTSaveData,
	BTBufferGetEmptySize,
	BTCheckBufferEmptySize,
	BTDataProcesser,
	BTDataIn,
	BTDataOut,
	BTRefreshRev,
	0,				//write id // @Alke init same 初始化必须相等
	0,				//read id
	0,				//waiting flag
	0,				//unused for BT
	0,				//unused for BT
	NULL,			//receive buffer
	false,   		//connectstate
	0      			//Total receive data len
};
Device_Rev *BT_RevC;
/*
---------------------------Bluetooth entrance fuction--------------------------------------------------
*/
void PrinterBTStart(void)
{
	BT_RevC=&BT_Rev;			//load the Bluetooth exclusive function
	BT_RevC->Initial();			//Initialize the Printer OEM data and task
}



