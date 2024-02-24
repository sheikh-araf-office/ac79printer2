#include "system/app_core.h"

#include "app_config.h"



#include "a2dp_media_codec.h"

#include "event/bt_event.h"

#include "syscfg/syscfg_id.h"

#include "asm/rf_coexistence_config.h"
#include "WeiTingCommon.h"

#define BTMAXBufferReceiveSize			30000
#define BTRevBusyLevelValue				15000
#define BTRevRefreshLevelValue			20000

extern Device_Rev *BT_RevC;


void PrinterBTStart(void);

void PrinterInstructionSet(unsigned char);

void BTPrinterTask(void *p);





