
import struct
import logger
from ubx import *

class UBXWriter( object ):
    def __init__( self, file ):
        self.file = file

    def send( self, clid, length, payload ):
        logger.debug( "Sending UBX packet of type %s: %s" % ( clid, payload ) )

        stream = struct.pack("<BBBBH", SYNC1, SYNC2, CLIDPAIR[clid][0], CLIDPAIR[clid][1], length)
        if length > 0:
            try:
                fmt_base = [length] + MSGFMT[(clid,length)]
                fmt_rep = [0, "", []]
                payload_base = payload
            except KeyError:
                format = MSGFMT[(clid, None)]
                fmt_base = format[:3]
                fmt_rep = format[3:]
                payload_base = payload[0]
                payload_rep = payload[1:]
                if (length - fmt_base[0])%fmt_rep[0] != 0:
                    logger.error( "Cannot send: Variable length message class \
                        0x%x, id 0x%x has wrong length %i" % ( cl, id, length ) )
                    return
            stream = stream + struct.pack(fmt_base[1], *[payload_base[i] for i in fmt_base[2]])
            if fmt_rep[0] != 0:
                for i in range(0, (length - fmt_base[0])/fmt_rep[0]):
                    stream = stream + struct.pack(fmt_rep[1], *[payload_rep[i][j] for j in fmt_rep[2]])
        stream = stream + struct.pack("<BB", *checksum( stream[2:] ))
        self.file.write( stream )
        #self.file.flush()

