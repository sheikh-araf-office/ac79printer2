#include "system/includes.h"
#include "action.h"
#include "app_config.h"
#include "storage_device.h"
#include "generic/log.h"
#include "os/os_api.h"
#include "event/key_event.h"
#include "event/bt_event.h"
#include "event/device_event.h"
#include "event/net_event.h"
#include "wifi/wifi_connect.h"
extern void PrinterBTStart(void);
extern void PrinterLEStart(void);

/*�ж��б� */
/* Interrupt List */
const struct irq_info irq_info_table[] = {
// Interrupt Number // Priority (0-7) // Registered CPU (0 or 1)
#ifdef CONFIG_IPMASK_ENABLE
// Non-maskable interrupt method: Supports writing to flash, but all interrupt functions, called functions, and constants must be placed in internal RAM
{ IRQ_SOFT5_IDX, 6, 0 }, // This interrupt is forcibly registered to CPU0
{ IRQ_SOFT4_IDX, 6, 1 }, // This interrupt is forcibly registered to CPU1
#if 0 // Below, SPI1 uses non-maskable interrupt settings
{ IRQ_SPI1_IDX, 7, 1 }, // Interrupt forcibly registered to CPU0/1
#endif
#endif
#if CPU_CORE_NUM == 1
{ IRQ_SOFT5_IDX, 7, 0 }, // This interrupt is forcibly registered to CPU0
{ IRQ_SOFT4_IDX, 7, 1 }, // This interrupt is forcibly registered to CPU1
{ -2, -2, -2 }, // If this line is added, only interrupts before this line are registered to the corresponding core, all other interrupts are forcibly registered to CPU0
#endif

    { -1,     -1,   -1    },
};


/* Create static task stack using os_task_create_static or task_create interface */
#define SYS_TIMER_STK_SIZE 512
#define SYS_TIMER_Q_SIZE 128
static u8 sys_timer_tcb_stk_q[sizeof(StaticTask_t) + SYS_TIMER_STK_SIZE * 4 + sizeof(struct task_queue) + SYS_TIMER_Q_SIZE] ALIGNE(4);

#define SYSTIMER_STK_SIZE 256
static u8 systimer_tcb_stk_q[sizeof(StaticTask_t) + SYSTIMER_STK_SIZE * 4] ALIGNE(4);

#define SYS_EVENT_STK_SIZE 512
static u8 sys_event_tcb_stk_q[sizeof(StaticTask_t) + SYS_EVENT_STK_SIZE * 4] ALIGNE(4);

#define APP_CORE_STK_SIZE 2048
#define APP_CORE_Q_SIZE 1024
static u8 app_core_tcb_stk_q[sizeof(StaticTask_t) + APP_CORE_STK_SIZE * 4 + sizeof(struct task_queue) + APP_CORE_Q_SIZE] ALIGNE(4);


/* Create static task stack using the thread_fork interface */
#define WIFI_TASKLET_STK_SIZE 1400
static u8 wifi_tasklet_tcb_stk_q[sizeof(struct thread_parm) + WIFI_TASKLET_STK_SIZE * 4] ALIGNE(4);

#define WIFI_CMDQ_STK_SIZE 300
static u8 wifi_cmdq_tcb_stk_q[sizeof(struct thread_parm) + WIFI_CMDQ_STK_SIZE * 4] ALIGNE(4);

#define WIFI_MLME_STK_SIZE 700
static u8 wifi_mlme_tcb_stk_q[sizeof(struct thread_parm) + WIFI_MLME_STK_SIZE * 4] ALIGNE(4);

#define WIFI_RX_STK_SIZE 256
static u8 wifi_rx_tcb_stk_q[sizeof(struct thread_parm) + WIFI_RX_STK_SIZE * 4] ALIGNE(4);


/*�����б� */
const struct task_info task_info_table[] = {
    {"thread_fork_kill",    25,      256,   0     },
    {"led_ui_server",       30,      256,   64    },
    {"app_core",            15,     APP_CORE_STK_SIZE,	  APP_CORE_Q_SIZE,		 app_core_tcb_stk_q },
    {"sys_event",           29,     SYS_EVENT_STK_SIZE,	   0, 					 sys_event_tcb_stk_q },
    {"systimer",            14,     SYSTIMER_STK_SIZE, 	   0,					 systimer_tcb_stk_q },
    {"sys_timer",            9,     SYS_TIMER_STK_SIZE,	  SYS_TIMER_Q_SIZE,		 sys_timer_tcb_stk_q },
   // {"audio_server",        16,      512,   64    },
   // {"audio_mix",           28,      512,   0     },
 //   {"audio_encoder",       12,      384,   64    },
    {"speex_encoder",       13,      512,   0     },
    //{"mp3_encoder",         13,      768,   0     },
    {"opus_encoder",        13,     1536,   0     },
    {"vir_dev_task",        14,      256,   0     },
    {"amr_encoder",         13,     1024,   0     },
    {"cvsd_encoder",        13,      512,   0     },
    {"vad_encoder",         14,      768,   0     },
    {"aec_encoder",         13,     1024,   0     },
    {"dns_encoder",         13,      512,   0     },
    {"msbc_encoder",        13,      256,   0     },
 //   {"sbc_encoder",         13,      512,   0     },
 //   {"adpcm_encoder",       13,      512,   0     },
 //   {"echo_deal",           11,     1024,   32    },
 //   {"uac_sync",            20,      512,   0     },
  //  {"uac_play0",           26,      512,   32    },
  //  {"uac_play1",           26,      512,   32    },
   // {"uac_record0",         26,      512,   0     },
  //  {"uac_record1",         26,      512,   0     },
#if CPU_CORE_NUM > 1
    {"#C0usb_msd0",          1,      512,   128   },
#else
    {"usb_msd0",             1,      512,   128   },
#endif
    {"usb_msd1",             1,      512,   128   },
    {"uda_main",             2,     7000,   0     },

    {"update",              21,      512,   32    },
    {"dw_update",           21,      512,   32    },
    {"iperf_test",          15,     1024,   0     },
#ifdef CONFIG_WIFI_ENABLE
    {"tasklet",             10,     WIFI_TASKLET_STK_SIZE,   0,		 wifi_tasklet_tcb_stk_q	 },//ͨ�������������ȼ�ƽ��WIFI�շ�ռ����CPU�ı���
   // {"RtmpMlmeTask",        17,     WIFI_MLME_STK_SIZE,  	 0, 	 wifi_mlme_tcb_stk_q	 },
   // {"RtmpCmdQTask",        17,     WIFI_CMDQ_STK_SIZE,   	 0,  	 wifi_cmdq_tcb_stk_q	 },
  //  {"wl_rx_irq_thread",     7,     WIFI_RX_STK_SIZE,    	 0,  	 wifi_rx_tcb_stk_q  	 },
#endif
#ifdef CONFIG_BT_ENABLE
#if CPU_CORE_NUM > 1
    {"#C0btencry",          14,      512,   128   },
#else
    {"btencry",             14,      512,   128   },
#endif
#if CPU_CORE_NUM > 1
    {"#C0btctrler",         19,      512,   384   },
    {"#C0btstack",          18,      1024,  384   },
#else
    {"btctrler",            19,      512,   384   },
    {"btstack",             18,      768,   384   },
#endif
#endif
/*
    {"wl80_test",            1,     2048,   64    },

    {"video_server",        16,      768,   128   },
    {"vpkg_server",         16,      512,   128   },
    {"video0_rec0",         20,      256,   128   },
    {"video0_rec1",         20,      256,   128   },
    {"video2_rec0",         20,      512,   128   },
    {"video2_rec1",         20,      512,   128   },
    {"net_video_server",    16,      256,   64    },

    {"net_avi0",            18,      512,   0     },
    {"net_avi1",            18,      512,   0     },

    {"avi0",                11,      320,   64    },
    {"jpg_dec",             10,     1024,   32    },
    {"video_dec_server",    16,     1024,   256   },
    {"vunpkg_server",       16,     1024,   128   },
    {"ui",                  21,      768,   256   },
    */
    {"WifiPrintTask",   30,     4096,   4096    },
    {"USBPrinterTask",  30,     4096,   4096    },
    {"BTPrinterTask",   30,     4096,   4096    },
    {"LEPrinterTask",   30,     4096,   4096    },
    {0, 0},
};
#if 0
char task_designated_cpu(const char *task_name)// Return 0 to designate CPU0 for execution, return 1 to designate CPU1 for execution, return -1 for both CPU0 and CPU1
{
    if (!strcmp(task_name, "init")) {
        return -1; // The operating system's first task "init" must be installed on core 0, and will be automatically deleted after execution
    }
    if (!strcmp(task_name, "thread_fork_kill")) {
        return -1;
    }

    // User can add designated tasks here

    return -1;
}
#endif



/*

. Default system event handling function
. This function will be called when the event handling functions of all active apps return false
*/
void app_default_event_handler(struct sys_event *event)
{
    switch (event->type) {
    case SYS_KEY_EVENT:
        break;
    case SYS_TOUCH_EVENT:
        break;
    case SYS_DEVICE_EVENT:
        break;
    case SYS_NET_EVENT:
        break;
    case SYS_BT_EVENT:
        break;
    default:
        ASSERT(0, "unknown event type: %s\n", __func__);
        break;
    }
}

/*
 * Ӧ�ó���������
 */

void app_main()
{
    struct intent it;
    puts("------------- wifi_BT Printer app main-------------\n");
    init_intent(&it);
    it.name = "app_music";
    it.action = ACTION_MUSIC_PLAY_MAIN;
    start_app(&it);

#if defined CONFIG_BT_ENABLE


    extern void bt_ble_module_init(void);
    bt_ble_module_init();
    PrinterBTStart();
    PrinterLEStart();
    malloc_stats();
#endif
}
