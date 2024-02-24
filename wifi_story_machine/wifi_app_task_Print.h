#ifndef __WIFI_APP_TASK_PRINT_H
#define __WIFI_APP_TASK_PRINT_H

#define MAXBufferReceiveSize		20000//20000
#define MAX_RECV_BUF_SIZE   		1000
#define SERVER_TCP_PORT 			9100
#define TCPIPRevBusyLevelValue		MAX_RECV_BUF_SIZE*3

struct MyEthernetP
{
    volatile unsigned long IP;
	volatile int SockID;
	volatile bool ConnectState;
	volatile bool TCPIPTaskRunning;
	volatile bool SocketTaskRunning;
	volatile unsigned long SocketPwrite;
	volatile unsigned long SocketPread;
	volatile unsigned char SocketRevBuffer[MAXBufferReceiveSize];

};
extern struct MyEthernetP MEP;
void MyApp_Monitor(void *p);
void MyTCIPPrinterTask(void *p);
void MyTCIPSocketTask(void *p);
void WifiPrintTask(void *p);
void SocketSaveData(uint16_t sizebytes,unsigned char *BufferReceive);
unsigned long GetEmptySize(void);
void CheckBufferEmptySize(void);
int tcp_send_data_sang(const void *buf, u32 len);




#endif
