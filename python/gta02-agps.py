#!/usr/bin/env python

from ubxlib.ubx_reader import UBXReader
from ubxlib.ubx_writer import UBXWriter
from ubxlib.ubx_load import UBXLoadHandler
from ubxlib.ubx_dump import UBXDumpHandler
from ubxlib.ubx import CLIDPAIR

import sys, time

reader = UBXReader(sys.stdin)
writer = UBXWriter(sys.stdout)

def reset():
    writer.send("CFG-RST", 4, {"nav_bbr" : 0xffff, "Reset" : 0x01})
    time.sleep(1)

def load_aid_data(aid_data):
    writer.send("CFG-MSG", 3, {"Class" : CLIDPAIR["AID-REQ"][0] , "MsgID" : CLIDPAIR["AID-REQ"][1] , "Rate": 1})
    reader.read(UBXLoadHandler(writer, aid_data))

def dump_aid_data():
    writer.send("AID-HUI", 0, {})
    writer.send("AID-ALM", 0, {})
    writer.send("AID-EPH", 0, {})
    writer.send("CFG-MSG", 3, {"Class" : CLIDPAIR["NAV-POSECEF"][0] , "MsgID" : CLIDPAIR["NAV-POSECEF"][1] , "Rate": 1})
    return reader.read(UBXDumpHandler(writer))

def main(argv):
    if '--reset' in sys.argv:
        while '--reset' in sys.argv:
            sys.argv.remove('--reset')
        reset()

    if '--dump' in sys.argv:
        while '--dump' in sys.argv:
            sys.argv.remove('--dump')
        dumpfile = sys.argv[1]
        data = dump_aid_data()
        open(dumpfile, 'w').write(str(data))
        return

    if '--load' in sys.argv:
        while '--load' in sys.argv:
            sys.argv.remove('--load')
        dumpfile = sys.argv[1]
        data = eval(open(dumpfile, 'r').read())
        load_aid_data(data)
        return

if __name__ == '__main__':
    main(sys.argv)

