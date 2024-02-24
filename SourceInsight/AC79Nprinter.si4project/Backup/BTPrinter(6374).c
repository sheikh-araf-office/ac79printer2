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
		memcpy(&BT_RevC->PBufferRev[BT_RevC->Pwrite],BufferReceive,sizebytes);
		if((BT_RevC->Pwrite+sizebytes)==BTMAXBufferReceiveSize)
		{
			BT_RevC->Pwrite=0;
		}
		else
		{
			BT_RevC->Pwrite+=sizebytes;
		}
	}
	else
	{
		memcpy(&BT_RevC->PBufferRev[BT_RevC->Pwrite],BufferReceive,BTMAXBufferReceiveSize-BT_RevC->Pwrite);
		memcpy(BT_RevC->PBufferRev,&BufferReceive[BTMAXBufferReceiveSize-BT_RevC->Pwrite],
		BT_RevC->Pwrite+sizebytes-BTMAXBufferReceiveSize);
		BT_RevC->Pwrite=BT_RevC->Pwrite+sizebytes-BTMAXBufferReceiveSize;

	}
}
unsigned long BTBufferGetEmptySize(void)
{
	if(BT_RevC->Pread<BT_RevC->Pwrite)
	{
		return BTMAXBufferReceiveSize-BT_RevC->Pwrite+BT_RevC->Pread;
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
    clear_sniff_cnt();
}

unsigned char BTCheckBufferEmptySize(void)
{
	if(BT_RevC->BuferGetEmptySize()<BTRevBusyLevelValue)
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
    // Alke @20230726 此处可以有SPP回传数据，单个字节返回，效果一般
	DealData(data);
#if DebugChannelRerutn
	BT_RevC->DataOut(&data,1);
#endif
}
void BTDataProcesser(void)
{
	unsigned int 		loopRev=0;
	while(1)
	{
        if(BufferPrintLineWrited() > BufferPrintLineFull) // Alke @20230629
            return;

		if((BT_RevC->Pread+1)<BT_RevC->Pwrite)
		{
            ++BT_RevC->Pread;
            PrinterInstructionSet(BT_RevC->PBufferRev[BT_RevC->Pread]);
            BT_RevC->ReVRecordTotalLen++;
		}
		else
		{
			if(BT_RevC->Pread>BT_RevC->Pwrite)
			{
				if(BT_RevC->Pread==(BTMAXBufferReceiveSize-1))
				{
					if(BT_RevC->Pwrite>0)
					{
                        BT_RevC->Pread=0;
                        PrinterInstructionSet(BT_RevC->PBufferRev[BT_RevC->Pread]);
                        BT_RevC->ReVRecordTotalLen++;
					}
					else
					{
						return;
					}
				}
				else
				{
                    ++BT_RevC->Pread;
                    PrinterInstructionSet(BT_RevC->PBufferRev[BT_RevC->Pread]);
                    BT_RevC->ReVRecordTotalLen++;
				}
			}
			else
			{
				return;
			}
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

	res = __os_taskq_pend(msg, ARRAY_SIZE(msg), 100);
	printf("sang:BT(SPP) PrinterTask....");
	while(1)
	{
		BT_RevC->RefreshRev();
		BT_RevC->DataProcesser();
		res = __os_taskq_pend(msg, ARRAY_SIZE(msg), 1);
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
	1,				//write id
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



