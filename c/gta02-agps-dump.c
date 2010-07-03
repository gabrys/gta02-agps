#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <sys/stat.h>

#include "ubx.h"
#include "help.h"
#include "ubx_io.h"
#include "dev_config.h"

int DUMP_FD;

int alm_dumped = 0;
int eph_dumped = 0;
int hui_dumped = 0;
int ecef_dumped = 0;

int VERBOSE = 0;

#define log(msg) if (VERBOSE) write(2, msg "\n", strlen(msg) + 1)

int main(int argc, char **argv) {
    GPS_UBX_CFG_MSG_SETCURRENT_t cfg_msg;
    
    /* -h, --help, no args, print help */
    if (argc < 2 || argc > 3 || strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
        help();
        return 0;
    }

    /* -v, --verbose */
    if (strcmp(argv[1], "--verbose") == 0 || strcmp(argv[1], "-v") == 0) {
        if (argc == 3) {
            VERBOSE = 1;
            argv[1] = argv[2];
        } else {
            help();
            return 3;
        }
    } else if (argc == 3) {
        if (strcmp(argv[2], "--verbose") == 0 || strcmp(argv[2], "-v") == 0) {
            VERBOSE = 1;
        } else {
            help();
            return 3;
        }
    }
    
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
    ubx_read(DEV_FD_IN);

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
        alm_dumped++;
        if (header->size == 40) {
            ubx_write(DUMP_FD, UBXID_AID_ALM, header->size, msg);
        }
    }
    if (msg_is(header, UBXID_AID_EPH)) {
        eph_dumped++;
        if (header->size == 104) {
            ubx_write(DUMP_FD, UBXID_AID_EPH, header->size, msg);
        }
    }
    if (msg_is(header, UBXID_AID_HUI)
        && header->size == sizeof(GPS_UBX_AID_HUI_t)
        && ! hui_dumped /* only one HUI needed */
    ) {
        ubx_write(DUMP_FD, UBXID_AID_HUI, header->size, msg);
        hui_dumped = 1;
    }
    if (msg_is(header, UBXID_NAV_POSECEF)
        && header->size == sizeof(GPS_UBX_NAV_POSECEF_t)
        && ! ecef_dumped /* only one ECEF needed */
    ) {
        ubx_write(DUMP_FD, UBXID_NAV_POSECEF, header->size, msg);
        ecef_dumped = 1;
    }
    if (alm_dumped && eph_dumped && hui_dumped && ecef_dumped) {
        if (VERBOSE) {
            fprintf(stderr, "Dumped messages: %d AID-ALM, %d AID-EPH, %d AID-HUI, %d NAV_POSECEF\n",
                alm_dumped, eph_dumped, hui_dumped, ecef_dumped
            );
        }
        return 1;
    }
    return 0;
}

