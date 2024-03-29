#ifndef APP_CONFIG_H
#define APP_CONFIG_H

#define AUDIO_ENC_SAMPLE_SOURCE_MIC         0
#define AUDIO_ENC_SAMPLE_SOURCE_PLNK0       1
#define AUDIO_ENC_SAMPLE_SOURCE_PLNK1       2
#define AUDIO_ENC_SAMPLE_SOURCE_IIS0        3
#define AUDIO_ENC_SAMPLE_SOURCE_IIS1        4
#define AUDIO_ENC_SAMPLE_SOURCE_LINEIN      5

#define CONFIG_AUDIO_DEC_PLAY_SOURCE        "dac"

#include "board_config.h"

#if !defined CONFIG_VIDEO_ENABLE || defined CONFIG_NO_SDRAM_ENABLE
#undef  CONFIG_RTOS_AND_MM_LIB_CODE_SECTION_IN_SDRAM
#endif

#ifdef CONFIG_NO_SDRAM_ENABLE
#undef  __SDRAM_SIZE__
#define __SDRAM_SIZE__    (0 * 1024 * 1024)
#endif

#ifndef TCFG_ADKEY_ENABLE
#define TCFG_ADKEY_ENABLE             0         //AD按键
#endif

#ifndef TCFG_IOKEY_ENABLE
#define TCFG_IOKEY_ENABLE             0         //IO按键
#define TCFG_IO_MULTIPLEX_WITH_SD     0
#endif

#ifndef TCFG_IRKEY_ENABLE
#define TCFG_IRKEY_ENABLE             0         //红外遥控按键
#endif

#ifndef TCFG_RDEC_KEY_ENABLE
#define TCFG_RDEC_KEY_ENABLE          0         //旋转编码�?
#endif

#ifndef TCFG_TOUCH_KEY_ENABLE
#define TCFG_TOUCH_KEY_ENABLE         0         //PLCNT触摸按键
#endif

#ifndef TCFG_CTMU_TOUCH_KEY_ENABLE
#define TCFG_CTMU_TOUCH_KEY_ENABLE    0         //CTMU触摸按键
#endif

#ifndef TCFG_SD0_ENABLE
#define TCFG_SD0_ENABLE         0
#endif

#ifndef TCFG_SD1_ENABLE
#define TCFG_SD1_ENABLE         0
#endif

#define CONFIG_DEBUG_ENABLE                     //打印�?�?
//#define RTOS_STACK_CHECK_ENABLE                 //是否启用定时�?查任务栈
// #define CONFIG_SAVE_EXCEPTION_LOG_IN_FLASH   //保存异常打印信息到flash
// #define MEM_LEAK_CHECK_ENABLE                //是否启用内存泄漏�?�?(�?要包含mem_leak_test.h头文�?)
// #define CONFIG_AUTO_SHUTDOWN_ENABLE          //自动倒数关机
//#define CONFIG_RTC_ENABLE                       //RTC�?�?
// #define CONFIG_SYS_VDD_CLOCK_ENABLE          //系统可使用动态电源�?�时钟配�?
//#define CONFIG_IPMASK_ENABLE				//系统使用不可屏蔽中断

//*********************************************************************************//
//                                  FCC测试相关配置                                //
//*********************************************************************************//
//#define RF_FCC_TEST_ENABLE//使能RF_FCC测试，详细配置见"apps/common/rf_fcc_tool/include/rf_fcc_main.h"

//*********************************************************************************//
//                                  网络相关配置                                   //
//*********************************************************************************//

#ifdef CONFIG_NET_ENABLE
#define CONFIG_WIFI_ENABLE  					/* 无线WIFI */
#ifdef CONFIG_NO_SDRAM_ENABLE
#define CONFIG_RF_TRIM_CODE_MOVABLE //把RF TRIM 的运行代码动态加载到ram运行(节省4K ram内存), 防止RF TRIM 期间500ms大电流访问flash造成flash挂掉持续大电�?
#else
#define CONFIG_RF_TRIM_CODE_AT_RAM //把RF TRIM 的运行代码定死到ram运行(浪费4K ram内存,否则若动态加载到sdram�?清cache), 防止RF TRIM 期间500ms大电流访问flash造成flash挂掉持续大电�?
#endif
#define WIFI_COLD_START_FAST_CONNECTION //启用WIFI冷启动快�?
// #define CONFIG_IPERF_ENABLE  				// iperf测试
// #define CONFIG_WIFIBOX_ENABLE                //WIFI测试盒�?�能测试
// #define CONFIG_MP_TEST_ENABLE                //RF量产测试
// #define CONFIG_READ_RF_PARAM_FROM_CFGTOOL_ENABLE    //从配置文件读取RF参数
#define CONFIG_SERVER_ASSIGN_PROFILE		//第三方平台的profile由杰理服务器分配
// #define CONFIG_PROFILE_UPDATE				//每次�?机都从杰理服务器获取第三方平台的profile,不读flash

// @20231220 张耿与弥龙测试只能用静态：动态的话，没有关机也会改变，无法使用
#define CONFIG_STATIC_IPADDR_ENABLE          // Alke 20230713 DHCP 记忆路由器分配的IP,下次直接使用记忆IP节省DHCP时间 // @20230712 Alke 默认关闭

#define CONFIG_ASSIGN_MACADDR_ENABLE        //第一次开机连上外网后，使用杰理服务器分配WIFI模块的MAC地址, 关闭则使�?<蓝牙地址更新工具*.exe>或�?�随机数作为MAC地址
// #define CONFIG_TURING_SDK_ENABLE             //使用图灵平台
// #define CONFIG_DEEPBRAIN_SDK_ENABLE			//使用义语平台
// #define CONFIG_DUER_SDK_ENABLE				//使用百度平台
// #define CONFIG_ECHO_CLOUD_SDK_ENABLE			//使用回声云平�?
// #define CONFIG_DUI_SDK_ENABLE	            //使用思必驰DUI平台
// #define CONFIG_ALI_SDK_ENABLE	            //使用天猫精灵平台
// #define CONFIG_TVS_SDK_ENABLE	            //使用腾讯云平�?
// #define CONFIG_TELECOM_SDK_ENABLE            //电信云平�?
// #define CONFIG_JL_CLOUD_SDK_ENABLE           //打开使用杰理云平�?
// #define CONFIG_DLNA_SDK_ENABLE               //打开DLNA音乐播放功能
// #define CONFIG_DOWNLOAD_SAVE_FILE            //保存网络下载的音乐文�?
// #define PAYMENT_AUDIO_SDK_ENABLE				//打开语音播报功能
// #define CONFIG_SCAN_PEN_ENABLE               //扫描笔图像拼�?
// #define CONFIG_HTTP_SERVER_ENABLE            //HTTP服务�?
// #define CONFIG_FTP_SERVER_ENABLE             //FTP服务�?


/*************电信云平台配网方式�?�择*************/
#ifdef CONFIG_TELECOM_SDK_ENABLE
#define CONFIG_APLINK_NET_CFG                 //电信AP配网。注意：1.电信AP配网不能与elink无感配网同时使能�?2.wifi库需要支持ap
#ifndef CONFIG_APLINK_NET_CFG
#define CONFIG_ELINK_QLINK_NET_CFG            //电信elink无感配网。注意：elink无感配网不能与AIRKISS配网同时使能�?
#endif
#define CONFIG_CTEI_DEVICE_ENABLE             //电信非蜂窝类串码对接设备使用
//#define CONFIG_MC_DEVICE_ENABLE               //电信mc接口类直连设备使�?
#endif

#ifdef CONFIG_VIDEO_ENABLE
/*************绘本识别服务�?*************/
#ifdef CONFIG_TURING_SDK_ENABLE
#define CONFIG_TURING_PAGE_TURNING_DET_ENABLE	//图灵翻页算法，需要配合图灵绘本平台使�?
#endif

// #define CONFIG_WT_SDK_ENABLE                 //新玩瞳接�?
//#define CONFIG_QR_CODE_NET_CFG					//二维码配�?
#endif

#ifdef CONFIG_WIFI_ENABLE
#define CONFIG_AIRKISS_NET_CFG					//AIRKISS配网
#endif

#ifdef CONFIG_AUDIO_ENABLE
//#define CONFIG_ACOUSTIC_COMMUNICATION_ENABLE	//声波配网

#ifndef CONFIG_NO_SDRAM_ENABLE

#define AISP_ALGORITHM 1 //思必驰双mic唤醒,未授权版本只支持运行10分钟
#define ROOBO_ALGORITHM 2 //ROOBO �?/双MIC唤醒 ROOBO_DUAL_MIC_ALGORITHM ,,,测试版本只支持运�?10分钟
#define WANSON_ALGORITHM 3 //华镇算法,测试版只能够正确识别1000�?
#define JLKWS_ALGORITHM 4 //杰理算法,现阶段仅用于测试效果
// #define CONFIG_ASR_ALGORITHM  AISP_ALGORITHM    //本地打断唤醒算法选择

#ifdef CONFIG_ASR_ALGORITHM
#define WIFI_PCM_STREAN_SOCKET_ENABLE           //打开打断唤醒pcm音频流局域网传输工具
#endif

#endif
#endif

#endif

//*********************************************************************************//
//                                  AUDIO配置                                      //
//*********************************************************************************//
#ifdef CONFIG_AUDIO_ENABLE

// #define CONFIG_DEC_DIGITAL_VOLUME_ENABLE     //数字音量淡入淡出功能
// #define CONFIG_DEC_ANALOG_VOLUME_ENABLE      //模拟音量淡入淡出功能
// #define CONFIG_RESUME_LOCAL_PLAY_FILE        //本地断点恢复播放

#ifdef CONFIG_BT_ENABLE
#define CONFIG_SBC_DEC_ENABLE
#if __FLASH_SIZE__ > (1 * 1024 * 1024)
//#define CONFIG_SBC_ENC_ENABLE
//#define CONFIG_MSBC_DEC_ENABLE
//#define CONFIG_MSBC_ENC_ENABLE
//#define CONFIG_CVSD_DEC_ENABLE
//#define CONFIG_CVSD_ENC_ENABLE
#endif
#endif

#define CONFIG_PCM_DEC_ENABLE
#define CONFIG_PCM_ENC_ENABLE
//#define CONFIG_WAV_DEC_ENABLE
//#define CONFIG_WAV_ENC_ENABLE
#if __FLASH_SIZE__ > (1 * 1024 * 1024)
//#define CONFIG_MP3_DEC_ENABLE
//#define CONFIG_M4A_DEC_ENABLE
//#define CONFIG_VIRTUAL_DEV_ENC_ENABLE
//#define CONFIG_SPEEX_ENC_ENABLE
//#define CONFIG_OPUS_ENC_ENABLE
//#define CONFIG_VAD_ENC_ENABLE
#endif
#if __FLASH_SIZE__ > (2 * 1024 * 1024)
//#define CONFIG_DTS_DEC_ENABLE
//#define CONFIG_ADPCM_DEC_ENABLE
//#define CONFIG_MP3_ENC_ENABLE
//#define CONFIG_WMA_DEC_ENABLE
//#define CONFIG_AMR_DEC_ENABLE
//#define CONFIG_APE_DEC_ENABLE
//#define CONFIG_FLAC_DEC_ENABLE
//#define CONFIG_SPEEX_DEC_ENABLE
//#define CONFIG_ADPCM_ENC_ENABLE
//#define CONFIG_OPUS_DEC_ENABLE
//#define CONFIG_AMR_ENC_ENABLE
//#define CONFIG_AEC_ENC_ENABLE
#define CONFIG_DNS_ENC_ENABLE

// #define CONFIG_SPECTRUM_FFT_EFFECT_ENABLE    //频谱运算
//#define CONFIG_REVERB_MODE_ENABLE            //打开混响功能
//#define CONFIG_AUDIO_MIX_ENABLE              //打开叠音功能
//#define CONFIG_AUDIO_PS_ENABLE               //打开变调变�?�功�?
#endif

#ifdef CONFIG_AEC_ENC_ENABLE
#define CONFIG_USB_AUDIO_AEC_ENABLE          //usb mic使能回声消除功能
// #define CONFIG_AEC_LINEIN_CHANNEL_ENABLE     //AEC回采使用硬件通道数据
#endif

//#define CONFIG_ALL_ADC_CHANNEL_OPEN_ENABLE   //四路ADC硬件全开

#ifndef CONFIG_AUDIO_LINEIN_SAMPLERATE
#define CONFIG_AUDIO_LINEIN_SAMPLERATE 48000 //LINEIN采样�?
#endif
#ifndef CONFIG_AUDIO_LINEIN_CHANNEL
#define CONFIG_AUDIO_LINEIN_CHANNEL    1     //LIENIN通道�?
#endif
#ifndef CONFIG_AUDIO_LINEIN_CHANNEL_MAP
#define CONFIG_AUDIO_LINEIN_CHANNEL_MAP (BIT(3)) //LIENIN通道选择
#endif
#ifndef CONFIG_AUDIO_LINEIN_ADC_GAIN
#define CONFIG_AUDIO_LINEIN_ADC_GAIN   60    //LIENIN的模拟增�?
#endif

#endif


//*********************************************************************************//
//                                  路径配置                                       //
//*********************************************************************************//
#if TCFG_SD0_ENABLE
#define CONFIG_STORAGE_PATH 	"storage/sd0"  //定义对应SD0的路�?
#define SDX_DEV					"sd0"
#endif

#if TCFG_SD1_ENABLE
#define CONFIG_STORAGE_PATH 	"storage/sd1" //定义对应SD1的路�?
#define SDX_DEV					"sd1"
#endif

#ifndef CONFIG_STORAGE_PATH
#define CONFIG_STORAGE_PATH		"storage/sdx" //不使用SD定义对应别的路径，防止编译出�?
#define SDX_DEV					"sdx"
#endif

#define CONFIG_UDISK_STORAGE_PATH	"storage/udisk0"

#define CONFIG_ROOT_PATH            CONFIG_STORAGE_PATH"/C/" //定义对应SD文件系统的根目录路径
#define CONFIG_UDISK_ROOT_PATH     	CONFIG_UDISK_STORAGE_PATH"/C/" //定义对应U盘文件系统的根目录路�?

#define CONFIG_MUSIC_PATH_SD        CONFIG_ROOT_PATH
#define CONFIG_MUSIC_PATH_SD0       "storage/sd0/C/"
#define CONFIG_MUSIC_PATH_SD1       "storage/sd1/C/"
#define CONFIG_MUSIC_PATH_UDISK     CONFIG_UDISK_ROOT_PATH
#define CONFIG_MUSIC_PATH_UDISK0    "storage/udisk0/C/"
#define CONFIG_MUSIC_PATH_UDISK1    "storage/udisk1/C/"

#define CONFIG_MUSIC_PATH_FLASH             "mnt/sdfile/res/"
#define CONFIG_EQ_FILE_NAME                 "mnt/sdfile/res/cfg/eq_cfg_hw.bin"
#ifdef CONFIG_AUDIO_ENABLE
#define CONFIG_VOICE_PROMPT_FILE_PATH       "mnt/sdfile/res/audlogo/"
#endif

#if __FLASH_SIZE__ > (2 * 1024 * 1024)
#define CONFIG_DOUBLE_BANK_ENABLE           1//双备份方式升�?
#else
#define CONFIG_DOUBLE_BANK_ENABLE           0//双备份方式升�?
#endif
#define CONFIG_UPGRADE_FILE_NAME            "update.ufw"
#define CONFIG_UPGRADE_PATH                 CONFIG_ROOT_PATH\
											CONFIG_UPGRADE_FILE_NAME	//备份方式升级

#define CONFIG_SD0_UPGRADE_PATH             "storage/sd0/C/"CONFIG_UPGRADE_FILE_NAME	//备份方式升级
#define CONFIG_SD1_UPGRADE_PATH             "storage/sd1/C/"CONFIG_UPGRADE_FILE_NAME	//备份方式升级
#define CONFIG_UDISK0_UPGRADE_PATH          "storage/udisk0/C/"CONFIG_UPGRADE_FILE_NAME	//备份方式升级
#define CONFIG_UDISK1_UPGRADE_PATH          "storage/udisk1/C/"CONFIG_UPGRADE_FILE_NAME	//备份方式升级


//*********************************************************************************//
//                                  EQ配置                                         //
//*********************************************************************************//
#define CONFIG_VOLUME_TAB_TEST_ENABLE             0     //音量表测�?
//EQ配置，使用在线EQ时，EQ文件和EQ模式无效。有EQ文件时，默认不用EQ模式切换功能
#ifdef CONFIG_AUDIO_ENABLE
#define TCFG_EQ_ENABLE                            0     //支持EQ功能
#else
#define TCFG_EQ_ENABLE                            0     //关闭EQ功能
#endif
#define TCFG_EQ_ONLINE_ENABLE                     0     //支持在线EQ调试
#define TCFG_HW_SOFT_EQ_ENABLE                    1     //�?3段使用软件运�?
#if __FLASH_SIZE__ > (1 * 1024 * 1024)
#define TCFG_LIMITER_ENABLE                       1     //限幅�?
#else
#define TCFG_LIMITER_ENABLE                       0     //限幅�?
#endif
#define TCFG_EQ_FILE_ENABLE                       1     //从bin文件读取eq配置数据
#define TCFG_DRC_ENABLE                           TCFG_LIMITER_ENABLE
//EQ在线调试通信类型
#define TCFG_NULL_COMM                            0     //不支持�?�信
#define TCFG_USB_COMM                             1     //USB通信
#if TCFG_EQ_ONLINE_ENABLE && defined EQ_CORE_V1
#define TCFG_COMM_TYPE                            TCFG_USB_COMM
#else
#define TCFG_COMM_TYPE                            TCFG_NULL_COMM
#endif


//*********************************************************************************//
//                                  VIDEO配置                                      //
//*********************************************************************************//
#ifdef CONFIG_VIDEO_ENABLE

#ifdef CONFIG_SCAN_PEN_ENABLE
#define CONFIG_VIDEO1_ENABLE
#define CONFIG_SPI_VIDEO_ENABLE
#define CONFIG_VIDEO_720P
#endif

//#define CONFIG_MASS_PRODUCTION_ENABLE //启用产测模式
//#define CONFIG_OSD_ENABLE			/* 视频OSD时间戳开�? */
#define CONFIG_VIDEO_REC_PPBUF_MODE	/*视频使用乒乓BUF模式(图传<=20�?),关闭则用lbuf模式(图传>20帧和写卡录像),缓冲区大小配置video_buf_config.h*/
//#define CONFIG_VIDEO_SPEC_DOUBLE_REC_MODE	/* 视频支持双路莫模�?(�?路实时流、一路录SD�?)*/

#ifdef CONFIG_MASS_PRODUCTION_ENABLE
#define STA_WIFI_SSID     "test"    //量产模式的路由器名称
#define STA_WIFI_PWD      "12345678"  //量产模式的路由器密码
//#define CONFIG_PRODUCTION_IO_PORT			IO_PORTB_01 //配置进入量产莫模式的IO
//#define CONFIG_PRODUCTION_IO_STATE		0 			//配置进入量产莫模式的IO状�?�：0低电平，1高电�?
#endif

//*********************************************************************************//
//                             编码图片分辨�?                                      //
//*********************************************************************************//
//#define CONFIG_VIDEO_720P
#ifdef CONFIG_VIDEO_720P
#define CONFIG_VIDEO_IMAGE_W    1280
#define CONFIG_VIDEO_IMAGE_H    720
#else
#define CONFIG_VIDEO_IMAGE_W    640
#define CONFIG_VIDEO_IMAGE_H    480
#undef  CONFIG_WMA_DEC_ENABLE
#endif

//*********************************************************************************//
//                             视频流相关配�?                                      //
//*********************************************************************************//
#define VIDEO_REC_AUDIO_SAMPLE_RATE		0    //视频流的音频采样�?,注意：硬件没MIC则为0
#define VIDEO_REC_FPS 					20   //录像SD卡视频帧率设�?,0为默�?

#define CONFIG_USR_VIDEO_ENABLE		//用户VIDEO使能
#define CONFIG_USR_PATH 	"192.168.1.1:8000" //用户路径，可随意设置，video_rt_usr.c的init函数看进行读�?

#endif

//*********************************************************************************//
//                                  USB配置                                        //
//*********************************************************************************//
#ifndef TCFG_PC_ENABLE
#if TCFG_EQ_ONLINE_ENABLE && defined EQ_CORE_V1
#define TCFG_PC_ENABLE                      1     //使用USB从机功能�?定要打开
#else
#define TCFG_PC_ENABLE                      1     //使用USB从机功能�?定要打开
#endif
#endif
#define USB_PC_NO_APP_MODE                  2
#define USB_MALLOC_ENABLE                   0
#ifndef USB_DEVICE_CLASS_CONFIG
#if TCFG_EQ_ONLINE_ENABLE && defined EQ_CORE_V1
#define USB_DEVICE_CLASS_CONFIG             (CDC_CLASS)
#else
#define USB_DEVICE_CLASS_CONFIG             (HID_CLASS|MASSSTORAGE_CLASS)
#endif
#endif
#ifndef TCFG_UDISK_ENABLE
#define TCFG_UDISK_ENABLE                   0     //U盘主机功�?
#endif
#ifndef TCFG_HOST_AUDIO_ENABLE
#define TCFG_HOST_AUDIO_ENABLE              0     //uac主机功能，用户需要自己补充uac_host_demo.c里面的两个函�?
#endif
#ifndef TCFG_HOST_UVC_ENABLE
#define TCFG_HOST_UVC_ENABLE                0     //打开USB 后拉摄像头功能，�?要使能住机模�?
#endif

#include "usb_std_class_def.h"
#include "usb_common_def.h"

#ifndef TCFG_VIR_UDISK_ENABLE
#define TCFG_VIR_UDISK_ENABLE               0     //虚拟U�?
#endif
#define TCFG_USER_VIRTUAL_PLAY_ENABLE       TCFG_VIR_UDISK_ENABLE
#define TCFG_VIR_UPDATE_ENABLE              0     //虚拟U盘升�?,依赖（TCFG_PC_ENABLE = 1 && TCFG_VIR_UDISK_ENABLE = 1�?
#define TCFG_USER_VIRTUAL_PLAY_SAMPLERATE   44100


//*********************************************************************************//
//                  EXTFLASH配置(截取flash中的�?段空间作为extflash)                //
//*********************************************************************************//
// #define TCFG_EXTFLASH_ENABLE
#ifdef TCFG_EXTFLASH_ENABLE
// #define TCFG_EXTFLASH_UDISK_ENABLE     //将extflash枚举为udisk设备
#endif


//*********************************************************************************//
//                                    FM配置                                       //
//*********************************************************************************//
#define CONFIG_FM_DEV_ENABLE                0        //打开外挂FM模块功能
#define CONFIG_FM_LINEIN_ADC_GAIN           100
#define CONFIG_FM_LINEIN_ADC_CHANNEL        3        //FM音频流回采AD通道
#define TCFG_FM_QN8035_ENABLE               1
#define TCFG_FM_BK1080_ENABLE               0
#define TCFG_FM_RDA5807_ENABLE              0


//*********************************************************************************//
//                                    电源配置                                     //
//*********************************************************************************//
// #define CONFIG_LOW_POWER_ENABLE              		//低功耗开�?
#define TCFG_LOWPOWER_BTOSC_DISABLE			0
#ifdef CONFIG_LOW_POWER_ENABLE
#define TCFG_LOWPOWER_LOWPOWER_SEL			(RF_SLEEP_EN | SYS_SLEEP_EN | RF_FORCE_SYS_SLEEP_EN | SYS_SLEEP_BY_IDLE)
#else
#define TCFG_LOWPOWER_LOWPOWER_SEL			0
#endif
#define TCFG_LOWPOWER_VDDIOM_LEVEL			VDDIOM_VOL_32V       //强VDDIO电压档位，不要高于外部DCDC的电�?
#define TCFG_LOWPOWER_VDDIOW_LEVEL			VDDIOW_VOL_21V       //弱VDDIO电压档位
#define VDC14_VOL_SEL_LEVEL			        VDC14_VOL_SEL_140V   //RF1.4V电压档位
#define SYSVDD_VOL_SEL_LEVEL				SYSVDD_VOL_SEL_126V  //内核电压档位�?


//*********************************************************************************//
//                                  BT_BLE配置                                     //
//*********************************************************************************//
#define CONFIG_BT_ENABLE      // Alke BT(BLE SPP) switch enable disable �?�?
#ifdef CONFIG_BT_ENABLE

#define BT_EMITTER_EN     1
#define BT_RECEIVER_EN    2

#define CONFIG_POWER_ON_ENABLE_EMITTER            1     //�?机自动打�?发射�?
#define CONFIG_POWER_ON_ENABLE_BT                 1     //�?机自动打�?经典蓝牙
#define CONFIG_POWER_ON_ENABLE_BLE                1     //�?机自动打�?BLE
#define TCFG_BD_NUM                               1     //可连接设备数量，可支持同时连�?2台设�?
#define TCFG_USER_BT_CLASSIC_ENABLE               1     //经典蓝牙功能
#define TCFG_USER_TWS_ENABLE                      0     //tws功能使能
#define TCFG_USER_BLE_ENABLE                      1     //BLE功能使能
#define TCFG_USER_EDR_ENABLE                      1     //EDR用户自定义协议功�?
#define TCFG_USER_EMITTER_ENABLE                  1     //蓝牙发射功能
#ifdef CONFIG_LOW_POWER_ENABLE
#define TCFG_BT_SNIFF_ENABLE                      1
#else
#define TCFG_BT_SNIFF_ENABLE                      0
#endif
#define BT_SUPPORT_MUSIC_VOL_SYNC                 0     //音量同步
#define BT_SUPPORT_DISPLAY_BAT                    0     //电池电量同步显示功能
#define BT_SUPPORT_EMITTER_AUTO_A2DP_START        0     //发射器连上后自动打开A2DP�?
#define BT_SUPPORT_EMITTER_PAGE_SCAN              0     //发射器打�?可发现�??

#if TCFG_USER_EDR_ENABLE
#define SPP_TRANS_DATA_EN                         1
#endif

#if defined CONFIG_CPU_WL82 && defined CONFIG_ASR_ALGORITHM && defined CONFIG_VIDEO_ENABLE
#define CONFIG_BT_RX_BUFF_SIZE  ((8 * 1024 + 512) * TCFG_BD_NUM)
#define CONFIG_BT_TX_BUFF_SIZE  (6 * 1024 * TCFG_BD_NUM)
#else
#define CONFIG_BT_RX_BUFF_SIZE  (12 * 1024 * TCFG_BD_NUM)
#define CONFIG_BT_TX_BUFF_SIZE  (6 * 1024 * TCFG_BD_NUM)
#endif


#if TCFG_USER_BLE_ENABLE

#define TCFG_BLE_SECURITY_EN                      0     //配对加密使能



#ifdef CONFIG_NET_ENABLE



#define CONFIG_BLE_MESH_ENABLE                    0

#ifdef CONFIG_DUI_SDK_ENABLE
#define BT_NET_CFG_DUI_EN                         1     //从机 思必驰配网专�?
#else
#define BT_NET_CFG_DUI_EN                         0     //从机 思必驰配网专�?
#endif

#ifdef CONFIG_TURING_SDK_ENABLE
#define BT_NET_CFG_TURING_EN                      1     //从机 图灵配网专用
#else
#define BT_NET_CFG_TURING_EN                      0     //从机 图灵配网专用
#endif

#ifdef CONFIG_TVS_SDK_ENABLE
#define BT_NET_CFG_TENCENT_EN                     1     //从机 腾讯云配网专�?
#else
#define BT_NET_CFG_TENCENT_EN                     0     //从机 腾讯云配网专�?
#endif

#if BT_NET_CFG_TURING_EN + BT_NET_CFG_DUI_EN + BT_NET_CFG_TENCENT_EN > 0
#define BT_NET_CFG_EN                             0     //从机 杰理配网专用
#else
#define BT_NET_CFG_EN                             1     //从机 杰理配网专用
#endif

#define TRANS_DATA_EN                             0     //从机 传输数据

#else

#define BT_NET_CFG_DUI_EN                         0     //从机 思必驰配网专�?
#define BT_NET_CFG_TURING_EN                      0     //从机 图灵配网专用
#define BT_NET_CFG_TENCENT_EN                     0     //从机 腾讯云配网专�?
#define BT_NET_CFG_EN                             0     //从机 配网专用
#define TRANS_DATA_EN                             1     //从机 传输数据

#endif

#define XIAOMI_EN                                 0     //从机 mi_server
#define BT_NET_CENTRAL_EN                         0     //主机 client角色
#define BT_NET_HID_EN                             0     //从机 hid
#define TRANS_MULTI_BLE_EN                        0     //多机通讯

#if (TRANS_MULTI_BLE_EN + BT_NET_CFG_TURING_EN + BT_NET_CFG_DUI_EN + BT_NET_CFG_EN + BT_NET_HID_EN + TRANS_DATA_EN + XIAOMI_EN > 1)
#error "they can not enable at the same time,just select one!!!"
#endif
#endif

#if TRANS_MULTI_BLE_EN
#define TRANS_MULTI_BLE_SLAVE_NUMS                1
#define TRANS_MULTI_BLE_MASTER_NUMS               1
#endif

#endif


//*********************************************************************************//
//                                  UI配置                                         //
//*********************************************************************************//
#ifdef CONFIG_UI_ENABLE

#define CONFIG_VIDEO_DEC_ENABLE             1  //打开视频解码�?
#define TCFG_LCD_ENABLE                     1  //使用lcd屏幕
#define TCFG_USE_SD_ADD_UI_FILE             0  //使用SD卡加载资源文�?

#if TCFG_LCD_ENABLE
#define TCFG_LCD_ILI9481_ENABLE             1
#define TCFG_LCD_ILI9341_ENABLE             0
#define TCFG_LCD_ST7789S_ENABLE             0
#define TCFG_LCD_ST7789V_ENABLE             0
#define TCFG_LCD_ST7735S_ENABLE             0
#define TCFG_LCD_480x272_8BITS              0
#endif

#if TCFG_LCD_ILI9341_ENABLE
#define TCFG_TOUCH_GT911_ENABLE             1
#else
#define TCFG_TOUCH_GT911_ENABLE             0
#endif

#if TCFG_LCD_ILI9481_ENABLE
#define TCFG_TOUCH_FT6236_ENABLE            1
#else
#define TCFG_TOUCH_FT6236_ENABLE            0
#endif

#if TCFG_LCD_480x272_8BITS || TCFG_LCD_ST7789V_ENABLE || TCFG_LCD_ILI9341_ENABLE || TCFG_LCD_ILI9481_ENABLE
#define HORIZONTAL_SCREEN                   0//0为使用竖�?
#else
#define HORIZONTAL_SCREEN                   1//1为使能横屏配�?
#endif

#if TCFG_LCD_ST7789S_ENABLE || TCFG_LCD_ILI9341_ENABLE || TCFG_LCD_ILI9481_ENABLE
#define USE_LCD_TE                          1
#endif

#endif//CONFIG_UI_ENABLE


#ifdef CONFIG_RELEASE_ENABLE
#define LIB_DEBUG    0
#else
#define LIB_DEBUG    1
#endif
#define CONFIG_DEBUG_LIB(x)         (x & LIB_DEBUG)

#include "video_buf_config.h"

#endif

