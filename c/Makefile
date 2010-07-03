
# make install will install to:
BIN_DIR=/usr/bin

# don't remove -fpack-struct from CFLAGS, it's needed
CFLAGS+=-fpack-struct -Wall

all: gta02-agps-dump gta02-agps-load

COMMON_H=help.h ubx.h ubx_io.h

gta02-agps-dump: ${COMMON_H}
gta02-agps-load: ${COMMON_H}

help.h: help.h.tpl README.md
	head -n 6 help.h.tpl > help.h
	head -n 29 README.md | sed -e 's/"/\\"/g' -e 's/^/"/' -e 's/$$/\\n"/' >> help.h
	tail -n 6 help.h.tpl >> help.h

clean:
	rm -f gta02-agps-dump gta02-agps-load *.o

install:
	cp gta02-agps-dump gta02-agps-load ${BIN_DIR}
