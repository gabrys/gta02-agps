#include <string.h>
#include <unistd.h>

void help() {
    char *help =

"gta02-apgs-dump, gta02-agps-load\n"
"\n"
"INFORMATION\n"
"===========\n"
"\n"
"These programs can set up the UBX GPS chip to obtain\n"
"faster fix after powering on (TTFF). To do this it\n"
"needs AGPS information.\n"
"\n"
"The AGPS information can be grabbed from GPS chip\n"
"when it has fix (reports valid position). Do:\n"
"\n"
"    gta02-agps-dump agpsdata < /dev/ttySAC1 > /dev/ttySAC1\n"
"\n"
"This will save AGPS data read from chip into the\n"
"file \"agpsdata\".\n"
"\n"
"This data can be loaded into the GPS after it is\n"
"stopped and started again. This should improve TTFF.\n"
"\n"
"To load data from agpsdata file issue:\n"
"\n"
"    gta02-agps-load agpsdata < /dev/ttySAC1 > /dev/ttySAC1\n"
"\n"
"Passing -h or --help argument gives you this message.\n"
"\n"
"Passing -v or --verbose argument prints some debug\n"
"information while operating.\n"
"\n"
    
    ;

    write(2, help, strlen(help));
}

