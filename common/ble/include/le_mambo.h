// binary representation
// attribute size in bytes (16), flags(16), handle (16), uuid (16/128), value(...)

#ifndef _LE_TRANS_DATA_H
#define _LE_TRANS_DATA_H

#include <stdint.h>
// #include <le_user.h>
//
// gatt profile include file, generated by jieli gatt_inc_generator.exe
//


static const uint8_t profile_data[] = {
    //////////////////////////////////////////////////////
    //
    // 0x0001 PRIMARY_SERVICE  1800
    //
    //////////////////////////////////////////////////////
    0x0a, 0x00, 0x02, 0x00, 0x01, 0x00, 0x00, 0x28, 0x00, 0x18,

    /* CHARACTERISTIC,  2a00, READ | DYNAMIC, */
    // 0x0002 CHARACTERISTIC 2a00 READ | DYNAMIC
    0x0d, 0x00, 0x02, 0x00, 0x02, 0x00, 0x03, 0x28, 0x02, 0x03, 0x00, 0x00, 0x2a,
    // 0x0003 VALUE 2a00 READ | DYNAMIC
    0x08, 0x00, 0x02, 0x01, 0x03, 0x00, 0x00, 0x2a,

    /* CHARACTERISTIC,  2a01, READ | DYNAMIC, */
    // 0x0004 CHARACTERISTIC 2a01 READ | DYNAMIC
    0x0d, 0x00, 0x02, 0x00, 0x04, 0x00, 0x03, 0x28, 0x02, 0x05, 0x00, 0x01, 0x2a,
    // 0x0005 VALUE 2a01 READ | DYNAMIC
    0x08, 0x00, 0x02, 0x01, 0x05, 0x00, 0x01, 0x2a,

    //////////////////////////////////////////////////////
    //
    // 0x0006 PRIMARY_SERVICE  1801
    //
    //////////////////////////////////////////////////////
    0x0a, 0x00, 0x02, 0x00, 0x06, 0x00, 0x00, 0x28, 0x01, 0x18,

    /* CHARACTERISTIC,  2a05, INDICATE, */
    // 0x0007 CHARACTERISTIC 2a05 INDICATE
    0x0d, 0x00, 0x02, 0x00, 0x07, 0x00, 0x03, 0x28, 0x20, 0x08, 0x00, 0x05, 0x2a,
    // 0x0008 VALUE 2a05 INDICATE
    0x08, 0x00, 0x20, 0x00, 0x08, 0x00, 0x05, 0x2a,
    // 0x0009 CLIENT_CHARACTERISTIC_CONFIGURATION
    0x0a, 0x00, 0x0a, 0x01, 0x09, 0x00, 0x02, 0x29, 0x00, 0x00,

    //////////////////////////////////////////////////////
    //
    // 0x000a PRIMARY_SERVICE  180a
    //
    //////////////////////////////////////////////////////
    0x0a, 0x00, 0x02, 0x00, 0x0a, 0x00, 0x00, 0x28, 0x0a, 0x18,

    /* CHARACTERISTIC,  2a24, READ | DYNAMIC, */
    // 0x000b CHARACTERISTIC 2a24 READ | DYNAMIC
    0x0d, 0x00, 0x02, 0x00, 0x0b, 0x00, 0x03, 0x28, 0x02, 0x0c, 0x00, 0x24, 0x2a,
    // 0x000c VALUE 2a24 READ | DYNAMIC
    0x08, 0x00, 0x02, 0x01, 0x0c, 0x00, 0x24, 0x2a,

    /* CHARACTERISTIC,  2a25, READ | DYNAMIC, */
    // 0x000d CHARACTERISTIC 2a25 READ | DYNAMIC
    0x0d, 0x00, 0x02, 0x00, 0x0d, 0x00, 0x03, 0x28, 0x02, 0x0e, 0x00, 0x25, 0x2a,
    // 0x000e VALUE 2a25 READ | DYNAMIC
    0x08, 0x00, 0x02, 0x01, 0x0e, 0x00, 0x25, 0x2a,

    /* CHARACTERISTIC,  2a26, READ | DYNAMIC, */
    // 0x000f CHARACTERISTIC 2a26 READ | DYNAMIC
    0x0d, 0x00, 0x02, 0x00, 0x0f, 0x00, 0x03, 0x28, 0x02, 0x10, 0x00, 0x26, 0x2a,
    // 0x0010 VALUE 2a26 READ | DYNAMIC
    0x08, 0x00, 0x02, 0x01, 0x10, 0x00, 0x26, 0x2a,

    /* CHARACTERISTIC,  2a27, READ | DYNAMIC, */
    // 0x0011 CHARACTERISTIC 2a27 READ | DYNAMIC
    0x0d, 0x00, 0x02, 0x00, 0x11, 0x00, 0x03, 0x28, 0x02, 0x12, 0x00, 0x27, 0x2a,
    // 0x0012 VALUE 2a27 READ | DYNAMIC
    0x08, 0x00, 0x02, 0x01, 0x12, 0x00, 0x27, 0x2a,

    /* CHARACTERISTIC,  2a28, READ | DYNAMIC, */
    // 0x0013 CHARACTERISTIC 2a28 READ | DYNAMIC
    0x0d, 0x00, 0x02, 0x00, 0x13, 0x00, 0x03, 0x28, 0x02, 0x14, 0x00, 0x28, 0x2a,
    // 0x0014 VALUE 2a28 READ | DYNAMIC
    0x08, 0x00, 0x02, 0x01, 0x14, 0x00, 0x28, 0x2a,

    /* CHARACTERISTIC,  2a29, READ | DYNAMIC, */
    // 0x0015 CHARACTERISTIC 2a29 READ | DYNAMIC
    0x0d, 0x00, 0x02, 0x00, 0x15, 0x00, 0x03, 0x28, 0x02, 0x16, 0x00, 0x29, 0x2a,
    // 0x0016 VALUE 2a29 READ | DYNAMIC
    0x08, 0x00, 0x02, 0x01, 0x16, 0x00, 0x29, 0x2a,

    //////////////////////////////////////////////////////
    //
    // 0x0017 PRIMARY_SERVICE  a602
    //
    //////////////////////////////////////////////////////
    0x0a, 0x00, 0x02, 0x00, 0x17, 0x00, 0x00, 0x28, 0x02, 0xa6,

    /* CHARACTERISTIC,  a620, INDICATE, */
    // 0x0018 CHARACTERISTIC a620 INDICATE
    0x0d, 0x00, 0x02, 0x00, 0x18, 0x00, 0x03, 0x28, 0x20, 0x19, 0x00, 0x20, 0xa6,
    // 0x0019 VALUE a620 INDICATE
    0x08, 0x00, 0x20, 0x00, 0x19, 0x00, 0x20, 0xa6,
    // 0x001a CLIENT_CHARACTERISTIC_CONFIGURATION
    0x0a, 0x00, 0x0a, 0x01, 0x1a, 0x00, 0x02, 0x29, 0x00, 0x00,

    /* CHARACTERISTIC,  a621, NOTIFY, */
    // 0x001b CHARACTERISTIC a621 NOTIFY
    0x0d, 0x00, 0x02, 0x00, 0x1b, 0x00, 0x03, 0x28, 0x10, 0x1c, 0x00, 0x21, 0xa6,
    // 0x001c VALUE a621 NOTIFY
    0x08, 0x00, 0x10, 0x00, 0x1c, 0x00, 0x21, 0xa6,
    // 0x001d CLIENT_CHARACTERISTIC_CONFIGURATION
    0x0a, 0x00, 0x0a, 0x01, 0x1d, 0x00, 0x02, 0x29, 0x00, 0x00,

    /* CHARACTERISTIC,  a622, WRITE_WITHOUT_RESPONSE | DYNAMIC, */
    // 0x001e CHARACTERISTIC a622 WRITE_WITHOUT_RESPONSE | DYNAMIC
    0x0d, 0x00, 0x02, 0x00, 0x1e, 0x00, 0x03, 0x28, 0x04, 0x1f, 0x00, 0x22, 0xa6,
    // 0x001f VALUE a622 WRITE_WITHOUT_RESPONSE | DYNAMIC
    0x08, 0x00, 0x04, 0x01, 0x1f, 0x00, 0x22, 0xa6,

    /* CHARACTERISTIC,  a623, WRITE | DYNAMIC, */
    // 0x0020 CHARACTERISTIC a623 WRITE | DYNAMIC
    0x0d, 0x00, 0x02, 0x00, 0x20, 0x00, 0x03, 0x28, 0x08, 0x21, 0x00, 0x23, 0xa6,
    // 0x0021 VALUE a623 WRITE | DYNAMIC
    0x08, 0x00, 0x08, 0x01, 0x21, 0x00, 0x23, 0xa6,

    /* CHARACTERISTIC,  a624, WRITE_WITHOUT_RESPONSE | DYNAMIC, */
    // 0x0022 CHARACTERISTIC a624 WRITE_WITHOUT_RESPONSE | DYNAMIC
    0x0d, 0x00, 0x02, 0x00, 0x22, 0x00, 0x03, 0x28, 0x04, 0x23, 0x00, 0x24, 0xa6,
    // 0x0023 VALUE a624 WRITE_WITHOUT_RESPONSE | DYNAMIC
    0x08, 0x00, 0x04, 0x01, 0x23, 0x00, 0x24, 0xa6,

    /* CHARACTERISTIC,  a625, NOTIFY, */
    // 0x0024 CHARACTERISTIC a625 NOTIFY
    0x0d, 0x00, 0x02, 0x00, 0x24, 0x00, 0x03, 0x28, 0x10, 0x25, 0x00, 0x25, 0xa6,
    // 0x0025 VALUE a625 NOTIFY
    0x08, 0x00, 0x10, 0x00, 0x25, 0x00, 0x25, 0xa6,
    // 0x0026 CLIENT_CHARACTERISTIC_CONFIGURATION
    0x0a, 0x00, 0x0a, 0x01, 0x26, 0x00, 0x02, 0x29, 0x00, 0x00,

    /* CHARACTERISTIC,  a640, READ | DYNAMIC, */
    // 0x0027 CHARACTERISTIC a640 READ | DYNAMIC
    0x0d, 0x00, 0x02, 0x00, 0x27, 0x00, 0x03, 0x28, 0x02, 0x28, 0x00, 0x40, 0xa6,
    // 0x0028 VALUE a640 READ | DYNAMIC
    0x08, 0x00, 0x02, 0x01, 0x28, 0x00, 0x40, 0xa6,

    /* CHARACTERISTIC,  a641, READ | DYNAMIC, */
    // 0x0029 CHARACTERISTIC a641 READ | DYNAMIC
    0x0d, 0x00, 0x02, 0x00, 0x29, 0x00, 0x03, 0x28, 0x02, 0x2a, 0x00, 0x41, 0xa6,
    // 0x002a VALUE a641 READ | DYNAMIC
    0x08, 0x00, 0x02, 0x01, 0x2a, 0x00, 0x41, 0xa6,

    // END
    0x00, 0x00,
};
//
// characteristics <--> handles
//
#define ATT_CHARACTERISTIC_2a00_01_VALUE_HANDLE 0x0003
#define ATT_CHARACTERISTIC_2a01_01_VALUE_HANDLE 0x0005
#define ATT_CHARACTERISTIC_2a05_01_VALUE_HANDLE 0x0008
#define ATT_CHARACTERISTIC_2a05_01_CLIENT_CONFIGURATION_HANDLE 0x0009
#define ATT_CHARACTERISTIC_2a24_01_VALUE_HANDLE 0x000c
#define ATT_CHARACTERISTIC_2a25_01_VALUE_HANDLE 0x000e
#define ATT_CHARACTERISTIC_2a26_01_VALUE_HANDLE 0x0010
#define ATT_CHARACTERISTIC_2a27_01_VALUE_HANDLE 0x0012
#define ATT_CHARACTERISTIC_2a28_01_VALUE_HANDLE 0x0014
#define ATT_CHARACTERISTIC_2a29_01_VALUE_HANDLE 0x0016
#define ATT_CHARACTERISTIC_a620_01_VALUE_HANDLE 0x0019
#define ATT_CHARACTERISTIC_a620_01_CLIENT_CONFIGURATION_HANDLE 0x001a
#define ATT_CHARACTERISTIC_a621_01_VALUE_HANDLE 0x001c
#define ATT_CHARACTERISTIC_a621_01_CLIENT_CONFIGURATION_HANDLE 0x001d
#define ATT_CHARACTERISTIC_a622_01_VALUE_HANDLE 0x001f
#define ATT_CHARACTERISTIC_a623_01_VALUE_HANDLE 0x0021
#define ATT_CHARACTERISTIC_a624_01_VALUE_HANDLE 0x0023
#define ATT_CHARACTERISTIC_a625_01_VALUE_HANDLE 0x0025
#define ATT_CHARACTERISTIC_a625_01_CLIENT_CONFIGURATION_HANDLE 0x0026
#define ATT_CHARACTERISTIC_a640_01_VALUE_HANDLE 0x0028
#define ATT_CHARACTERISTIC_a641_01_VALUE_HANDLE 0x002a

#endif