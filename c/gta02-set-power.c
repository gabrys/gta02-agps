#include "common.h"

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

int main(int argc, char **argv) {
    GPS_UBX_CFG_RXM_t cfg_rxm;

    /* parse --verbose and --help */
    parse_common_args(&argc, argv, 2, 2);

    log("Waking up GPS");
    write(DEV_FD_OUT, WAKEUP_SEQ, WAKEUP_SEQ_LEN);

    /* send CFG-RXM message */
    cfg_rxm.gpsMode = 3;
    if (strcmp(argv[1], "max") == 0) {
        log("Setting power mode to Max");
        cfg_rxm.lpMode = 0; /* continuous tracking */
    }
    if (strcmp(argv[1], "fixnow") == 0) {
        cfg_rxm.lpMode = 1; /* FixNow */
        log("Setting power mode to FixNow");
    }
    ubx_write(DEV_FD_OUT, UBXID_CFG_RXM, sizeof(GPS_UBX_CFG_RXM_t), (char *) &cfg_rxm);

    return 0;
}

