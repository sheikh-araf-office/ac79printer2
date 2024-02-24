
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


extern u8 sDeviceDescriptor[];

// @20230601 按照新的程序架构，此函数为收到数据后的触发事件
// 此函数按照一个字节一个字节进行处理，此过程中不能有死循环，只能一次过，可以设置标志位

// @20230713 Alke USB回传在此应用中已经可以
extern Device_Rev *USB_RevC;
unsigned char UsbDataBack[64] = {"Usb to PC!\r\n"};
unsigned char cUsbDataBack = 0;

// @20230713 Alke Wifi回传在此应用中仍然不行，但在Wifi层是可以的
extern int tcp_send_data_sang(const void *buf, u32 len);//

// @20230713 Alke  Spp回传数据，
// @20230713 Alke  BLE有很多通道不处理
//BT_RevC->DataOut(&data,1);
extern Device_Rev *BT_RevC;

struct netif_info netif_infoA;
extern void lwip_get_netif_info(u8 is_wireless, struct netif_info *info);



// 0x1B 0X0B 0X01 USB Printer！ 0X0A // 及时表明USB身份
// 0x1B 0X0B 0X02 WIFI SSID+PWD！ 0X0A // 获得SSID+PWD及时回复


// 0x1B 0X0B 0X03 IP"192.168.0.105" 0X0A // 3 WIFI_EVENT_STA_NETWORK_STACK_DHCP_SUCC就是上面的3状态，即获得了IP地址，USB设置网络完成，后面就切换到WIFI通讯

// 0x1B 0X0B 0X06 NOT_FOUND_SSID Try restarting the printer 0X0A
// 0x1B 0X0B 0X07 ASSOCIAT_FAIL 0X0A
// 0x1B 0X0B 0X08 ASSOCIAT_TIMEOUT 0X0A
// 0x1B 0X0B 0X09 DHCP_TIMEOUT 0X0A


// 0x1B 0X0B 0X05 Mode&PID&IP&MCUID&Printer 0X0A // WIFI中询问时回复的信息，需要根据相关信息安装打印机驱动程序
void USBReturnWifi(unsigned char Step) // 最多64字节
{
	u8 flag = 0;
	u8 c8cc = 0;
	u8 c8d1 = 0;

	c8cc = 0;
	switch (Step)
	{
	default:
	case 1:
		UsbDataBack[c8cc++] = 0x1b;
		UsbDataBack[c8cc++] = 0x0b;
		UsbDataBack[c8cc++] = 0x12;
		UsbDataBack[c8cc++] = 'U';
		UsbDataBack[c8cc++] = 'S';
		UsbDataBack[c8cc++] = 'B';
		UsbDataBack[c8cc++] = ' ';
		UsbDataBack[c8cc++] = 'P';
		UsbDataBack[c8cc++] = 'r';
		UsbDataBack[c8cc++] = 'i';
		UsbDataBack[c8cc++] = 'n';
		UsbDataBack[c8cc++] = 't';
		UsbDataBack[c8cc++] = 'e';
		UsbDataBack[c8cc++] = 'r';
		UsbDataBack[c8cc++] = '!';
		UsbDataBack[c8cc++] = 0x0a;
		break;
	/*case 1:
		UsbDataBack[c8cc++] = 0x1b;
		UsbDataBack[c8cc++] = 0x0b;
		UsbDataBack[c8cc++] = 0x01;
		UsbDataBack[c8cc++] = 'W';
		UsbDataBack[c8cc++] = 'I';
		UsbDataBack[c8cc++] = 'F';
		UsbDataBack[c8cc++] = 'I';
		UsbDataBack[c8cc++] = ' ';
		UsbDataBack[c8cc++] = 'C';
		UsbDataBack[c8cc++] = 'O';
		UsbDataBack[c8cc++] = 'N';
		UsbDataBack[c8cc++] = 'N';
		UsbDataBack[c8cc++] = 'E';
		UsbDataBack[c8cc++] = 'C';
		UsbDataBack[c8cc++] = 'T';
		UsbDataBack[c8cc++] = 'E';
		UsbDataBack[c8cc++] = 'D';
		UsbDataBack[c8cc++] = '！';
		UsbDataBack[c8cc++] = 0x0a;
		break;*/

	case 2:
		UsbDataBack[c8cc++] = 0x1b;
		UsbDataBack[c8cc++] = 0x0b;
		UsbDataBack[c8cc++] = 0x02;
		UsbDataBack[c8cc++] = 'W';
		UsbDataBack[c8cc++] = 'I';
		UsbDataBack[c8cc++] = 'F';
		UsbDataBack[c8cc++] = 'I';
		UsbDataBack[c8cc++] = ' ';
		UsbDataBack[c8cc++] = 'S';
		UsbDataBack[c8cc++] = 'S';
		UsbDataBack[c8cc++] = 'I';
		UsbDataBack[c8cc++] = 'D';
		UsbDataBack[c8cc++] = '+';
		UsbDataBack[c8cc++] = 'P';
		UsbDataBack[c8cc++] = 'W';
		UsbDataBack[c8cc++] = 'D';
		UsbDataBack[c8cc++] = '!';
		UsbDataBack[c8cc++] = 0x0a;
		break;

	case 3:
		lwip_get_netif_info(1, &netif_infoA);

		UsbDataBack[c8cc++] = 0x1b;
		UsbDataBack[c8cc++] = 0x0b;
		UsbDataBack[c8cc++] = 0x03;
		UsbDataBack[c8cc++] = 'I';
		UsbDataBack[c8cc++] = 'P';
		UsbDataBack[c8cc++] = '"';

		flag = 0; // 非0数据，后面为0也得显示
		c8d1 = netif_infoA.ip&0x000000ff;
		if(c8d1 > 99)
			{UsbDataBack[c8cc++] = 0x30+c8d1/100; flag = 1;}
		c8d1 %= 100;
		if(c8d1 > 9)
			UsbDataBack[c8cc++] = 0x30+c8d1/10;
		else if(flag) // 首位非0，此时0也得显示
			UsbDataBack[c8cc++] = 0x30;
		c8d1 %= 10;
		UsbDataBack[c8cc++] = 0x30+c8d1;
		UsbDataBack[c8cc++] = '.';

		flag = 0;
		c8d1 = (netif_infoA.ip>>8)&0x000000ff;
		if(c8d1 > 99)
			{UsbDataBack[c8cc++] = 0x30+c8d1/100; flag = 1;}
		c8d1 %= 100;
		if(c8d1 > 9)
			UsbDataBack[c8cc++] = 0x30+c8d1/10;
		else if(flag)
			UsbDataBack[c8cc++] = 0x30;
		c8d1 %= 10;
		UsbDataBack[c8cc++] = 0x30+c8d1;
		UsbDataBack[c8cc++] = '.';

		flag = 0;
		c8d1 = (netif_infoA.ip>>16)&0x000000ff;
		if(c8d1 > 99)
			{UsbDataBack[c8cc++] = 0x30+c8d1/100; flag = 1;}
		c8d1 %= 100;
		if(c8d1 > 9)
			UsbDataBack[c8cc++] = 0x30+c8d1/10;
		else if(flag)
			UsbDataBack[c8cc++] = 0x30;
		c8d1 %= 10;
		UsbDataBack[c8cc++] = 0x30+c8d1;
		UsbDataBack[c8cc++] = '.';

		flag = 0;
		c8d1 = (netif_infoA.ip>>24)&0x000000ff;
		if(c8d1 > 99)
			{UsbDataBack[c8cc++] = 0x30+c8d1/100; flag = 1;}
		c8d1 %= 100;
		if(c8d1 > 9)
			UsbDataBack[c8cc++] = 0x30+c8d1/10;
		else if(flag)
			UsbDataBack[c8cc++] = 0x30;
		c8d1 %= 10;
		UsbDataBack[c8cc++] = 0x30+c8d1;

		UsbDataBack[c8cc++] = '"';
		UsbDataBack[c8cc++] = 0x0a; // '\n';
		break;

	case 4:
		UsbDataBack[c8cc++] = 0x1b;
		UsbDataBack[c8cc++] = 0x0b;
		UsbDataBack[c8cc++] = 0x04;
		UsbDataBack[c8cc++] = 'W';
		UsbDataBack[c8cc++] = 'I';
		UsbDataBack[c8cc++] = 'F';
		UsbDataBack[c8cc++] = 'I';
		UsbDataBack[c8cc++] = ' ';
		UsbDataBack[c8cc++] = 'R';
		UsbDataBack[c8cc++] = 'E';
		UsbDataBack[c8cc++] = 'A';
		UsbDataBack[c8cc++] = 'D';
		UsbDataBack[c8cc++] = 'Y';
		UsbDataBack[c8cc++] = '!';
		UsbDataBack[c8cc++] = 0x0a;
		break;

	case 5:
		lwip_get_netif_info(1, &netif_infoA);

		UsbDataBack[c8cc++] = 0x1b;
		UsbDataBack[c8cc++] = 0x0b;
		UsbDataBack[c8cc++] = 0x05;
		UsbDataBack[c8cc++] = 'W';
		UsbDataBack[c8cc++] = 'i';
		UsbDataBack[c8cc++] = 'f';
		UsbDataBack[c8cc++] = 'i';
		UsbDataBack[c8cc++] = 'P';
		UsbDataBack[c8cc++] = 'r';
		UsbDataBack[c8cc++] = 'i';
		UsbDataBack[c8cc++] = 'n';
		UsbDataBack[c8cc++] = 't';
		UsbDataBack[c8cc++] = 'e';
		UsbDataBack[c8cc++] = 'r';
		UsbDataBack[c8cc++] = '&';
		c8d1 = (sDeviceDescriptor[11]&0xf0)>>4;
		UsbDataBack[c8cc++] = ((c8d1<10)?(c8d1+'0'):(c8d1-10+'a'));
		c8d1 = (sDeviceDescriptor[11]&0x0f);
		UsbDataBack[c8cc++] = ((c8d1<10)?(c8d1+'0'):(c8d1-10+'a'));
		c8d1 = (sDeviceDescriptor[10]&0xf0)>>4;
		UsbDataBack[c8cc++] = ((c8d1<10)?(c8d1+'0'):(c8d1-10+'a'));
		c8d1 = (sDeviceDescriptor[10]&0x0f);
		UsbDataBack[c8cc++] = ((c8d1<10)?(c8d1+'0'):(c8d1-10+'a'));
		UsbDataBack[c8cc++] = '&';

		flag = 0; // 非0数据，后面为0也得显示
		c8d1 = netif_infoA.ip&0x000000ff;
		if(c8d1 > 99)
			{UsbDataBack[c8cc++] = 0x30+c8d1/100; flag = 1;}
		c8d1 %= 100;
		if(c8d1 > 9)
			UsbDataBack[c8cc++] = 0x30+c8d1/10;
		else if(flag) // 首位非0，此时0也得显示
			UsbDataBack[c8cc++] = 0x30;
		c8d1 %= 10;
		UsbDataBack[c8cc++] = 0x30+c8d1;
		UsbDataBack[c8cc++] = '.';

		flag = 0;
		c8d1 = (netif_infoA.ip>>8)&0x000000ff;
		if(c8d1 > 99)
			{UsbDataBack[c8cc++] = 0x30+c8d1/100; flag = 1;}
		c8d1 %= 100;
		if(c8d1 > 9)
			UsbDataBack[c8cc++] = 0x30+c8d1/10;
		else if(flag)
			UsbDataBack[c8cc++] = 0x30;
		c8d1 %= 10;
		UsbDataBack[c8cc++] = 0x30+c8d1;
		UsbDataBack[c8cc++] = '.';

		flag = 0;
		c8d1 = (netif_infoA.ip>>16)&0x000000ff;
		if(c8d1 > 99)
			{UsbDataBack[c8cc++] = 0x30+c8d1/100; flag = 1;}
		c8d1 %= 100;
		if(c8d1 > 9)
			UsbDataBack[c8cc++] = 0x30+c8d1/10;
		else if(flag)
			UsbDataBack[c8cc++] = 0x30;
		c8d1 %= 10;
		UsbDataBack[c8cc++] = 0x30+c8d1;
		UsbDataBack[c8cc++] = '.';

		flag = 0;
		c8d1 = (netif_infoA.ip>>24)&0x000000ff;
		if(c8d1 > 99)
			{UsbDataBack[c8cc++] = 0x30+c8d1/100; flag = 1;}
		c8d1 %= 100;
		if(c8d1 > 9)
			UsbDataBack[c8cc++] = 0x30+c8d1/10;
		else if(flag)
			UsbDataBack[c8cc++] = 0x30;
		c8d1 %= 10;
		UsbDataBack[c8cc++] = 0x30+c8d1;

		UsbDataBack[c8cc++] = '&';
		UsbDataBack[c8cc++] = '0';
		UsbDataBack[c8cc++] = '1';
		UsbDataBack[c8cc++] = '2';
		UsbDataBack[c8cc++] = '3';
		UsbDataBack[c8cc++] = '4';
		UsbDataBack[c8cc++] = '5';
		UsbDataBack[c8cc++] = '6';
		UsbDataBack[c8cc++] = '7';
		UsbDataBack[c8cc++] = '8';
		UsbDataBack[c8cc++] = '9';
		UsbDataBack[c8cc++] = '&';
		UsbDataBack[c8cc++] = 'J';
		UsbDataBack[c8cc++] = 'I';
		UsbDataBack[c8cc++] = 'A';
		UsbDataBack[c8cc++] = ' ';
		UsbDataBack[c8cc++] = 'P';
		UsbDataBack[c8cc++] = 'U';
		UsbDataBack[c8cc++] = ' ';
		UsbDataBack[c8cc++] = 'W';
		UsbDataBack[c8cc++] = 'E';
		UsbDataBack[c8cc++] = 'I';
		UsbDataBack[c8cc++] = 0x0a;
		break;

	case 6:	// NOT_FIND_SSID
		UsbDataBack[c8cc++] = 0x1b;
		UsbDataBack[c8cc++] = 0x0b;
		UsbDataBack[c8cc++] = 0x06;
		UsbDataBack[c8cc++] = 'N';
		UsbDataBack[c8cc++] = 'O';
		UsbDataBack[c8cc++] = 'T';
		UsbDataBack[c8cc++] = '_';
		UsbDataBack[c8cc++] = 'F';
		UsbDataBack[c8cc++] = 'O';
		UsbDataBack[c8cc++] = 'U';
		UsbDataBack[c8cc++] = 'N';
		UsbDataBack[c8cc++] = 'D';
		UsbDataBack[c8cc++] = '_';
		UsbDataBack[c8cc++] = 'S';
		UsbDataBack[c8cc++] = 'S';
		UsbDataBack[c8cc++] = 'I';
		UsbDataBack[c8cc++] = 'D';
		UsbDataBack[c8cc++] = ' ';
		UsbDataBack[c8cc++] = 'T';
		UsbDataBack[c8cc++] = 'r';
		UsbDataBack[c8cc++] = 'y';
		UsbDataBack[c8cc++] = ' ';
		UsbDataBack[c8cc++] = 'r';
		UsbDataBack[c8cc++] = 'e';
		UsbDataBack[c8cc++] = 's';
		UsbDataBack[c8cc++] = 't';
		UsbDataBack[c8cc++] = 'a';
		UsbDataBack[c8cc++] = 'r';
		UsbDataBack[c8cc++] = 't';
		UsbDataBack[c8cc++] = 'i';
		UsbDataBack[c8cc++] = 'n';
		UsbDataBack[c8cc++] = 'g';
		UsbDataBack[c8cc++] = ' ';
		UsbDataBack[c8cc++] = 't';
		UsbDataBack[c8cc++] = 'h';
		UsbDataBack[c8cc++] = 'e';
		UsbDataBack[c8cc++] = ' ';
		UsbDataBack[c8cc++] = 'p';
		UsbDataBack[c8cc++] = 'r';
		UsbDataBack[c8cc++] = 'i';
		UsbDataBack[c8cc++] = 'n';
		UsbDataBack[c8cc++] = 't';
		UsbDataBack[c8cc++] = 'e';
		UsbDataBack[c8cc++] = 'r';
		UsbDataBack[c8cc++] = 0x0a;
		break;

	case 7:	// ASSOCIAT_FAIL
		UsbDataBack[c8cc++] = 0x1b;
		UsbDataBack[c8cc++] = 0x0b;
		UsbDataBack[c8cc++] = 0x07;
		UsbDataBack[c8cc++] = 'A';
		UsbDataBack[c8cc++] = 'S';
		UsbDataBack[c8cc++] = 'S';
		UsbDataBack[c8cc++] = 'O';
		UsbDataBack[c8cc++] = 'C';
		UsbDataBack[c8cc++] = 'I';
		UsbDataBack[c8cc++] = 'A';
		UsbDataBack[c8cc++] = 'T';
		UsbDataBack[c8cc++] = '_';
		UsbDataBack[c8cc++] = 'F';
		UsbDataBack[c8cc++] = 'A';
		UsbDataBack[c8cc++] = 'I';
		UsbDataBack[c8cc++] = 'L';
		UsbDataBack[c8cc++] = '!';
		UsbDataBack[c8cc++] = 0x0a;
		break;

	case 8:	// ASSOCIAT_TIMEOUT
		UsbDataBack[c8cc++] = 0x1b;
		UsbDataBack[c8cc++] = 0x0b;
		UsbDataBack[c8cc++] = 0x08;
		UsbDataBack[c8cc++] = 'A';
		UsbDataBack[c8cc++] = 'S';
		UsbDataBack[c8cc++] = 'S';
		UsbDataBack[c8cc++] = 'O';
		UsbDataBack[c8cc++] = 'C';
		UsbDataBack[c8cc++] = 'I';
		UsbDataBack[c8cc++] = 'A';
		UsbDataBack[c8cc++] = 'T';
		UsbDataBack[c8cc++] = '_';
		UsbDataBack[c8cc++] = 'T';
		UsbDataBack[c8cc++] = 'M';
		UsbDataBack[c8cc++] = 'E';
		UsbDataBack[c8cc++] = 'O';
		UsbDataBack[c8cc++] = 'U';
		UsbDataBack[c8cc++] = 'T';
		UsbDataBack[c8cc++] = '!';
		UsbDataBack[c8cc++] = 0x0a;
		break;

	case 9:	//DHCP_TIMEOUT
		UsbDataBack[c8cc++] = 0x1b;
		UsbDataBack[c8cc++] = 0x0b;
		UsbDataBack[c8cc++] = 0x09;
		UsbDataBack[c8cc++] = 'D';
		UsbDataBack[c8cc++] = 'H';
		UsbDataBack[c8cc++] = 'C';
		UsbDataBack[c8cc++] = 'P';
		UsbDataBack[c8cc++] = '_';
		UsbDataBack[c8cc++] = 'T';
		UsbDataBack[c8cc++] = 'I';
		UsbDataBack[c8cc++] = 'M';
		UsbDataBack[c8cc++] = 'E';
		UsbDataBack[c8cc++] = 'O';
		UsbDataBack[c8cc++] = 'U';
		UsbDataBack[c8cc++] = 'T';
		UsbDataBack[c8cc++] = '!';
		UsbDataBack[c8cc++] = 0x0a;
		break;
	}
	while(c8cc < 64)
		UsbDataBack[c8cc++] = 0;

	USB_RevC->DataOut(UsbDataBack, 64);
}
void WifiReturnMsg(void)
{
	u8 flag = 0;
	u8 c8cc = 0;
	u8 c8d1 = 0;

	lwip_get_netif_info(1, &netif_infoA);

	c8cc = 0;
	UsbDataBack[c8cc++] = 0x1b;
	UsbDataBack[c8cc++] = 0x0b;
	UsbDataBack[c8cc++] = 0x05;
	UsbDataBack[c8cc++] = 'W';
	UsbDataBack[c8cc++] = 'i';
	UsbDataBack[c8cc++] = 'f';
	UsbDataBack[c8cc++] = 'i';
	UsbDataBack[c8cc++] = 'P';
	UsbDataBack[c8cc++] = 'r';
	UsbDataBack[c8cc++] = 'i';
	UsbDataBack[c8cc++] = 'n';
	UsbDataBack[c8cc++] = 't';
	UsbDataBack[c8cc++] = 'e';
	UsbDataBack[c8cc++] = 'r';
	UsbDataBack[c8cc++] = '&';
	c8d1 = (sDeviceDescriptor[11]&0xf0)>>4;
	UsbDataBack[c8cc++] = ((c8d1<10)?(c8d1+'0'):(c8d1-10+'a'));
	c8d1 = (sDeviceDescriptor[11]&0x0f);
	UsbDataBack[c8cc++] = ((c8d1<10)?(c8d1+'0'):(c8d1-10+'a'));
	c8d1 = (sDeviceDescriptor[10]&0xf0)>>4;
	UsbDataBack[c8cc++] = ((c8d1<10)?(c8d1+'0'):(c8d1-10+'a'));
	c8d1 = (sDeviceDescriptor[10]&0x0f);
	UsbDataBack[c8cc++] = ((c8d1<10)?(c8d1+'0'):(c8d1-10+'a'));
	UsbDataBack[c8cc++] = '&';

	flag = 0; // 非0数据，后面为0也得显示
	c8d1 = netif_infoA.ip&0x000000ff;
	if(c8d1 > 99)
		{UsbDataBack[c8cc++] = 0x30+c8d1/100; flag = 1;}
	c8d1 %= 100;
	if(c8d1 > 9)
		UsbDataBack[c8cc++] = 0x30+c8d1/10;
	else if(flag) // 首位非0，此时0也得显示
		UsbDataBack[c8cc++] = 0x30;
	c8d1 %= 10;
	UsbDataBack[c8cc++] = 0x30+c8d1;
	UsbDataBack[c8cc++] = '.';

	flag = 0;
	c8d1 = (netif_infoA.ip>>8)&0x000000ff;
	if(c8d1 > 99)
		{UsbDataBack[c8cc++] = 0x30+c8d1/100; flag = 1;}
	c8d1 %= 100;
	if(c8d1 > 9)
		UsbDataBack[c8cc++] = 0x30+c8d1/10;
	else if(flag)
		UsbDataBack[c8cc++] = 0x30;
	c8d1 %= 10;
	UsbDataBack[c8cc++] = 0x30+c8d1;
	UsbDataBack[c8cc++] = '.';

	flag = 0;
	c8d1 = (netif_infoA.ip>>16)&0x000000ff;
	if(c8d1 > 99)
		{UsbDataBack[c8cc++] = 0x30+c8d1/100; flag = 1;}
	c8d1 %= 100;
	if(c8d1 > 9)
		UsbDataBack[c8cc++] = 0x30+c8d1/10;
	else if(flag)
		UsbDataBack[c8cc++] = 0x30;
	c8d1 %= 10;
	UsbDataBack[c8cc++] = 0x30+c8d1;
	UsbDataBack[c8cc++] = '.';

	flag = 0;
	c8d1 = (netif_infoA.ip>>24)&0x000000ff;
	if(c8d1 > 99)
		{UsbDataBack[c8cc++] = 0x30+c8d1/100; flag = 1;}
	c8d1 %= 100;
	if(c8d1 > 9)
		UsbDataBack[c8cc++] = 0x30+c8d1/10;
	else if(flag)
		UsbDataBack[c8cc++] = 0x30;
	c8d1 %= 10;
	UsbDataBack[c8cc++] = 0x30+c8d1;

	UsbDataBack[c8cc++] = '&';
	UsbDataBack[c8cc++] = '0';
	UsbDataBack[c8cc++] = '1';
	UsbDataBack[c8cc++] = '2';
	UsbDataBack[c8cc++] = '3';
	UsbDataBack[c8cc++] = '4';
	UsbDataBack[c8cc++] = '5';
	UsbDataBack[c8cc++] = '6';
	UsbDataBack[c8cc++] = '7';
	UsbDataBack[c8cc++] = '8';
	UsbDataBack[c8cc++] = '9';
	UsbDataBack[c8cc++] = '&';
	UsbDataBack[c8cc++] = 'J';
	UsbDataBack[c8cc++] = 'I';
	UsbDataBack[c8cc++] = 'A';
	UsbDataBack[c8cc++] = ' ';
	UsbDataBack[c8cc++] = 'P';
	UsbDataBack[c8cc++] = 'U';
	UsbDataBack[c8cc++] = ' ';
	UsbDataBack[c8cc++] = 'W';
	UsbDataBack[c8cc++] = 'E';
	UsbDataBack[c8cc++] = 'I';
	UsbDataBack[c8cc++] = 0x0a;

	while(c8cc < 64)
		UsbDataBack[c8cc++] = 0;

	tcp_send_data_sang(UsbDataBack, 64);
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
void ScanWifiCmd(u8 data)
{
	static u8 State = 0;
	static u8 Size = 0;
	u8 c8c1;
	unsigned char ssid_stored_cnt;

	struct wifi_stored_sta_infoAlke
	{
    	u8 pwd[64];
	    u8 ssid[33];
	}	wifi_stored_sta_info[1]; // Alke 20230722

	struct wifi_stored_sta_infoAlke wifi_sta_info; // Alke 20230722


	switch(State)
	{
	default:
	case 0:
		if(data == 0x1b)
			State = 1;
		break;
	case 1:
		if(data == 0x0b)
			State = 2;
		else
			State = 0;
		break;
	case 2:
		if(data == 0x0e)
		{
			State = 10;
			Size = 0;
		}
		else if(data == 0x0f)
			State = 20;
		else
			State = 0;
		break;

	case 10:
		if(data == 0x0b)
		{
			// 没有占满时，最后一位字符后面跟一个0作为结束标记
			if(Size < 33) // wifi_stored_sta_info.ssid[33]
			{
				wifi_sta_info.ssid[Size] = 0;
				Size++;
			}

			State++; // 命令有效
		}
		else
		{
			if(Size < 33)
			{
				wifi_sta_info.ssid[Size] = data;
				Size++;
			}
		}
		break;
	case 11:
		if(data == 0x0a)
		{
			// 没有占满时，最后一位字符后面跟一个0作为结束标记
			if(Size < 64) // wifi_stored_sta_info.pwd[64]
			{
				wifi_sta_info.pwd[Size] = 0;
				Size++;
			}

			if(WifiState)
				ReturnMsg = 2; //USBReturnWifi(2);
			else
				ReturnMsg = 1; //USBReturnWifi(1);

			ssid_stored_cnt = wifi_get_stored_sta_info(wifi_stored_sta_info);
    		for (int i = 0; i < ssid_stored_cnt; i++)
			{
	        	//printf("1:wifi_get_stored_sta_info[%d]= %s \r\n", i, wifi_stored_sta_info[i].ssid);
				wifi_del_stored_sta_info(wifi_stored_sta_info[i].ssid);// @ Alke 20230711 删除存储网络
	    	}
			os_time_dly(100);
			wifi_store_mode_info(STA_MODE, wifi_sta_info.ssid, wifi_sta_info.pwd);

			State = 0;
			Size = 0;
		}
		else
		{
			if(Size < 64)
			{
				wifi_sta_info.pwd[Size] = data;
				Size++;
			}
		}
		break;

	case 20:
		if(data == 0x0b)
			State++;
		break;
	case 21:
		if(data == 0x0a)
		{
			// 需要WIFI返回信息
			ReturnMsg = 0xff;//WifiReturnMsg();
			State = 0;
		}
		break;
	}
}


// 忽略此命令
void Cmd0aPrint(void) // 该命令没有后续数据
{
#if SendCmd
	printf("Cmd0aPrint\n");
//	USB_RevC->DataOut("USB: Cmd0dPrint \r\n", 64);
//  tcp_send_data_sang("Wifi: Cmd0dPrint \r\n",17);
//	BT_RevC->DataOut("Spp: Cmd0dPrint \r\n",6);
#endif
	 
/*	for(cBufferPrintDataWrite = (BufferPRTSize-1); cBufferPrintDataWrite; cBufferPrintDataWrite--)
	{
		BufferPrint[cBufferPrintLineWrite].BufferA[cBufferPrintDataWrite] = 0x00;
		BufferPrint[cBufferPrintLineWrite].BufferB[cBufferPrintDataWrite] = 0x00;
	}
	//cBufferPrintDataWrite = 0; // 出去时已经刚好为0
	//cBufferPrintDataRead = 0;
	BufferPrint[cBufferPrintLineWrite].cLeft=0;
	BufferPrint[cBufferPrintLineWrite].cRight=0;
	
	BufferPrint[cBufferPrintLineWrite].Step = 24;
	if(PaperState == sPaperFrontNormal)	BufferPrint[cBufferPrintLineWrite].Dir = sPaperDirFrontToBack;
	else if(PaperState == sPaperBackNormal)	BufferPrint[cBufferPrintLineWrite].Dir = sPaperDirBackToFront;
	
	if(cBufferPrintLineWrite >= (BufferPrintLineNum-1))
		cBufferPrintLineWrite = 0;
	else
		cBufferPrintLineWrite++;*/
}
// 忽略此命令
void Cmd0dPrint(void) //该命令没有后续数据
{
#if SendCmd
	printf("Cmd0dPrint\n");
//	USB_RevC->DataOut("USB: Cmd0dPrint \r\n", 64);
//  tcp_send_data_sang("Wifi: Cmd0dPrint \r\n",17);
//	BT_RevC->DataOut("Spp: Cmd0dPrint \r\n",6);
#endif
}

// 命令首字节为1b的命令类
void Cmd1bClass(unsigned char data)
{
	switch(data)
	{
	case 0x0a:
		Command.Type = eCmd1b0aMode;
		break;

	case 0x0b:
		Command.Type = eCmd1b0bWifi;
		break;

	case 0x2a:
		Command.Type = eCmd1b2aDots;
		break;

	case 0x4a:
		Command.Type = eCmd1b4aFeed;
		break;

	case 0x4b:
		Command.Type = eCmd1b4bBack;
		break;

	case 0x55:
		Command.Type = eCmd1b55Direction;
		break;

	case 0x7a:
		Command.Type = eCmd1b7aSpeed;
		break;

	case 0x80:
		Command.Type = eCmd1b80Pid;
		break;

	case 0x89:
		Command.Type = eCmd1b89Page;
		break;

	case 0x9c:
		Command.Type = eCmd1b9cEnd;
		break;

	case 0xa0:
		Command.Type = eCmd1ba0Start;
		break;

	case 0xf8:
		Command.Type = eCmd1bf8Delay;
		break;

	default:
		Command.Type = eCmdInit;
		Command.State = 0;
		Command.Size = 0;
		break;
	}
}
// return 0: 正常结束	1:等待数据		2： 数据异常，不是期望数据，好留该数据再次解析
u8 Cmd1b0aMode(unsigned char data)
{
	u8 result;
	
	switch(Command.State)
	{
	default:
	case 0:
#if SendCmd
		printf("eCmd1b0aMode\n");
		//USB_RevC->DataOut("USB eCmd1b0aMode Msg!\r\n", sizeof("USB eCmd1b0aMode Msg!\r\n"));
//		USB_RevC->DataOut("USB: Cmd1b0a \r\n", 64);
//		tcp_send_data_sang("Wifi: Cmd1b0a \r\n", 14);
//		BT_RevC->DataOut("Spp: Cmd1b0a \r\n", 13);
#endif
		if(data == 1)
		{
			Command.State = 10; // 命令有效
			result = 1;
		}
		else
			result = 2;
		break;

	case 10:
		if(data == 0x01)
			FlagPrinter = eFlagPRTStand;
		else if(data == 0x02)
			FlagPrinter = eFlagPRTDeep;
		else if(data == 0x03)
			FlagPrinter = eFlagPRTStand;
		else
			FlagPrinter = eFlagPRTFast;
FlagPrinter = eFlagPRTFast;

		result = 0;
		break;
	}
	
	return result;
}
u8 Cmd1b0bWifi(unsigned char data)
{
	u8 result;
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
#if SendCmd
		//USBReturnWifi(1);
		//USB_RevC->DataOut("USB Back Msg!\r\n", sizeof("USB Back Msg!\r\n"));

		if(data == 0x0e)
			printf("----------CmdWifi-0E+ssid+pwd\n");
		else if(data == 0x0f)
			printf("----------CmdWifi-0F+printer?\n");
		else
			printf("----------CmdWifi+Msg!\n");


		//printf("SSID: %s\r\n",wifi_setup_sta_info.ssid);// @20230711 Alke
//		USB_RevC->DataOut("SSID: %s \r\n", wifi_setup_sta_info.ssid);//, 64);
//		tcp_send_data_sang("Wifi: Cmd1d56CutPaper \r\n",18+2);
//		BT_RevC->DataOut("Spp: Cmd1d56CutPaper \r\n",17+3);
//		USBReturnWifi(1);
//		USBReturnWifi(2);
//		USBReturnWifi(3);
//		USBReturnWifi(4);
#endif
		if(data == 0x0e)
		{
			Command.State = 10;
			result = 1;
		}
		else if(data == 0x0f)
		{
			Command.State = 20;
			result = 1;
		}
		else
		{
			result = 2;
		}
		break;

	case 10:
		if(data == 0x0b)
		{
			// 没有占满时，最后一位字符后面跟一个0作为结束标记
			if(Command.Size < 33) // wifi_stored_sta_info.ssid[33]
			{
				wifi_setup_sta_info.ssid[Command.Size] = 0;
				Command.Size++;
			}

			Command.State++; // 命令有效
			Command.Size = 0;
		}
		else
		{
			if(Command.Size < 33)
			{
				wifi_setup_sta_info.ssid[Command.Size] = data;
				Command.Size++;
			}
		}
		result = 1;
		break;
	case 11:
		if(data == 0x0a)
		{
			// 没有占满时，最后一位字符后面跟一个0作为结束标记
			if(Command.Size < 64) // wifi_stored_sta_info.pwd[64]
			{
				wifi_setup_sta_info.pwd[Command.Size] = 0;
				Command.Size++;
			}

			if(WifiState)
				USBReturnWifi(2);
			else
				USBReturnWifi(1);
			// 次处不再发送消息，仅通过WIFI状态机中将各种状态返回，软件中也能够显示相关信息，软件仅关心是否获得IP
			//USBReturnWifi(1);
			//USBReturnWifi(2);
			//USBReturnWifi(3);
			//USBReturnWifi(4);
			//USBReturnWifi(5);

			ssid_stored_cnt = wifi_get_stored_sta_info(wifi_stored_sta_info);
    		for (int i = 0; i < ssid_stored_cnt; i++)
			{
	        	//printf("1:wifi_get_stored_sta_info[%d]= %s \r\n", i, wifi_stored_sta_info[i].ssid);
				wifi_del_stored_sta_info(wifi_stored_sta_info[i].ssid);// @ Alke 20230711 删除存储网络
	    	}
			os_time_dly(100);
			wifi_store_mode_info(STA_MODE, wifi_setup_sta_info.ssid, wifi_setup_sta_info.pwd);

			result = 0;
		}
		else
		{
			if(Command.Size < 64)
			{
				wifi_setup_sta_info.pwd[Command.Size] = data;
				Command.Size++;
			}
			result = 1;
		}
		break;

	case 20:
		if(data == 0x0b)
			Command.State++;
		result = 1;
		break;
	case 21:
		if(data == 0x0a)
		{
#if DebugMsg
	printf("1B0B0F*0B*0A\r\n");// @20230711 Alke
//	printf(&wifi_setup_sta_info.ssid, sizeof(wifi_setup_sta_info.ssid));
//	printf(&wifi_setup_sta_info.pwd, sizeof(wifi_setup_sta_info.pwd));
#endif
			// 需要WIFI返回信息
			WifiReturnMsg();
			result = 0;
		}
		else
			result = 1;
		break;

	default:
		result = 2;
		break;
	}

	return result;
}

u8 Cmd1b2aDots(unsigned char data)
{
	u8 result;
	volatile unsigned int c16c1;
	volatile unsigned char c8c1;

	switch(Command.State)
	{
	case 0:
#if SendCmd
		printf("Cmd1b2aDots\n");
//		USB_RevC->DataOut("USB: Cmd1b2aDots \r\n", 64);
//		tcp_send_data_sang("Wifi: Cmd1b2aDots \r\n",22);
//		BT_RevC->DataOut("Spp: Cmd1b2aDots \r\n",21);
#endif

//printf("Dots-00\n");
		if(data == 0x00)
		{
			Command.State = 10; // 整点图形数据
			result = 1;
		}
		else if(data == 0x01)
		{
			Command.State = 20; // 半点图形数据
			result = 1;
		}
		else result = 2;
		break;

	case 10: // 整点图形数据，Size低8位
		Command.Size = data;
		Command.State++;
		result = 1;
		break;
	case 11: // 整点图形数据，Size高8位
		c16c1 = data;
		c16c1 <<= 8;
		Command.Size += c16c1;
		cBufferPrintDataWrite = 0;
		Command.State++;
		result = 1;
		break;
	case 12: // 整点图形数据开始，奇数行数据
		// 前进纸模式：前一半数据需要写入偶数行打印缓冲区（后一半数据需要写入奇数行打印缓冲区），两个缓冲区中表现在纸张上的数据需要前后调换，走纸方向原因每个数据需要按位高低互换
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
				//剩余写0
				for(c16c1 = cBufferPrintDataWrite; c16c1 < BufferPRTSize; c16c1++)
					BufferPrint[cBufferPrintLineWrite].BufferA[c16c1] = 0;

				Command.State++;
			}
		}
		result = 1;
		break;
	case 13: // 整点图形数据开始，偶数行数据
		if(PaperState == sPaperFrontNormal) // 前进纸模式
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
				//剩余写0
				for(c16c1 = cBufferPrintDataWrite; c16c1 < BufferPRTSize*2; c16c1++)
					BufferPrint[cBufferPrintLineWrite].BufferB[c16c1-Command.Size/2] = data;
				Command.State = 30;
				goto PrintDataDeal;
			}
		}
		break;

	case 20: // 半点图形数据，Size低8位
//printf("Cmd1b2aDots-20\n");
		Command.Size = data;
		Command.State++;
		result = 1;
		break;
	case 21: // 半点图形数据，Size高8位
//printf("Cmd1b2aDots-21-%d\n", Command.Size);
		c16c1 = data;
		c16c1 <<= 8;
		Command.Size += c16c1;
		cBufferPrintDataWrite = 0;
		Command.State++;
		result = 1;
		break;
	case 22: // 半点图形数据开始，奇数行数据
//printf("22-%d\n", cBufferPrintDataWrite);
		if(PaperState == sPaperFrontNormal) // 前进纸模式
		{
			if(cBufferPrintDataWrite < BufferPRTSize)
			{
				BufferPrint[cBufferPrintLineWrite].BufferB[BufferPRTSize-1-cBufferPrintDataWrite] = ByteBitChang(data);
				cBufferPrintDataWrite++;
			}
			// 确保数据一旦足够，立马换到下个状态
			if(cBufferPrintDataWrite >= (Command.Size/2))
			{
				//剩余写0
				if(cBufferPrintDataWrite < BufferPRTSize)
				{
					for(c16c1 = 0; c16c1 < (BufferPRTSize-Command.Size/2); c16c1++)
						BufferPrint[cBufferPrintLineWrite].BufferB[c16c1] = 0;
				}
				Command.State++;
			}
		}
		else
		{
			BufferPrint[cBufferPrintLineWrite].BufferA[cBufferPrintDataWrite] = data;
			cBufferPrintDataWrite++;
			// 确保数据一旦足够，立马换到下个状态
			if(cBufferPrintDataWrite >= (Command.Size/2))
			{
				//剩余写0
				for(c16c1 = cBufferPrintDataWrite; c16c1 < BufferPRTSize; c16c1++)
					BufferPrint[cBufferPrintLineWrite].BufferA[c16c1] = 0;
				Command.State++;
			}
		}
		result = 1;
		break;
	case 23: // 半点图形数据开始，偶数行数据
//printf("23-%d\n", cBufferPrintDataWrite);
		if(PaperState == sPaperFrontNormal) // 前进纸模式
		{
			if((cBufferPrintDataWrite-Command.Size/2) < BufferPRTSize)
			{
				BufferPrint[cBufferPrintLineWrite].BufferA[BufferPRTSize-1-(cBufferPrintDataWrite-Command.Size/2)] = ByteBitChang(data);
				cBufferPrintDataWrite++;
			}
			// 确保数据一旦足够，立马换到下个状态
			if(cBufferPrintDataWrite >= Command.Size)
			{
				//剩余写0
				if((cBufferPrintDataWrite-Command.Size/2) < BufferPRTSize)
				{
					for(c16c1 = 0; c16c1 < (BufferPRTSize-Command.Size/2); c16c1++)
						BufferPrint[cBufferPrintLineWrite].BufferA[c16c1] = 0;
				}
				Command.State = 30;
				goto PrintDataDeal;
			}
		}
		else
		{
			BufferPrint[cBufferPrintLineWrite].BufferB[cBufferPrintDataWrite-Command.Size/2] = data; // Alke @20230704 -Command.Size/2
			cBufferPrintDataWrite++;
			// 确保数据一旦足够，立马换到下个状态
			if(cBufferPrintDataWrite >= (Command.Size))
			{
				//剩余写0
				for(c16c1 = cBufferPrintDataWrite; c16c1 < BufferPRTSize*2; c16c1++)
					BufferPrint[cBufferPrintLineWrite].BufferB[c16c1-Command.Size/2] = data;
				Command.State = 30;
				goto PrintDataDeal;
			}
		}
		break;

	case 30: // 图形数据虚拟半点处理
		PrintDataDeal:
//printf("30-0\n");
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
		//原始数据开始处理
		// @20230707 Alke 如果前进纸需要数据左右、上下颠倒一下
		// @@@@@@@@@@@@@@@@@@@@@@@@@@
	#if 0
		if(PaperState == sPaperFrontNormal) // 前进纸模式
		{
			// 打印效果上的数据左右调换，就是打印缓冲区前后调换；同时A B缓冲区数据调换，就是奇数行和偶数行数据调换
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
//printf("30-1\n");
/*for(cBufferPrintDataWrite = 0; cBufferPrintDataWrite < BufferPRTSize; cBufferPrintDataWrite++)
{
	(BufferPrint[cBufferPrintLineWrite].BufferA[cBufferPrintDataWrite]) = 0xff;
	(BufferPrint[cBufferPrintLineWrite].BufferB[cBufferPrintDataWrite]) = 0xff;
}*/
		// 计算左右非0数据位置
		cBufferPrintDataWrite = 0;
		while((BufferPrint[cBufferPrintLineWrite].BufferA[cBufferPrintDataWrite] == 0) && (BufferPrint[cBufferPrintLineWrite].BufferB[cBufferPrintDataWrite] == 0))
		//while((BufferPrint[cBufferPrintLineWrite].BufferA[cBufferPrintDataWrite]) || (BufferPrint[cBufferPrintLineWrite].BufferB[cBufferPrintDataWrite]))
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
//printf("30-2\n");

		//虚拟半点数据开始处理 VirtualHalfDotsDataDeal:
		if(BufferPrint[cBufferPrintLineWrite].cLeft < BufferPrint[cBufferPrintLineWrite].cRight)
		{
			cBufferPrintDataWrite = BufferPrint[cBufferPrintLineWrite].cLeft;
			//while(cBufferPrintDataWrite < (BufferPrint[cBufferPrintLineWrite].cRight-1)) // (BufferPRTSize-2)Command.Size
			while(cBufferPrintDataWrite < BufferPrint[cBufferPrintLineWrite].cRight) // (BufferPRTSize-2)Command.Size
			{
				BufferPrint[cBufferPrintLineWrite].BufferA[cBufferPrintDataWrite+1] &= ~BufferPrint[cBufferPrintLineWrite].BufferA[cBufferPrintDataWrite];
				BufferPrint[cBufferPrintLineWrite].BufferB[cBufferPrintDataWrite+1] &= ~BufferPrint[cBufferPrintLineWrite].BufferB[cBufferPrintDataWrite];
				cBufferPrintDataWrite++;
			}
		}
		else
		{
			BufferPrint[cBufferPrintLineWrite].cRight = BufferPrint[cBufferPrintLineWrite].cLeft;
		}
//printf("30-3\n");

		// 左起数据位置折算到整步数上
		cBufferPrintDataRead = BufferPrint[cBufferPrintLineWrite].cLeft*BufferOneDataTimePosition;
		cBufferPrintDataWrite = cBufferPrintDataRead%CRGOneStepTimePosition;
		BufferPrint[cBufferPrintLineWrite].cLeft = cBufferPrintDataRead - cBufferPrintDataWrite;
		/*BufferPrint[cBufferPrintLineWrite].cLeft *= BufferOneDataTimePosition;//CRGOneStepTimePosition; // 左起应该要从0对齐才能有加速过程
		if(BufferPrint[cBufferPrintLineWrite].cLeft >= BufferPrint[cBufferPrintLineWrite].cLeft%CRGOneStepTimePosition)
			BufferPrint[cBufferPrintLineWrite].cLeft -= BufferPrint[cBufferPrintLineWrite].cLeft%CRGOneStepTimePosition;*/			// adjust to th 16*n
		// 右起数据位置折算到整步数上
		cBufferPrintDataRead = BufferPrint[cBufferPrintLineWrite].cRight*BufferOneDataTimePosition;
		cBufferPrintDataWrite = cBufferPrintDataRead%CRGOneStepTimePosition;
		BufferPrint[cBufferPrintLineWrite].cRight = cBufferPrintDataRead +CRGOneStepTimePosition- cBufferPrintDataWrite;
		/*BufferPrint[cBufferPrintLineWrite].cRight *= BufferOneDataTimePosition;//CRGOneStepTimePosition; // 右边结束的位置需要增加加速和减速步数
		BufferPrint[cBufferPrintLineWrite].cRight += CRGVolatileStep*CRGOneStepTimePosition*2; // 右边结束的位置需要增加加速和减速步数
		BufferPrint[cBufferPrintLineWrite].cRight += CRGOneStepTimePosition-BufferPrint[cBufferPrintLineWrite].cRight%CRGOneStepTimePosition;*/	 // adjust to th 16*n
		if(BufferPrint[cBufferPrintLineWrite].cRight > TimePositionAll)
			BufferPrint[cBufferPrintLineWrite].cRight = TimePositionAll;

		// 左起数据位置折算到马达起始运动位置上
		if(BufferPrint[cBufferPrintLineWrite].cLeft > (CRGVolatileStep*CRGOneStepTimePosition+5*CRGOneStepTimePosition))
			BufferPrint[cBufferPrintLineWrite].cLeft -= (CRGVolatileStep*CRGOneStepTimePosition+5*CRGOneStepTimePosition);
		else
			BufferPrint[cBufferPrintLineWrite].cLeft = 0;
		// 右起数据位置折算到马达起始运动位置上
		if((BufferPrint[cBufferPrintLineWrite].cRight+CRGVolatileStep*CRGOneStepTimePosition*2+15*CRGOneStepTimePosition)<TimePositionAll)
			BufferPrint[cBufferPrintLineWrite].cRight += CRGVolatileStep*CRGOneStepTimePosition*2+15*CRGOneStepTimePosition;
		else
			BufferPrint[cBufferPrintLineWrite].cRight = TimePositionAll;


//BufferPrint[cBufferPrintLineWrite].cLeft = 0;
//BufferPrint[cBufferPrintLineWrite].cRight = TimePositionAll;


//printf("30-4\n");
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

		//break; 没有break，直接复位参数后退出
	default:
//printf("df--\n");
		cBufferPrintDataWrite = 0;
		result = 0;
		break;
	}

	return result;
}
u8 Cmd1b4aFeed(unsigned char data)
{
#if SendCmd
		printf("Cmd1b4aFeed\n");
	//	USB_RevC->DataOut("USB: Cmd1b4aPAP \r\n", 64);
	//	tcp_send_data_sang("Wifi: Cmd1b4aPAP \r\n",21);
	//	BT_RevC->DataOut("Spp: Cmd1b4aPAP \r\n",20);
	//printf("N%d-S%d-P%d\n", CRGNowTimePosition, CRGStopTimePosition, PrintingState);
#endif

	for(cBufferPrintDataWrite = (BufferPRTSize-1); cBufferPrintDataWrite; cBufferPrintDataWrite--)
	{
		BufferPrint[cBufferPrintLineWrite].BufferA[cBufferPrintDataWrite] = 0x00;
		BufferPrint[cBufferPrintLineWrite].BufferB[cBufferPrintDataWrite] = 0x00;
	}
	//cBufferPrintDataWrite = 0; // 出去时已经刚好为0
	//cBufferPrintDataRead = 0;
	BufferPrint[cBufferPrintLineWrite].cLeft=0;
	BufferPrint[cBufferPrintLineWrite].cRight=0;

	//BufferPrint[cBufferPrintLineWrite].Step += data;
	BufferPrint[cBufferPrintLineWrite].Step = data;
	// 通常默认为16步，没有0D命令切换下一行时，本次步数增加，使得走纸连贯:   此处必须严格按照命令操作，不能增加步数；中断中走马达时可以考虑增加步数
	if(PaperState == sPaperFrontNormal)
	{
		BufferPrint[cBufferPrintLineWrite].Dir = sPaperDirFrontToBack;
	}
	else if(PaperState == sPaperBackNormal)
	{
		BufferPrint[cBufferPrintLineWrite].Dir = sPaperDirBackToFront;
	}

	if(cBufferPrintLineWrite >= (BufferPrintLineNum-1))
		cBufferPrintLineWrite = 0;
	else
		cBufferPrintLineWrite++;

	return 0;
}
u8 Cmd1b4bBack(unsigned char data)
{
#if SendCmd
	printf("Cmd1b4bBack\n");
//	USB_RevC->DataOut("USB: Cmd1b4bPAP \r\n", 64);
//	tcp_send_data_sang("Wifi: Cmd1b4bPAP \r\n",21);
//	BT_RevC->DataOut("Spp: Cmd1b4bPAP \r\n",20);
//printf("N%d-S%d-P%d\n", CRGNowTimePosition, CRGStopTimePosition, PrintingState);
#endif

	for(cBufferPrintDataWrite = (BufferPRTSize-1); cBufferPrintDataWrite; cBufferPrintDataWrite--)
	{
		BufferPrint[cBufferPrintLineWrite].BufferA[cBufferPrintDataWrite] = 0x00;
		BufferPrint[cBufferPrintLineWrite].BufferB[cBufferPrintDataWrite] = 0x00;
	}
	//cBufferPrintDataWrite = 0; // 出去时已经刚好为0
	//cBufferPrintDataWrite = 0; // 出去时已经刚好为0
	cBufferPrintDataRead = 0;
	BufferPrint[cBufferPrintLineWrite].cLeft=0;
	BufferPrint[cBufferPrintLineWrite].cRight=0;

	//BufferPrint[cBufferPrintLineWrite].Step += data;
	BufferPrint[cBufferPrintLineWrite].Step = data;
	if(PaperState == sPaperFrontNormal)
	{
		BufferPrint[cBufferPrintLineWrite].Dir = sPaperDirBackToFront;
		//PAPBackToFrontSet
	}
	else if(PaperState == sPaperBackNormal)
	{
		BufferPrint[cBufferPrintLineWrite].Dir = sPaperDirFrontToBack;
	}

	if(cBufferPrintLineWrite >= (BufferPrintLineNum-1))
		cBufferPrintLineWrite = 0;
	else
		cBufferPrintLineWrite++;

	return 0;
}

u8 Cmd1b55Direction(unsigned char data)
{
#if SendCmd
	printf("Cmd1b55Direction\n");
//	USB_RevC->DataOut("USB: Cmd1b55PrtDir \r\n", 64);
//	tcp_send_data_sang("Wifi: Cmd1b55PrtDir \r\n",24);
//	BT_RevC->DataOut("Spp: Cmd1b55PrtDir \r\n",23);
#endif
	if(data)
		CMDOneDirSet
	else
		CMDTwoDirSet

	return 0;
}
u8 Cmd1b7aSpeed(unsigned char data)
{
#if SendCmd
	printf("Cmd1b7aSpeed\n");
//	USB_RevC->DataOut("USB: Cmd1b7a \r\n", 64);
//	tcp_send_data_sang("Wifi: Cmd1b7a \r\n",18);
//	BT_RevC->DataOut("Spp: Cmd1b7a \r\n",17);
#endif
	if(data)
		FlagPrinter = eFlagPRTFast;
	else
		FlagPrinter = eFlagPRTStand;
FlagPrinter = eFlagPRTFast;

	return 0;
}
u8 Cmd1b80Pid(unsigned char data)
{
	u8 result;

	switch(Command.State)
	{
	default:
	case 0:
#if SendCmd
		printf("Cmd1b80Pid\n");
//		USB_RevC->DataOut("USB: Cmd1b80 \r\n", 64);
//		tcp_send_data_sang("Wifi: Cmd1b80 \r\n",18);
//		BT_RevC->DataOut("Spp: Cmd1b80 \r\n",17);
#endif
		sDeviceDescriptor[10] = data;
		Command.State++;
		result = 1;
		break;

	case 1:
		sDeviceDescriptor[11] = data;
		FlashDataWrite();
		result = 0;
		break; // 没有break，直接复位参数后退出
	}
	return result;
}

u8 Cmd1b89Page(unsigned char data)
{
	u8 result;
	
	switch(Command.State)
	{
	default:
	case 0:
#if SendCmd
		printf("Cmd1b89Page\n");
//		USB_RevC->DataOut("USB: Cmd1b89 \r\n", 64);
//		tcp_send_data_sang("Wifi: Cmd1b89 \r\n",18);
//		BT_RevC->DataOut("Spp: Cmd1b89 \r\n",17);
#endif
		StepPageLenth = data;
		StepPageLenth <<= 8;
		Command.State = 1;
		result = 1;
		break;

	case 1:
		StepPageLenth += data;	// Alke @20230728 驱动里面纸张长度不对，临时屏蔽
		StepPageLenth *= 2;
		result = 0;
	}
	return result;
}
u8 Cmd1b9cEnd(unsigned char data)
{
#if SendCmd
	printf("Cmd1b9cEnd\n");
//	USB_RevC->DataOut("USB: Cmd1b9c \r\n", 64);
//	tcp_send_data_sang("Wifi: Cmd1b9c \r\n",18);
//	BT_RevC->DataOut("Spp: Cmd1b9c \r\n",17);
#endif

	/*if(PaperState == sPaperFrontNormal)
	{
		BufferPrint[cBufferPrintLineWrite].Step = PaperCount+data+StepFrontToPrint; // 多退纸StepFrontToPrint，确保纸张退出去
		BufferPrint[cBufferPrintLineWrite].Dir = sPaperDirBackToFront;

		if(cBufferPrintLineWrite >= (BufferPrintLineNum-1))
			cBufferPrintLineWrite = 0;
		else
			cBufferPrintLineWrite++;
	}*/
	// 此命令主要针对后进纸，后续需要增加补充
	if(PaperState == sPaperBackNormal)
	{
		for(cBufferPrintDataWrite = (BufferPRTSize-1); cBufferPrintDataWrite; cBufferPrintDataWrite--)
		{
			BufferPrint[cBufferPrintLineWrite].BufferA[cBufferPrintDataWrite] = 0x00;
			BufferPrint[cBufferPrintLineWrite].BufferB[cBufferPrintDataWrite] = 0x00;
		}
		//cBufferPrintDataWrite = 0; // 出去时已经刚好为0
		//cBufferPrintDataRead = 0;
		BufferPrint[cBufferPrintLineWrite].cLeft=0;
		BufferPrint[cBufferPrintLineWrite].cRight=0;

		BufferPrint[cBufferPrintLineWrite].Step = StepPageLenth+data+StepBackToPrint-PaperCount;
		BufferPrint[cBufferPrintLineWrite].Dir = sPaperDirFrontToBack;

		if(cBufferPrintLineWrite >= (BufferPrintLineNum-1))
			cBufferPrintLineWrite = 0;
		else
			cBufferPrintLineWrite++;
	}

	return 0;
}
u8 Cmd1ba0Start(unsigned char data)
{
#if SendCmd
	printf("Cmd1ba0Start\n");
//	USB_RevC->DataOut("USB: Cmd1ba0 \r\n", 64);
//	tcp_send_data_sang("Wifi: Cmd1ba0 \r\n",18);
//	BT_RevC->DataOut("Spp: Cmd1ba0 \r\n",17);
#endif
	/*if(PaperState == sPaperFrontNormal)
	{
		BufferPrint[cBufferPrintLineWrite].Step = data+StepFrontToPrint; // 多退纸StepFrontToPrint，确保纸张退出去
		BufferPrint[cBufferPrintLineWrite].Dir = sPaperDirBackToFront;

		if(cBufferPrintLineWrite >= (BufferPrintLineNum-1))
			cBufferPrintLineWrite = 0;
		else
			cBufferPrintLineWrite++;
	}*/
	// 此命令主要针对后进纸，后续需要增加补充
	if(PaperState == sPaperBackNormal)
	{
		for(cBufferPrintDataWrite = (BufferPRTSize-1); cBufferPrintDataWrite; cBufferPrintDataWrite--)
		{
			BufferPrint[cBufferPrintLineWrite].BufferA[cBufferPrintDataWrite] = 0x00;
			BufferPrint[cBufferPrintLineWrite].BufferB[cBufferPrintDataWrite] = 0x00;
		}
		//cBufferPrintDataWrite = 0; // 出去时已经刚好为0
		//cBufferPrintDataRead = 0;
		BufferPrint[cBufferPrintLineWrite].cLeft=0;
		BufferPrint[cBufferPrintLineWrite].cRight=0;

		BufferPrint[cBufferPrintLineWrite].Step = StepBackToPrint+data-PaperCount; // 多退纸StepFrontToPrint，确保纸张退出去
		BufferPrint[cBufferPrintLineWrite].Dir = sPaperDirFrontToBack;

		if(cBufferPrintLineWrite >= (BufferPrintLineNum-1))
			cBufferPrintLineWrite = 0;
		else
			cBufferPrintLineWrite++;
	}

	return 0;
}
u8 Cmd1bf8Dealy(unsigned char data)
{
	u8 result;
	
	switch(Command.State)
	{
	case 0:
#if SendCmd
		printf("Cmd1bf8Dealy\n");
//		USB_RevC->DataOut("USB: Cmd1b89 \r\n", 64);
//		tcp_send_data_sang("Wifi: Cmd1b89 \r\n",18);
//		BT_RevC->DataOut("Spp: Cmd1b89 \r\n",17);
#endif
		if(data == 0xaa)
		{
			Command.State = 10;		// 正常，继续
			result = 1;
		}
		else
			result = 2;
		break;

	case 10:
		if(data)
		{
			FlagHeadDelayClr;
			Command.State++;
		}
		else
		{
			FlagHeadDelaySet;
			Command.State = 20;
		}
		result = 1;
		break;
	case 11:
		HeadDelayCount = data;
		FlashDataWrite();
		result = 0;
		break;
	case 20:
		HeadDelayCount = data;
		result = 0;
		break;
	}
	return result;
}


// 1d打头命令
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
		Command.State = 0;
		Command.Size = 0;
		break;
	}
}
// 切纸命令
u8 Cmd1d56CutPaper(unsigned char data)
{
#if SendCmd
	printf("Cmd1d56CutPaper\n");
//	USB_RevC->DataOut("USB: Cmd1d56CutPaper \r\n", 64);
//	tcp_send_data_sang("Wifi: Cmd1d56CutPaper \r\n",26);
//	BT_RevC->DataOut("Spp: Cmd1d56CutPaper \r\n",25);
#endif

	if(data == 0x53)
	{
		if(PaperState == sPaperFrontNormal)
		{
			for(cBufferPrintDataWrite = (BufferPRTSize-1); cBufferPrintDataWrite; cBufferPrintDataWrite--)
			{
				BufferPrint[cBufferPrintLineWrite].BufferA[cBufferPrintDataWrite] = 0x00;
				BufferPrint[cBufferPrintLineWrite].BufferB[cBufferPrintDataWrite] = 0x00;
			}
			//cBufferPrintDataWrite = 0; // 出去时已经刚好为0
			//cBufferPrintDataRead = 0;
			BufferPrint[cBufferPrintLineWrite].cLeft=0;
			BufferPrint[cBufferPrintLineWrite].cRight=0;

			BufferPrint[cBufferPrintLineWrite].Step = StepPageLenth+StepFrontToPrint*2; // 多退纸StepFrontToPrint，确保纸张退出去
			BufferPrint[cBufferPrintLineWrite].Dir = sPaperDirBackToFront;

			if(cBufferPrintLineWrite >= (BufferPrintLineNum-1))
				cBufferPrintLineWrite = 0;
			else
				cBufferPrintLineWrite++;
		}
		return 0;
	}
	else
		return 2;
}



#if 1
// 根据首字节进行命令分类
//void InstructionParser(volatile unsigned char data)//命令解析
// 此函数为接收到一个字节数据进行解析处理，前提是预先判断打印缓冲区有空白可以写入，否则会在USB/WIFI/BL端置忙停止接收数据
void DealData(unsigned char data) //命令解析
{
	u8 result = 0;
	
#if DebugOnlyData
	printf("%c",data);
	return;
#endif
	// 连续记录最近的两个字节，方便回滚命令时使用
	Command.SizeSave[2] = Command.SizeSave[1];
	Command.SizeSave[1] = data;
	Command.SizeSave[0]++;

	DealCommandStart:
	switch(Command.Type)
	{
	// 这是第1个字节的判断，然后才能进行命令的分类处理
	default://eCmdInit
	case eCmdInit:
		if(Command.SizeSave[0] == 2)
			data = Command.SizeSave[2];
			
		if(Command.SizeSave[0])
		{
			if(data == 0x0a)		Command.Type = eCmd0aPrint;
			else if(data == 0x0d)	Command.Type = eCmd0dPrint;
			else if(data == 0x1b)	Command.Type = eCmd1bClass;
			else if(data == 0x1d)	Command.Type = eCmd1dClass;
			else
			{
				Command.Type = eCmdInit;
				Command.SizeSave[0]--;
				Command.State = 0;
				Command.Size = 0;
				goto DealCommandStart;
			}
			if(Command.SizeSave[0])
				Command.SizeSave[0]--;	// 单字节命令必须直接处理，不能等后续数据
		}
		break;
		
	case eCmd0aPrint:	// 单字节命令必须在此次循环中直接处理完毕，不能等下个数据再处理; 命令一旦处理，则肯定没有存储的待解析处理的数据
		Cmd0aPrint();
		
		Command.Type = eCmdInit;
		Command.State = 0;
		Command.Size = 0;
		if(Command.SizeSave[0])
			Command.SizeSave[0]--;
		if(Command.SizeSave[0])
			goto DealCommandStart;
		break;
		
	case eCmd0dPrint:	// 单字节命令必须在此次循环中直接处理完毕，不能等下个数据再处理
		Cmd0dPrint();
		
		Command.Type = eCmdInit;
		Command.State = 0;
		Command.Size = 0;
		if(Command.SizeSave[0])
			Command.SizeSave[0]--;
		if(Command.SizeSave[0])
			goto DealCommandStart;
		break;

	case eCmd1bClass:
		if(Command.SizeSave[0] == 2)
			data = Command.SizeSave[2];
			
		if(Command.SizeSave[0])
		{
			if(data == 0x0a)		Command.Type = eCmd1b0aMode;
			else if(data == 0x0b)	Command.Type = eCmd1b0bWifi;
			else
			{
				Command.Type = eCmdInit;
				Command.SizeSave[0]--;
				Command.State = 0;
				Command.Size = 0;
				goto DealCommandStart;
			}
			if(Command.SizeSave[0])
				Command.SizeSave[0]--;
		}
		break;

	case eCmd1dClass:
		if(Command.SizeSave[0] == 2)
			data = Command.SizeSave[2];
			
		if(Command.SizeSave[0])
		{
			if(data == 0x56)		Command.Type = eCmd1d56CutPaper;
			else
			{
				Command.Type = eCmdInit;
				Command.SizeSave[0]--;
				Command.State = 0;
				Command.Size = 0;
				goto DealCommandStart;
			}
			if(Command.SizeSave[0])
				Command.SizeSave[0]--;	// 单字节命令必须直接处理，不能等后续数据
		}
		break;

	case eCmd1b0aMode:
		result = Cmd1b0aMode(data);
		if(result==0) // 命令正常处理结束，才进行后面解析
		{
			Command.Type = eCmdInit;
			Command.State = 0;
			Command.Size = 0;
			if(Command.SizeSave[0])
				Command.SizeSave[0]--;
			if(Command.SizeSave[0])
				goto DealCommandStart;
		}
		//else if(result==1) {;} // 继续传递函数需要的参数
		else if(result==2) //数据异常，需要抛弃第一个数据再次解析
		{
			Command.Type = eCmdInit;
			Command.State = 0;
			Command.Size = 0;
			//if(Command.SizeSave[0])	// 本次数据没有使用，返回后再次解析
				//Command.SizeSave[0]--;
			if(Command.SizeSave[0])
				goto DealCommandStart;
		}
		break;

	case eCmd1b0bWifi:
		result = Cmd1b0bWifi(data);
		if(result==0) // 命令正常处理结束，才进行后面解析
		{
			Command.Type = eCmdInit;
			Command.State = 0;
			Command.Size = 0;
			if(Command.SizeSave[0])
				Command.SizeSave[0]--;
			if(Command.SizeSave[0])
				goto DealCommandStart;
		}
		//else if(result==1) {;} // 继续传递函数需要的参数
		else if(result==2) //数据异常，需要抛弃第一个数据再次解析
		{
			Command.Type = eCmdInit;
			Command.State = 0;
			Command.Size = 0;
			//if(Command.SizeSave[0])	// 本次数据没有使用，返回后再次解析
				//Command.SizeSave[0]--;
			if(Command.SizeSave[0])
				goto DealCommandStart;
		}
		break;

	case eCmd1b2aDots:
		result = Cmd1b2aDots(data);
		if(result==0) // 命令正常处理结束，才进行后面解析
		{
			Command.Type = eCmdInit;
			Command.State = 0;
			Command.Size = 0;
			if(Command.SizeSave[0])
				Command.SizeSave[0]--;
			if(Command.SizeSave[0])
				goto DealCommandStart;
		}
		//else if(result==1) {;} // 继续传递函数需要的参数
		else if(result==2) //数据异常，需要抛弃第一个数据再次解析
		{
			Command.Type = eCmdInit;
			Command.State = 0;
			Command.Size = 0;
			//if(Command.SizeSave[0])	// 本次数据没有使用，返回后再次解析
				//Command.SizeSave[0]--;
			if(Command.SizeSave[0])
				goto DealCommandStart;
		}
		break;

	case eCmd1b4aFeed:
		result = Cmd1b4aFeed(data);
		if(result==0) // 命令正常处理结束，才进行后面解析
		{
			Command.Type = eCmdInit;
			Command.State = 0;
			Command.Size = 0;
			if(Command.SizeSave[0])
				Command.SizeSave[0]--;
			if(Command.SizeSave[0])
				goto DealCommandStart;
		}
		//else if(result==1) {;} // 继续传递函数需要的参数
		else if(result==2) //数据异常，需要抛弃第一个数据再次解析
		{
			Command.Type = eCmdInit;
			Command.State = 0;
			Command.Size = 0;
			//if(Command.SizeSave[0])	// 本次数据没有使用，返回后再次解析
				//Command.SizeSave[0]--;
			if(Command.SizeSave[0])
				goto DealCommandStart;
		}
		break;

	case eCmd1b4bBack:
		result = Cmd1b4bBack(data);
		if(result==0) // 命令正常处理结束，才进行后面解析
		{
			Command.Type = eCmdInit;
			Command.State = 0;
			Command.Size = 0;
			if(Command.SizeSave[0])
				Command.SizeSave[0]--;
			if(Command.SizeSave[0])
				goto DealCommandStart;
		}
		//else if(result==1) {;} // 继续传递函数需要的参数
		else if(result==2) //数据异常，需要抛弃第一个数据再次解析
		{
			Command.Type = eCmdInit;
			Command.State = 0;
			Command.Size = 0;
			//if(Command.SizeSave[0])	// 本次数据没有使用，返回后再次解析
				//Command.SizeSave[0]--;
			if(Command.SizeSave[0])
				goto DealCommandStart;
		}
		break;

	case eCmd1b55Direction:
		result = Cmd1b55Direction(data);
		if(result==0) // 命令正常处理结束，才进行后面解析
		{
			Command.Type = eCmdInit;
			Command.State = 0;
			Command.Size = 0;
			if(Command.SizeSave[0])
				Command.SizeSave[0]--;
			if(Command.SizeSave[0])
				goto DealCommandStart;
		}
		//else if(result==1) {;} // 继续传递函数需要的参数
		else if(result==2) //数据异常，需要抛弃第一个数据再次解析
		{
			Command.Type = eCmdInit;
			Command.State = 0;
			Command.Size = 0;
			//if(Command.SizeSave[0])	// 本次数据没有使用，返回后再次解析
				//Command.SizeSave[0]--;
			if(Command.SizeSave[0])
				goto DealCommandStart;
		}
		break;

	case eCmd1b7aSpeed:
		result = Cmd1b7aSpeed(data);
		if(result==0) // 命令正常处理结束，才进行后面解析
		{
			Command.Type = eCmdInit;
			Command.State = 0;
			Command.Size = 0;
			if(Command.SizeSave[0])
				Command.SizeSave[0]--;
			if(Command.SizeSave[0])
				goto DealCommandStart;
		}
		//else if(result==1) {;} // 继续传递函数需要的参数
		else if(result==2) //数据异常，需要抛弃第一个数据再次解析
		{
			Command.Type = eCmdInit;
			Command.State = 0;
			Command.Size = 0;
			//if(Command.SizeSave[0])	// 本次数据没有使用，返回后再次解析
				//Command.SizeSave[0]--;
			if(Command.SizeSave[0])
				goto DealCommandStart;
		}
		break;

	case eCmd1b80Pid:
		result = Cmd1b80Pid(data);
		if(result==0) // 命令正常处理结束，才进行后面解析
		{
			Command.Type = eCmdInit;
			Command.State = 0;
			Command.Size = 0;
			if(Command.SizeSave[0])
				Command.SizeSave[0]--;
			if(Command.SizeSave[0])
				goto DealCommandStart;
		}
		//else if(result==1) {;} // 继续传递函数需要的参数
		else if(result==2) //数据异常，需要抛弃第一个数据再次解析
		{
			Command.Type = eCmdInit;
			Command.State = 0;
			Command.Size = 0;
			//if(Command.SizeSave[0])	// 本次数据没有使用，返回后再次解析
				//Command.SizeSave[0]--;
			if(Command.SizeSave[0])
				goto DealCommandStart;
		}
		break;

	case eCmd1b89Page:
		result = Cmd1b89Page(data);
		if(result==0) // 命令正常处理结束，才进行后面解析
		{
			Command.Type = eCmdInit;
			Command.State = 0;
			Command.Size = 0;
			if(Command.SizeSave[0])
				Command.SizeSave[0]--;
			if(Command.SizeSave[0])
				goto DealCommandStart;
		}
		//else if(result==1) {;} // 继续传递函数需要的参数
		else if(result==2) //数据异常，需要抛弃第一个数据再次解析
		{
			Command.Type = eCmdInit;
			Command.State = 0;
			Command.Size = 0;
			//if(Command.SizeSave[0])	// 本次数据没有使用，返回后再次解析
				//Command.SizeSave[0]--;
			if(Command.SizeSave[0])
				goto DealCommandStart;
		}
		break;

	case eCmd1b9cEnd:
		result = Cmd1b9cEnd(data);
		if(result==0) // 命令正常处理结束，才进行后面解析
		{
			Command.Type = eCmdInit;
			Command.State = 0;
			Command.Size = 0;
			if(Command.SizeSave[0])
				Command.SizeSave[0]--;
			if(Command.SizeSave[0])
				goto DealCommandStart;
		}
		//else if(result==1) {;} // 继续传递函数需要的参数
		else if(result==2) //数据异常，需要抛弃第一个数据再次解析
		{
			Command.Type = eCmdInit;
			Command.State = 0;
			Command.Size = 0;
			//if(Command.SizeSave[0])	// 本次数据没有使用，返回后再次解析
				//Command.SizeSave[0]--;
			if(Command.SizeSave[0])
				goto DealCommandStart;
		}
		break;

	case eCmd1ba0Start:
		result = Cmd1ba0Start(data);
		if(result==0) // 命令正常处理结束，才进行后面解析
		{
			Command.Type = eCmdInit;
			Command.State = 0;
			Command.Size = 0;
			if(Command.SizeSave[0])
				Command.SizeSave[0]--;
			if(Command.SizeSave[0])
				goto DealCommandStart;
		}
		//else if(result==1) {;} // 继续传递函数需要的参数
		else if(result==2) //数据异常，需要抛弃第一个数据再次解析
		{
			Command.Type = eCmdInit;
			Command.State = 0;
			Command.Size = 0;
			//if(Command.SizeSave[0])	// 本次数据没有使用，返回后再次解析
				//Command.SizeSave[0]--;
			if(Command.SizeSave[0])
				goto DealCommandStart;
		}
		break;

	case eCmd1bf8Delay:
		result = Cmd1bf8Dealy(data);
		if(result==0) // 命令正常处理结束，才进行后面解析
		{
			Command.Type = eCmdInit;
			Command.State = 0;
			Command.Size = 0;
			if(Command.SizeSave[0])
				Command.SizeSave[0]--;
			if(Command.SizeSave[0])
				goto DealCommandStart;
		}
		//else if(result==1) {;} // 继续传递函数需要的参数
		else if(result==2) //数据异常，需要抛弃第一个数据再次解析
		{
			Command.Type = eCmdInit;
			Command.State = 0;
			Command.Size = 0;
			//if(Command.SizeSave[0])	// 本次数据没有使用，返回后再次解析
				//Command.SizeSave[0]--;
			if(Command.SizeSave[0])
				goto DealCommandStart;
		}
		break;

	case eCmd1d56CutPaper:
		result = Cmd1d56CutPaper(data);
		if(result==0) // 命令正常处理结束，才进行后面解析
		{
			Command.Type = eCmdInit;
			Command.State = 0;
			Command.Size = 0;
			if(Command.SizeSave[0])
				Command.SizeSave[0]--;
			if(Command.SizeSave[0])
				goto DealCommandStart;
		}
		//else if(result==1) {;} // 继续传递函数需要的参数
		else if(result==2) //数据异常，需要抛弃第一个数据再次解析
		{
			Command.Type = eCmdInit;
			Command.State = 0;
			Command.Size = 0;
			//if(Command.SizeSave[0])	// 本次数据没有使用，返回后再次解析
				//Command.SizeSave[0]--;
			if(Command.SizeSave[0])
				goto DealCommandStart;
		}
		break;
	}
}
#else
void DealData(unsigned char data) //命令解析
{
	u8 result = 0;
	
#if DebugOnlyData
	printf("%c",data);
	return;
#endif
	// 连续记录最近的两个字节，方便回滚命令时使用
	Command.SizeSave[2] = Command.SizeSave[1];
	Command.SizeSave[1] = data;
	Command.SizeSave[0]++;

	DealCommandStart:
	switch(Command.Type)
	{
	// 这是第1个字节的判断，然后才能进行命令的分类处理
	default://eCmdInit
	case eCmdInit:
		if(Command.SizeSave[0] == 1)
		{
			if(Command.SizeSave[1] == 0x0a)
			{
				Command.Type = eCmd0aPrint;
				goto DealCommandStart;		// 单字节命令必须直接处理，不能等后续数据
			}
			else if(Command.SizeSave[1] == 0x0d)
			{
				Command.Type = eCmd0dPrint;
				goto DealCommandStart;		// 单字节命令必须直接处理，不能等后续数据
			}
			else if(Command.SizeSave[1] == 0x1b)
			{
				Command.Type = eCmd1bClass;	// 多字节命令必须等待后续数据才能继续解析
			}
			else if(Command.SizeSave[1] == 0x1d)
			{
				Command.Type = eCmd1dClass;	// 多字节命令必须等待后续数据才能继续解析
			}
			else
			{
				Command.Type = eCmdInit;
				Command.SizeSave[0] = 0;
				Command.State = 0;
				Command.Size = 0;
			}
		}
		else if(Command.SizeSave[0] == 2)
		{
			if(Command.SizeSave[2] == 0x0a)
			{
				Command.SizeSave[0] = 1;
				Command.Type = eCmd0aPrint;
				goto DealCommandStart;		// 单字节命令必须直接处理，不能等后续数据
			}
			else if(Command.SizeSave[2] == 0x0d)
			{
				Command.SizeSave[0] = 1;
				Command.Type = eCmd0dPrint;
				goto DealCommandStart;		// 单字节命令必须直接处理，不能等后续数据
			}
			else if(Command.SizeSave[2] == 0x1b)
			{
				Command.SizeSave[0] = 1;
				Command.Type = eCmd1bClass;
				goto DealCommandStart;
			}
			else if(Command.SizeSave[2] == 0x1d)
			{
				Command.SizeSave[0] = 1;
				Command.Type = eCmd1dClass;
				goto DealCommandStart;
			}
			else
			{
				Command.Type = eCmdInit;
				Command.SizeSave[0] = 0;
				Command.State = 0;
				Command.Size = 0;
				goto DealCommandStart;
			}
		}
		break;
		
	case eCmd0aPrint:	// 单字节命令必须在此次循环中直接处理完毕，不能等下个数据再处理; 命令一旦处理，则肯定没有存储的待解析处理的数据
		Cmd0aPrint();
		
		Command.Type = eCmdInit;
		Command.State = 0;
		Command.Size = 0;
		if(Command.SizeSave[0])
			Command.SizeSave[0]--;
		if(Command.SizeSave[0])
			goto DealCommandStart;
		break;
		
	case eCmd0dPrint:	// 单字节命令必须在此次循环中直接处理完毕，不能等下个数据再处理
		Cmd0dPrint();
		
		Command.Type = eCmdInit;
		Command.State = 0;
		Command.Size = 0;
		if(Command.SizeSave[0])
			Command.SizeSave[0]--;
		if(Command.SizeSave[0])
			goto DealCommandStart;
		break;

	case eCmd1bClass:
		if(Command.SizeSave[0] == 1)
		{
			switch(Command.SizeSave[1])
			{
			case 0x0a:
				Command.Type = eCmd1b0aMode;
				break;

			case 0x0b:
				Command.Type = eCmd1b0bWifi;
				break;

			case 0x2a:
				Command.Type = eCmd1b2aDots;
				break;

			case 0x4a:
				Command.Type = eCmd1b4aFeed;
				break;

			case 0x4b:
				Command.Type = eCmd1b4bBack;
				break;

			case 0x55:
				Command.Type = eCmd1b55Direction;
				break;

			case 0x7a:
				Command.Type = eCmd1b7aSpeed;
				break;

			case 0x80:
				Command.Type = eCmd1b80Pid;
				break;

			case 0x89:
				Command.Type = eCmd1b89Page;
				break;

			case 0x9c:
				Command.Type = eCmd1b9cEnd;
				break;

			case 0xa0:
				Command.Type = eCmd1ba0Start;
				break;

			case 0xf8:
				Command.Type = eCmd1bf8Delay;
				break;

			default:
				Command.Type = eCmdInit;
				Command.SizeSave[0] =0;
				Command.State = 0;
				Command.Size = 0;
				goto DealCommandStart;
				break;
			}
		}
		else if(Command.SizeSave[0] == 2)
		{
			switch(Command.SizeSave[2])
			{
			case 0x0a:
				Command.Type = eCmd1b0aMode;
				goto DealCommandStart;
				break;

			case 0x0b:
				Command.Type = eCmd1b0bWifi;
				goto DealCommandStart;
				break;

			case 0x2a:
				Command.Type = eCmd1b2aDots;
				goto DealCommandStart;
				break;

			case 0x4a:
				Command.Type = eCmd1b4aFeed;
				goto DealCommandStart;
				break;

			case 0x4b:
				Command.Type = eCmd1b4bBack;
				goto DealCommandStart;
				break;

			case 0x55:
				Command.Type = eCmd1b55Direction;
				goto DealCommandStart;
				break;

			case 0x7a:
				Command.Type = eCmd1b7aSpeed;
				goto DealCommandStart;
				break;

			case 0x80:
				Command.Type = eCmd1b80Pid;
				goto DealCommandStart;
				break;

			case 0x89:
				Command.Type = eCmd1b89Page;
				goto DealCommandStart;
				break;

			case 0x9c:
				Command.Type = eCmd1b9cEnd;
				goto DealCommandStart;
				break;

			case 0xa0:
				Command.Type = eCmd1ba0Start;
				goto DealCommandStart;
				break;

			case 0xf8:
				Command.Type = eCmd1bf8Delay;
				goto DealCommandStart;
				break;

			default:
				Command.Type = eCmdInit;
				Command.SizeSave[0] = 1;
				Command.State = 0;
				Command.Size = 0;
				goto DealCommandStart;
				break;
			}
		}
		break;

	case eCmd1b0aMode:
		result = Cmd1b0aMode(data);
		if(result==0) // 命令正常处理结束，才进行后面解析
		{
			Command.Type = eCmdInit;
			Command.State = 0;
			Command.Size = 0;

			Command.SizeSave[0] = 0;
			//goto DealCommandStart;
		}
		//else if(result==1) {;} // 继续传递函数需要的参数
		else if(result==2) //数据异常，需要抛弃第一个数据再次解析
		{
			Command.Type = eCmdInit;
			Command.State = 0;
			Command.Size = 0;
			if(Command.SizeSave[0])
			{
				Command.SizeSave[0]--;
				if(Command.SizeSave[0])
					goto DealCommandStart;
			}
		}
		break;

	case eCmd1b0bWifi:
		result = Cmd1b0bWifi(data);
		if(result==0) // 命令正常处理结束，才进行后面解析
		{
			Command.Type = eCmdInit;
			Command.State = 0;
			Command.Size = 0;

			Command.SizeSave[0] = 0;
			//goto DealCommandStart;
		}
		//else if(result==1) {;} // 继续传递函数需要的参数
		else if(result==2) //数据异常，需要抛弃第一个数据再次解析
		{
			Command.Type = eCmdInit;
			Command.State = 0;
			Command.Size = 0;
			if(Command.SizeSave[0])
			{
				Command.SizeSave[0]--;
				if(Command.SizeSave[0])
					goto DealCommandStart;
			}
		}
		break;

	case eCmd1b2aDots:
		result = Cmd1b2aDots(data);
		if(result==0) // 命令正常处理结束，才进行后面解析
		{
			Command.Type = eCmdInit;
			Command.State = 0;
			Command.Size = 0;

			Command.SizeSave[0] = 0;
			//goto DealCommandStart;
		}
		//else if(result==1) {;} // 继续传递函数需要的参数
		else if(result==2) //数据异常，需要抛弃第一个数据再次解析
		{
			Command.Type = eCmdInit;
			Command.State = 0;
			Command.Size = 0;
			if(Command.SizeSave[0])
			{
				Command.SizeSave[0]--;
				if(Command.SizeSave[0])
					goto DealCommandStart;
			}
		}
		break;

	case eCmd1b4aFeed:
		Cmd1b4aFeed(data);
		Command.SizeSave[0] = 0;
		break;

	case eCmd1b4bBack:
		Cmd1b4bBack(data);
		Command.SizeSave[0] = 0;
		break;

	case eCmd1b55Direction:
		Cmd1b55Direction(data);
		Command.SizeSave[0] = 0;
		break;

	case eCmd1b7aSpeed:
		Cmd1b7aSpeed(data);
		Command.SizeSave[0] = 0;
		break;

	case eCmd1b80Pid:
		Cmd1b80Pid(data);
		Command.SizeSave[0] = 0;
		break;

	case eCmd1b89Page:
		Cmd1b89Page(data);
		Command.SizeSave[0] = 0;
		break;

	case eCmd1b9cEnd:
		Cmd1b9cEnd(data);
		Command.SizeSave[0] = 0;
		break;

	case eCmd1ba0Start:
		Cmd1ba0Start(data);
		Command.SizeSave[0] = 0;
		break;

	case eCmd1bf8Delay:
		Cmd1bf8Dealy(data);
		Command.SizeSave[0] = 0;
		break;

	case eCmd1dClass:
		switch(data)
		{
		case 0x56:
			Command.Type = eCmd1d56CutPaper;
			break;

		default:
			Command.Type = eCmdInit;
			Command.State = 0;
			Command.Size = 0;
			break;
		}
		break;

	case eCmd1d56CutPaper:
		Cmd1d56CutPaper(data);
		Command.SizeSave[0] = 0;
		break;

	}
}}
#endif


/**************************************************************************************************************
	Driver.c	Driver.h
**************************************************************************************************************/

