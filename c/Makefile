
# make install will install to:
BIN_DIR=/usr/bin

# don't remove -fpack-struct from CFLAGS, it's needed
CFLAGS+=-fpack-struct -Wall

COMMON_H=common.h config.h help.h ubx.h ubx_io.h
ALL_PROGS=gta02-agps-dump gta02-agps-load gta02-set-power stdin-serial-set-raw

all: ${ALL_PROGS}


gta02-agps-dump: ${COMMON_H}
gta02-agps-load: ${COMMON_H}
gta02-set-power: ${COMMON_H}

help.h: help.h.tpl README.md
	head -n 6 help.h.tpl > help.h
	head -n 59 README.md | sed -e 's/"/\\"/g' -e 's/^/"/' -e 's/$$/\\n"/' >> help.h
	tail -n 6 help.h.tpl >> help.h

clean:
	rm -f ${ALL_PROGS} *.o

install:
	cp ${ALL_PROGS} ${BIN_DIR}
