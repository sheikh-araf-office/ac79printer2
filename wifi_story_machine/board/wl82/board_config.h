#ifndef BOARD_CONFIG_H
#define BOARD_CONFIG_H

/*
 *  板级配置选择，需要删去app_config.h中前面跟此头文件重复的宏定义，不然此头文件宏定义无效
 */

#define CONFIG_BOARD_7911B
// #define CONFIG_BOARD_7911BA
// #define CONFIG_BOARD_7911BB
// #define CONFIG_BOARD_7911B0
#define CONFIG_BOARD_7911B8
// #define CONFIG_BOARD_7911D
// #define CONFIG_BOARD_7912D
// #define CONFIG_BOARD_7913A
// #define CONFIG_BOARD_7913A6
// #define CONFIG_BOARD_7915AA
// #define CONFIG_BOARD_7915B6A
// #define CONFIG_BOARD_7916A
// #define CONFIG_BOARD_7916AA
// #define CONFIG_BOARD_7916AB

// #define CONFIG_BOARD_DUI
// #define CONFIG_BOARD_DEMO
// #define CONFIG_BOARD_DEV_KIT
 #define CONFIG_BOARD_DEVELOP

#if defined CONFIG_BOARD_7916AA || defined CONFIG_BOARD_7916AB
#define CONFIG_BOARD_7916A
#endif

#if defined CONFIG_BOARD_7911BA || defined CONFIG_BOARD_7911BB || defined CONFIG_BOARD_7911B0 || defined CONFIG_BOARD_7911B8
#define CONFIG_BOARD_7911B
#endif

#ifdef CONFIG_BOARD_7913A6
#define CONFIG_BOARD_7913A
#define CONFIG_FLASH_FOUR_WIDTH_ENABLE                 //FLASH四线模式
#endif

#if defined CONFIG_BOARD_7915AA || defined CONFIG_BOARD_7915B6A
#define CONFIG_BOARD_7911B
#define CONFIG_ALL_USB_ENABLE
#endif

#ifdef CONFIG_BOARD_7911D
#define __FLASH_SIZE__    (2 * 1024 * 1024)
#define __SDRAM_SIZE__    (0 * 1024 * 1024)

#undef CONFIG_AUDIO_ENABLE
#define TCFG_SD0_ENABLE                     1
#define TCFG_ADKEY_ENABLE                   1          //AD按键
#define CONFIG_PRESS_LONG_KEY_POWERON                  //长按开关机功能

#define TCFG_DEBUG_PORT                     IO_PORTH_03

#define TCFG_SD_PORTS                       'B'			//SD0/SD1的ABCD组(默认为开发板SD0-D),注意:IO占用问题
#define TCFG_SD_DAT_WIDTH                   4			//1:单线模式, 4:四线模式
#define TCFG_SD_DET_MODE                    SD_CLK_DECT	//检测模式:命令检测，时钟检测，IO检测
#define TCFG_SD_DET_IO                      IO_PORTB_08	//SD_DET_MODE为SD_IO_DECT时有效
#define TCFG_SD_DET_IO_LEVEL                0			//IO检卡上线的电平(0/1),SD_DET_MODE为SD_IO_DECT时有效
#define TCFG_SD_CLK                         24000000	//SD时钟

//*********************************************************************************//
//                            AUDIO_ADC应用的通道配置                              //
//*********************************************************************************//
#define CONFIG_AUDIO_ENC_SAMPLE_SOURCE      AUDIO_ENC_SAMPLE_SOURCE_MIC
#endif


#ifdef CONFIG_BOARD_7912D
#define __FLASH_SIZE__    (2 * 1024 * 1024)
#define __SDRAM_SIZE__    (0 * 1024 * 1024)

#undef CONFIG_AUDIO_ENABLE
#define TCFG_SD0_ENABLE                     1
#define TCFG_ADKEY_ENABLE                   1          //AD按键
#define CONFIG_PRESS_LONG_KEY_POWERON                  //长按开关机功能

#define TCFG_DEBUG_PORT                     IO_PORTA_03

#define TCFG_SD_PORTS                       'B'			//SD0/SD1的ABCD组(默认为开发板SD0-D),注意:IO占用问题
#define TCFG_SD_DAT_WIDTH                   1			//1:单线模式, 4:四线模式
#define TCFG_SD_DET_MODE                    SD_CLK_DECT	//检测模式:命令检测，时钟检测，IO检测
#define TCFG_SD_DET_IO                      IO_PORTB_08	//SD_DET_MODE为SD_IO_DECT时有效
#define TCFG_SD_DET_IO_LEVEL                0			//IO检卡上线的电平(0/1),SD_DET_MODE为SD_IO_DECT时有效
#define TCFG_SD_CLK                         24000000	//SD时钟

//*********************************************************************************//
//                            AUDIO_ADC应用的通道配置                              //
//*********************************************************************************//
#define CONFIG_AUDIO_ENC_SAMPLE_SOURCE      AUDIO_ENC_SAMPLE_SOURCE_MIC
#endif


#ifdef CONFIG_BOARD_7913A
#undef CONFIG_VIDEO_ENABLE
#undef CONFIG_RTOS_AND_MM_LIB_CODE_SECTION_IN_SDRAM
#if defined CONFIG_BOARD_7913A6
#define __FLASH_SIZE__    (2 * 1024 * 1024)
#else
#define __FLASH_SIZE__    (2 * 1024 * 1024)
#endif
#define __SDRAM_SIZE__    (0 * 1024 * 1024)

#define TCFG_SD0_ENABLE                     1
// #define TCFG_SD1_ENABLE                     1
#define TCFG_ADKEY_ENABLE                   1          //AD按键
#define CONFIG_PRESS_LONG_KEY_POWERON                  //长按开关机功能

#define TCFG_DEBUG_PORT                     IO_PORTC_00
#define TCFG_DAC_MUTE_PORT                  0xff
#define TCFG_DAC_MUTE_VALUE                 0

#define TCFG_SD_PORTS                       'D'			//SD0/SD1的ABCD组(默认为开发板SD0-D),注意:IO占用问题
#define TCFG_SD_DAT_WIDTH                   1			//1:单线模式, 4:四线模式
#define TCFG_SD_DET_MODE                    SD_CLK_DECT	//检测模式:命令检测，时钟检测，IO检测
#define TCFG_SD_DET_IO                      IO_PORTB_08	//SD_DET_MODE为SD_IO_DECT时有效
#define TCFG_SD_DET_IO_LEVEL                0			//IO检卡上线的电平(0/1),SD_DET_MODE为SD_IO_DECT时有效
#define TCFG_SD_CLK                         24000000	//SD时钟

//*********************************************************************************//
//                            AUDIO_ADC应用的通道配置                              //
//*********************************************************************************//
#define CONFIG_AUDIO_ENC_SAMPLE_SOURCE      AUDIO_ENC_SAMPLE_SOURCE_MIC
// #define CONFIG_AUDIO_ENC_SAMPLE_SOURCE      AUDIO_ENC_SAMPLE_SOURCE_PLNK0
#if CONFIG_AUDIO_ENC_SAMPLE_SOURCE == AUDIO_ENC_SAMPLE_SOURCE_PLNK0
#define TCFG_MIC_CHANNEL_MAP                LADC_CH_MIC0_P_N
#define TCFG_MIC_CHANNEL_NUM                1
#else
#define TCFG_MIC_CHANNEL_MAP                (LADC_CH_MIC0_P_N | LADC_CH_MIC1_P_N)
#define TCFG_MIC_CHANNEL_NUM                2
#endif
#define TCFG_LINEIN_CHANNEL_MAP             LADC_CH_AUX2
#define TCFG_LINEIN_CHANNEL_NUM             1

#define CONFIG_AISP_DIFFER_MIC_REPLACE_LINEIN       //用差分mic代替aec回采
#define CONFIG_ASR_CLOUD_ADC_CHANNEL        1		//云端识别mic通道
#define CONFIG_VOICE_NET_CFG_ADC_CHANNEL    1		//声波配网mic通道
#define CONFIG_AISP_MIC0_ADC_CHANNEL        1		//本地唤醒左mic通道
#define CONFIG_AISP_MIC_ADC_GAIN            80		//本地唤醒mic增益
#define CONFIG_AISP_LINEIN_ADC_CHANNEL      0		//本地唤醒LINEIN回采DAC通道
#define CONFIG_AISP_MIC1_ADC_CHANNEL        0		//本地唤醒右mic通道
#define CONFIG_REVERB_ADC_CHANNEL           1		//混响mic通道
#define CONFIG_PHONE_CALL_ADC_CHANNEL       1		//通话mic通道
#define CONFIG_UAC_MIC_ADC_CHANNEL          1		//UAC mic通道
#define CONFIG_AISP_LINEIN_ADC_GAIN         10		//本地唤醒LINEIN增益
#define CONFIG_AUDIO_LINEIN_CHANNEL         1		//LIENIN通道数
#define CONFIG_AUDIO_LINEIN_CHANNEL_MAP     TCFG_LINEIN_CHANNEL_MAP
#endif


#ifdef CONFIG_BOARD_7916A
#define __FLASH_SIZE__    (8 * 1024 * 1024)
#if defined CONFIG_BOARD_7916AA
#define __SDRAM_SIZE__    (2 * 1024 * 1024)
#else
#define __SDRAM_SIZE__    (8 * 1024 * 1024)
#endif

#ifdef CONFIG_BOARD_DEMO	//测试DEMO板
#define TCFG_SD0_ENABLE                     1
#define TCFG_ADKEY_ENABLE                   1           //AD按键
#define CONFIG_PRESS_LONG_KEY_POWERON                   //长按开关机功能
#define CONFIG_CAMERA_H_V_EXCHANGE          1

#define TCFG_DEBUG_PORT                     IO_PORTB_08
#define TCFG_DAC_MUTE_PORT                  0xff
#define TCFG_DAC_MUTE_VALUE                 0

#define TCFG_CAMERA_XCLK_PORT               0xff
#define TCFG_CAMERA_RESET_PORT              0xff

#define TCFG_SW_IIC_CLK_PORT                IO_PORTH_00
#define TCFG_SW_IIC_DAT_PORT                IO_PORTH_01

#define TCFG_SD_PORTS                       'A'			//SD0/SD1的ABCD组(默认为开发板SD0-D),注意:IO占用问题
#define TCFG_SD_DAT_WIDTH                   4			//1:单线模式, 4:四线模式
#define TCFG_SD_DET_MODE                    SD_CLK_DECT	//检测模式:命令检测，时钟检测，IO检测
#define TCFG_SD_DET_IO                      IO_PORTB_08	//SD_DET_MODE为SD_IO_DECT时有效
#define TCFG_SD_DET_IO_LEVEL                0			//IO检卡上线的电平(0/1),SD_DET_MODE为SD_IO_DECT时有效
#define TCFG_SD_CLK                         24000000	//SD时钟

//*********************************************************************************//
//                            AUDIO_ADC应用的通道配置                              //
//*********************************************************************************//
#define CONFIG_AUDIO_ENC_SAMPLE_SOURCE      AUDIO_ENC_SAMPLE_SOURCE_MIC
// #define CONFIG_AUDIO_ENC_SAMPLE_SOURCE      AUDIO_ENC_SAMPLE_SOURCE_PLNK0
#if CONFIG_AUDIO_ENC_SAMPLE_SOURCE == AUDIO_ENC_SAMPLE_SOURCE_PLNK0
#define TCFG_MIC_CHANNEL_MAP                LADC_CH_MIC3_P_N
#define TCFG_MIC_CHANNEL_NUM                1
#else
#define TCFG_MIC_CHANNEL_MAP                (LADC_CH_MIC0_P_N | LADC_CH_MIC1_P_N | LADC_CH_MIC3_P_N)
#define TCFG_MIC_CHANNEL_NUM                3
#endif
#define TCFG_LINEIN_CHANNEL_MAP             (LADC_CH_AUX1 | LADC_CH_AUX3)
#define TCFG_LINEIN_CHANNEL_NUM             2

#define CONFIG_AISP_DIFFER_MIC_REPLACE_LINEIN       //用差分mic代替aec回采
#define CONFIG_ASR_CLOUD_ADC_CHANNEL        1		//云端识别mic通道
#define CONFIG_VOICE_NET_CFG_ADC_CHANNEL    1		//声波配网mic通道
#define CONFIG_AISP_MIC0_ADC_CHANNEL        1		//本地唤醒左mic通道
#define CONFIG_AISP_MIC_ADC_GAIN            80		//本地唤醒mic增益
#define CONFIG_AISP_LINEIN_ADC_CHANNEL      3		//本地唤醒LINEIN回采DAC通道
#define CONFIG_AISP_MIC1_ADC_CHANNEL        0		//本地唤醒右mic通道
#define CONFIG_REVERB_ADC_CHANNEL           1		//混响mic通道
#define CONFIG_PHONE_CALL_ADC_CHANNEL       1		//通话mic通道
#define CONFIG_UAC_MIC_ADC_CHANNEL          1		//UAC mic通道
#define CONFIG_AISP_LINEIN_ADC_GAIN         10		//本地唤醒LINEIN增益
#define CONFIG_AUDIO_LINEIN_CHANNEL         2		//LIENIN通道数
#define CONFIG_AUDIO_LINEIN_CHANNEL_MAP     TCFG_LINEIN_CHANNEL_MAP
#endif

#ifdef CONFIG_BOARD_DEV_KIT		//开源学习开发板
#define TCFG_SD0_ENABLE                     1
#define TCFG_ADKEY_ENABLE                   1           //AD按键
#define CONFIG_PRESS_LONG_KEY_POWERON                   //长按开关机功能
#define CONFIG_FLASH_FOUR_WIDTH_ENABLE                  //FLASH四线模式
// #define CONFIG_OVERCLOCKING_ENABLE                      //超频模式，暂时不能量产，仅供测试
#define CONFIG_CAMERA_H_V_EXCHANGE          1
// #define CONFIG_VIDEO_720P
#ifndef CONFIG_VIDEO_720P
#define CONFIG_VIDEO_YUV_PPBUF_ENABLE
#endif
#define USER_ISC_STATIC_BUFF_ENABLE         1

#define TCFG_DEBUG_PORT                     IO_PORTB_03
#define TCFG_DAC_MUTE_PORT                  IO_PORTB_02
#define TCFG_DAC_MUTE_VALUE                 0
#define TCFG_DAC_MUTE_ENABLE                1

#define TCFG_CAMERA_XCLK_PORT               IO_PORTH_02
#define TCFG_CAMERA_RESET_PORT              IO_PORTH_03

#define TCFG_SW_IIC_CLK_PORT                IO_PORTH_00
#define TCFG_SW_IIC_DAT_PORT                IO_PORTH_01

#define TCFG_SD_PORTS                       'A'			//SD0/SD1的ABCD组(默认为开发板SD0-D),注意:IO占用问题
#define TCFG_SD_DAT_WIDTH                   1			//1:单线模式, 4:四线模式
#define TCFG_SD_DET_MODE                    SD_CLK_DECT	//检测模式:命令检测，时钟检测，IO检测
#define TCFG_SD_DET_IO                      IO_PORTB_08	//SD_DET_MODE为SD_IO_DECT时有效
#define TCFG_SD_DET_IO_LEVEL                0			//IO检卡上线的电平(0/1),SD_DET_MODE为SD_IO_DECT时有效
#if defined CONFIG_OVERCLOCKING_ENABLE
#define TCFG_SD_CLK                         33000000	//SD时钟
#else
#define TCFG_SD_CLK                         24000000	//SD时钟
#endif

//*********************************************************************************//
//                            AUDIO_ADC应用的通道配置                              //
//*********************************************************************************//
#define CONFIG_AUDIO_ENC_SAMPLE_SOURCE      AUDIO_ENC_SAMPLE_SOURCE_MIC
// #define CONFIG_AUDIO_ENC_SAMPLE_SOURCE      AUDIO_ENC_SAMPLE_SOURCE_PLNK0
#if CONFIG_AUDIO_ENC_SAMPLE_SOURCE == AUDIO_ENC_SAMPLE_SOURCE_PLNK0
#define TCFG_MIC_CHANNEL_MAP                LADC_CH_MIC3_P_N
#define TCFG_MIC_CHANNEL_NUM                1
#else
#define TCFG_MIC_CHANNEL_MAP                (LADC_CH_MIC1_P_N | LADC_CH_MIC3_P_N)
#define TCFG_MIC_CHANNEL_NUM                2
#endif
#define TCFG_LINEIN_CHANNEL_MAP             (LADC_CH_AUX1 | LADC_CH_AUX3)
#define TCFG_LINEIN_CHANNEL_NUM             2

#define CONFIG_AISP_DIFFER_MIC_REPLACE_LINEIN       //用差分mic代替aec回采
#define CONFIG_ASR_CLOUD_ADC_CHANNEL        1		//云端识别mic通道
#define CONFIG_VOICE_NET_CFG_ADC_CHANNEL    1		//声波配网mic通道
#define CONFIG_AISP_MIC0_ADC_CHANNEL        1		//本地唤醒左mic通道
#define CONFIG_AISP_MIC_ADC_GAIN            100		//本地唤醒mic增益
#define CONFIG_AISP_LINEIN_ADC_CHANNEL      3		//本地唤醒LINEIN回采DAC通道
#define CONFIG_AISP_MIC1_ADC_CHANNEL        0		//本地唤醒右mic通道
#define CONFIG_REVERB_ADC_CHANNEL           1		//混响mic通道
#define CONFIG_PHONE_CALL_ADC_CHANNEL       1		//通话mic通道
#define CONFIG_UAC_MIC_ADC_CHANNEL          1		//UAC mic通道
#define CONFIG_AISP_LINEIN_ADC_GAIN         10		//本地唤醒LINEIN增益
#define CONFIG_AUDIO_LINEIN_CHANNEL         2		//LIENIN通道数
#define CONFIG_AUDIO_LINEIN_CHANNEL_MAP     TCFG_LINEIN_CHANNEL_MAP

//*********************************************************************************//
//                                        USB配置                                  //
//*********************************************************************************//
#define TCFG_PC_ENABLE                      1     //使用USB从机功能一定要打开
#define USB_DEVICE_CLASS_CONFIG             (UVC_CLASS | SPEAKER_CLASS | CDC_CLASS)
#define USB_DEVICE_CLASS_CONFIG_2_0         (MASSSTORAGE_CLASS | UVC_CLASS | HID_CLASS | AUDIO_CLASS)
#define TCFG_UDISK_ENABLE                   1     //U盘主机功能
#define TCFG_HOST_AUDIO_ENABLE              1     //uac主机功能，用户需要自己补充uac_host_demo.c里面的两个函数
#if defined CONFIG_BOARD_7916AA
#define TCFG_HOST_UVC_ENABLE                0     //UVC主机功能
#else
#define TCFG_HOST_UVC_ENABLE                1     //UVC主机功能
#endif
#define TCFG_HID_HOST_ENABLE                1     //HID主机功能
#define TCFG_VIR_UDISK_ENABLE               0     //虚拟U盘
#if TCFG_HOST_UVC_ENABLE
#define CONFIG_UI_ENABLE
#define CONFIG_UVC_VIDEO2_ENABLE
#define CONFIG_VIDEO_DISPLAY_ENABLE
#endif
#endif

#ifdef CONFIG_BOARD_DEVELOP
#define TCFG_SD0_ENABLE                     1
#define TCFG_ADKEY_ENABLE                   1          //AD按键
#define CONFIG_PRESS_LONG_KEY_POWERON                  //长按开关机功能

#define TCFG_DEBUG_PORT                     IO_PORTC_06
#define TCFG_DAC_MUTE_PORT                  0xff
#define TCFG_DAC_MUTE_VALUE                 0
#define TCFG_DAC_MUTE_ENABLE                1

#define TCFG_CAMERA_XCLK_PORT               IO_PORTC_00
#define TCFG_CAMERA_RESET_PORT              -1

#define TCFG_SW_IIC_CLK_PORT                IO_PORTC_01
#define TCFG_SW_IIC_DAT_PORT                IO_PORTC_02

#define TCFG_SD_PORTS                       'D'			//SD0/SD1的ABCD组(默认为开发板SD0-D),注意:IO占用问题
#define TCFG_SD_DAT_WIDTH                   1			//1:单线模式, 4:四线模式
#define TCFG_SD_DET_MODE                    SD_CLK_DECT	//检测模式:命令检测，时钟检测，IO检测
#define TCFG_SD_DET_IO                      IO_PORTA_01	//SD_DET_MODE为SD_IO_DECT时有效
#define TCFG_SD_DET_IO_LEVEL                0			//IO检卡上线的电平(0/1),SD_DET_MODE为SD_IO_DECT时有效
#define TCFG_SD_CLK                         24000000	//SDIO时钟

//*********************************************************************************//
//                            AUDIO_ADC应用的通道配置                              //
//*********************************************************************************//
#define CONFIG_AUDIO_ENC_SAMPLE_SOURCE      AUDIO_ENC_SAMPLE_SOURCE_MIC
#define TCFG_MIC_CHANNEL_MAP                LADC_CH_MIC1_P_N
#define TCFG_MIC_CHANNEL_NUM                1
#define TCFG_LINEIN_CHANNEL_MAP             LADC_CH_AUX3
#define TCFG_LINEIN_CHANNEL_NUM             1

#define CONFIG_ASR_CLOUD_ADC_CHANNEL        1		//云端识别mic通道
#define CONFIG_VOICE_NET_CFG_ADC_CHANNEL    1		//声波配网mic通道
#define CONFIG_AISP_MIC0_ADC_CHANNEL        1		//本地唤醒左mic通道
#define CONFIG_AISP_MIC1_ADC_CHANNEL        3		//本地唤醒右mic通道
#define CONFIG_REVERB_ADC_CHANNEL           1		//混响mic通道
#define CONFIG_PHONE_CALL_ADC_CHANNEL       1		//通话mic通道
#define CONFIG_UAC_MIC_ADC_CHANNEL          1		//UAC mic通道
#define CONFIG_AISP_LINEIN_ADC_CHANNEL      3		//本地唤醒LINEIN回采DAC通道
#define CONFIG_AISP_MIC_ADC_GAIN            80		//本地唤醒mic增益
#define CONFIG_AISP_LINEIN_ADC_GAIN         60		//本地唤醒LINEIN增益
#define CONFIG_AUDIO_LINEIN_CHANNEL         1		//LIENIN通道数
#define CONFIG_AUDIO_LINEIN_CHANNEL_MAP     TCFG_LINEIN_CHANNEL_MAP
#endif

#endif

#ifdef CONFIG_BOARD_7911B
#if defined CONFIG_BOARD_7911B8
#undef CONFIG_AUDIO_ENABLE
#define __FLASH_SIZE__    (1 * 1024 * 1024)
#define CONFIG_FLASH_FOUR_WIDTH_ENABLE                 //FLASH四线模式
#elif defined CONFIG_BOARD_7915B6A
#undef CONFIG_AUDIO_ENABLE
#define __FLASH_SIZE__    (2 * 1024 * 1024)
#define CONFIG_FLASH_FOUR_WIDTH_ENABLE                 //FLASH四线模式
#else
#define __FLASH_SIZE__    (8 * 1024 * 1024)
#endif
#if defined CONFIG_BOARD_7911B0 || defined CONFIG_BOARD_7911B8
#define __SDRAM_SIZE__    (0 * 1024 * 1024)
#elif defined CONFIG_BOARD_7911BB
#define __SDRAM_SIZE__    (8 * 1024 * 1024)
#else
#define __SDRAM_SIZE__    (8 * 1024 * 1024)
#endif

#ifdef CONFIG_BOARD_DEVELOP
#define TCFG_SD0_ENABLE                     0
#define TCFG_ADKEY_ENABLE                   0          //AD按键
//#define CONFIG_PRESS_LONG_KEY_POWERON                  //长按开关机功能

//#define TCFG_DEBUG_PORT                     IO_PORTH_07//H08
#define TCFG_DEBUG_PORT                     IO_PORTH_04//H06
#define TCFG_DAC_MUTE_PORT                  0xff
#define TCFG_DAC_MUTE_VALUE                 0
#define TCFG_DAC_MUTE_ENABLE                0

#define TCFG_SD_PORTS                       'D'			//SD0/SD1的ABCD组(默认为开发板SD0-D),注意:IO占用问题
#define TCFG_SD_DAT_WIDTH                   0			//1:单线模式, 4:四线模式
#define TCFG_SD_DET_MODE                    SD_CLK_DECT	//检测模式:命令检测，时钟检测，IO检测
#define TCFG_SD_DET_IO                      IO_PORTA_01	//SD_DET_MODE为SD_IO_DECT时有效
#define TCFG_SD_DET_IO_LEVEL                0			//IO检卡上线的电平(0/1),SD_DET_MODE为SD_IO_DECT时有效
#define TCFG_SD_CLK                         24000000	//SDIO时钟

//*********************************************************************************//
//                            AUDIO_ADC应用的通道配置                              //
//*********************************************************************************//
#define CONFIG_AUDIO_ENC_SAMPLE_SOURCE      AUDIO_ENC_SAMPLE_SOURCE_MIC
#define TCFG_MIC_CHANNEL_MAP                LADC_CH_MIC1_P_N
#define TCFG_MIC_CHANNEL_NUM                1
#define TCFG_LINEIN_CHANNEL_MAP             LADC_CH_AUX3
#define TCFG_LINEIN_CHANNEL_NUM             1

#define CONFIG_ASR_CLOUD_ADC_CHANNEL        1		//云端识别mic通道
#define CONFIG_VOICE_NET_CFG_ADC_CHANNEL    1		//声波配网mic通道
#define CONFIG_AISP_MIC0_ADC_CHANNEL        1		//本地唤醒左mic通道
#define CONFIG_AISP_MIC1_ADC_CHANNEL        3		//本地唤醒右mic通道
#define CONFIG_REVERB_ADC_CHANNEL           1		//混响mic通道
#define CONFIG_PHONE_CALL_ADC_CHANNEL       1		//通话mic通道
#define CONFIG_UAC_MIC_ADC_CHANNEL          1		//UAC mic通道
#define CONFIG_AISP_LINEIN_ADC_CHANNEL      3		//本地唤醒LINEIN回采DAC通道
#define CONFIG_AISP_MIC_ADC_GAIN            80		//本地唤醒mic增益
#define CONFIG_AISP_LINEIN_ADC_GAIN         60		//本地唤醒LINEIN增益
#define CONFIG_AUDIO_LINEIN_CHANNEL         1		//LIENIN通道数
#define CONFIG_AUDIO_LINEIN_CHANNEL_MAP     TCFG_LINEIN_CHANNEL_MAP
#endif

#ifdef CONFIG_BOARD_DUI
#undef CONFIG_VIDEO_ENABLE
#undef CONFIG_RTOS_AND_MM_LIB_CODE_SECTION_IN_SDRAM
#define TCFG_SD0_ENABLE                     1
#define TCFG_IOKEY_ENABLE                   1           //IO按键

#define TCFG_DEBUG_PORT                     IO_PORTA_06
#define TCFG_DAC_MUTE_PORT                  IO_PORTA_10
#define TCFG_DAC_MUTE_VALUE                 0
#define PORT_VCC33_CTRL_IO                  IO_PORTA_09

#define TCFG_SD_POWER_ENABLE                1
#define TCFG_SD_PORTS                       'D'			//SD0/SD1的ABCD组(默认为开发板SD0-D),注意:IO占用问题
#define TCFG_SD_DAT_WIDTH                   1			//1:单线模式, 4:四线模式
#define TCFG_SD_DET_MODE                    SD_CMD_DECT	//检测模式:命令检测，时钟检测，IO检测
#define TCFG_SD_DET_IO                      IO_PORTA_01	//SD_DET_MODE为SD_IO_DECT时有效
#define TCFG_SD_DET_IO_LEVEL                0			//IO检卡上线的电平(0/1),SD_DET_MODE为SD_IO_DECT时有效
#define TCFG_SD_CLK                         24000000	//SDIO时钟

//*********************************************************************************//
//                            AUDIO_ADC应用的通道配置                              //
//*********************************************************************************//
#define CONFIG_AUDIO_ENC_SAMPLE_SOURCE      AUDIO_ENC_SAMPLE_SOURCE_MIC
// #define CONFIG_AUDIO_ENC_SAMPLE_SOURCE      AUDIO_ENC_SAMPLE_SOURCE_PLNK0
#if CONFIG_AUDIO_ENC_SAMPLE_SOURCE == AUDIO_ENC_SAMPLE_SOURCE_PLNK0
#define TCFG_MIC_CHANNEL_MAP                LADC_CH_MIC2_P_N
#define TCFG_MIC_CHANNEL_NUM                1
#else
#define TCFG_MIC_CHANNEL_MAP                (LADC_CH_MIC0_P_N | LADC_CH_MIC1_P_N | LADC_CH_MIC2_P_N)
#define TCFG_MIC_CHANNEL_NUM                3
#endif
#define TCFG_LINEIN_CHANNEL_MAP             0
#define TCFG_LINEIN_CHANNEL_NUM             0

#define CONFIG_AISP_DIFFER_MIC_REPLACE_LINEIN       //用差分mic代替aec回采
#define CONFIG_ASR_CLOUD_ADC_CHANNEL        0		//云端识别mic通道
#define CONFIG_VOICE_NET_CFG_ADC_CHANNEL    0		//声波配网mic通道
#define CONFIG_AISP_MIC0_ADC_CHANNEL        0		//本地唤醒左mic通道
#define CONFIG_AISP_MIC_ADC_GAIN            100		//本地唤醒mic增益
#define CONFIG_AISP_LINEIN_ADC_CHANNEL      2		//本地唤醒LINEIN回采DAC通道
#define CONFIG_AISP_MIC1_ADC_CHANNEL        1		//本地唤醒右mic通道
#define CONFIG_REVERB_ADC_CHANNEL           0		//混响mic通道
#define CONFIG_PHONE_CALL_ADC_CHANNEL       0		//通话mic通道
#define CONFIG_UAC_MIC_ADC_CHANNEL          0		//UAC mic通道
#define CONFIG_AISP_LINEIN_ADC_GAIN         10		//本地唤醒LINEIN增益

//#define CONFIG_DUI_SDK_ENABLE	            //使用思必驰DUI平台
#define CONFIG_ASR_ALGORITHM  AISP_ALGORITHM    //本地打断唤醒算法选择
#endif

#endif

//*********************************************************************************//
//                        SD 配置（暂只支持打开一个SD外设）                        //
//*********************************************************************************//
//SD0 	cmd,  clk,  data0, data1, data2, data3
//A     PB6   PB7   PB5    PB5    PB3    PB2
//B     PA7   PA8   PA9    PA10   PA5    PA6
//C     PH1   PH2   PH0    PH3    PH4    PH5
//D     PC9   PC10  PC8    PC7    PC6    PC5

//SD1 	cmd,  clk,  data0, data1, data2, data3
//A     PH6   PH7   PH5    PH4    PH3    PH2
//B     PC0   PC1   PC2    PC3    PC4    PC5

#endif
