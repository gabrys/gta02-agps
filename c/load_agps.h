#include <time.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <sys/stat.h>
#include <stdio.h>

#include "common.h"

int DEV_OUT_FD;

void load_onalarm(int signum) {
    log("Device has not asked for aid data. Probably it is already set up.");
    exit(2);
}

int wait_for_aid_data_request(int fd, GPS_UBX_HEAD_pt header, char *msg);
int handle_message_from_file(int fd, GPS_UBX_HEAD_pt header, char *msg);

int load_agps(int dev_in, int dev_out, char *dump_file) {
    int dump_fd;
    GPS_UBX_CFG_MSG_SETCURRENT_t cfg_msg;

    DEV_OUT_FD = dev_out;
    
    /* open dump file */
    dump_fd = open(dump_file, O_RDONLY);
    if (dump_fd < 0) {
        perror("Cannot open file");
        return 1;
    }

    /* prepare CFG-MSG message */
    cfg_msg.klass = UBXID_AID_REQ / 256;
    cfg_msg.msgID = UBXID_AID_REQ % 256;
    cfg_msg.rate = 1;

    ubx_write(DEV_OUT_FD, UBXID_CFG_MSG, sizeof(GPS_UBX_CFG_MSG_SETCURRENT_t), (char*) &cfg_msg);
    
    /* set alarm. device may be already initialized and doesn't need any aid data */
    /* if it does not request it in AID_DATA_TIMEOUT_S seconds, program quits */
    signal(SIGALRM, load_onalarm);
    alarm(AID_DATA_TIMEOUT_S);
    
    ubx_read(dev_in, wait_for_aid_data_request);

    /* got request for aid data from device */    
    /* disable alarm */
    alarm(0);

    /* read stuff from file and feed it to device */
    ubx_read(dump_fd, handle_message_from_file);
    
    /* clean up */
    close(dump_fd);
    return 0;
}

/*
    AID-DATA comes from chip,
    AID_ALM, AID_ELM, AID_HUI and NAV_POSECEF from file
*/

int wait_for_aid_data_request(int fd, GPS_UBX_HEAD_pt header, char *msg) {
    if (msg_is(header, UBXID_AID_DATA)) {
        return 1;
    }
    return 0;
}

int handle_message_from_file(int fd, GPS_UBX_HEAD_pt header, char *msg) {
    GPS_UBX_AID_INI_U5__pt ini;
    GPS_UBX_NAV_POSECEF_pt posecef;
    unsigned int gps_time, gps_week, gps_second;
    
    if (msg_is(header, UBXID_AID_ALM)) {
        log("Sending AID_ALM message (almanac info for one sattelite) to GPS");
        ubx_write(DEV_OUT_FD, UBXID_AID_ALM, header->size, msg);
    }
    if (msg_is(header, UBXID_AID_EPH)) {
        log("Sending AID_EPH message (ephemeris info for one sattelite) to GPS");
        ubx_write(DEV_OUT_FD, UBXID_AID_EPH, header->size, msg);
    }
    if (msg_is(header, UBXID_AID_HUI)) {
        log("Sending AID_HUI message (health information, leap seconds, etc) to GPS");
        ubx_write(DEV_OUT_FD, UBXID_AID_EPH, header->size, msg);
    }
    if (msg_is(header, UBXID_NAV_POSECEF)) {
        /* get some data from message and construct AID_INI message */
        posecef = (GPS_UBX_NAV_POSECEF_pt) msg;

        /* construct AID_INI message initialized with 0s */
        ini = (GPS_UBX_AID_INI_U5__pt) calloc(1, sizeof(GPS_UBX_AID_INI_U5__t));

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
        ubx_write(DEV_OUT_FD, UBXID_AID_INI, sizeof(GPS_UBX_AID_INI_U5__t), (char *) ini);
        
        /* this should be the last message in file */
        return 1;
    }
    return 0;
}

