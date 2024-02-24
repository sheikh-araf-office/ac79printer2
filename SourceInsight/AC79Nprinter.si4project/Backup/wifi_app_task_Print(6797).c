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

#include "Driver.h"
#include "McuInt.h"
#include "UserInterface.h"

//tcp_server数据结构
struct tcp_server_info {
    struct list_head client_head;  //链表
    struct sockaddr_in local_addr; //tcp_server地址信息
    void *fd;                      //tcp_server套接字
    OS_MUTEX tcp_mutex;            //互斥锁
};
struct tcp_client_info {
    struct list_head entry;         //链表节点
    struct sockaddr_in remote_addr; //tcp_client地址信息
    void *fd;                       //建立连接后的套接字
};
static struct tcp_client_info *get_tcp_client_info(void);
static void tcp_client_quit(struct tcp_client_info *priv);

static struct tcp_server_info Printer_server_info;
struct MyEthernetP MEP;
static struct tcp_client_info *get_tcp_client_info(void)
{
    struct list_head *pos = NULL;
    struct tcp_client_info *client_info = NULL;
    struct tcp_client_info *old_client_info = NULL;


    os_mutex_pend(&Printer_server_info.tcp_mutex, 0);

    list_for_each(pos, &Printer_server_info.client_head) {
        client_info = list_entry(pos, struct tcp_client_info, entry);
    }

    os_mutex_post(&Printer_server_info.tcp_mutex);

    return client_info;
}
static int tcp_recv_data(void *sock_hdl, void *buf, u32 len)
{
    return sock_recv(sock_hdl, buf, len, 0);
}

static int accept_pid = -1;
static int recv_pid = -1;
static void tcp_sock_accepet(void);
static void tcp_recv_handler(void);

static int tcp_send_data(void *sock_hdl, const void *buf, u32 len)
{
    return sock_send(sock_hdl, buf, len, 0);
}
int tcp_send_data_sang(const void *buf, u32 len)
{
    struct tcp_client_info *client_info = NULL;
    char recv_buf[MAX_RECV_BUF_SIZE] = {0};

    client_info = get_tcp_client_info();
    os_time_dly(100);
    if(client_info == NULL)
    {
        printf("not connnect client\r\n");
        return 0;
    }
    return sock_send(client_info->fd, buf, len, 0);
}


unsigned long Tlen=0;
unsigned long Tlen1=0;

static void Printer_Tcp_Recv_handler(void)
{
    struct tcp_client_info *client_info = NULL;
    char recv_buf[MAX_RECV_BUF_SIZE] = {0};
	int len;

#if DebugMsg
	printf("sang:Printer_Tcp_Recv_handler Start.............\r\n");
#endif
	_reconnect_:
    do
	{
        client_info = get_tcp_client_info();
        os_time_dly(200); // 100
    }
	while (client_info == NULL);
#if DebugMsg
	printf("sang:tcp_client_info valid!!!!!!!!!!!!!!!!!!!\r\n");
#endif
	MEP.SocketTaskRunning=true;
    while(1)
	{
		memset(recv_buf, 0, sizeof(recv_buf));
		len=tcp_recv_data(client_info->fd, recv_buf, sizeof(recv_buf));
        if (len>0)
		{
			SocketSaveData(len,recv_buf);
			//CheckBufferEmptySize();
			Tlen+=len;
        #if DebugMsg
			printf("Tlen:%d,Tlen1:%d\r\n", Tlen, Tlen1);
		#endif
        }
		else
		{
        #if DebugMsg
			printf("sang:TCP tcp_recv_data err!!!!!!!!!!");
        #endif
            tcp_client_quit(client_info);
			MEP.SocketTaskRunning=false;
            goto _reconnect_;
        }
    }
}

static void Printer_Tcp_Sock_Accpet(void)
//static void Printer_Tcp_Sock_Accept(void)
{
    socklen_t len = sizeof(Printer_server_info.local_addr);
	MEP.SocketTaskRunning=false;
    while(1)
	{
        struct tcp_client_info *client_info = calloc(1, sizeof(struct tcp_client_info));
        if (client_info == NULL)
		{
            printf(" %s calloc fail\n", __FILE__);
            return;
        }
		while(MEP.SocketTaskRunning)
		{
			os_time_dly(10);//10
		}

    #if DebugMsg
		printf("sang:sock_accept........................\n");
    #endif
        client_info->fd  = sock_accept(Printer_server_info.fd, (struct sockaddr *)&client_info->remote_addr, &len, NULL, NULL);
        if (client_info->fd == NULL)
		{
        #if DebugMsg
            printf("sang:%s socket_accept fail\n",  __FILE__);
        #endif
            return;
        }
		else
		{
			Tlen=0;
        #if DebugMsg
			printf("sang:sock_accept success!!!!!!!!!!!!!!!!!!!!!!!\n");
        #endif
		}

        os_mutex_pend(&Printer_server_info.tcp_mutex, 0);
        list_add_tail(&client_info->entry, &Printer_server_info.client_head);
        os_mutex_post(&Printer_server_info.tcp_mutex);

    #if DebugMsg
        printf("sang:%s, build connnect success!!!!!!!!!!!!!!!!!!!!!!!\n", inet_ntoa(client_info->remote_addr.sin_addr));
    #endif
    }
}

static int Printer_Tcp_Server_Init(int port)
{
    u32 opt = 1;
	printf("sang:Printer_Tcp_Server_Init...................");
    memset(&Printer_server_info, 0, sizeof(Printer_server_info));
    Printer_server_info.local_addr.sin_family = AF_INET;
    Printer_server_info.local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    Printer_server_info.local_addr.sin_port = htons(port);

    Printer_server_info.fd = sock_reg(AF_INET, SOCK_STREAM, 0, NULL, NULL);
    if (Printer_server_info.fd == NULL)
	{
        printf("%s build socket fail\n",  __FILE__);
        return -1;
    }
    if (sock_setsockopt(Printer_server_info.fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
	{
        printf("%s sock_setsockopt fail\n", __FILE__);
        return -1;
    }
    if (sock_bind(Printer_server_info.fd, (struct sockaddr *)&Printer_server_info.local_addr, sizeof(struct sockaddr)))
	{
        printf("%s sock_bind fail\n", __FILE__);
        return -1;
    }
    if (sock_listen(Printer_server_info.fd, 0x2) != 0)
	{
        printf("%s sock_listen fail\n", __FILE__);
        return -1;
    }
    if (os_mutex_create(&Printer_server_info.tcp_mutex) != OS_NO_ERR)
	{
        printf("%s os_mutex_create fail\n", __FILE__);
        return -1;
    }
    INIT_LIST_HEAD(&Printer_server_info.client_head);
    if (thread_fork("tcp_sock_accept", 26, 4096, 0, &accept_pid, Printer_Tcp_Sock_Accpet, NULL) != OS_NO_ERR)
    //if (thread_fork("tcp_sock_accept", 26, 4096, 0, &accept_pid, Printer_Tcp_Sock_Accept, NULL) != OS_NO_ERR)
	{
        printf("%s thread fork fail\n", __FILE__);
        return -1;
    }
    if (thread_fork("tcp_recv_handler", 25, 4096, 0, &recv_pid, Printer_Tcp_Recv_handler, NULL) != OS_NO_ERR)
	{
        printf("%s thread fork fail\n", __FILE__);
        return -1;
    }
    return 0;
}
static void tcp_client_quit(struct tcp_client_info *priv)
{
    list_del(&priv->entry);
    sock_set_quit(priv->fd);
    sock_unreg(priv->fd);
    priv->fd = NULL;
    free(priv);
}
static void tcp_server_exit(void)
{
    struct list_head *pos = NULL;
    struct tcp_client_info *client_info = NULL;
    thread_kill(&accept_pid, KILL_WAIT);
    thread_kill(&recv_pid, KILL_WAIT);
    os_mutex_pend(&Printer_server_info.tcp_mutex, 0);
    list_for_each(pos, &Printer_server_info.client_head)
	{
        client_info = list_entry(pos, struct tcp_client_info, entry);
        if (client_info) {
            list_del(&client_info->entry);
            sock_unreg(client_info->fd);
            client_info->fd = NULL;
            free(client_info);
        }
    }
    os_mutex_post(&Printer_server_info.tcp_mutex);
    os_mutex_del(&Printer_server_info.tcp_mutex, OS_DEL_NO_PEND);
}
static void PrinterTcp_Server_Start(void *priv)
{
    int err;
    enum wifi_sta_connect_state state;
    while (1)
	{
    #if DebugMsg
        printf("sang:Connecting to the network.......................\n");
    #endif
        state = wifi_get_sta_connect_state();
        if (WIFI_STA_NETWORK_STACK_DHCP_SUCC == state)
		{
        #if DebugMsg
            printf("sang:Network connection is successful!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
        #endif
            break;
        }
        os_time_dly(1500);
    }
    err = Printer_Tcp_Server_Init(SERVER_TCP_PORT);
    if (err == -1)
	{
    #if DebugMsg
        printf("sang:tcp_server_init faile\n");
    #endif
        tcp_server_exit();
    }
}

unsigned long GetEmptySize(void)
{
	if(MEP.SocketPread<=MEP.SocketPwrite)
	{
		return MAXBufferReceiveSize-MEP.SocketPwrite+MEP.SocketPread;
	}
	else
	{
        return MEP.SocketPread-MEP.SocketPwrite;
	}
}
void CheckBufferEmptySize(void)
{
	while (GetEmptySize()<TCPIPRevBusyLevelValue)
	{
		// Alke 20230725
	#if 0
        //printf("sang:delay P:%d,W:%d...\r\n",MEP.SocketPread,MEP.SocketPwrite);
		os_time_dly(1);
    #else
		printf("sang:delay P:%d,W:%d...\r\n",MEP.SocketPread,MEP.SocketPwrite);
		os_time_dly(5);
    #endif
	}
}

// 此处应该是接收到数据后，向对应接收缓冲区写入数据，类似中断函数
// *BufferReceive后面必须有连续sizebytes个数据
void SocketSaveData(uint16_t sizebytes, unsigned char *BufferReceive)
{
    unsigned int i;

	if((MEP.SocketPwrite+sizebytes)<=MAXBufferReceiveSize)
	{
//printf("s1");
		memcpy(&MEP.SocketRevBuffer[MEP.SocketPwrite], BufferReceive, sizebytes);

		// 扫描WIFI命令
/*		for(i=0; i++; i<sizebytes)
			ScanWifiCmd(*(BufferReceive+i));*/

		if((MEP.SocketPwrite+sizebytes)==MAXBufferReceiveSize)
			MEP.SocketPwrite=0;
		else
			MEP.SocketPwrite+=sizebytes;
	}
	else
	{
//printf("s2");
		memcpy(&MEP.SocketRevBuffer[MEP.SocketPwrite], BufferReceive, MAXBufferReceiveSize-MEP.SocketPwrite);
		// 扫描WIFI命令
/*		for(i=0; i++; i<(MAXBufferReceiveSize-MEP.SocketPwrite))
			ScanWifiCmd(*(BufferReceive+i));*/

		memcpy(MEP.SocketRevBuffer, &BufferReceive[MAXBufferReceiveSize-MEP.SocketPwrite], MEP.SocketPwrite+sizebytes-MAXBufferReceiveSize);
		// 扫描WIFI命令
/*		for(i=0; i++; i<(MEP.SocketPwrite+sizebytes-MAXBufferReceiveSize))
			ScanWifiCmd(*(BufferReceive+i));*/

		MEP.SocketPwrite=MEP.SocketPwrite+sizebytes-MAXBufferReceiveSize;
	}
}
// WIFI
void WIFIDealData(unsigned char data)
{
    // Alke @20230726 此处可以有WIFI回传数据，速度较慢
	DealData(data);
printf("%c-", data);
#if DebugChannelRerutn
	tcp_send_data_sang(&data, 1);
#endif
}
// 操作系统级别任务切换，大约10ms执行一次
void SocketDataProcesser(void)
{
//    static u8 ss = 0;
//    u8 c;

	while(1)
	{
//printf("-");
		if(BufferPrintLineWrited() > BufferPrintLineFull) // Alke @20230629
			return;
//printf("!");

		// R==W: 空
    	if(MEP.SocketPread==MEP.SocketPwrite) //空的返回
    	{
//printf("0");
			return;
		}
		else							// 有数据
		{
			WIFIDealData(MEP.SocketRevBuffer[MEP.SocketPread]);
			Tlen1++;
			if(MEP.SocketPread==(MAXBufferReceiveSize-1))
			{
				MEP.SocketPread = 0;
printf("1-");
			}
			else
			{
				MEP.SocketPread++;
printf("2-");
			}
		}
	}
}

// 10ms 执行一次，需要计数的必须定义静态变量
void WifiPrintTask(void *p)
{
    int res;
    int msg[16];
static int cc;

    printf("sang:WifiPrintTask..............\r\n");
    MEP.SocketPwrite=0;
    MEP.SocketPread=0;	// R==W : 空
    malloc_stats();
    res = __os_taskq_pend(msg, ARRAY_SIZE(msg), 100);

    while(1)
    {
		SocketDataProcesser();
		res = __os_taskq_pend(msg, ARRAY_SIZE(msg), 1);

/*		if(ReturnMsg == 0xff)
		{
		printf("WifiReturnMsg()--------------------------------------------");
			WifiReturnMsg();
			ReturnMsg = 0;
		}
		else if(ReturnMsg)
		{
		printf("USBReturnWifi(ReturnMsg)--------------------------------------------");
			USBReturnWifi(ReturnMsg);
			ReturnMsg = 0;
		}*/
    }
}
void WifiPrinterStart(void)
{
	WifiState = 0x01;
	if (thread_fork("PrinterTcp_Server_Start", 10, 512, 0, NULL, PrinterTcp_Server_Start, NULL) != OS_NO_ERR)
	{
        printf("thread fork fail\n");
    }
	task_create(WifiPrintTask, NULL, "WifiPrintTask");
}

late_initcall(WifiPrinterStart);




