
# make install will install to:
BIN_DIR=/usr/bin

# don't remove -fpack-struct from CFLAGS, it's needed
CFLAGS+=-fpack-struct -Wall

gta02-gps: common.h config.h dump_agps.h fixnow.h help.h invocation.h load_agps.h parse_args.h setup_serial.h ubx.h ubx_io.h

help.h: help.h.tpl README.md Makefile
	head -n 6 help.h.tpl > help.h
	head -n 76 README.md | sed -e 's/"/\\"/g' -e 's/^/"/' -e 's/$$/\\n"/' >> help.h
	tail -n 6 help.h.tpl >> help.h

invocation.h: invocation.h.tpl README.md Makefile
	head -n 6 invocation.h.tpl > invocation.h
	head -n 11 README.md | tail -n 6 | sed -e 's/"/\\"/g' -e 's/^/"/' -e 's/$$/\\n"/' >> invocation.h
	tail -n 6 invocation.h.tpl >> invocation.h

clean:
	rm -f gta02-gps *.o

install: gta02-gps
	cp gta02-gps ${BIN_DIR}

uninstall:
	rm -f ${BIN_DIR}/gta02-gps

