#ifndef _UBX_IO_H
#define _UBX_IO_H

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "ubx.h"

/* header only, implement in calling file! */
int handle_message(int fd, GPS_UBX_HEAD_pt header, char *msg);

void panic(char *msg) {
    write(2, msg, strlen(msg));
    write(2, "\n", 1);
    exit(1);
}

void checksum(GPS_UBX_HEAD_pt header, char *msg, char *chksum1, char *chksum2) {
    char ck_a = 0, ck_b = 0;
    char *header_ch = (char*) header;
    int len = header->size;
    int hlen = 4;

    /* omit magic ubx bytes */
    header_ch++;
    header_ch++;

    while (hlen--) {
        ck_a = ck_a + *header_ch;
        ck_b = ck_b + ck_a;
        header_ch++;
    }
    
    while (len--) {
        ck_a = ck_a + *msg;
        ck_b = ck_b + ck_a;
        msg++;
    }

    *chksum1 = ck_a;
    *chksum2 = ck_b;
}

int msg_is(GPS_UBX_HEAD_pt header, int UBXID) {
    return header->msgId == UBXID % 256
        && header->classId == UBXID / 256;
}

void ubx_read(int fd) {
    char buffer_start[16 * 1024];
    char *buffer;
    char *buffer_end;
    int buffer_read = 0;
    int bytes_read;
    int real_size;
    
    int quit = 0, has_msg;
    GPS_UBX_HEAD_pt header;
    char *msg;
    int buffer_size, need_size;
    char chksum1, chksum2;
    
    buffer = buffer_start;
    buffer_end = buffer;
    
    while (! quit) {
        /* clean up if we're going past the buffer */
        if (buffer_read > 15 * 1024) {
            real_size = buffer_end - buffer;
            bcopy(buffer, buffer_start, real_size);
            buffer_read = real_size;
            buffer = buffer_start;
            buffer_end = buffer_start + real_size;
        }
        /* let's read next 32 bytes */
        bytes_read = read(fd, buffer_end, 32);
        if (bytes_read < 0) {
            panic("(ubx_read) Can't read from the device");
        }
        buffer_read += bytes_read;
        buffer_end += bytes_read;

        /* find magic UBX bytes */
        has_msg = 0;
        while (buffer < buffer_end - 8) {
            if (*buffer != (char) GPS_UBX_SYNC_CHAR_1
                || *(buffer + 1) != (char) GPS_UBX_SYNC_CHAR_2
            ) {
                buffer++;
                /* would be cool to delete unused memory */
            } else {
                has_msg = 1;
                break;
            }
        }
        if (! has_msg) {
            continue;
        }

        /* now we have full UBX_HEADER under buffer */
        header = (GPS_UBX_HEAD_pt) buffer;
        
        /* let's see if all msg is in buffer */
        buffer_size = (buffer_end - buffer);
        need_size = header->size + 8; /* add 6 bytes for header and 2 for checksum */
        if (buffer_size < need_size) {
            continue;
        }

        /* now we have message available under buffer + 6 with length header->size */
        msg = (buffer + 6);
        
        /* checksum */
        checksum(header, msg, &chksum1, &chksum2);
        if (chksum1 != *(msg + header->size)
            || chksum2 != *(msg + header->size + 1)
        ) {
            /* wrong checksum, maybe not UBX */
            write(2, "Wrong checksum\n", strlen("Wrong checksum\n"));
            buffer += 2;
            continue;
        }

        buffer = buffer + need_size;
        /*
            now msg is real UBX message with good checksum, call handle on it
            and if return value is true, stop reading
        */
        quit = handle_message(fd, header, msg);
    }
}

void ubx_write(int fd, int UBXID, int size, char *payload) {
    GPS_UBX_HEAD_t header;
    char chksum1, chksum2;
    char *buffer;

    header.prefix = GPS_UBX_PREFIX;
    header.classId = UBXID / 256;
    header.msgId = UBXID % 256;
    header.size = size;

    checksum(&header, payload, &chksum1, &chksum2);
    
    buffer = malloc(size + 8);
    memcpy(buffer, &header, 6);
    memcpy(buffer + 6, payload, size);
    memcpy(buffer + 6 + size, &chksum1, 1);
    memcpy(buffer + 6 + size + 1, &chksum2, 1);

    if (write(fd, buffer, size + 8) < size + 8) {
        panic("(ubx_write) Can't write to the device");
    }
}

#endif
