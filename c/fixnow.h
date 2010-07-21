#include "common.h"

/* need to define additional struct not found in ubx.h */

typedef struct GPS_UBX_CFG_RXM_s {
    U1  gpsMode;
    U1  lpMode;
} GPS_UBX_CFG_RXM_t,*GPS_UBX_CFG_RXM_pt;

#define UBXID_CFG_RXM 0x0611

/*
  wake-up sequence:
   8 times FF
   and then empty RXM-POSREQ
*/
#define WAKEUP_SEQ "\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xB5\x62\x02\x40\x00\x00\x42\xC8"
#define WAKEUP_SEQ_LEN 16

void fixnow(int dev_out, int enable) {
    GPS_UBX_CFG_RXM_t cfg_rxm;

    /* send CFG-RXM message */
    cfg_rxm.gpsMode = 3;
    if (enable) {
        cfg_rxm.lpMode = 1; /* FixNow */
        log("Putting the device to FixNow(R) sleep mode");
    } else {
        cfg_rxm.lpMode = 0; /* continuous tracking */
        log("Waking the device up from FixNow(R) sleep mode");
        write(dev_out, WAKEUP_SEQ, WAKEUP_SEQ_LEN);
    }
    ubx_write(dev_out, UBXID_CFG_RXM, sizeof(GPS_UBX_CFG_RXM_t), (char *) &cfg_rxm);
}

void fixnow_sleep(int dev_out) {
    fixnow(dev_out, 1);
}

void fixnow_wake_up(int dev_out) {
    fixnow(dev_out, 0);
}

