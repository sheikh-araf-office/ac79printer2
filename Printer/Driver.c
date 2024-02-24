
//#define Driver 0
/**************************************************************************************************************
	Driver.c	Driver.h
**************************************************************************************************************/

#include "Driver.h"
#include "McuInt.h"
#include "UserInterface.h"

const u8 TABLELetterA[95][7] =		// 7*94=658	7 byte/letter
{
	0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, // 
	0x00, 0x00, 0x00, 0xf2,	0x00, 0x00, 0x00, // !
	0x00, 0xe0, 0x00, 0x00,	0x00, 0xe0, 0x00, // "
	0x28, 0xd6, 0x28, 0x00,	0x28, 0xd6, 0x28, // #
	0xa8, 0x40, 0x28, 0x16,	0x28, 0x40, 0xa8, // ��
	0xc2, 0x04, 0xc8, 0x10,	0x26, 0x40, 0x86, // %
	0x0c, 0x52, 0xa0, 0x12,	0xa8, 0x44, 0x0a, // &
	0x00, 0x00, 0xa0, 0x00,	0xc0, 0x00, 0x00, // '
	0x00, 0x00, 0x38, 0x44,	0x82, 0x00, 0x00, // (
	0x00, 0x00, 0x82, 0x44,	0x38, 0x00, 0x00, // )
	0x10, 0x44, 0x38, 0x00,	0x38, 0x44, 0x10, // *
	0x10, 0x00, 0x10, 0x6c,	0x10, 0x00, 0x10, // +
	0x00, 0x00, 0x0a, 0x00,	0x0c, 0x00, 0x00, // ,
	0x10, 0x00, 0x10, 0x00,	0x10, 0x00, 0x10, // -
	0x00, 0x00, 0x06, 0x00,	0x06, 0x00, 0x00, // .
	0x02, 0x04, 0x08, 0x10,	0x20, 0x40, 0x80, // /
	0x38, 0x44, 0x8a, 0x10,	0xa2, 0x44, 0x38, // 0
	0x00, 0x42, 0x00, 0xfe,	0x00, 0x02, 0x00, // 1
	0x46, 0x80, 0x0a, 0x80,	0x12, 0x80, 0x62, // 2
	0x84, 0x02, 0x80, 0x22,	0x80, 0x52, 0x8c, // 3
	0x18, 0x20, 0x48, 0x80,	0x3e, 0x00, 0x08, // 4
	0xe4, 0x02, 0xa0, 0x02,	0xa0, 0x02, 0x9c, // 5
	0x0c, 0x12, 0x20, 0x52,	0x80, 0x12, 0x0c, // 6
	0xc0, 0x00, 0x80, 0x0e,	0x90, 0x20, 0xc0, // 7
	0x6c, 0x92, 0x00, 0x92,	0x00, 0x92, 0x6c, // 8
	0x60, 0x90, 0x02, 0x94,	0x08, 0x90, 0x60, // 9
	0x00, 0x00, 0x6c, 0x00,	0x6c, 0x00, 0x00, // :
	0x00, 0x00, 0x6a, 0x00,	0x6c, 0x00, 0x00, // ;
	0x00, 0x10, 0x00, 0x28,	0x00, 0x44, 0x00, // <
	0x28, 0x00, 0x28, 0x00,	0x28, 0x00, 0x28, // =
	0x00, 0x44, 0x00, 0x28,	0x00, 0x10, 0x00, // >
	0x40, 0x80, 0x00, 0x8a,	0x10, 0x80, 0x60, // ?
	0x7c, 0x82, 0x00, 0x92,	0x28, 0x82, 0x78, // @
	0x1e, 0x20, 0x48, 0x80,	0x48, 0x20, 0x1e, // A
	0x82, 0x7c, 0x82, 0x10,	0x82, 0x10, 0x6c, // B
	0x7c, 0x82, 0x00, 0x82,	0x00, 0x82, 0x44, // C
	0x82, 0x7c, 0x82, 0x00,	0x82, 0x00, 0x7c, // D
	0xfe, 0x00, 0x92, 0x00,	0x92, 0x00, 0x82, // E
	0xfe, 0x00, 0x90, 0x00,	0x90, 0x00, 0x80, // F
	0x7c, 0x82, 0x00, 0x82,	0x10, 0x82, 0x5c, // G
	0xfe, 0x00, 0x10, 0x00,	0x10, 0x00, 0xfe, // H
	0x00, 0x82, 0x00, 0xfe,	0x00, 0x82, 0x00, // I
	0x04, 0x82, 0x00, 0x82,	0x7c, 0x80, 0x00, // J
	0xfe, 0x00, 0x10, 0x28,	0x44, 0x82, 0x00, // K
	0xfe, 0x00, 0x02, 0x00,	0x02, 0x00, 0x02, // L
	0xbe, 0x40, 0x20, 0x10,	0x20, 0x40, 0xbe, // M
	0xbe, 0x40, 0x20, 0x10,	0x08, 0x04, 0xfa, // N
	0x7c, 0x82, 0x00, 0x82,	0x00, 0x82, 0x7c, // O
	0xfe, 0x00, 0x90, 0x00,	0x90, 0x00, 0x60, // P
	0x7c, 0x82, 0x00, 0x8a,	0x00, 0x84, 0x7a, // Q
	0xfe, 0x00, 0x90, 0x00,	0x98, 0x04, 0x62, // R
	0x64, 0x92, 0x00, 0x92,	0x00, 0x92, 0x4c, // S
	0x80, 0x00, 0x80, 0x7e,	0x80, 0x00, 0x80, // T
	0xfc, 0x02, 0x00, 0x02,	0x00, 0x02, 0xfc, // U
	0xf0, 0x08, 0x04, 0x02,	0x04, 0x08, 0xf0, // V
	0xfc, 0x02, 0x04, 0x18,	0x04, 0x02, 0xfc, // W
	0x82, 0x44, 0x28, 0x10,	0x28, 0x44, 0x82, // X
	0x80, 0x40, 0x20, 0x1e,	0x20, 0x40, 0x80, // Y
	0x82, 0x04, 0x8a, 0x10,	0xa2, 0x40, 0x82, // Z
	0x00, 0xfe, 0x00, 0x82,	0x00, 0x82, 0x00, // [
	0x80, 0x40, 0x20, 0x10,	0x08, 0x04, 0x02, // \ @20220727 ���з�����ӵ������ַ��ո��
	0x00, 0x82, 0x00, 0x82,	0x00, 0xfe, 0x00, // ]
	0x00, 0x20, 0x40, 0x80,	0x40, 0x20, 0x00, // ^
	0x02, 0x00, 0x02, 0x00,	0x02, 0x00, 0x02, // _
	0x00, 0x00, 0xc0, 0x00,	0xa0, 0x00, 0x00, // '
	0x04, 0x20, 0x0a, 0x20,	0x0a, 0x20, 0x1e, // a
	0xfe, 0x00, 0x22, 0x00,	0x22, 0x00, 0x1c, // b
	0x1c, 0x00, 0x22, 0x00,	0x22, 0x00, 0x22, // c
	0x1c, 0x00, 0x22, 0x00,	0x22, 0x00, 0xfe, // d
	0x1c, 0x22, 0x08, 0x22,	0x08, 0x22, 0x18, // e
	0x20, 0x00, 0x20, 0x5e,	0xa0, 0x00, 0xa0, // f
	0x10, 0x02, 0x28, 0x02,	0x28, 0x02, 0x3c, // g
	0xfe, 0x00, 0x20, 0x00,	0x20, 0x00, 0x1e, // h
	0x00, 0x22, 0x00, 0xbe,	0x00, 0x02, 0x00, // i
	0x04, 0x00, 0x02, 0x20,	0x02, 0xbc, 0x00, // j
	0xfe, 0x00, 0x08, 0x00,	0x14, 0x00, 0x22, // k
	0x00, 0x82, 0x00, 0xfe,	0x00, 0x02, 0x00, // l
	0x3e, 0x00, 0x20, 0x1e,	0x20, 0x00, 0x1e, // m
	0x20, 0x1e, 0x00, 0x20,	0x00, 0x20, 0x1e, // n
	0x1c, 0x22, 0x00, 0x22,	0x00, 0x22, 0x1c, // o
	0x3e, 0x00, 0x28, 0x00,	0x28, 0x10, 0x00, // p
	0x10, 0x28, 0x00, 0x28,	0x00, 0x3e, 0x00, // q
	0x3e, 0x00, 0x10, 0x20,	0x00, 0x20, 0x00, // r
	0x10, 0x2a, 0x00, 0x2a,	0x00, 0x2a, 0x04, // s
	0x20, 0x00, 0x20, 0xdc,	0x22, 0x00, 0x22, // t
	0x3c, 0x02, 0x00, 0x02,	0x00, 0x3c, 0x02, // u
	0x30, 0x08, 0x04, 0x02,	0x04, 0x08, 0x30, // v
	0x3c, 0x02, 0x04, 0x08,	0x04, 0x02, 0x3c, // w
	0x22, 0x14, 0x00, 0x08,	0x00, 0x14, 0x22, // x
	0x22, 0x10, 0x0a, 0x04,	0x08, 0x10, 0x20, // y
	0x22, 0x04, 0x22, 0x08,	0x22, 0x10, 0x22, // z
	0x10, 0x00, 0x6c, 0x82,	0x00, 0x82, 0x00, // {
	0x00, 0x00, 0x00, 0xee,	0x00, 0x00, 0x00, // |
	0x82, 0x00, 0x82, 0x00,	0x6c, 0x00, 0x10, // }
	0x40, 0x00, 0x80, 0x00,	0x40, 0x00, 0x80  // ~
};

struct wifi_stored_sta_infoAlke
{
    u8 pwd[64];
    u8 ssid[33];
}	wifi_setup_sta_info;


extern u8 sDeviceDescriptor[];

// @20230601 �����µĳ���ܹ����˺���Ϊ�յ����ݺ�Ĵ����¼�
// �˺�������һ���ֽ�һ���ֽڽ��д������˹����в�������ѭ����ֻ��һ�ι����������ñ�־λ

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

struct netif_info netif_infoA;
extern void lwip_get_netif_info(u8 is_wireless, struct netif_info *info);




// 0x1B 0X0B 0X01 USB Printer�� 0X0A // ��ʱ����USB����
// 0x1B 0X0B 0X02 WIFI SSID+PWD�� 0X0A // ���SSID+PWD��ʱ�ظ�


// 0x1B 0X0B 0X03 IP"192.168.0.105" 0X0A // 3 WIFI_EVENT_STA_NETWORK_STACK_DHCP_SUCC���������3״̬���������IP��ַ��USB����������ɣ�������л���WIFIͨѶ

// 0x1B 0X0B 0X06 NOT_FOUND_SSID Try restarting the printer 0X0A
// 0x1B 0X0B 0X07 ASSOCIAT_FAIL 0X0A
// 0x1B 0X0B 0X08 ASSOCIAT_TIMEOUT 0X0A
// 0x1B 0X0B 0X09 DHCP_TIMEOUT 0X0A


// 0x1B 0X0B 0X05 Mode&PID&IP&MCUID&Printer 0X0A // WIFI��ѯ��ʱ�ظ�����Ϣ����Ҫ���������Ϣ��װ��ӡ����������
void USBReturnWifi(unsigned char Step) // ���64�ֽ�
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
		UsbDataBack[c8cc++] = '��';
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

		flag = 0; // ��0���ݣ�����Ϊ0Ҳ����ʾ
		c8d1 = netif_infoA.ip&0x000000ff;
		if(c8d1 > 99)
			{UsbDataBack[c8cc++] = 0x30+c8d1/100; flag = 1;}
		c8d1 %= 100;
		if(c8d1 > 9)
			UsbDataBack[c8cc++] = 0x30+c8d1/10;
		else if(flag) // ��λ��0����ʱ0Ҳ����ʾ
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

		flag = 0; // ��0���ݣ�����Ϊ0Ҳ����ʾ
		c8d1 = netif_infoA.ip&0x000000ff;
		if(c8d1 > 99)
			{UsbDataBack[c8cc++] = 0x30+c8d1/100; flag = 1;}
		c8d1 %= 100;
		if(c8d1 > 9)
			UsbDataBack[c8cc++] = 0x30+c8d1/10;
		else if(flag) // ��λ��0����ʱ0Ҳ����ʾ
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

	// @20231224 Alke use mac_address as ID number 
	const u8 *mac_addr;
	mac_addr = bt_get_mac_addr();
	//printf("MAC:%02X%02X%02X%02X%02X%02X", mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
	UsbDataBack[c8cc++] = '&';
	
	UsbDataBack[c8cc] = mac_addr[0];
	UsbDataBack[c8cc] = (UsbDataBack[c8cc]&0xf0)>>8;
	if(UsbDataBack[c8cc]<10)
		UsbDataBack[c8cc] = UsbDataBack[c8cc]+'0';
	else
		UsbDataBack[c8cc] = UsbDataBack[c8cc]-10+'A';
	c8cc++;
	UsbDataBack[c8cc] = mac_addr[0];
	UsbDataBack[c8cc] &= 0x0f;
	if(UsbDataBack[c8cc]<10)
		UsbDataBack[c8cc] = UsbDataBack[c8cc]+'0';
	else
		UsbDataBack[c8cc] = UsbDataBack[c8cc]-10+'A';
	c8cc++;
	
	UsbDataBack[c8cc] = mac_addr[1];
	UsbDataBack[c8cc] = (UsbDataBack[c8cc]&0xf0)>>8;
	if(UsbDataBack[c8cc]<10)
		UsbDataBack[c8cc] = UsbDataBack[c8cc]+'0';
	else
		UsbDataBack[c8cc] = UsbDataBack[c8cc]-10+'A';
	c8cc++;
	UsbDataBack[c8cc] = mac_addr[1];
	UsbDataBack[c8cc] &= 0x0f;
	if(UsbDataBack[c8cc]<10)
		UsbDataBack[c8cc] = UsbDataBack[c8cc]+'0';
	else
		UsbDataBack[c8cc] = UsbDataBack[c8cc]-10+'A';
	c8cc++;
	
	UsbDataBack[c8cc] = mac_addr[2];
	UsbDataBack[c8cc] = (UsbDataBack[c8cc]&0xf0)>>8;
	if(UsbDataBack[c8cc]<10)
		UsbDataBack[c8cc] = UsbDataBack[c8cc]+'0';
	else
		UsbDataBack[c8cc] = UsbDataBack[c8cc]-10+'A';
	c8cc++;
	UsbDataBack[c8cc] = mac_addr[2];
	UsbDataBack[c8cc] &= 0x0f;
	if(UsbDataBack[c8cc]<10)
		UsbDataBack[c8cc] = UsbDataBack[c8cc]+'0';
	else
		UsbDataBack[c8cc] = UsbDataBack[c8cc]-10+'A';
	c8cc++;
	
	UsbDataBack[c8cc] = mac_addr[3];
	UsbDataBack[c8cc] = (UsbDataBack[c8cc]&0xf0)>>8;
	if(UsbDataBack[c8cc]<10)
		UsbDataBack[c8cc] = UsbDataBack[c8cc]+'0';
	else
		UsbDataBack[c8cc] = UsbDataBack[c8cc]-10+'A';
	c8cc++;
	UsbDataBack[c8cc] = mac_addr[3];
	UsbDataBack[c8cc] &= 0x0f;
	if(UsbDataBack[c8cc]<10)
		UsbDataBack[c8cc] = UsbDataBack[c8cc]+'0';
	else
		UsbDataBack[c8cc] = UsbDataBack[c8cc]-10+'A';
	c8cc++;
	
	UsbDataBack[c8cc] = mac_addr[4];
	UsbDataBack[c8cc] = (UsbDataBack[c8cc]&0xf0)>>8;
	if(UsbDataBack[c8cc]<10)
		UsbDataBack[c8cc] = UsbDataBack[c8cc]+'0';
	else
		UsbDataBack[c8cc] = UsbDataBack[c8cc]-10+'A';
	c8cc++;
	UsbDataBack[c8cc] = mac_addr[4];
	UsbDataBack[c8cc] &= 0x0f;
	if(UsbDataBack[c8cc]<10)
		UsbDataBack[c8cc] = UsbDataBack[c8cc]+'0';
	else
		UsbDataBack[c8cc] = UsbDataBack[c8cc]-10+'A';
	c8cc++;
	
	UsbDataBack[c8cc] = mac_addr[5];
	UsbDataBack[c8cc] = (UsbDataBack[c8cc]&0xf0)>>8;
	if(UsbDataBack[c8cc]<10)
		UsbDataBack[c8cc] = UsbDataBack[c8cc]+'0';
	else
		UsbDataBack[c8cc] = UsbDataBack[c8cc]-10+'A';
	c8cc++;
	UsbDataBack[c8cc] = mac_addr[5];
	UsbDataBack[c8cc] &= 0x0f;
	if(UsbDataBack[c8cc]<10)
		UsbDataBack[c8cc] = UsbDataBack[c8cc]+'0';
	else
		UsbDataBack[c8cc] = UsbDataBack[c8cc]-10+'A';
	c8cc++;
		/*UsbDataBack[c8cc++] = '&';
		UsbDataBack[c8cc++] = '0';
		UsbDataBack[c8cc++] = '1';
		UsbDataBack[c8cc++] = '2';
		UsbDataBack[c8cc++] = '3';
		UsbDataBack[c8cc++] = '4';
		UsbDataBack[c8cc++] = '5';
		UsbDataBack[c8cc++] = '6';
		UsbDataBack[c8cc++] = '7';
		UsbDataBack[c8cc++] = '8';
		UsbDataBack[c8cc++] = '9';*/
		
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

	flag = 0; // ��0���ݣ�����Ϊ0Ҳ����ʾ
	c8d1 = netif_infoA.ip&0x000000ff;
	if(c8d1 > 99)
		{UsbDataBack[c8cc++] = 0x30+c8d1/100; flag = 1;}
	c8d1 %= 100;
	if(c8d1 > 9)
		UsbDataBack[c8cc++] = 0x30+c8d1/10;
	else if(flag) // ��λ��0����ʱ0Ҳ����ʾ
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

	// @20231224 Alke use mac_address as ID number 
	const u8 *mac_addr;
	mac_addr = bt_get_mac_addr();
	//printf("MAC:%02X%02X%02X%02X%02X%02X", mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
	UsbDataBack[c8cc++] = '&';
	
	UsbDataBack[c8cc] = mac_addr[0];
	UsbDataBack[c8cc] = (UsbDataBack[c8cc]&0xf0)>>8;
	if(UsbDataBack[c8cc]<10)
		UsbDataBack[c8cc] = UsbDataBack[c8cc]+'0';
	else
		UsbDataBack[c8cc] = UsbDataBack[c8cc]-10+'A';
	c8cc++;
	UsbDataBack[c8cc] = mac_addr[0];
	UsbDataBack[c8cc] &= 0x0f;
	if(UsbDataBack[c8cc]<10)
		UsbDataBack[c8cc] = UsbDataBack[c8cc]+'0';
	else
		UsbDataBack[c8cc] = UsbDataBack[c8cc]-10+'A';
	c8cc++;
	
	UsbDataBack[c8cc] = mac_addr[1];
	UsbDataBack[c8cc] = (UsbDataBack[c8cc]&0xf0)>>8;
	if(UsbDataBack[c8cc]<10)
		UsbDataBack[c8cc] = UsbDataBack[c8cc]+'0';
	else
		UsbDataBack[c8cc] = UsbDataBack[c8cc]-10+'A';
	c8cc++;
	UsbDataBack[c8cc] = mac_addr[1];
	UsbDataBack[c8cc] &= 0x0f;
	if(UsbDataBack[c8cc]<10)
		UsbDataBack[c8cc] = UsbDataBack[c8cc]+'0';
	else
		UsbDataBack[c8cc] = UsbDataBack[c8cc]-10+'A';
	c8cc++;
	
	UsbDataBack[c8cc] = mac_addr[2];
	UsbDataBack[c8cc] = (UsbDataBack[c8cc]&0xf0)>>8;
	if(UsbDataBack[c8cc]<10)
		UsbDataBack[c8cc] = UsbDataBack[c8cc]+'0';
	else
		UsbDataBack[c8cc] = UsbDataBack[c8cc]-10+'A';
	c8cc++;
	UsbDataBack[c8cc] = mac_addr[2];
	UsbDataBack[c8cc] &= 0x0f;
	if(UsbDataBack[c8cc]<10)
		UsbDataBack[c8cc] = UsbDataBack[c8cc]+'0';
	else
		UsbDataBack[c8cc] = UsbDataBack[c8cc]-10+'A';
	c8cc++;
	
	UsbDataBack[c8cc] = mac_addr[3];
	UsbDataBack[c8cc] = (UsbDataBack[c8cc]&0xf0)>>8;
	if(UsbDataBack[c8cc]<10)
		UsbDataBack[c8cc] = UsbDataBack[c8cc]+'0';
	else
		UsbDataBack[c8cc] = UsbDataBack[c8cc]-10+'A';
	c8cc++;
	UsbDataBack[c8cc] = mac_addr[3];
	UsbDataBack[c8cc] &= 0x0f;
	if(UsbDataBack[c8cc]<10)
		UsbDataBack[c8cc] = UsbDataBack[c8cc]+'0';
	else
		UsbDataBack[c8cc] = UsbDataBack[c8cc]-10+'A';
	c8cc++;
	
	UsbDataBack[c8cc] = mac_addr[4];
	UsbDataBack[c8cc] = (UsbDataBack[c8cc]&0xf0)>>8;
	if(UsbDataBack[c8cc]<10)
		UsbDataBack[c8cc] = UsbDataBack[c8cc]+'0';
	else
		UsbDataBack[c8cc] = UsbDataBack[c8cc]-10+'A';
	c8cc++;
	UsbDataBack[c8cc] = mac_addr[4];
	UsbDataBack[c8cc] &= 0x0f;
	if(UsbDataBack[c8cc]<10)
		UsbDataBack[c8cc] = UsbDataBack[c8cc]+'0';
	else
		UsbDataBack[c8cc] = UsbDataBack[c8cc]-10+'A';
	c8cc++;
	
	UsbDataBack[c8cc] = mac_addr[5];
	UsbDataBack[c8cc] = (UsbDataBack[c8cc]&0xf0)>>8;
	if(UsbDataBack[c8cc]<10)
		UsbDataBack[c8cc] = UsbDataBack[c8cc]+'0';
	else
		UsbDataBack[c8cc] = UsbDataBack[c8cc]-10+'A';
	c8cc++;
	UsbDataBack[c8cc] = mac_addr[5];
	UsbDataBack[c8cc] &= 0x0f;
	if(UsbDataBack[c8cc]<10)
		UsbDataBack[c8cc] = UsbDataBack[c8cc]+'0';
	else
		UsbDataBack[c8cc] = UsbDataBack[c8cc]-10+'A';
	c8cc++;
	
	/*UsbDataBack[c8cc++] = '0';
	UsbDataBack[c8cc++] = '1';
	UsbDataBack[c8cc++] = '2';
	UsbDataBack[c8cc++] = '3';
	UsbDataBack[c8cc++] = '4';
	UsbDataBack[c8cc++] = '5';
	UsbDataBack[c8cc++] = '6';
	UsbDataBack[c8cc++] = '7';
	UsbDataBack[c8cc++] = '8';
	UsbDataBack[c8cc++] = '9';*/
	
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
			// û��ռ��ʱ�����һλ�ַ������һ��0��Ϊ�������
			if(Size < 33) // wifi_stored_sta_info.ssid[33]
			{
				wifi_sta_info.ssid[Size] = 0;
				Size++;
			}

			State++; // ������Ч
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
			// û��ռ��ʱ�����һλ�ַ������һ��0��Ϊ�������
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
				wifi_del_stored_sta_info(wifi_stored_sta_info[i].ssid);// @ Alke 20230711 ɾ���洢����
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
			// ��ҪWIFI������Ϣ
			ReturnMsg = 0xff;//WifiReturnMsg();
			State = 0;
		}
		break;
	}
}


// ���Դ�����
void Cmd0aPrint(void) // ������û�к�������
{
	u8 data;
#if SendCmd
	printf("Cmd0aPrint\n");
//	USB_RevC->DataOut("USB: Cmd0dPrint \r\n", 64);
//  tcp_send_data_sang("Wifi: Cmd0dPrint \r\n",17);
//	BT_RevC->DataOut("Spp: Cmd0dPrint \r\n",6);
#endif

	//if(cBufferPrintDataWrite == 0)
	//	return;
	
	// Ĭ���ǰ��պ��ֽ��ʽ��֯����;���ǰ��ֽģʽ����Ҫ����ǰ�������������Ҫ���µ�ת
	if(PaperState < sPaperBackNo)
	{
		BufferPrint[cBufferPrintLineWrite].Dir = sPaperDirFrontToBack;
		for(cBufferPrintDataWrite = 0; cBufferPrintDataWrite < (BufferPRTSize/2); cBufferPrintDataWrite++)
		{
			data = ByteBitChang(BufferPrint[cBufferPrintLineWrite].BufferA[cBufferPrintDataWrite]);
			BufferPrint[cBufferPrintLineWrite].BufferA[cBufferPrintDataWrite] = ByteBitChang(BufferPrint[cBufferPrintLineWrite].BufferA[BufferPRTSize-1-cBufferPrintDataWrite]);
			BufferPrint[cBufferPrintLineWrite].BufferA[BufferPRTSize-1-cBufferPrintDataWrite] = data;
			
			data = ByteBitChang(BufferPrint[cBufferPrintLineWrite].BufferB[cBufferPrintDataWrite]);
			BufferPrint[cBufferPrintLineWrite].BufferB[cBufferPrintDataWrite] = ByteBitChang(BufferPrint[cBufferPrintLineWrite].BufferB[BufferPRTSize-1-cBufferPrintDataWrite]);
			BufferPrint[cBufferPrintLineWrite].BufferB[BufferPRTSize-1-cBufferPrintDataWrite] = data;
		}
	}
	else
		BufferPrint[cBufferPrintLineWrite].Dir = sPaperDirBackToFront;

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

	if(BufferPrint[cBufferPrintLineWrite].cLeft > BufferPrint[cBufferPrintLineWrite].cRight)
		BufferPrint[cBufferPrintLineWrite].cLeft = BufferPrint[cBufferPrintLineWrite].cRight;

	// ��������λ�����㵽��������
	cBufferPrintDataRead = BufferPrint[cBufferPrintLineWrite].cLeft*BufferOneDataTimePosition;
	cBufferPrintDataWrite = cBufferPrintDataRead%CRGOneStepTimePosition;
	BufferPrint[cBufferPrintLineWrite].cLeft = cBufferPrintDataRead - cBufferPrintDataWrite;			// adjust to th 16*n
	// ��������λ�����㵽��������
	cBufferPrintDataRead = BufferPrint[cBufferPrintLineWrite].cRight*BufferOneDataTimePosition;
	cBufferPrintDataWrite = cBufferPrintDataRead%CRGOneStepTimePosition;
	BufferPrint[cBufferPrintLineWrite].cRight = cBufferPrintDataRead +CRGOneStepTimePosition- cBufferPrintDataWrite;	 // adjust to th 16*n
	if(BufferPrint[cBufferPrintLineWrite].cRight > TimePositionAll)
		BufferPrint[cBufferPrintLineWrite].cRight = TimePositionAll;
	// ��������λ�����㵽������ʼ�˶�λ����
	if(BufferPrint[cBufferPrintLineWrite].cLeft > (CRGVolatileStep*CRGOneStepTimePosition+5*CRGOneStepTimePosition))
		BufferPrint[cBufferPrintLineWrite].cLeft -= (CRGVolatileStep*CRGOneStepTimePosition+5*CRGOneStepTimePosition);
	else
		BufferPrint[cBufferPrintLineWrite].cLeft = 0;
	// ��������λ�����㵽������ʼ�˶�λ����
	if((BufferPrint[cBufferPrintLineWrite].cRight+CRGVolatileStep*CRGOneStepTimePosition*2+15*CRGOneStepTimePosition)<TimePositionAll)
		BufferPrint[cBufferPrintLineWrite].cRight += CRGVolatileStep*CRGOneStepTimePosition*2+15*CRGOneStepTimePosition;
	else
		BufferPrint[cBufferPrintLineWrite].cRight = TimePositionAll;

	// @20231231 ÿ����ӡ�п��Լ��д�ӡ���ݣ�Ҳ��������ֽ��������ӡ�������ȣ�����ֻ�д�ӡ���ݣ�����ֻ�в���
	BufferPrint[cBufferPrintLineWrite].Step = 24;
	if(cBufferPrintLineWrite >= (BufferPrintLineNum-1))
		cBufferPrintLineWrite = 0;
	else
		cBufferPrintLineWrite++;

	cBufferPrintDataWrite = 0;

	// ������һ�е�����ֽ�����ܻ���ֻ������������������
	//if(BufferPrintLineWrited() < BufferPrintLineFull) //BufferPrintLineNum
/*	while(BufferPrintLineWrited() >= BufferPrintLineFull);
	{
		for(cBufferPrintDataWrite = 0; cBufferPrintDataWrite < BufferPRTSize; cBufferPrintDataWrite++)
		{
			BufferPrint[cBufferPrintLineWrite].BufferA[cBufferPrintDataWrite] = 0x00;
			BufferPrint[cBufferPrintLineWrite].BufferB[cBufferPrintDataWrite] = 0x00;
		}
		cBufferPrintDataWrite = 0;
		BufferPrint[cBufferPrintLineWrite].cLeft=0;
		BufferPrint[cBufferPrintLineWrite].cRight=0;

		BufferPrint[cBufferPrintLineWrite].Step = 24;
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
	}*/
}
// ���Դ�����
void Cmd0dPrint(void) //������û�к�������
{
	u8 data;
#if SendCmd
	printf("Cmd0dPrint\n");
//	USB_RevC->DataOut("USB: Cmd0dPrint \r\n", 64);
//  tcp_send_data_sang("Wifi: Cmd0dPrint \r\n",17);
//	BT_RevC->DataOut("Spp: Cmd0dPrint \r\n",6);
#endif

	//if(cBufferPrintDataWrite == 0)
	//	return;

	// Ĭ���ǰ��պ��ֽ��ʽ��֯����;���ǰ��ֽģʽ����Ҫ����ǰ�������������Ҫ���µ�ת
	if(PaperState < sPaperBackNo)
	{
		for(cBufferPrintDataWrite = 0; cBufferPrintDataWrite < (BufferPRTSize/2); cBufferPrintDataWrite++)
		{
			data = ByteBitChang(BufferPrint[cBufferPrintLineWrite].BufferA[cBufferPrintDataWrite]);
			BufferPrint[cBufferPrintLineWrite].BufferA[cBufferPrintDataWrite] = ByteBitChang(BufferPrint[cBufferPrintLineWrite].BufferA[BufferPRTSize-1-cBufferPrintDataWrite]);
			BufferPrint[cBufferPrintLineWrite].BufferA[BufferPRTSize-1-cBufferPrintDataWrite] = data;
			
			data = ByteBitChang(BufferPrint[cBufferPrintLineWrite].BufferB[cBufferPrintDataWrite]);
			BufferPrint[cBufferPrintLineWrite].BufferB[cBufferPrintDataWrite] = ByteBitChang(BufferPrint[cBufferPrintLineWrite].BufferB[BufferPRTSize-1-cBufferPrintDataWrite]);
			BufferPrint[cBufferPrintLineWrite].BufferB[BufferPRTSize-1-cBufferPrintDataWrite] = data;
		}
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

	if(BufferPrint[cBufferPrintLineWrite].cLeft > BufferPrint[cBufferPrintLineWrite].cRight)
		BufferPrint[cBufferPrintLineWrite].cLeft = BufferPrint[cBufferPrintLineWrite].cRight;

	// ��������λ�����㵽��������
	cBufferPrintDataRead = BufferPrint[cBufferPrintLineWrite].cLeft*BufferOneDataTimePosition;
	cBufferPrintDataWrite = cBufferPrintDataRead%CRGOneStepTimePosition;
	BufferPrint[cBufferPrintLineWrite].cLeft = cBufferPrintDataRead - cBufferPrintDataWrite;			// adjust to th 16*n
	// ��������λ�����㵽��������
	cBufferPrintDataRead = BufferPrint[cBufferPrintLineWrite].cRight*BufferOneDataTimePosition;
	cBufferPrintDataWrite = cBufferPrintDataRead%CRGOneStepTimePosition;
	BufferPrint[cBufferPrintLineWrite].cRight = cBufferPrintDataRead +CRGOneStepTimePosition- cBufferPrintDataWrite;	 // adjust to th 16*n
	if(BufferPrint[cBufferPrintLineWrite].cRight > TimePositionAll)
		BufferPrint[cBufferPrintLineWrite].cRight = TimePositionAll;
	// ��������λ�����㵽������ʼ�˶�λ����
	if(BufferPrint[cBufferPrintLineWrite].cLeft > (CRGVolatileStep*CRGOneStepTimePosition+5*CRGOneStepTimePosition))
		BufferPrint[cBufferPrintLineWrite].cLeft -= (CRGVolatileStep*CRGOneStepTimePosition+5*CRGOneStepTimePosition);
	else
		BufferPrint[cBufferPrintLineWrite].cLeft = 0;
	// ��������λ�����㵽������ʼ�˶�λ����
	if((BufferPrint[cBufferPrintLineWrite].cRight+CRGVolatileStep*CRGOneStepTimePosition*2+15*CRGOneStepTimePosition)<TimePositionAll)
		BufferPrint[cBufferPrintLineWrite].cRight += CRGVolatileStep*CRGOneStepTimePosition*2+15*CRGOneStepTimePosition;
	else
		BufferPrint[cBufferPrintLineWrite].cRight = TimePositionAll;

	BufferPrint[cBufferPrintLineWrite].Step = 0;
	if(cBufferPrintLineWrite >= (BufferPrintLineNum-1))
		cBufferPrintLineWrite = 0;
	else
		cBufferPrintLineWrite++;

	cBufferPrintDataWrite = 0;
}
void WriteLetter(u8 Letter)
{
	u8 c8;

	if( (Letter < 0x20) || (Letter > 0x7e) )	// 0x20~0x7e
		return;

	for(c8 = 0; c8 < 7; c8++)
	{
		BufferPrint[cBufferPrintLineWrite].BufferA[cBufferPrintDataWrite] = TABLELetterA[Letter-0x20][c8];
		BufferPrint[cBufferPrintLineWrite].BufferB[cBufferPrintDataWrite] = TABLELetterA[Letter-0x20][c8];
		cBufferPrintDataWrite++;
	}

	for(c8 = 0; c8 < 3; c8++)
	{
		BufferPrint[cBufferPrintLineWrite].BufferA[cBufferPrintDataWrite] = 0x00;
		BufferPrint[cBufferPrintLineWrite].BufferB[cBufferPrintDataWrite] = 0x00;
		cBufferPrintDataWrite++;
	}
}


// �������ֽ�Ϊ1b��������
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
// return 0: ��������	1:�ȴ�����		2�� �����쳣�������������ݣ������������ٴν���
u8 Cmd1b01Printer(unsigned char data)
{
	u8 result;

	switch(Command.State)
	{
	default:
	case 0:
#if SendCmd
		printf("Cmd1b01Printer\n");
#endif
		if(data == 0x0a)
		{
			Command.State = 10; // ������Ч
			result = 1;
		}
		break;

	case 10:
		if(data == 0x0a)
		{
			Command.State = 0; // ������Ч
			result = 0;
		}
		break;
	}
	
	return result;
}
// return 0: ��������	1:�ȴ�����		2�� �����쳣�������������ݣ������������ٴν���
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
			Command.State = 10; // ������Ч
			result = 1;
		}
		else
			result = 2;
		break;

	case 10:
		/*if(data == 0x01)
			FlagPrinter = eFlagPRTStand;
		else if(data == 0x02)
			FlagPrinter = eFlagPRTDeep;
		else if(data == 0x03)
			FlagPrinter = eFlagPRTStand;
		else
			FlagPrinter = eFlagPRTFast;*/
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
	default:
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
			// û��ռ��ʱ�����һλ�ַ������һ��0��Ϊ�������
			if(Command.Size < 33) // wifi_stored_sta_info.ssid[33]
			{
				wifi_setup_sta_info.ssid[Command.Size] = 0;
				Command.Size++;
			}

			Command.State++; // ������Ч
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
			// û��ռ��ʱ�����һλ�ַ������һ��0��Ϊ�������
			if(Command.Size < 64) // wifi_stored_sta_info.pwd[64]
			{
				wifi_setup_sta_info.pwd[Command.Size] = 0;
				Command.Size++;
			}

			if(WifiState)
				USBReturnWifi(2);
			else
				USBReturnWifi(1);
			// �δ����ٷ�����Ϣ����ͨ��WIFI״̬���н�����״̬���أ�������Ҳ�ܹ���ʾ�����Ϣ�������������Ƿ���IP
			//USBReturnWifi(1);
			//USBReturnWifi(2);
			//USBReturnWifi(3);
			//USBReturnWifi(4);
			//USBReturnWifi(5);

			ssid_stored_cnt = wifi_get_stored_sta_info(wifi_stored_sta_info);
    		for (int i = 0; i < ssid_stored_cnt; i++)
			{
	        	//printf("1:wifi_get_stored_sta_info[%d]= %s \r\n", i, wifi_stored_sta_info[i].ssid);
				wifi_del_stored_sta_info(wifi_stored_sta_info[i].ssid);// @ Alke 20230711 ɾ���洢����
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
			Command.State=21;
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
			// ��ҪWIFI������Ϣ
			WifiReturnMsg();
			result = 0;
		}
		else
			result = 1;
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
	default:
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
			Command.State = 10; // ����ͼ������
			result = 1;
		}
		else if(data == 0x01)
		{
			Command.State = 20; // ���ͼ������
			result = 1;
		}
		else
			result = 2;
		break;

	case 10: // ����ͼ�����ݣ�Size��8λ
		Command.Size = data;
		Command.State++;
		result = 1;
		break;
	case 11: // ����ͼ�����ݣ�Size��8λ
		c16c1 = data;
		c16c1 <<= 8;
		Command.Size += c16c1;
		cBufferPrintDataWrite = 0;
		Command.State++;
		result = 1;
		break;
	case 12: // ����ͼ�����ݿ�ʼ������������
		// ǰ��ֽģʽ��ǰһ��������Ҫд��ż���д�ӡ����������һ��������Ҫд�������д�ӡ���������������������б�����ֽ���ϵ�������Ҫǰ���������ֽ����ԭ��ÿ��������Ҫ��λ�ߵͻ���
		if(PaperState >= sPaperBackNo)//if(PaperState == sPaperBackNormal)
		{
			BufferPrint[cBufferPrintLineWrite].BufferA[cBufferPrintDataWrite] = data;
			cBufferPrintDataWrite++;
			BufferPrint[cBufferPrintLineWrite].BufferA[cBufferPrintDataWrite] = 0x00;
			cBufferPrintDataWrite++;

			if(cBufferPrintDataWrite >= Command.Size)
			{
				//ʣ��д0
				for(c16c1 = cBufferPrintDataWrite; c16c1 < BufferPRTSize; c16c1++)
					BufferPrint[cBufferPrintLineWrite].BufferA[c16c1] = 0x00;

				Command.State++;
			}
		}
		else //if(PaperState == sPaperFrontNormal)
		{
			if(cBufferPrintDataWrite < BufferPRTSize)
			{
				BufferPrint[cBufferPrintLineWrite].BufferB[BufferPRTSize-1-cBufferPrintDataWrite] = ByteBitChang(data);
				cBufferPrintDataWrite++;
			}
			if(cBufferPrintDataWrite < BufferPRTSize)
			{
				BufferPrint[cBufferPrintLineWrite].BufferB[BufferPRTSize-1-cBufferPrintDataWrite] = 0x00;
				cBufferPrintDataWrite++;
			}
			// ȷ������һ���㹻�����������¸�״̬
			if(cBufferPrintDataWrite >= Command.Size)
			{
				//ʣ��д0
				if(cBufferPrintDataWrite < BufferPRTSize)
				{
					for(c16c1 = 0; c16c1 < (BufferPRTSize-Command.Size); c16c1++)
						BufferPrint[cBufferPrintLineWrite].BufferB[c16c1] = 0x00;
				}
				Command.State++;
			}
		}
		result = 1;
		break;
	case 13: // ����ͼ�����ݿ�ʼ��ż��������
		if(PaperState >= sPaperBackNo)//if(PaperState == sPaperBackNormal)
		{
			BufferPrint[cBufferPrintLineWrite].BufferB[cBufferPrintDataWrite-Command.Size/2] = data; // Alke @20230704 -Command.Size/2
			cBufferPrintDataWrite++;
			BufferPrint[cBufferPrintLineWrite].BufferB[cBufferPrintDataWrite-Command.Size/2] = 0x00; // Alke @20230704 -Command.Size/2
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
		else//if(PaperState == sPaperFrontNormal)
		{
			if((cBufferPrintDataWrite-Command.Size/2) < BufferPRTSize)
			{
				BufferPrint[cBufferPrintLineWrite].BufferA[BufferPRTSize-1-(cBufferPrintDataWrite-Command.Size/2)] = ByteBitChang(data);
				cBufferPrintDataWrite++;
			}
			if((cBufferPrintDataWrite-Command.Size/2) < BufferPRTSize)
			{
				BufferPrint[cBufferPrintLineWrite].BufferA[BufferPRTSize-1-(cBufferPrintDataWrite-Command.Size/2)] = 0x00;
				cBufferPrintDataWrite++;
			}
			// ȷ������һ���㹻�����������¸�״̬
			if(cBufferPrintDataWrite >= (Command.Size*2))
			{
				//ʣ��д0
				if((cBufferPrintDataWrite-Command.Size) < BufferPRTSize)
				{
					for(c16c1 = 0; c16c1 < (BufferPRTSize-Command.Size); c16c1++)
						BufferPrint[cBufferPrintLineWrite].BufferA[c16c1] = 0;
				}
				Command.State = 30;
//printf("23-2");
				goto PrintDataDeal;
			}
//printf("23-1");
		}
		break;

	case 20: // ���ͼ�����ݣ�Size��8λ
//printf("20\n");
		Command.Size = data;
		Command.State++;
		result = 1;
		break;
	case 21: // ���ͼ�����ݣ�Size��8λ
//printf("21%d\n", Command.Size);
		c16c1 = data;
		c16c1 <<= 8;
		Command.Size += c16c1;
		cBufferPrintDataWrite = 0;
		Command.State++;
		result = 1;
		break;
	case 22: // ���ͼ�����ݿ�ʼ������������
//printf("22-%d\n", cBufferPrintDataWrite);
		// @20231021 �˴���Ҫ����ǰ���ֽ�����⣬ǰ��ֽĳҳ��ӡ��������ʱ���ڴ������ݣ���ҪĬ�ϰ���ǰ��ֽ����
		if(PaperState >= sPaperBackNo)//if(PaperState == sPaperBackNormal)//else
		{
			BufferPrint[cBufferPrintLineWrite].BufferA[cBufferPrintDataWrite] = data;
			cBufferPrintDataWrite++;
			// ȷ������һ���㹻�����������¸�״̬
			if(cBufferPrintDataWrite >= (Command.Size/2))
			{
				//ʣ��д0
				for(c16c1 = cBufferPrintDataWrite; c16c1 < BufferPRTSize; c16c1++)
					BufferPrint[cBufferPrintLineWrite].BufferA[c16c1] = 0;
				Command.State++;
			}
		}
		else //if(PaperState == sPaperFrontNormal)
		{
			if(cBufferPrintDataWrite < BufferPRTSize)
			{
				BufferPrint[cBufferPrintLineWrite].BufferB[BufferPRTSize-1-cBufferPrintDataWrite] = ByteBitChang(data);
				cBufferPrintDataWrite++;
			}
			// ȷ������һ���㹻�����������¸�״̬
			if(cBufferPrintDataWrite >= (Command.Size/2))
			{
				//ʣ��д0
				if(cBufferPrintDataWrite < BufferPRTSize)
				{
					for(c16c1 = 0; c16c1 < (BufferPRTSize-Command.Size/2); c16c1++)
						BufferPrint[cBufferPrintLineWrite].BufferB[c16c1] = 0;
				}
				Command.State++;
			}
		}
		result = 1;
		break;
	case 23: // ���ͼ�����ݿ�ʼ��ż��������
//printf("23+%d\n", cBufferPrintDataWrite);
		if(PaperState >= sPaperBackNo)//if(PaperState == sPaperBackNormal)
		{
			BufferPrint[cBufferPrintLineWrite].BufferB[cBufferPrintDataWrite-Command.Size/2] = data; // Alke @20230704 -Command.Size/2
			cBufferPrintDataWrite++;
			// ȷ������һ���㹻�����������¸�״̬
			if(cBufferPrintDataWrite >= (Command.Size))
			{
				//ʣ��д0
				for(c16c1 = cBufferPrintDataWrite; c16c1 < BufferPRTSize*2; c16c1++)
					BufferPrint[cBufferPrintLineWrite].BufferB[c16c1-Command.Size/2] = data;
				Command.State = 30;
				goto PrintDataDeal;
//printf("23-f");
			}
//printf("23-e");
		}
		else //if(PaperState == sPaperFrontNormal)
		{
			if((cBufferPrintDataWrite-Command.Size/2) < BufferPRTSize)
			{
				BufferPrint[cBufferPrintLineWrite].BufferA[BufferPRTSize-1-(cBufferPrintDataWrite-Command.Size/2)] = ByteBitChang(data);
				cBufferPrintDataWrite++;
			}
			// ȷ������һ���㹻�����������¸�״̬
			if(cBufferPrintDataWrite >= Command.Size)
			{
				//ʣ��д0
				if((cBufferPrintDataWrite-Command.Size/2) < BufferPRTSize)
				{
					for(c16c1 = 0; c16c1 < (BufferPRTSize-Command.Size/2); c16c1++)
						BufferPrint[cBufferPrintLineWrite].BufferA[c16c1] = 0;
				}
				Command.State = 30;
//printf("23-2");
				goto PrintDataDeal;
			}
//printf("23-1");
		}
		result = 1;
		break;

	case 30: // ͼ�����������㴦�� @20231229 ȷ����4F16��ʵ��180DPI���룬����Ҫ�������ӡ 
		PrintDataDeal:
//printf("PAPState:0x%x-PrintingState:%d-PaperState:%d-FlagMotorIC:0x%x-PAPMoveStep:%d-cBufferPrintLineWrite:%d-cBufferPrintLineRead:%d\r\n", PAPState, PrintingState, PaperState, FlagMotorIC, PAPMoveStep, cBufferPrintLineWrite, cBufferPrintLineRead);
//printf("30-0\n");

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
//printf("30-2\n");

	#if 0
		//���������ݿ�ʼ���� VirtualHalfDotsDataDeal:
		if(BufferPrint[cBufferPrintLineWrite].cLeft < BufferPrint[cBufferPrintLineWrite].cRight)
		{
			cBufferPrintDataWrite = BufferPrint[cBufferPrintLineWrite].cLeft;
			//while(cBufferPrintDataWrite < (BufferPrint[cBufferPrintLineWrite].cRight-1)) // (BufferPRTSize-2)Command.Size
			while(cBufferPrintDataWrite < BufferPrint[cBufferPrintLineWrite].cRight) // (BufferPRTSize-2)Command.Size
			{
//@@@@@@@@@@				BufferPrint[cBufferPrintLineWrite].BufferA[cBufferPrintDataWrite+1] &= ~BufferPrint[cBufferPrintLineWrite].BufferA[cBufferPrintDataWrite];
//@@@@@@@@@@				BufferPrint[cBufferPrintLineWrite].BufferB[cBufferPrintDataWrite+1] &= ~BufferPrint[cBufferPrintLineWrite].BufferB[cBufferPrintDataWrite];
				cBufferPrintDataWrite++;
			}
		}
		else
		{
			BufferPrint[cBufferPrintLineWrite].cRight = BufferPrint[cBufferPrintLineWrite].cLeft;
		}
	#else
		if(BufferPrint[cBufferPrintLineWrite].cLeft > BufferPrint[cBufferPrintLineWrite].cRight)
			BufferPrint[cBufferPrintLineWrite].cLeft = BufferPrint[cBufferPrintLineWrite].cRight;
	#endif
//printf("30-3\n");

		// ��������λ�����㵽��������
		cBufferPrintDataRead = BufferPrint[cBufferPrintLineWrite].cLeft*BufferOneDataTimePosition;
		cBufferPrintDataWrite = cBufferPrintDataRead%CRGOneStepTimePosition;
		BufferPrint[cBufferPrintLineWrite].cLeft = cBufferPrintDataRead - cBufferPrintDataWrite;			// adjust to th 16*n
		// ��������λ�����㵽��������
		cBufferPrintDataRead = BufferPrint[cBufferPrintLineWrite].cRight*BufferOneDataTimePosition;
		cBufferPrintDataWrite = cBufferPrintDataRead%CRGOneStepTimePosition;
		BufferPrint[cBufferPrintLineWrite].cRight = cBufferPrintDataRead +CRGOneStepTimePosition- cBufferPrintDataWrite;	 // adjust to th 16*n
		if(BufferPrint[cBufferPrintLineWrite].cRight > TimePositionAll)
			BufferPrint[cBufferPrintLineWrite].cRight = TimePositionAll;

		// ��������λ�����㵽������ʼ�˶�λ����
		if(BufferPrint[cBufferPrintLineWrite].cLeft > (CRGVolatileStep*CRGOneStepTimePosition+5*CRGOneStepTimePosition))
			BufferPrint[cBufferPrintLineWrite].cLeft -= (CRGVolatileStep*CRGOneStepTimePosition+5*CRGOneStepTimePosition);
		else
			BufferPrint[cBufferPrintLineWrite].cLeft = 0;
		// ��������λ�����㵽������ʼ�˶�λ����
		if((BufferPrint[cBufferPrintLineWrite].cRight+CRGVolatileStep*CRGOneStepTimePosition*2+15*CRGOneStepTimePosition)<TimePositionAll)
			BufferPrint[cBufferPrintLineWrite].cRight += CRGVolatileStep*CRGOneStepTimePosition*2+15*CRGOneStepTimePosition;
		else
			BufferPrint[cBufferPrintLineWrite].cRight = TimePositionAll;

		BufferPrint[cBufferPrintLineWrite].Step = 0;
		if(cBufferPrintLineWrite >= (BufferPrintLineNum-1))
			cBufferPrintLineWrite = 0;
		else
			cBufferPrintLineWrite++;

//printf("df--\n");
		cBufferPrintDataWrite = 0;
		result = 0;
		break;
	}

	return result;
}
// ��USB���ջ�����һ����д���Լ��ӡ���ݣ����������ʼλ�ÿ�ʼд��
void Cmd1b3aSeftPrint(void)
{
	u16 c16c1;
	u8 c8c1;
	u8 c8d1;
	u8 flag;
	const u8 *mac_addr;

	USB_RevC->Pwrite = 0;
	USB_RevC->Pread = 0;

	USB_RevC->PBufferRev[USB_RevC->Pwrite++] = 0x1b; //(USB_RevC->Pwrite++) < USBMAXBufferReceiveSize
	USB_RevC->PBufferRev[USB_RevC->Pwrite++] = 0xa0;
	USB_RevC->PBufferRev[USB_RevC->Pwrite++] = 0x00;
	
	USB_RevC->PBufferRev[USB_RevC->Pwrite++] = 0x1b;
	USB_RevC->PBufferRev[USB_RevC->Pwrite++] = 0x4a;
	USB_RevC->PBufferRev[USB_RevC->Pwrite++] = 0x50;

	c16c1 = 0;
	while(TABLEHardWare[c16c1])
	{
		USB_RevC->PBufferRev[USB_RevC->Pwrite++] = TABLEHardWare[c16c1++];
	}
	USB_RevC->PBufferRev[USB_RevC->Pwrite++] = 0x0a;
	c16c1 = 0;
	while(TABLESoftWare[c16c1])
	{
		USB_RevC->PBufferRev[USB_RevC->Pwrite++] = TABLESoftWare[c16c1++];
	}
	USB_RevC->PBufferRev[USB_RevC->Pwrite++] = 0x0a;
	c16c1 = 0;
	while(TABLEInterface[c16c1])
	{
		USB_RevC->PBufferRev[USB_RevC->Pwrite++] = TABLEInterface[c16c1++];
	}
	USB_RevC->PBufferRev[USB_RevC->Pwrite++] = 0x0a;
	USB_RevC->PBufferRev[USB_RevC->Pwrite++] = 0x0a;

	c16c1 = 0;
	while(TABLECld[c16c1])
		USB_RevC->PBufferRev[USB_RevC->Pwrite++] = TABLECld[c16c1++];
	USB_RevC->PBufferRev[USB_RevC->Pwrite++] = '0'+HeadDelayCount/10;
	USB_RevC->PBufferRev[USB_RevC->Pwrite++] = '0'+HeadDelayCount%10;
	USB_RevC->PBufferRev[USB_RevC->Pwrite++] = 0x0a;
	
	c16c1 = 0;
	while(TABLEPid[c16c1])
		USB_RevC->PBufferRev[USB_RevC->Pwrite++] = TABLEPid[c16c1++];
	c8d1 = (sDeviceDescriptor[11]&0xf0)>>4;
	USB_RevC->PBufferRev[USB_RevC->Pwrite++] = ((c8d1<10)?(c8d1+'0'):(c8d1-10+'a'));
	c8d1 = (sDeviceDescriptor[11]&0x0f);
	USB_RevC->PBufferRev[USB_RevC->Pwrite++] = ((c8d1<10)?(c8d1+'0'):(c8d1-10+'a'));
	c8d1 = (sDeviceDescriptor[10]&0xf0)>>4;
	USB_RevC->PBufferRev[USB_RevC->Pwrite++] = ((c8d1<10)?(c8d1+'0'):(c8d1-10+'a'));
	c8d1 = (sDeviceDescriptor[10]&0x0f);
	USB_RevC->PBufferRev[USB_RevC->Pwrite++] = ((c8d1<10)?(c8d1+'0'):(c8d1-10+'a'));
	USB_RevC->PBufferRev[USB_RevC->Pwrite++] = 0x0a;
		
	c16c1 = 0;
	while(TABLEMac[c16c1])
		USB_RevC->PBufferRev[USB_RevC->Pwrite++] = TABLEMac[c16c1++];
	// @20231224 Alke use mac_address as ID number 
	mac_addr = bt_get_mac_addr();
	//printf("MAC:%02X%02X%02X%02X%02X%02X", mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);	
	USB_RevC->PBufferRev[USB_RevC->Pwrite] = mac_addr[0]>>4;
	if(USB_RevC->PBufferRev[USB_RevC->Pwrite]<10)
		USB_RevC->PBufferRev[USB_RevC->Pwrite] = USB_RevC->PBufferRev[USB_RevC->Pwrite]+'0';
	else
		USB_RevC->PBufferRev[USB_RevC->Pwrite] =USB_RevC->PBufferRev[USB_RevC->Pwrite]-10+'a';
	USB_RevC->Pwrite++;
	USB_RevC->PBufferRev[USB_RevC->Pwrite] = mac_addr[0]&0x0f;
	if(USB_RevC->PBufferRev[USB_RevC->Pwrite]<10)
		USB_RevC->PBufferRev[USB_RevC->Pwrite] = USB_RevC->PBufferRev[USB_RevC->Pwrite]+'0';
	else
		USB_RevC->PBufferRev[USB_RevC->Pwrite] =USB_RevC->PBufferRev[USB_RevC->Pwrite]-10+'a';
	USB_RevC->Pwrite++;
	USB_RevC->PBufferRev[USB_RevC->Pwrite++] = 0x3a;
	
	USB_RevC->PBufferRev[USB_RevC->Pwrite] = mac_addr[1]>>4;
	if(USB_RevC->PBufferRev[USB_RevC->Pwrite]<10)
		USB_RevC->PBufferRev[USB_RevC->Pwrite] = USB_RevC->PBufferRev[USB_RevC->Pwrite]+'0';
	else
		USB_RevC->PBufferRev[USB_RevC->Pwrite] =USB_RevC->PBufferRev[USB_RevC->Pwrite]-10+'a';
	USB_RevC->Pwrite++;
	USB_RevC->PBufferRev[USB_RevC->Pwrite] = mac_addr[1]&0x0f;
	if(USB_RevC->PBufferRev[USB_RevC->Pwrite]<10)
		USB_RevC->PBufferRev[USB_RevC->Pwrite] = USB_RevC->PBufferRev[USB_RevC->Pwrite]+'0';
	else
		USB_RevC->PBufferRev[USB_RevC->Pwrite] =USB_RevC->PBufferRev[USB_RevC->Pwrite]-10+'a';
	USB_RevC->Pwrite++;
	USB_RevC->PBufferRev[USB_RevC->Pwrite++] = 0x3a;
	
	USB_RevC->PBufferRev[USB_RevC->Pwrite] = mac_addr[2]>>4;
	if(USB_RevC->PBufferRev[USB_RevC->Pwrite]<10)
		USB_RevC->PBufferRev[USB_RevC->Pwrite] = USB_RevC->PBufferRev[USB_RevC->Pwrite]+'0';
	else
		USB_RevC->PBufferRev[USB_RevC->Pwrite] =USB_RevC->PBufferRev[USB_RevC->Pwrite]-10+'a';
	USB_RevC->Pwrite++;
	USB_RevC->PBufferRev[USB_RevC->Pwrite] = mac_addr[2]&0x0f;
	if(USB_RevC->PBufferRev[USB_RevC->Pwrite]<10)
		USB_RevC->PBufferRev[USB_RevC->Pwrite] = USB_RevC->PBufferRev[USB_RevC->Pwrite]+'0';
	else
		USB_RevC->PBufferRev[USB_RevC->Pwrite] =USB_RevC->PBufferRev[USB_RevC->Pwrite]-10+'a';
	USB_RevC->Pwrite++;
	USB_RevC->PBufferRev[USB_RevC->Pwrite++] = 0x3a;
	
	USB_RevC->PBufferRev[USB_RevC->Pwrite] = mac_addr[3]>>4;
	if(USB_RevC->PBufferRev[USB_RevC->Pwrite]<10)
		USB_RevC->PBufferRev[USB_RevC->Pwrite] = USB_RevC->PBufferRev[USB_RevC->Pwrite]+'0';
	else
		USB_RevC->PBufferRev[USB_RevC->Pwrite] =USB_RevC->PBufferRev[USB_RevC->Pwrite]-10+'a';
	USB_RevC->Pwrite++;
	USB_RevC->PBufferRev[USB_RevC->Pwrite] = mac_addr[3]&0x0f;
	if(USB_RevC->PBufferRev[USB_RevC->Pwrite]<10)
		USB_RevC->PBufferRev[USB_RevC->Pwrite] = USB_RevC->PBufferRev[USB_RevC->Pwrite]+'0';
	else
		USB_RevC->PBufferRev[USB_RevC->Pwrite] =USB_RevC->PBufferRev[USB_RevC->Pwrite]-10+'a';
	USB_RevC->Pwrite++;
	USB_RevC->PBufferRev[USB_RevC->Pwrite++] = 0x3a;
	
	USB_RevC->PBufferRev[USB_RevC->Pwrite] = mac_addr[4]>>4;
	if(USB_RevC->PBufferRev[USB_RevC->Pwrite]<10)
		USB_RevC->PBufferRev[USB_RevC->Pwrite] = USB_RevC->PBufferRev[USB_RevC->Pwrite]+'0';
	else
		USB_RevC->PBufferRev[USB_RevC->Pwrite] =USB_RevC->PBufferRev[USB_RevC->Pwrite]-10+'a';
	USB_RevC->Pwrite++;
	USB_RevC->PBufferRev[USB_RevC->Pwrite] = mac_addr[4]&0x0f;
	if(USB_RevC->PBufferRev[USB_RevC->Pwrite]<10)
		USB_RevC->PBufferRev[USB_RevC->Pwrite] = USB_RevC->PBufferRev[USB_RevC->Pwrite]+'0';
	else
		USB_RevC->PBufferRev[USB_RevC->Pwrite] =USB_RevC->PBufferRev[USB_RevC->Pwrite]-10+'a';
	USB_RevC->Pwrite++;
	USB_RevC->PBufferRev[USB_RevC->Pwrite++] = 0x3a;
	
	USB_RevC->PBufferRev[USB_RevC->Pwrite] = mac_addr[5]>>4;
	if(USB_RevC->PBufferRev[USB_RevC->Pwrite]<10)
		USB_RevC->PBufferRev[USB_RevC->Pwrite] = USB_RevC->PBufferRev[USB_RevC->Pwrite]+'0';
	else
		USB_RevC->PBufferRev[USB_RevC->Pwrite] =USB_RevC->PBufferRev[USB_RevC->Pwrite]-10+'a';
	USB_RevC->Pwrite++;
	USB_RevC->PBufferRev[USB_RevC->Pwrite] = mac_addr[5]&0x0f;
	if(USB_RevC->PBufferRev[USB_RevC->Pwrite]<10)
		USB_RevC->PBufferRev[USB_RevC->Pwrite] = USB_RevC->PBufferRev[USB_RevC->Pwrite]+'0';
	else
		USB_RevC->PBufferRev[USB_RevC->Pwrite] =USB_RevC->PBufferRev[USB_RevC->Pwrite]-10+'a';
	USB_RevC->Pwrite++;
	USB_RevC->PBufferRev[USB_RevC->Pwrite++] = 0x0a;

	//wifi_store_mode_info(STA_MODE, wifi_setup_sta_info.ssid, wifi_setup_sta_info.pwd);
	struct wifi_mode_info info;
	info.mode = STA_MODE;
	wifi_get_mode_cur_info(&info);
//	printf(info.ssid);
//	printf(info.pwd);
	c16c1 = 0;
	while(TABLESsid[c16c1])
		USB_RevC->PBufferRev[USB_RevC->Pwrite++] = TABLESsid[c16c1++];
	//strncpy(USB_RevC->PBufferRev[USB_RevC->Pwrite], (char *)info.ssid, sizeof(info.ssid));
	//USB_RevC->Pwrite += sizeof(info.ssid);
	c16c1 = 0;
	while(c16c1 < 33)
	{
		c8c1 = info.ssid[c16c1++];
		if(c8c1)
			USB_RevC->PBufferRev[USB_RevC->Pwrite++] = c8c1;
		else
			break;
	}
	/*c16c1 = 0;
	while(c16c1 < 33)
	{
		c8c1 = wifi_setup_sta_info.ssid[c16c1++];
		if(c8c1)
			USB_RevC->PBufferRev[USB_RevC->Pwrite++] = c8c1;
		else
			break;
	}*/
	USB_RevC->PBufferRev[USB_RevC->Pwrite++] = 0x0a;

	c16c1 = 0;
	while(TABLEPwd[c16c1])
		USB_RevC->PBufferRev[USB_RevC->Pwrite++] = TABLEPwd[c16c1++];
	//strncpy(USB_RevC->PBufferRev[USB_RevC->Pwrite], (char *)info.pwd, sizeof(info.pwd));
	//USB_RevC->Pwrite += sizeof(info.pwd);
	c16c1 = 0;
	while(c16c1 < 33)
	{
		c8c1 = info.pwd[c16c1++];
		if(c8c1)
			USB_RevC->PBufferRev[USB_RevC->Pwrite++] = c8c1;
		else
			break;
	}
	/*c16c1 = 0;
	while(c16c1 < 33)
	{
		c8c1 = wifi_setup_sta_info.pwd[c16c1++];
		if(c8c1)
			USB_RevC->PBufferRev[USB_RevC->Pwrite++] = c8c1;
		else
			break;
	}*/
	USB_RevC->PBufferRev[USB_RevC->Pwrite++] = 0x0a;
	
	c16c1 = 0;
	while(TABLEIp[c16c1])
		USB_RevC->PBufferRev[USB_RevC->Pwrite++] = TABLEIp[c16c1++];
	lwip_get_netif_info(1, &netif_infoA);
	flag = 0; // ��0���ݣ�����Ϊ0Ҳ����ʾ
	c8d1 = netif_infoA.ip&0x000000ff;
	if(c8d1 > 99)
		{USB_RevC->PBufferRev[USB_RevC->Pwrite++] = 0x30+c8d1/100; flag = 1;}
	c8d1 %= 100;
	if(c8d1 > 9)
		USB_RevC->PBufferRev[USB_RevC->Pwrite++] = 0x30+c8d1/10;
	else if(flag) // ��λ��0����ʱ0Ҳ����ʾ
		USB_RevC->PBufferRev[USB_RevC->Pwrite++] = 0x30;
	c8d1 %= 10;
	USB_RevC->PBufferRev[USB_RevC->Pwrite++] = 0x30+c8d1;
	USB_RevC->PBufferRev[USB_RevC->Pwrite++] = '.';

	flag = 0;
	c8d1 = (netif_infoA.ip>>8)&0x000000ff;
	if(c8d1 > 99)
		{USB_RevC->PBufferRev[USB_RevC->Pwrite++] = 0x30+c8d1/100; flag = 1;}
	c8d1 %= 100;
	if(c8d1 > 9)
		USB_RevC->PBufferRev[USB_RevC->Pwrite++] = 0x30+c8d1/10;
	else if(flag) // ��λ��0����ʱ0Ҳ����ʾ
		USB_RevC->PBufferRev[USB_RevC->Pwrite++] = 0x30;
	c8d1 %= 10;
	USB_RevC->PBufferRev[USB_RevC->Pwrite++] = 0x30+c8d1;
	USB_RevC->PBufferRev[USB_RevC->Pwrite++] = '.';

	flag = 0;
	c8d1 = (netif_infoA.ip>>16)&0x000000ff;
	if(c8d1 > 99)
		{USB_RevC->PBufferRev[USB_RevC->Pwrite++] = 0x30+c8d1/100; flag = 1;}
	c8d1 %= 100;
	if(c8d1 > 9)
		USB_RevC->PBufferRev[USB_RevC->Pwrite++] = 0x30+c8d1/10;
	else if(flag) // ��λ��0����ʱ0Ҳ����ʾ
		USB_RevC->PBufferRev[USB_RevC->Pwrite++] = 0x30;
	c8d1 %= 10;
	USB_RevC->PBufferRev[USB_RevC->Pwrite++] = 0x30+c8d1;
	USB_RevC->PBufferRev[USB_RevC->Pwrite++] = '.';

	flag = 0;
	c8d1 = (netif_infoA.ip>>24)&0x000000ff;
	if(c8d1 > 99)
		{USB_RevC->PBufferRev[USB_RevC->Pwrite++] = 0x30+c8d1/100; flag = 1;}
	c8d1 %= 100;
	if(c8d1 > 9)
		USB_RevC->PBufferRev[USB_RevC->Pwrite++] = 0x30+c8d1/10;
	else if(flag) // ��λ��0����ʱ0Ҳ����ʾ
		USB_RevC->PBufferRev[USB_RevC->Pwrite++] = 0x30;
	c8d1 %= 10;
	USB_RevC->PBufferRev[USB_RevC->Pwrite++] = 0x30+c8d1;
	USB_RevC->PBufferRev[USB_RevC->Pwrite++] = 0x0a;
	USB_RevC->PBufferRev[USB_RevC->Pwrite++] = 0x0a;

	/*for(c8c1 = 0; c8c1 < 3; c8c1++)
	{
		c16c1 = 0;
		while(TABLESeftPrintSmall[c16c1])
			USB_RevC->PBufferRev[USB_RevC->Pwrite++] = TABLESeftPrintSmall[c16c1++];
		USB_RevC->PBufferRev[USB_RevC->Pwrite++] = 0x0a;
	}
	for(c8c1 = 0; c8c1 < 3; c8c1++)
	{
		c16c1 = 0;
		while(TABLESeftPrintBig[c16c1])
			USB_RevC->PBufferRev[USB_RevC->Pwrite++] = TABLESeftPrintBig[c16c1++];
		USB_RevC->PBufferRev[USB_RevC->Pwrite++] = 0x0a;
	}*/
	for(c16c1 = 0; c16c1 < 5; c16c1++)
	{
		for(c8c1 = 0; c8c1 < 15; c8c1++)
			USB_RevC->PBufferRev[USB_RevC->Pwrite++] = 0x20;
		for(c8c1 = 0x21; c8c1 < 0x7f; c8c1++)
			USB_RevC->PBufferRev[USB_RevC->Pwrite++] = c8c1;
		USB_RevC->PBufferRev[USB_RevC->Pwrite++] = 0x0a;
	}
	USB_RevC->PBufferRev[USB_RevC->Pwrite++] = 0x0a;

	c16c1 = 0;
	while(TABLEEnd[c16c1])
	{
		USB_RevC->PBufferRev[USB_RevC->Pwrite++] = TABLEEnd[c16c1++];
	}
	USB_RevC->PBufferRev[USB_RevC->Pwrite++] = 0x0a;
	USB_RevC->PBufferRev[USB_RevC->Pwrite++] = 0x0a;
	
	USB_RevC->PBufferRev[USB_RevC->Pwrite++] = 0x1d;
	USB_RevC->PBufferRev[USB_RevC->Pwrite++] = 0x56;
	USB_RevC->PBufferRev[USB_RevC->Pwrite++] = 0x53;

//printf_buf(USB_RevC->PBufferRev, USB_RevC->Pwrite);
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
//printf("PAPState:0x%x-PrintingState:%d-PaperState:%d-FlagMotorIC:0x%x-PAPMoveStep:%d-cBufferPrintLineWrite:%d-cBufferPrintLineRead:%d\r\n", PAPState, PrintingState, PaperState, FlagMotorIC, PAPMoveStep, cBufferPrintLineWrite, cBufferPrintLineRead);

	for(cBufferPrintDataWrite = 0; cBufferPrintDataWrite < BufferPRTSize; cBufferPrintDataWrite++)
	{
		BufferPrint[cBufferPrintLineWrite].BufferA[cBufferPrintDataWrite] = 0x00;
		BufferPrint[cBufferPrintLineWrite].BufferB[cBufferPrintDataWrite] = 0x00;
	}
	cBufferPrintDataWrite = 0;
	cBufferPrintDataRead = 0;
	BufferPrint[cBufferPrintLineWrite].cLeft=0;
	BufferPrint[cBufferPrintLineWrite].cRight=0;

	//BufferPrint[cBufferPrintLineWrite].Step += data;
	BufferPrint[cBufferPrintLineWrite].Step = data;//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@data;
	// ͨ��Ĭ��Ϊ16����û��0D�����л���һ��ʱ�����β������ӣ�ʹ����ֽ����:   �˴������ϸ�������������������Ӳ������ж���������ʱ���Կ������Ӳ���
	// û��ֽʱ��ֽ��״̬��ȷ�����޷�д�뷽��           	��ʱ��ǰ��ֽ����Ϊ��������ʱ����
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

	for(cBufferPrintDataWrite = 0; cBufferPrintDataWrite < BufferPRTSize; cBufferPrintDataWrite++)
	{
		BufferPrint[cBufferPrintLineWrite].BufferA[cBufferPrintDataWrite] = 0x00;
		BufferPrint[cBufferPrintLineWrite].BufferB[cBufferPrintDataWrite] = 0x00;
	}
	cBufferPrintDataWrite = 0;
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
	/*if(data)
		FlagPrinter = eFlagPRTFast;
	else
		FlagPrinter = eFlagPRTStand;*/
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
		break; // û��break��ֱ�Ӹ�λ�������˳�
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
		StepPageLenth += data;	// Alke @20230728 ��������ֽ�ų��Ȳ��ԣ���ʱ����
		StepPageLenth *= 2;
		result = 0;
	}
	return result;
}
u8 Cmd1b9cEnd(unsigned char data) // ������1D5653һ�����˴������κζ���
{
#if SendCmd
	printf("Cmd1b9cEnd\n");
//	USB_RevC->DataOut("USB: Cmd1b9c \r\n", 64);
//	tcp_send_data_sang("Wifi: Cmd1b9c \r\n",18);
//	BT_RevC->DataOut("Spp: Cmd1b9c \r\n",17);
#endif

		if(PaperState == sPaperBackNormal)
		{
			if(PaperCount < (StepBackToPrint+StepFrontToPrint+StepToCutPos))//-5
			{
				BufferPrint[cBufferPrintLineWrite].Step = StepBackToPrint+StepFrontToPrint+StepToCutPos-PaperCount;
				goto PaperCountMorethanStepBackToPrintAndStepFrontToPrintAndStepToCutPos;
			}
			else if(PaperCount<(StepPageLenth+StepBackToPrint+StepFrontToPrint+StepToCutPos-StepHeadWide))//-20
			{
				BufferPrint[cBufferPrintLineWrite].Step = (StepPageLenth+StepBackToPrint+StepFrontToPrint+StepToCutPos-StepHeadWide)-PaperCount;

			PaperCountMorethanStepBackToPrintAndStepFrontToPrintAndStepToCutPos:
			BufferPrint[cBufferPrintLineWrite].Step /= 2;
				BufferPrint[cBufferPrintLineWrite].Dir = sPaperDirBackToFront;
				//PaperState = sPaperFrontEnd; // @20231022 ���������ϵ�����1401�������ҵ����bug!
				//BufferPrint[cBufferPrintLineWrite].Step = StepPageLenth+StepFrontToPrint*2; // ����ֽStepFrontToPrint��ȷ��ֽ���˳�ȥ
				//BufferPrint[cBufferPrintLineWrite].Dir = sPaperDirFrontToBack;
//printf("-%d\r\n", BufferPrint[cBufferPrintLineWrite].Step);
				
				for(cBufferPrintDataWrite = (BufferPRTSize-1); cBufferPrintDataWrite; cBufferPrintDataWrite--)
				{
					BufferPrint[cBufferPrintLineWrite].BufferA[cBufferPrintDataWrite] = 0x00;
					BufferPrint[cBufferPrintLineWrite].BufferB[cBufferPrintDataWrite] = 0x00;
				}
				//cBufferPrintDataWrite = 0; // ��ȥʱ�Ѿ��պ�Ϊ0
				//cBufferPrintDataRead = 0;
				BufferPrint[cBufferPrintLineWrite].cLeft=0;
				BufferPrint[cBufferPrintLineWrite].cRight=0;

				if(cBufferPrintLineWrite >= (BufferPrintLineNum-1))
					cBufferPrintLineWrite = 0;
				else
					cBufferPrintLineWrite++;
			}
		}
	/*if(PaperState == sPaperFrontNormal)
	{
		BufferPrint[cBufferPrintLineWrite].Step = PaperCount+data+StepFrontToPrint; // ����ֽStepFrontToPrint��ȷ��ֽ���˳�ȥ
		BufferPrint[cBufferPrintLineWrite].Dir = sPaperDirBackToFront;

		if(cBufferPrintLineWrite >= (BufferPrintLineNum-1))
			cBufferPrintLineWrite = 0;
		else
			cBufferPrintLineWrite++;
	}*/
	// ��������Ҫ��Ժ��ֽ��������Ҫ���Ӳ���
	/*if(PaperState == sPaperBackNormal)
	{
		for(cBufferPrintDataWrite = (BufferPRTSize-1); cBufferPrintDataWrite; cBufferPrintDataWrite--)
		{
			BufferPrint[cBufferPrintLineWrite].BufferA[cBufferPrintDataWrite] = 0x00;
			BufferPrint[cBufferPrintLineWrite].BufferB[cBufferPrintDataWrite] = 0x00;
		}
		//cBufferPrintDataWrite = 0; // ��ȥʱ�Ѿ��պ�Ϊ0
		//cBufferPrintDataRead = 0;
		BufferPrint[cBufferPrintLineWrite].cLeft=0;
		BufferPrint[cBufferPrintLineWrite].cRight=0;

		BufferPrint[cBufferPrintLineWrite].Step = StepPageLenth+data+StepBackToPrint-PaperCount;
		BufferPrint[cBufferPrintLineWrite].Dir = sPaperDirFrontToBack;

		if(cBufferPrintLineWrite >= (BufferPrintLineNum-1))
			cBufferPrintLineWrite = 0;
		else
			cBufferPrintLineWrite++;
	}*/
	/*if(PaperState == sPaperFrontNormal)
	{
		for(cBufferPrintDataWrite = (BufferPRTSize-1); cBufferPrintDataWrite; cBufferPrintDataWrite--)
		{
			BufferPrint[cBufferPrintLineWrite].BufferA[cBufferPrintDataWrite] = 0x00;
			BufferPrint[cBufferPrintLineWrite].BufferB[cBufferPrintDataWrite] = 0x00;
		}
		//cBufferPrintDataWrite = 0; // ��ȥʱ�Ѿ��պ�Ϊ0
		//cBufferPrintDataRead = 0;
		BufferPrint[cBufferPrintLineWrite].cLeft=0;
		BufferPrint[cBufferPrintLineWrite].cRight=0;

		BufferPrint[cBufferPrintLineWrite].Step = StepPageLenth+StepFrontToPrint*2; // ����ֽStepFrontToPrint��ȷ��ֽ���˳�ȥ
		BufferPrint[cBufferPrintLineWrite].Dir = sPaperDirBackToFront;
		PaperState = sPaperFrontEnd;
		//BufferPrint[cBufferPrintLineWrite].Step = StepPageLenth+StepFrontToPrint*2; // ����ֽStepFrontToPrint��ȷ��ֽ���˳�ȥ
		//BufferPrint[cBufferPrintLineWrite].Dir = sPaperDirFrontToBack;

		if(cBufferPrintLineWrite >= (BufferPrintLineNum-1))
			cBufferPrintLineWrite = 0;
		else
			cBufferPrintLineWrite++;
	}*/

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
		BufferPrint[cBufferPrintLineWrite].Step = data+StepFrontToPrint; // ����ֽStepFrontToPrint��ȷ��ֽ���˳�ȥ
		BufferPrint[cBufferPrintLineWrite].Dir = sPaperDirBackToFront;

		if(cBufferPrintLineWrite >= (BufferPrintLineNum-1))
			cBufferPrintLineWrite = 0;
		else
			cBufferPrintLineWrite++;
	}*/
	// ��������Ҫ��Ժ��ֽ��������Ҫ���Ӳ���
	if(PaperState == sPaperBackNormal)
	{
		BufferPrint[cBufferPrintLineWrite].Step = (StepFrontToPrint+StepToCutPos)/2;
		BufferPrint[cBufferPrintLineWrite].Dir = sPaperDirFrontToBack;
		for(cBufferPrintDataWrite = (BufferPRTSize-1); cBufferPrintDataWrite; cBufferPrintDataWrite--)
		{
			BufferPrint[cBufferPrintLineWrite].BufferA[cBufferPrintDataWrite] = 0x00;
			BufferPrint[cBufferPrintLineWrite].BufferB[cBufferPrintDataWrite] = 0x00;
		}
		//cBufferPrintDataWrite = 0; // ��ȥʱ�Ѿ��պ�Ϊ0
		//cBufferPrintDataRead = 0;
		BufferPrint[cBufferPrintLineWrite].cLeft=0;
		BufferPrint[cBufferPrintLineWrite].cRight=0;

		if(cBufferPrintLineWrite >= (BufferPrintLineNum-1))
			cBufferPrintLineWrite = 0;
		else
			cBufferPrintLineWrite++;
			
		/*if((PaperCount<(StepBackToPrint-20)) || (PaperCount>(StepBackToPrint+20)))
		{
			if((SensorRecorderFrontOut)&&(SensorRecorderFrontIn))
			{
				PaperState=sPaperBackHead;
			}
			else
				PaperState = sPaperBackNo;
		}*/
		/*if(PaperCount > (StepBackToPrint+5)) // �����˴�ӡ��ʼλ�ã���Ҫ�ȶ���ֽ����ǰ��ֽ���Ա��������ּ�϶
		{
			for(cBufferPrintDataWrite = (BufferPRTSize-1); cBufferPrintDataWrite; cBufferPrintDataWrite--)
			{
				BufferPrint[cBufferPrintLineWrite].BufferA[cBufferPrintDataWrite] = 0x00;
				BufferPrint[cBufferPrintLineWrite].BufferB[cBufferPrintDataWrite] = 0x00;
			}
			//cBufferPrintDataWrite = 0; // ��ȥʱ�Ѿ��պ�Ϊ0
			//cBufferPrintDataRead = 0;
			BufferPrint[cBufferPrintLineWrite].cLeft=0;
			BufferPrint[cBufferPrintLineWrite].cRight=0;	
			
			BufferPrint[cBufferPrintLineWrite].Step = PaperCount+50+data-StepBackToPrint;
			BufferPrint[cBufferPrintLineWrite].Dir = sPaperDirFrontToBack;

			if(cBufferPrintLineWrite >= (BufferPrintLineNum-1))
				cBufferPrintLineWrite = 0;
			else
				cBufferPrintLineWrite++;
			
			for(cBufferPrintDataWrite = (BufferPRTSize-1); cBufferPrintDataWrite; cBufferPrintDataWrite--)
			{
				BufferPrint[cBufferPrintLineWrite].BufferA[cBufferPrintDataWrite] = 0x00;
				BufferPrint[cBufferPrintLineWrite].BufferB[cBufferPrintDataWrite] = 0x00;
			}
			//cBufferPrintDataWrite = 0; // ��ȥʱ�Ѿ��պ�Ϊ0
			//cBufferPrintDataRead = 0;
			BufferPrint[cBufferPrintLineWrite].cLeft=0;
			BufferPrint[cBufferPrintLineWrite].cRight=0;	
			
			BufferPrint[cBufferPrintLineWrite].Step = 50+data;
			BufferPrint[cBufferPrintLineWrite].Dir = sPaperDirBackToFront;

			if(cBufferPrintLineWrite >= (BufferPrintLineNum-1))
				cBufferPrintLineWrite = 0;
			else
				cBufferPrintLineWrite++;
		}
		else if(PaperCount < (StepBackToPrint-5))
		{
			for(cBufferPrintDataWrite = (BufferPRTSize-1); cBufferPrintDataWrite; cBufferPrintDataWrite--)
			{
				BufferPrint[cBufferPrintLineWrite].BufferA[cBufferPrintDataWrite] = 0x00;
				BufferPrint[cBufferPrintLineWrite].BufferB[cBufferPrintDataWrite] = 0x00;
			}
			//cBufferPrintDataWrite = 0; // ��ȥʱ�Ѿ��պ�Ϊ0
			//cBufferPrintDataRead = 0;
			BufferPrint[cBufferPrintLineWrite].cLeft=0;
			BufferPrint[cBufferPrintLineWrite].cRight=0;	
			
			BufferPrint[cBufferPrintLineWrite].Step = StepBackToPrint-PaperCount;
			BufferPrint[cBufferPrintLineWrite].Dir = sPaperDirBackToFront;

			if(cBufferPrintLineWrite >= (BufferPrintLineNum-1))
				cBufferPrintLineWrite = 0;
			else
				cBufferPrintLineWrite++;
		}*/
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
			Command.State = 10;		// ����������
			result = 1;
		}
		else
			result = 2;
		break;

	case 10:
		if(data)
		{
			FlagHeadDelayClr;//agHeadDelaySet;//FlagHeadDelayClr;
			Command.State++;
		}
		else
		{
			FlagHeadDelaySet;//agHeadDelayClr;//FlagHeadDelaySet;
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


// 1d��ͷ����
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
// ��ֽ����
u8 Cmd1d56CutPaper(unsigned char data)
{
#if SendCmd
	printf("Cmd1d56CutPaper\n");
//	USB_RevC->DataOut("USB: Cmd1d56CutPaper \r\n", 64);
//	tcp_send_data_sang("Wifi: Cmd1d56CutPaper \r\n",26);
//	BT_RevC->DataOut("Spp: Cmd1d56CutPaper \r\n",25);
#endif
//	����У�ʹ��1D5653 1B9C00ʱ���ᶪʧ������ȴ�ӡ���ٷ���������OK�ġ�

	if(data == 0x53)
	{
//printf("PAPState:0x%x-PrintingState:%d-PaperState:%d-FlagMotorIC:0x%x-PAPMoveStep:%d-cBufferPrintLineWrite:%d-cBufferPrintLineRead:%d\r\n", PAPState, PrintingState, PaperState, FlagMotorIC, PAPMoveStep, cBufferPrintLineWrite, cBufferPrintLineRead);
		// ȡ��ǰ��ֽ��˺ֽ����Կ���
		if(PaperState == sPaperFrontNormal)
		{
			// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
			/*FlagSystemPrintClr
			//cBufferPrintDataWrite = 0; // ��ȥʱ�Ѿ��պ�Ϊ0
			//cBufferPrintDataRead = 0;
			BufferPrint[cBufferPrintLineWrite].cLeft=TimePositionAll/2;
			BufferPrint[cBufferPrintLineWrite].cRight=TimePositionAll/2-1;

			BufferPrint[cBufferPrintLineWrite].Step = 0;
			BufferPrint[cBufferPrintLineWrite].Dir = sPaperDirBackToFront;
			//PaperState = sPaperFrontEnd;
			//BufferPrint[cBufferPrintLineWrite].Step = StepPageLenth+StepFrontToPrint*2; // ����ֽStepFrontToPrint��ȷ��ֽ���˳�ȥ
			//BufferPrint[cBufferPrintLineWrite].Dir = sPaperDirFrontToBack;

			if(cBufferPrintLineWrite >= (BufferPrintLineNum-1))
				cBufferPrintLineWrite = 0;
			else
				cBufferPrintLineWrite++;*/
			// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
			
			for(cBufferPrintDataWrite = (BufferPRTSize-1); cBufferPrintDataWrite; cBufferPrintDataWrite--)
			{
				BufferPrint[cBufferPrintLineWrite].BufferA[cBufferPrintDataWrite] = 0x00;
				BufferPrint[cBufferPrintLineWrite].BufferB[cBufferPrintDataWrite] = 0x00;
			}
			//cBufferPrintDataWrite = 0; // ��ȥʱ�Ѿ��պ�Ϊ0
			//cBufferPrintDataRead = 0;
			BufferPrint[cBufferPrintLineWrite].cLeft=0;
			BufferPrint[cBufferPrintLineWrite].cRight=0;

			BufferPrint[cBufferPrintLineWrite].Step = StepPageLenth+StepFrontToPrint*2; // ����ֽStepFrontToPrint��ȷ��ֽ���˳�ȥ
			BufferPrint[cBufferPrintLineWrite].Dir = sPaperDirBackToFront;
			//PaperState = sPaperFrontEnd; // @20231022 ���������ϵ�����1401�������ҵ����bug!
			//BufferPrint[cBufferPrintLineWrite].Step = StepPageLenth+StepFrontToPrint*2; // ����ֽStepFrontToPrint��ȷ��ֽ���˳�ȥ
			//BufferPrint[cBufferPrintLineWrite].Dir = sPaperDirFrontToBack;

			if(cBufferPrintLineWrite >= (BufferPrintLineNum-1))
				cBufferPrintLineWrite = 0;
			else
				cBufferPrintLineWrite++;
		}
		// 
		else if(PaperState == sPaperBackNormal)
		{
			if(PaperCount < (StepBackToPrint+StepFrontToPrint+StepToCutPos))//-5
			{
				BufferPrint[cBufferPrintLineWrite].Step = StepBackToPrint+StepFrontToPrint+StepToCutPos-PaperCount;
				goto PaperCountMorethanStepBackToPrintAndStepFrontToPrintAndStepToCutPos;
			}
			else if(PaperCount<(StepPageLenth+StepBackToPrint+StepFrontToPrint+StepToCutPos))//-20
			{
				BufferPrint[cBufferPrintLineWrite].Step = (StepPageLenth+StepBackToPrint+StepFrontToPrint+StepToCutPos)-PaperCount;

			PaperCountMorethanStepBackToPrintAndStepFrontToPrintAndStepToCutPos:
			BufferPrint[cBufferPrintLineWrite].Step /= 2;
				BufferPrint[cBufferPrintLineWrite].Dir = sPaperDirBackToFront;
				//PaperState = sPaperFrontEnd; // @20231022 ���������ϵ�����1401�������ҵ����bug!
				//BufferPrint[cBufferPrintLineWrite].Step = StepPageLenth+StepFrontToPrint*2; // ����ֽStepFrontToPrint��ȷ��ֽ���˳�ȥ
				//BufferPrint[cBufferPrintLineWrite].Dir = sPaperDirFrontToBack;
//printf("-%d\r\n", BufferPrint[cBufferPrintLineWrite].Step);
				
				for(cBufferPrintDataWrite = (BufferPRTSize-1); cBufferPrintDataWrite; cBufferPrintDataWrite--)
				{
					BufferPrint[cBufferPrintLineWrite].BufferA[cBufferPrintDataWrite] = 0x00;
					BufferPrint[cBufferPrintLineWrite].BufferB[cBufferPrintDataWrite] = 0x00;
				}
				//cBufferPrintDataWrite = 0; // ��ȥʱ�Ѿ��պ�Ϊ0
				//cBufferPrintDataRead = 0;
				BufferPrint[cBufferPrintLineWrite].cLeft=0;
				BufferPrint[cBufferPrintLineWrite].cRight=0;

				if(cBufferPrintLineWrite >= (BufferPrintLineNum-1))
					cBufferPrintLineWrite = 0;
				else
					cBufferPrintLineWrite++;
			}
		}
		return 0;
	}
	else
		return 2;
}

// �������ֽڽ����������
//void InstructionParser(volatile unsigned char data)//�������
// �˺���Ϊ���յ�һ���ֽ����ݽ��н���������ǰ����Ԥ���жϴ�ӡ�������пհ׿���д�룬�������USB/WIFI/BL����æֹͣ��������
void DealData(unsigned char data) //�������
{
	u8 result = 0;
	
#if DebugOnlyData
	printf("0x%x",data);
	return;
#endif
	// ������¼����������ֽڣ�����ع�����ʱʹ��    			�����Ҫ�ع����ݸ��ֻ࣬��Ҫ����Command.SizeSave[]��С��ÿ�ζ�ѹջд�뱣������
	Command.SizeSave[2] = Command.SizeSave[1];
	Command.SizeSave[1] = data;
	Command.SizeSave[0]++;

	DealCommandStart:
	switch(Command.Type)
	{
	// ���ǵ�1���ֽڵ��жϣ�Ȼ����ܽ�������ķ��ദ��
	default://eCmdInit
	case eCmdInit:
		if(Command.SizeSave[0]) // ==1: ֱ��ʹ�ã����Բ����ٴθ�ֵ	>=2: ��Ҫ��ֵ
			data = Command.SizeSave[Command.SizeSave[0]];

		if(Command.SizeSave[0])
		{
			if(data == 0x0a)		{Command.Type = eCmd0aPrint; goto DealCommandStart;}
			else if(data == 0x0d)	{Command.Type = eCmd0dPrint; goto DealCommandStart;}
			else if((data >= 0x20)&&(data <= 0x7e))	{Command.Type = eCmd20Letter; goto DealCommandStart;}
			else if(data == 0x1b)	Command.Type = eCmd1bClass; // ��Ҫ�¸��ֽڲ��ܼ�������
			else if(data == 0x1d)	Command.Type = eCmd1dClass; // ��Ҫ�¸��ֽڲ��ܼ�������
			else
			{
				Command.Type = eCmdInit;
				Command.SizeSave[0]--;
				Command.State = 0;
				Command.Size = 0;
			}
			if(Command.SizeSave[0])
				Command.SizeSave[0]--;	// ���ֽ��������ֱ�Ӵ��������ܵȺ�������
		}
		break;

	case eCmd0aPrint:	// ���ֽ���������ڴ˴�ѭ����ֱ�Ӵ�����ϣ����ܵ��¸������ٴ���; ����һ����������϶�û�д洢�Ĵ���������������
		Cmd0aPrint();
		
		Command.Type = eCmdInit;
		Command.State = 0;
		Command.Size = 0;
		if(Command.SizeSave[0])
			Command.SizeSave[0]--;
		if(Command.SizeSave[0])
			goto DealCommandStart;
		break;

	case eCmd0dPrint:	// ���ֽ���������ڴ˴�ѭ����ֱ�Ӵ�����ϣ����ܵ��¸������ٴ���
		Cmd0dPrint();
		
		Command.Type = eCmdInit;
		Command.State = 0;
		Command.Size = 0;
		if(Command.SizeSave[0])
			Command.SizeSave[0]--;
		if(Command.SizeSave[0])
			goto DealCommandStart;
		break;

	case eCmd20Letter:	// �����ַ�
		WriteLetter(data);
		
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
			else if(data == 0x01)	Command.Type = eCmd1b01Printer;
			else if(data == 0x0b)	Command.Type = eCmd1b0bWifi;
			else if(data == 0x2a)	Command.Type = eCmd1b2aDots;
			else if(data == 0x3a)	Command.Type = eCmd1b3aSeftPrint;
			else if(data == 0x4a)	Command.Type = eCmd1b4aFeed;
			else if(data == 0x4b)	Command.Type = eCmd1b4bBack;
			else if(data == 0x55)	Command.Type = eCmd1b55Direction;
			else if(data == 0x7a)	Command.Type = eCmd1b7aSpeed;
			else if(data == 0x80)	Command.Type = eCmd1b80Pid;
			else if(data == 0x89)	Command.Type = eCmd1b89Page;
			else if(data == 0x9c)	Command.Type = eCmd1b9cEnd;
			else if(data == 0xa0)	Command.Type = eCmd1ba0Start;
			else if(data == 0xf8)	Command.Type = eCmd1bf8Delay;
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

	case eCmd1b01Printer:
		result = Cmd1b01Printer(data);
		if(result==0) // �������������������Ž��к������
		{
			Command.Type = eCmdInit;
			Command.State = 0;
			Command.Size = 0;
			if(Command.SizeSave[0])
				Command.SizeSave[0]--;
		}
		else if(result==1) // �������ݺ�����Ҫ�Ĳ���
		{
			if(Command.SizeSave[0])
				Command.SizeSave[0]--;
		}
		else if(result==2) //���ݲ����ڱ���������Ҫ�Ӹ����ݿ�ʼ�ٴν���
		{
			Command.Type = eCmdInit;
			Command.State = 0;
			Command.Size = 0;
			//if(Command.SizeSave[0])	// ��������û��ʹ�ã����غ��ٴν���
				//Command.SizeSave[0]--;
		}
		if(Command.SizeSave[0])
			goto DealCommandStart;
		break;

	case eCmd1b0aMode:
		result = Cmd1b0aMode(data);
		if(result==0) // �������������������Ž��к������
		{
			Command.Type = eCmdInit;
			Command.State = 0;
			Command.Size = 0;
			if(Command.SizeSave[0])
				Command.SizeSave[0]--;
		}
		else if(result==1) // �������ݺ�����Ҫ�Ĳ���
		{
			if(Command.SizeSave[0])
				Command.SizeSave[0]--;
		}
		else if(result==2) //���ݲ����ڱ���������Ҫ�Ӹ����ݿ�ʼ�ٴν���
		{
			Command.Type = eCmdInit;
			Command.State = 0;
			Command.Size = 0;
			//if(Command.SizeSave[0])	// ��������û��ʹ�ã����غ��ٴν���
				//Command.SizeSave[0]--;
		}
		if(Command.SizeSave[0])
			goto DealCommandStart;
		break;

	case eCmd1b0bWifi:
		result = Cmd1b0bWifi(data);
		if(result==0) // �������������������Ž��к������
		{
			Command.Type = eCmdInit;
			Command.State = 0;
			Command.Size = 0;
			if(Command.SizeSave[0])
				Command.SizeSave[0]--;
		}
		else if(result==1) // �������ݺ�����Ҫ�Ĳ���
		{
			if(Command.SizeSave[0])
				Command.SizeSave[0]--;
		}
		else if(result==2) //���ݲ����ڱ���������Ҫ�Ӹ����ݿ�ʼ�ٴν���
		{
			Command.Type = eCmdInit;
			Command.State = 0;
			Command.Size = 0;
			//if(Command.SizeSave[0])	// ��������û��ʹ�ã����غ��ٴν���
				//Command.SizeSave[0]--;
		}
		if(Command.SizeSave[0])
			goto DealCommandStart;
		break;

	case eCmd1b2aDots:
		result = Cmd1b2aDots(data);
		if(result==0) // �������������������Ž��к������
		{
			Command.Type = eCmdInit;
			Command.State = 0;
			Command.Size = 0;
			if(Command.SizeSave[0])
				Command.SizeSave[0]--;
		}
		else if(result==1) // �������ݺ�����Ҫ�Ĳ���
		{
			if(Command.SizeSave[0])
				Command.SizeSave[0]--;
		}
		else if(result==2) //�����쳣����Ҫ������һ�������ٴν���
		{
			Command.Type = eCmdInit;
			Command.State = 0;
			Command.Size = 0;
			//if(Command.SizeSave[0])	// ��������û��ʹ�ã����غ��ٴν���
				//Command.SizeSave[0]--;
		}
		if(Command.SizeSave[0])
			goto DealCommandStart;
		break;

	case eCmd1b3aSeftPrint:
		Cmd1b3aSeftPrint();
		{
			Command.Type = eCmdInit;
			Command.State = 0;
			Command.Size = 0;
			if(Command.SizeSave[0])
				Command.SizeSave[0]--;
		}
		if(Command.SizeSave[0])
			goto DealCommandStart;
		break;

	case eCmd1b4aFeed:
		result = Cmd1b4aFeed(data);
		if(result==0) // �������������������Ž��к������
		{
			Command.Type = eCmdInit;
			Command.State = 0;
			Command.Size = 0;
			if(Command.SizeSave[0])
				Command.SizeSave[0]--;
		}
		else if(result==1) // �������ݺ�����Ҫ�Ĳ���
		{
			if(Command.SizeSave[0])
				Command.SizeSave[0]--;
		}
		else if(result==2) //�����쳣����Ҫ������һ�������ٴν���
		{
			Command.Type = eCmdInit;
			Command.State = 0;
			Command.Size = 0;
			//if(Command.SizeSave[0])	// ��������û��ʹ�ã����غ��ٴν���
				//Command.SizeSave[0]--;
		}
		if(Command.SizeSave[0])
			goto DealCommandStart;
		break;

	case eCmd1b4bBack:
		result = Cmd1b4bBack(data);
		if(result==0) // �������������������Ž��к������
		{
			Command.Type = eCmdInit;
			Command.State = 0;
			Command.Size = 0;
			if(Command.SizeSave[0])
				Command.SizeSave[0]--;
		}
		else if(result==1) // �������ݺ�����Ҫ�Ĳ���
		{
			if(Command.SizeSave[0])
				Command.SizeSave[0]--;
		}
		else if(result==2) //�����쳣����Ҫ������һ�������ٴν���
		{
			Command.Type = eCmdInit;
			Command.State = 0;
			Command.Size = 0;
			//if(Command.SizeSave[0])	// ��������û��ʹ�ã����غ��ٴν���
				//Command.SizeSave[0]--;
		}
		if(Command.SizeSave[0])
			goto DealCommandStart;
		break;

	case eCmd1b55Direction:
		result = Cmd1b55Direction(data);
		if(result==0) // �������������������Ž��к������
		{
			Command.Type = eCmdInit;
			Command.State = 0;
			Command.Size = 0;
			if(Command.SizeSave[0])
				Command.SizeSave[0]--;
		}
		else if(result==1) // �������ݺ�����Ҫ�Ĳ���
		{
			if(Command.SizeSave[0])
				Command.SizeSave[0]--;
		}
		else if(result==2) //�����쳣����Ҫ������һ�������ٴν���
		{
			Command.Type = eCmdInit;
			Command.State = 0;
			Command.Size = 0;
			//if(Command.SizeSave[0])	// ��������û��ʹ�ã����غ��ٴν���
				//Command.SizeSave[0]--;
		}
		if(Command.SizeSave[0])
			goto DealCommandStart;
		break;

	case eCmd1b7aSpeed:
		result = Cmd1b7aSpeed(data);
		if(result==0) // �������������������Ž��к������
		{
			Command.Type = eCmdInit;
			Command.State = 0;
			Command.Size = 0;
			if(Command.SizeSave[0])
				Command.SizeSave[0]--;
		}
		else if(result==1) // �������ݺ�����Ҫ�Ĳ���
		{
			if(Command.SizeSave[0])
				Command.SizeSave[0]--;
		}
		else if(result==2) //�����쳣����Ҫ������һ�������ٴν���
		{
			Command.Type = eCmdInit;
			Command.State = 0;
			Command.Size = 0;
			//if(Command.SizeSave[0])	// ��������û��ʹ�ã����غ��ٴν���
				//Command.SizeSave[0]--;
		}
		if(Command.SizeSave[0])
			goto DealCommandStart;
		break;

	case eCmd1b80Pid:
		result = Cmd1b80Pid(data);
		if(result==0) // �������������������Ž��к������
		{
			Command.Type = eCmdInit;
			Command.State = 0;
			Command.Size = 0;
			if(Command.SizeSave[0])
				Command.SizeSave[0]--;
		}
		else if(result==1) // �������ݺ�����Ҫ�Ĳ���
		{
			if(Command.SizeSave[0])
				Command.SizeSave[0]--;
		}
		else if(result==2) //�����쳣����Ҫ������һ�������ٴν���
		{
			Command.Type = eCmdInit;
			Command.State = 0;
			Command.Size = 0;
			//if(Command.SizeSave[0])	// ��������û��ʹ�ã����غ��ٴν���
				//Command.SizeSave[0]--;
		}
		if(Command.SizeSave[0])
			goto DealCommandStart;
		break;

	case eCmd1b89Page:
		result = Cmd1b89Page(data);
		if(result==0) // �������������������Ž��к������
		{
			Command.Type = eCmdInit;
			Command.State = 0;
			Command.Size = 0;
			if(Command.SizeSave[0])
				Command.SizeSave[0]--;
		}
		else if(result==1) // �������ݺ�����Ҫ�Ĳ���
		{
			if(Command.SizeSave[0])
				Command.SizeSave[0]--;
		}
		else if(result==2) //�����쳣����Ҫ������һ�������ٴν���
		{
			Command.Type = eCmdInit;
			Command.State = 0;
			Command.Size = 0;
			//if(Command.SizeSave[0])	// ��������û��ʹ�ã����غ��ٴν���
				//Command.SizeSave[0]--;
		}
		if(Command.SizeSave[0])
			goto DealCommandStart;
		break;

	case eCmd1b9cEnd:
		result = Cmd1b9cEnd(data);
		if(result==0) // �������������������Ž��к������
		{
			Command.Type = eCmdInit;
			Command.State = 0;
			Command.Size = 0;
			if(Command.SizeSave[0])
				Command.SizeSave[0]--;
		}
		else if(result==1) // �������ݺ�����Ҫ�Ĳ���
		{
			if(Command.SizeSave[0])
				Command.SizeSave[0]--;
		}
		else if(result==2) //�����쳣����Ҫ������һ�������ٴν���
		{
			Command.Type = eCmdInit;
			Command.State = 0;
			Command.Size = 0;
			//if(Command.SizeSave[0])	// ��������û��ʹ�ã����غ��ٴν���
				//Command.SizeSave[0]--;
		}
		if(Command.SizeSave[0])
			goto DealCommandStart;
		break;

	case eCmd1ba0Start:
		result = Cmd1ba0Start(data);
		if(result==0) // �������������������Ž��к������
		{
			Command.Type = eCmdInit;
			Command.State = 0;
			Command.Size = 0;
			if(Command.SizeSave[0])
				Command.SizeSave[0]--;
		}
		else if(result==1) // �������ݺ�����Ҫ�Ĳ���
		{
			if(Command.SizeSave[0])
				Command.SizeSave[0]--;
		}
		else if(result==2) //�����쳣����Ҫ������һ�������ٴν���
		{
			Command.Type = eCmdInit;
			Command.State = 0;
			Command.Size = 0;
			//if(Command.SizeSave[0])	// ��������û��ʹ�ã����غ��ٴν���
				//Command.SizeSave[0]--;
		}
		if(Command.SizeSave[0])
			goto DealCommandStart;
		break;

	case eCmd1bf8Delay:
		result = Cmd1bf8Dealy(data);
		if(result==0) // �������������������Ž��к������
		{
			Command.Type = eCmdInit;
			Command.State = 0;
			Command.Size = 0;
			if(Command.SizeSave[0])
				Command.SizeSave[0]--;
		}
		else if(result==1) // �������ݺ�����Ҫ�Ĳ���
		{
			if(Command.SizeSave[0])
				Command.SizeSave[0]--;
		}
		else if(result==2) //�����쳣����Ҫ������һ�������ٴν���
		{
			Command.Type = eCmdInit;
			Command.State = 0;
			Command.Size = 0;
			//if(Command.SizeSave[0])	// ��������û��ʹ�ã����غ��ٴν���
				//Command.SizeSave[0]--;
		}
		if(Command.SizeSave[0])
			goto DealCommandStart;
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
				Command.SizeSave[0]--;	// ���ֽ��������ֱ�Ӵ��������ܵȺ�������
		}
		break;

	case eCmd1d56CutPaper:
		result = Cmd1d56CutPaper(data);
		if(result==0) // �������������������Ž��к������
		{
			Command.Type = eCmdInit;
			Command.State = 0;
			Command.Size = 0;
			if(Command.SizeSave[0])
				Command.SizeSave[0]--;
		}
		else if(result==1) // �������ݺ�����Ҫ�Ĳ���
		{
			if(Command.SizeSave[0])
				Command.SizeSave[0]--;
		}
		else if(result==2) //�����쳣����Ҫ������һ�������ٴν���
		{
			Command.Type = eCmdInit;
			Command.State = 0;
			Command.Size = 0;
			//if(Command.SizeSave[0])	// ��������û��ʹ�ã����غ��ٴν���
				//Command.SizeSave[0]--;
		}
		if(Command.SizeSave[0])
			goto DealCommandStart;
		break;
	}
}


/**************************************************************************************************************
	Driver.c	Driver.h
**************************************************************************************************************/
