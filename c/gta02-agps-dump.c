#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

#include "common.h"

int DUMP_FD;

int alm_got = 0;
int alm_dumped = 0;
int eph_got = 0;
int eph_dumped = 0;
int hui_got = 0;
int ecef_got = 0;

int handle_message(int fd, GPS_UBX_HEAD_pt header, char *msg);

int main(int argc, char **argv) {
    GPS_UBX_CFG_MSG_SETCURRENT_t cfg_msg;
    
    /* parse --help and --verbose */
    parse_common_args(&argc, argv, 1, 1);
    
    /* open dump file */
    DUMP_FD = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (DUMP_FD < 0) {
        perror("Can't open file");
        return 1;
    }
    
    /* prepare CFG-MSG message */
    cfg_msg.class = UBXID_NAV_POSECEF / 256;
    cfg_msg.msgID = UBXID_NAV_POSECEF % 256;
    
    /* request AID_ALM, AID_EPH and AID_HUI */
    ubx_write(DEV_FD_OUT, UBXID_AID_HUI, 0, 0);
    ubx_write(DEV_FD_OUT, UBXID_AID_ALM, 0, 0);
    ubx_write(DEV_FD_OUT, UBXID_AID_EPH, 0, 0);
    
    /* enable ECEF messages */
    cfg_msg.rate = 8;
    ubx_write(DEV_FD_OUT, UBXID_CFG_MSG, sizeof(GPS_UBX_CFG_MSG_SETCURRENT_t), (char*) &cfg_msg);

    /* start read loop */
    ubx_read(DEV_FD_IN, handle_message);

    /* disable ECEF messages */
    cfg_msg.rate = 0;
    ubx_write(DEV_FD_OUT, UBXID_CFG_MSG, sizeof(GPS_UBX_CFG_MSG_SETCURRENT_t), (char*) &cfg_msg);
    
    /* clean up */
    close(DUMP_FD);
    
    return 0;
}

/*
    Dump NAV-POSECEF, AID-ALM, AID-EPH, AID-HUI, AID-DATA messages to file
*/

int handle_message(int fd, GPS_UBX_HEAD_pt header, char *msg) {
    if (msg_is(header, UBXID_AID_ALM)) {
        alm_got++;
        if (header->size == 40) {
            alm_dumped++;
            ubx_write(DUMP_FD, UBXID_AID_ALM, header->size, msg);
        }
    }
    if (msg_is(header, UBXID_AID_EPH)) {
        eph_got++;
        if (header->size == 104) {
            eph_dumped++;
            ubx_write(DUMP_FD, UBXID_AID_EPH, header->size, msg);
        }
    }
    if (msg_is(header, UBXID_AID_HUI)
        && header->size == sizeof(GPS_UBX_AID_HUI_t)
        && ! hui_got /* only one HUI needed */
    ) {
        ubx_write(DUMP_FD, UBXID_AID_HUI, header->size, msg);
        hui_got = 1;
    }
    if (msg_is(header, UBXID_NAV_POSECEF)
        && header->size == sizeof(GPS_UBX_NAV_POSECEF_t)
        && ! ecef_got /* only one ECEF needed */
    ) {
        ubx_write(DUMP_FD, UBXID_NAV_POSECEF, header->size, msg);
        ecef_got = 1;
    }
    if (alm_got && eph_got && hui_got && ecef_got) {
        logf("Got/dumped messages: %d/%d AID-ALM, %d/%d AID-EPH, %d/%d AID-HUI, %d/%d NAV-POSECEF\n",
            alm_got, alm_dumped, eph_got, eph_dumped, hui_got, hui_got, ecef_got, ecef_got
        );
        return 1;
    }
    return 0;
}

