#include "typedef.h"

#include "app_config.h"
#include "system/includes.h"
#include "wifi/wifi_connect.h"
#include "lwip.h"
#include "dhcp_srv/dhcp_srv.h"
#include "event/net_event.h"
#include "net/assign_macaddr.h"
#include "syscfg_id.h"
#include "lwip/sockets.h"
#include "wifi_app_task_Print.h"
#include "sock_api/sock_api.h"
#include "os/os_api.h"
#include "lwip/netdb.h"
#include "UsbPrinter.h"
#include "device/gpio.h"
#include "system/timer.h"


#include "Printer/Driver.h"
#include "Printer/McuInt.h"
#include "Printer/UserInterface.H"

//#include "..\\apps\wifi_story_machine/user/UserInterface.h"
//#include "..\\user/McuIO.h"
//#include "..\\apps\wifi_story_machine/user/McuIO.h"
void USBDataOut(unsigned char *buffer,unsigned int len);
unsigned long USBBuferGetEmptySize(void);
static void Putbyte(char a)
{
	int msg[16],ret = 0;

	while ((JL_UART0->CON0 & BIT(15)) == 0)
	{
		ret = __os_taskq_pend(msg, ARRAY_SIZE(msg), 1);
	}

	JL_UART0->BUF = a;
	while ((JL_UART0->CON0 & BIT(15)) == 0)
	{
	}
	JL_UART0->CON0 |= BIT(13);//clear pending
}

//extern void USBInitial(void);
Device_Rev USB_Rev=
{
	USBInitial,
	USBSaveData,
	USBBuferGetEmptySize,
	USBCheckBufferEmptySize,
	USBDataProcesser,
	USBDataIn,
	USBDataOut,
	USBRefreshRev,
	0,	 // @Alke init same 初始化必须相等
	0,
	0,
	0,
	0,
	0,
	0
};
Device_Rev *USB_RevC;
unsigned long TTU1=0,TTU2=0;


void USBInitial(void)
{
	USB_RevC->PBufferRev=(unsigned char*)malloc(USBMAXBufferReceiveSize);
	task_create(USBPrinterTask, NULL, "USBPrinterTask");
}
void USBSaveData(unsigned    long  sizebytes,unsigned char *BufferReceive)
{
    unsigned int i;
	TTU2+=sizebytes;
//printf("U0");
	if((USB_RevC->Pwrite+sizebytes)<=USBMAXBufferReceiveSize)
	{
		memcpy(&USB_RevC->PBufferRev[USB_RevC->Pwrite], BufferReceive, sizebytes); 
		if((USB_RevC->Pwrite+sizebytes)==USBMAXBufferReceiveSize)
			USB_RevC->Pwrite=0;
		else
			USB_RevC->Pwrite+=sizebytes;
//printf("U1");
	}
	else
	{
		memcpy(&USB_RevC->PBufferRev[USB_RevC->Pwrite], BufferReceive, USBMAXBufferReceiveSize-USB_RevC->Pwrite);
		memcpy(USB_RevC->PBufferRev, &BufferReceive[USBMAXBufferReceiveSize-USB_RevC->Pwrite], USB_RevC->Pwrite+sizebytes-USBMAXBufferReceiveSize);
		USB_RevC->Pwrite=USB_RevC->Pwrite+sizebytes-USBMAXBufferReceiveSize;
//printf("U2");
	}
}

unsigned long USBBuferGetEmptySize(void)
{
	if(USB_RevC->Pread<USB_RevC->Pwrite)
	{
		return USBMAXBufferReceiveSize-USB_RevC->Pwrite+USB_RevC->Pread;
	}
	else
	{
        return USB_RevC->Pread-USB_RevC->Pwrite;

	}
}
unsigned char USBCheckBufferEmptySize(void)
{
	if(USB_RevC->BuferGetEmptySize()<USBRevBusyLevelValue)
	//if(USBBuferGetEmptySize()<USBRevBusyLevelValue) // @Alke @20231018 
	{
		return true;
	}
	return false;
}
void USBRefreshRev(void)
{
	if(USB_RevC->Waitting)
	{
		if(USB_RevC->BuferGetEmptySize()>USBRevRefreshLevelValue)
		{
			USB_RevC->DataIn();
        #if DebugMsg
			printf("sang:USB-Busy clear..............................\r\n");
        #endif
		}
	}
}

// USB
void PrinterTask(unsigned char data)
{
//	Alke @20230726 此处绝对不能有USB回传数据，否则必定发生错误
    DealData(data);
//	printf("%c-",data);
//	USB_RevC->DataOut(&data,1);  // Alke @20230726 再次按照之前的改动
}



void USBDataProcesser(void)
{
	while(1)
	{
        if(BufferPrintLineWrited() > BufferPrintLineFull) // Alke @20230629
            return;

		// R==W: 空
		if(USB_RevC->Pread==USB_RevC->Pwrite)	//空的返回
		{
			return;
printf("r0");
		}
		else									// 有数据
		{
			PrinterTask(USB_RevC->PBufferRev[USB_RevC->Pread]);
			TTU1++; //USB_RevC->ReVRecordTotalLen++;
			if(USB_RevC->Pread==(USBMAXBufferReceiveSize-1))
				USB_RevC->Pread = 0;
			else
				USB_RevC->Pread++;
printf("r+");
		}
	}
}

void USBDataIn(void)
{
	u8 ep_buffer[MAXP_SIZE_HIDOUT];

	USB_RevC->Waitting=false;
	u32 rx_len = usb_g_bulk_read(USB_RevC->IDHandle, USB_RevC->EP, ep_buffer, MAXP_SIZE_HIDOUT,0);
	USB_RevC->SaveData(rx_len,ep_buffer);
}
void USBDataOut(unsigned char *buffer,unsigned int len)
{
    usb_g_bulk_write(USB_RevC->IDHandle, USB_RevC->EP, buffer,len);
}
void USB_rx_data(struct usb_device_t *usb_device, u32 ep)
{
    //printf("Write:%d,Read:%d",cBufferPrintLineWrite,cBufferPrintLineRead);

	USB_RevC->IDHandle=usb_device2id(usb_device);
	USB_RevC->EP=ep;
	if(USB_RevC->CheckBufferEmptySize())
	{
    #if DebugMsg
		printf("sang:USB-Busy set..............................\r\n");
    #endif
		USB_RevC->Waitting=true;
		return;
	}
	USB_RevC->DataIn();
}
void PrinterUSBStart(void *priv)
{
	USB_RevC=&USB_Rev;
	USB_RevC->Initial();
}

#define AWINLINE   __attribute__((always_inline))
#define TSEC SEC_USED(.volatile_ram_code)
void USBPrinterTask(void *p)
{
    static int res;
    int msg[16];

#if DebugOnlyData
    printf("USBPrinterTask ..............................\r\n");
 	InitialIO();
	Timer2Initial(); // system timer
    while(1)
    {
	if(!HOME)
	{
//    printf("LEDPAPER 1..............................\r\n");
    //res = 1000;
    DelayTimeCount20mS = 10;
    while(DelayTimeCount20mS)
    {
		LedLineClose
		LedErrOpen
		USB_RevC->RefreshRev();
		USB_RevC->DataProcesser();
		res = __os_taskq_pend(msg, ARRAY_SIZE(msg), 1);
    }
	}
	//else
	{
//    printf("LEDPAPER 0..............................\r\n");
    //res = 1000;
    //while(res--)
    DelayTimeCount20mS = 10;
    while(DelayTimeCount20mS)
    {
		LedLineOpen
		LedErrClose
		USB_RevC->RefreshRev();
		USB_RevC->DataProcesser();
		res = __os_taskq_pend(msg, ARRAY_SIZE(msg), 1);
    }
    }
    }
#else

    #if 0
    PowonInit();
    // PowEnb C5/C2 same pin
/*	gpio_set_die(IO_PORTC_02,1);
	gpio_set_pull_up(IO_PORTC_02,0);
	gpio_set_pull_down(IO_PORTC_02, 1);
	gpio_set_direction(IO_PORTC_02,0);
	gpio_set_die(IO_PORTC_05,1);
	gpio_set_pull_up(IO_PORTC_05,0);
	gpio_set_pull_down(IO_PORTC_05, 1);
	gpio_set_direction(IO_PORTC_05,0);
	MainPowerOn*/
    //@20230710 Sang 需要延时确保SPP BLE WIFI配置完成后才能初始化马达等工作
 	res = __os_taskq_pend(msg, ARRAY_SIZE(msg), 500); // @20230710 Alke 1000

    PrinterInit();
    res = __os_taskq_pend(msg, ARRAY_SIZE(msg), 500); // @20230710 Alke 1000

    while(1)
    {
    	USB_RevC->RefreshRev();
    	USB_RevC->DataProcesser();
		res = __os_taskq_pend(msg, ARRAY_SIZE(msg), 1);
    }
    #else
    PowonInit();

	// 此处延时为了WIFI配置，马达运行可能会影响WIFI信号
	DelayTimeCount20mS = 50*2;
    while(DelayTimeCount20mS)
    {
    	USB_RevC->RefreshRev();
    	USB_RevC->DataProcesser();
		res = __os_taskq_pend(msg, ARRAY_SIZE(msg), 1);
    }
printf("PrinterInit(); ..............................--------------------------------------------------------\r\n");
    PrinterInit();

    DelayTimeCount20mS = 50*2;
    while(DelayTimeCount20mS)
    {
    	USB_RevC->RefreshRev();
    	USB_RevC->DataProcesser();
		res = __os_taskq_pend(msg, ARRAY_SIZE(msg), 1);
    }
printf("Running..............................-------------------------------------------------------------\r\n");

    while(1)
    {
    	USB_RevC->RefreshRev();
    	USB_RevC->DataProcesser();
		res = __os_taskq_pend(msg, ARRAY_SIZE(msg), 1);
    }
    #endif
#endif
}

late_initcall(PrinterUSBStart);




