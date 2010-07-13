#include <time.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <sys/stat.h>

#include "common.h"

int DUMP_FD;

void onalarm(int signum) {
    log("Device has not asked for aid data. Probably it is already set up.");
    exit(2);
}

int handle_message(int fd, GPS_UBX_HEAD_pt header, char *msg);

int main(int argc, char **argv) {
    GPS_UBX_CFG_MSG_SETCURRENT_t cfg_msg;
    
    /* parse --verbose and --help */
    parse_common_args(&argc, argv, 1, 1);

    /* open dump file */
    DUMP_FD = open(argv[1], O_RDONLY);
    if (DUMP_FD < 0) {
        perror("Cannot open file");
        return 1;
    }

    /* prepare CFG-MSG message */
    cfg_msg.class = UBXID_AID_REQ / 256;
    cfg_msg.msgID = UBXID_AID_REQ % 256;
    cfg_msg.rate = 1;

    ubx_write(DEV_FD_OUT, UBXID_CFG_MSG, sizeof(GPS_UBX_CFG_MSG_SETCURRENT_t), (char*) &cfg_msg);
    
    /* set alarm. device may be already initialized and doesn't need any aid data */
    /* if it does not request it in AID_DATA_TIMEOUT_S seconds, program quits */
    signal(SIGALRM, onalarm);
    alarm(AID_DATA_TIMEOUT_S);
    
    ubx_read(DEV_FD_IN, handle_message);
    
    close(DUMP_FD);
    return 0;
}

/*
    AID-DATA comes from chip,
    AID_ALM, AID_ELM, AID_HUI and NAV_POSECEF from file
*/

int handle_message(int fd, GPS_UBX_HEAD_pt header, char *msg) {
    GPS_UBX_AID_INI_U5__pt ini;
    GPS_UBX_NAV_POSECEF_pt posecef;
    unsigned int gps_time, gps_week, gps_second;
    
    if (fd == DEV_FD_IN && msg_is(header, UBXID_AID_DATA)) {
        /* got request for data, let's read it from file */
        ubx_read(DUMP_FD, handle_message);
        return 1;
    }
    if (fd == DUMP_FD && msg_is(header, UBXID_AID_ALM)) {
        log("Sending AID_ALM message (almanac info for one sattelite) to GPS");
        ubx_write(DEV_FD_OUT, UBXID_AID_ALM, header->size, msg);
    }
    if (fd == DUMP_FD && msg_is(header, UBXID_AID_EPH)) {
        log("Sending AID_EPH message (ephemeris info for one sattelite) to GPS");
        ubx_write(DEV_FD_OUT, UBXID_AID_EPH, header->size, msg);
    }
    if (fd == DUMP_FD && msg_is(header, UBXID_AID_HUI)) {
        log("Sending AID_HUI message (health information, leap seconds, etc) to GPS");
        ubx_write(DEV_FD_OUT, UBXID_AID_EPH, header->size, msg);
    }
    if (fd == DUMP_FD && msg_is(header, UBXID_NAV_POSECEF)) {
        /* get some data from message and construct AID_INI message */
        posecef = (GPS_UBX_NAV_POSECEF_pt) msg;

        /* construct AID_INI message initialized with 0s */
        ini = calloc(1, sizeof(GPS_UBX_AID_INI_U5__t));

        /* set last known position */
        ini->ecefXOrLat = posecef->ecefX;
        ini->ecefYOrLon = posecef->ecefY;
        ini->ecefZOrAlt = posecef->ecefZ;
        ini->posAcc = posecef->pAcc;
        if (ini->posAcc < POSITION_ACCURACY_MIN_CM) {
            ini->posAcc = POSITION_ACCURACY_MIN_CM;
        }
        if (ini->posAcc < POSITION_ACCURACY_MAX_CM) {
            /* position is valid */
            ini->flags |= 1;
        }

        /* set current time */
        gps_time = time(NULL) - 315964800 + 15;
        gps_week = gps_time / (7 * 24 * 3600);
        gps_second = gps_time % (7 * 24 * 3600);
        /* 315964800 is GPS epoch time measured in UTC epoch time */
        /* 15 is approx. number of leap seconds since that time */
        /* this is not so much important, since we state */
        /* the time passed to GPS has accuracy of 10 minutes */
        ini->wn = gps_week;
        ini->tow = gps_second * 1000;
        ini->tAccMs = TIME_ACCURACY_MS;
        ini->flags |= 2; /* time is valid */
        
        /* finally send time and position info */
        log("Sending AID_INI message (known position and time) to GPS");
        ubx_write(DEV_FD_OUT, UBXID_AID_INI, sizeof(GPS_UBX_AID_INI_U5__t), (char *) ini);
        
        /* this should be the last message in file */
        return 1;
    }
    return 0;
}

