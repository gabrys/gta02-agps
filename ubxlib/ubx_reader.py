
import struct

import logger
from ubx import *

class UBXReader( object ):
    def __init__( self, file ):
        self.buffer = ""
        self.file = file

    def read( self, handler ):
        self.handler = handler
        buffer_offset = 0
        while True:
            # Minimum packet length is 8
            data = self.file.read(8)
            self.buffer += data
            # logger.debug("BUFFER_OFFSET: %d BUFFER: " % buffer_offset + " ".join(["%02x" % ord(x) for x in self.buffer]))

            # Find the beginning of a UBX message
            start = self.buffer.find( chr( SYNC1 ) + chr( SYNC2 ), buffer_offset )

            if buffer_offset == 0 and start != 0:
                logger.debug( "Discarded data not UBX %s" % repr(self.buffer[:start]) )
                self.buffer = self.buffer[start:]
                continue

            if start == -1 or start + 8 > len(self.buffer):
                continue

            (cl, id, length) = struct.unpack("<BBH", self.buffer[start+2:start+6])
            if len(self.buffer) < start + length + 8:
                # not enough data
                continue

            if checksum(self.buffer[start+2:start+length+6]) != struct.unpack("<BB", self.buffer[start+length+6:start+length+8]):
                buffer_offset = start + 2
                continue

            if start != 0:
                logger.warning(" UBX packet ignored %s" % repr(self.buffer[:start]) )
                self.buffer = self.buffer[start:]
                buffer_offset = 0
                continue

            exit_after = self.decode(cl, id, length, self.buffer[start+6:start+length+6])

            # Discard packet
            self.buffer = self.buffer[start+length+8:]
            buffer_offset = 0

            if exit_after != None:
                return exit_after


    # if this returns True, read is stopped
    def decode( self, cl, id, length, payload ):
        data = []
        try:
            format = MSGFMT_INV[((cl, id), length)]
            data.append(dict(zip(format[1], struct.unpack(format[0], payload))))
        except KeyError:
            try:
                # Try if this is one of the variable field messages
                format = MSGFMT_INV[((cl, id), None)]
                fmt_base = format[:3]
                fmt_rep = format[3:]
                # Check if the length matches
                if (length - fmt_base[0])%fmt_rep[0] != 0:
                    logger.error( "Variable length message class 0x%x, id 0x%x \
                        has wrong length %i" % ( cl, id, length ) )
                    return True # continue
                data.append(dict(zip(fmt_base[2], struct.unpack(fmt_base[1], payload[:fmt_base[0]]))))
                for i in range(0, (length - fmt_base[0])/fmt_rep[0]):
                    offset = fmt_base[0] + fmt_rep[0] * i
                    data.append(dict(zip(fmt_rep[2], struct.unpack(fmt_rep[1], payload[offset:offset+fmt_rep[0]]))))

            except KeyError:
                logger.info( "Unknown message class 0x%x, id 0x%x, length %i" % ( cl, id, length ) )
                return

        logger.debug( "Got UBX packet of type %s: %s" % (format[-1] , data ) )
        methodname = "handle_"+format[-1].replace("-", "_")
        try:
            method = getattr( self.handler, methodname )
        except AttributeError:
            logger.debug( "No method to handle %s: %s" % ( format[-1], data ) )
        else:
            try:
                # if this returns True, read is stopped
                return method( data )
            except Exception, e:
                logger.error( "Error in %s method: %s" % ( methodname, e ) )

