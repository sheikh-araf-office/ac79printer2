#include "os/os_api.h"
#include "usb/device/usb_stack.h"
#include "usb/device/hid.h"
#include "usb_config.h"
#include "app_config.h"
#include "circular_buf.h"
#include "WeiTingCommon.h"

#define USBMAXBufferReceiveSize			30000//10000//30000//10000 @20230618Alke
#define USBRevBusyLevelValue			3000//1000//3000//1000
#define USBRevRefreshLevelValue			9000//5000//9000//5000



extern struct MyUSBPrinter MUP;
void USBInitial(void);
void USBSaveData(unsigned    long  sizebytes,unsigned char *BufferReceive);
unsigned long USBBufferGetEmptySize(void);

void USBPrinterTask(void *p);
void USB_rx_data(struct usb_device_t *usb_device, u32 ep);
void USBDataProcesser(void);
unsigned char USBCheckBufferEmptySize(void);

void USBDataIn(void);
void USBRefreshRev(void);
//void PrinterTask(unsigned char data);






