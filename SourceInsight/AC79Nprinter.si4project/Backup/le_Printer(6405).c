#include "system/app_core.h"
#include "system/includes.h"
#include "event/net_event.h"
#include "app_config.h"

#include "btstack/btstack_task.h"
#include "btstack/bluetooth.h"
#include "btcontroller_modules.h"
#include "bt_common.h"
#include "system/wait.h"
#include "third_party/common/ble_user.h"
#include "le_net_cfg.h"
#include "le_common.h"
#include "le_Printer.h"

#include "Driver.h"
#include "McuInt.h"
#include "UserInterface.h"

void LEInitial(void);
void LESaveData(unsigned long  sizebytes,unsigned char *BufferReceive);
unsigned long LEBufferGetEmptySize(void);
unsigned char LECheckBufferEmptySize(void);
void LEDataProcesser(void);
void LEDataIn(void);
void LERefreshRev(void);
void LEDataOUT(unsigned char *buffer,unsigned int len);
extern hci_con_handle_t HCI_le_Printer;

Device_Rev LE_Rev=
{
	LEInitial,
	LESaveData,
	LEBufferGetEmptySize,
	LECheckBufferEmptySize,
	LEDataProcesser,
	LEDataIn,
	LEDataOUT,
	LERefreshRev,
	1,				//write id
	0,				//read id
	0,				//waiting flag
	0,				//unused for BT
	0,				//unused for BT
	NULL,			//receive buffer
	false,   		//connectstate
	0      			//Total receive data len
};
Device_Rev *LE_RevC;

void LEInitial(void)
{
	LE_RevC->PBufferRev=(unsigned char*)malloc(LEMAXBufferReceiveSize);
	task_create(LEPrinterTask, NULL, "LEPrinterTask");
}
void LESaveData(unsigned long  sizebytes,unsigned char *BufferReceive)
{
    unsigned int i;
	if((LE_RevC->Pwrite+sizebytes)<=LEMAXBufferReceiveSize)
	{
		memcpy(&LE_RevC->PBufferRev[LE_RevC->Pwrite],BufferReceive,sizebytes);
		if((LE_RevC->Pwrite+sizebytes)==LEMAXBufferReceiveSize)
		{
			LE_RevC->Pwrite=0;
		}
		else
		{
			LE_RevC->Pwrite+=sizebytes;
		}
	}
	else
	{
		memcpy(&LE_RevC->PBufferRev[LE_RevC->Pwrite],BufferReceive,LEMAXBufferReceiveSize-LE_RevC->Pwrite);
		memcpy(LE_RevC->PBufferRev,&BufferReceive[LEMAXBufferReceiveSize-LE_RevC->Pwrite],
		LE_RevC->Pwrite+sizebytes-LEMAXBufferReceiveSize);
		LE_RevC->Pwrite=LE_RevC->Pwrite+sizebytes-LEMAXBufferReceiveSize;

	}
}
unsigned long LEBufferGetEmptySize(void)
{
	if(LE_RevC->Pread<LE_RevC->Pwrite)
	{
		return LEMAXBufferReceiveSize-LE_RevC->Pwrite+LE_RevC->Pread;
	}
	else
	{
        return LE_RevC->Pread-LE_RevC->Pwrite;

	}
}
void LEDataIn(void)
{
	att_server_flow_hold(HCI_le_Printer,0);//enable data in
}

void LEDataOUT(unsigned char *buffer,unsigned int len)
{
;
}

unsigned char LECheckBufferEmptySize(void)
{
	if(LE_RevC->BuferGetEmptySize()<LERevBusyLevelValue)
	{
		return true;
	}
	return false;
}
void LERefreshRev(void)
{
	if(LE_RevC->Waitting)
	{
		if(LE_RevC->BuferGetEmptySize()>LERevRefreshLevelValue)
		{

			LE_RevC->Waitting=false;
        #if DebugMsg
			printf("sang:BT(BLE) busy clear:%d..............................\r\n",LE_RevC->BuferGetEmptySize());
        #endif
			LE_RevC->DataIn();
		}
	}

}
// BLE
void LEPrinterInstructionSet(unsigned char data)
{
    // Alke @20230726 此处不能有BLE回传数据，BLE回传通道太多，需要APP配合，比较复杂，暂不处理
	DealData(data);
}
//extern void DealData(unsigned char data);
void LEDataProcesser(void)
{
	unsigned int loopRev=0;

	while(1)
	{
	    if(BufferPrintLineWrited() > BufferPrintLineFull) // Alke @20230629
            return;

		if((LE_RevC->Pread+1)<LE_RevC->Pwrite)
		{
			++LE_RevC->Pread;
            LEPrinterInstructionSet(LE_RevC->PBufferRev[LE_RevC->Pread]);
            LE_RevC->ReVRecordTotalLen++;
		}
		else
		{
			if(LE_RevC->Pread>LE_RevC->Pwrite)
			{
				if(LE_RevC->Pread==(LEMAXBufferReceiveSize-1))
				{
					if(LE_RevC->Pwrite>0)
					{
					    LE_RevC->Pread=0;
                        LEPrinterInstructionSet(LE_RevC->PBufferRev[LE_RevC->Pread]);
                        LE_RevC->ReVRecordTotalLen++;
					}
					else
					{
						return;
					}
				}
				else
				{
				    ++LE_RevC->Pread;
                    LEPrinterInstructionSet(LE_RevC->PBufferRev[LE_RevC->Pread]);
                    LE_RevC->ReVRecordTotalLen++;
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
------LE Bluetooth main task------
*/
void LEPrinterTask(void *p)
{
	int res;
	int msg[16];

#if DebugMsg
	printf("sang:BLE-PrinterTask....\n");
#endif
	while(1)
	{
		LE_RevC->RefreshRev();
		LE_RevC->DataProcesser();
		res = __os_taskq_pend(msg, ARRAY_SIZE(msg), 1);
	}
}

/*
---------------------------Bluetooth entrance fuction--------------------------------------------------
*/
void PrinterLEStart(void)
{
	LE_RevC=&LE_Rev;			//load the Bluetooth exclusive function
	LE_RevC->Initial();			//Initialize the Printer OEM data and task
}

