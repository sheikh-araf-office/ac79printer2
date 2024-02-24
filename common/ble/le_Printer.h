#include "system/app_core.h"

#include "app_config.h"



#include "a2dp_media_codec.h"

#include "event/bt_event.h"

#include "syscfg/syscfg_id.h"

#include "asm/rf_coexistence_config.h"
#include "WeiTingCommon.h"

#define LEMAXBufferReceiveSize			10000
#define LERevBusyLevelValue				5000
#define LERevRefreshLevelValue			1000

extern Device_Rev *LE_RevC;


void PrinterLEStart(void);
void LEPrinterTask(void *p);





