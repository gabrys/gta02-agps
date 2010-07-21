#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <sys/stat.h>

#include "common.h"

int alm_got = 0;
int alm_dumped = 0;
int eph_got = 0;
int eph_dumped = 0;
int hui_got = 0;
int ecef_got = 0;

char dump_buffer_start[16000];
char *dump_buffer = dump_buffer_start;

int handle_message(int fd, GPS_UBX_HEAD_pt header, char *msg);

void save_to_buffer(int UBXID, int size, char *payload) {
    char *msg;

    msg = ubx_construct(UBXID, size, payload);
    memcpy(dump_buffer, msg, size + 8); /* header + checksum is 8 bytes */
    dump_buffer += size + 8;

    free(msg);
} 

void onalarm(int signum) {
    log("Device has not sent aid data in specified time. Quitting.");
    exit(2);
}

int main(int argc, char **argv) {
    GPS_UBX_CFG_MSG_SETCURRENT_t cfg_msg;
    int dump_fd;
    
    /* parse --help and --verbose */
    parse_common_args(&argc, argv, 1, 1);
    
    /* prepare CFG-MSG message */
    cfg_msg.klass = UBXID_NAV_POSECEF / 256;
    cfg_msg.msgID = UBXID_NAV_POSECEF % 256;
    
    /* request AID_ALM, AID_EPH and AID_HUI */
    ubx_write(DEV_FD_OUT, UBXID_AID_HUI, 0, 0);
    ubx_write(DEV_FD_OUT, UBXID_AID_ALM, 0, 0);
    ubx_write(DEV_FD_OUT, UBXID_AID_EPH, 0, 0);
    
    /* enable ECEF messages */
    cfg_msg.rate = 8;
    ubx_write(DEV_FD_OUT, UBXID_CFG_MSG, sizeof(GPS_UBX_CFG_MSG_SETCURRENT_t), (char*) &cfg_msg);

    /* set alarm. device may be not configured correctly and can not send us valid aid data */
    /* if it does send them in AID_DATA_TIMEOUT_S seconds, program quits */
    signal(SIGALRM, onalarm);
    alarm(AID_DATA_TIMEOUT_S);

    /* start read loop */
    ubx_read(DEV_FD_IN, handle_message);

    /* disable alarm */
    alarm(0);

    /* disable ECEF messages */
    cfg_msg.rate = 0;
    ubx_write(DEV_FD_OUT, UBXID_CFG_MSG, sizeof(GPS_UBX_CFG_MSG_SETCURRENT_t), (char*) &cfg_msg);

    /* save from buffer to file */
    dump_fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (dump_fd < 0) {
        perror("Can't open file");
        return 1;
    }
    write(dump_fd, dump_buffer_start, dump_buffer - dump_buffer_start);
    close(dump_fd);
    
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
            save_to_buffer(UBXID_AID_ALM, header->size, msg);
        }
    }
    if (msg_is(header, UBXID_AID_EPH)) {
        eph_got++;
        if (header->size == 104) {
            eph_dumped++;
            save_to_buffer(UBXID_AID_EPH, header->size, msg);
        }
    }
    if (msg_is(header, UBXID_AID_HUI)
        && header->size == sizeof(GPS_UBX_AID_HUI_t)
        && ! hui_got /* only one HUI needed */
    ) {
        save_to_buffer(UBXID_AID_HUI, header->size, msg);
        hui_got = 1;
    }
    if (msg_is(header, UBXID_NAV_POSECEF)
        && header->size == sizeof(GPS_UBX_NAV_POSECEF_t)
        && ! ecef_got /* only one ECEF needed */
    ) {
        save_to_buffer(UBXID_NAV_POSECEF, header->size, msg);
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

