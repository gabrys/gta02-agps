
SYNC1=0xb5
SYNC2=0x62

CLASS = {
    "NAV" : 0x01,
    "RXM" : 0x02,
    "INF" : 0x04,
    "ACK" : 0x05,
    "CFG" : 0x06,
    "UPD" : 0x09,
    "MON" : 0x0a,
    "AID" : 0x0b,
    "TIM" : 0x0d,
    "USR" : 0x40
}

CLIDPAIR = {
    "ACK-ACK" : (0x05, 0x01),
    "ACK-NACK" : (0x05, 0x00),
    "AID-ALM" : (0x0b, 0x30),
    "AID-DATA" : (0x0b, 0x10),
    "AID-EPH" : (0x0b, 0x31),
    "AID-HUI" : (0x0b, 0x02),
    "AID-INI" : (0x0b, 0x01),
    "AID-REQ" : (0x0b, 0x00),
    "CFG-ANT" : (0x06, 0x13),
    "CFG-CFG" : (0x06, 0x09),
    "CFG-DAT" : (0x06, 0x06),
    "CFG-EKF" : (0x06, 0x12),
    "CFG-FXN" : (0x06, 0x0e),
    "CFG-INF" : (0x06, 0x02),
    "CFG-LIC" : (0x06, 0x80),
    "CFG-MSG" : (0x06, 0x01),
    "CFG-NAV2" : (0x06, 0x1a),
    "CFG-NMEA" : (0x06, 0x17),
    "CFG-PRT" : (0x06, 0x00),
    "CFG-RATE" : (0x06, 0x08),
    "CFG-RST" : (0x06, 0x04),
    "CFG-RXM" : (0x06, 0x11),
    "CFG-SBAS" : (0x06, 0x16),
    "CFG-TM" : (0x06, 0x10),
    "CFG-TM2" : (0x06, 0x19),
    "CFG-TMODE" : (0x06, 0x1d),
    "CFG-TP" : (0x06, 0x07),
    "CFG-USB" : (0x06, 0x1b),
    "INF-DEBUG" : (0x04, 0x04),
    "INF-ERROR" : (0x04, 0x00),
    "INF-NOTICE" : (0x04, 0x02),
    "INF-TEST" : (0x04, 0x03),
    "INF-USER" : (0x04, 0x07),
    "INF-WARNING" : (0x04, 0x01),
    "MON-EXCEPT" : (0x0a, 0x05),
    "MON-HW" : (0x0a, 0x09),
    "MON-IO" : (0x0a, 0x02),
    "MON-IPC" : (0x0a, 0x03),
    "MON-MSGPP" : (0x0a, 0x06),
    "MON-RXBUF" : (0x0a, 0x07),
    "MON-SCHD" : (0x0a, 0x01),
    "MON-TXBUF" : (0x0a, 0x08),
    "MON-USB" : (0x0a, 0x0a),
    "MON-VER" : (0x0a, 0x04),
    "NAV-CLOCK" : (0x01, 0x22),
    "NAV-DGPS" : (0x01, 0x31),
    "NAV-DOP" : (0x01, 0x04),
    "NAV-EKFSTATUS" : (0x01, 0x40),
    "NAV-POSECEF" : (0x01, 0x01),
    "NAV-POSLLH" : (0x01, 0x02),
    "NAV-POSUTM" : (0x01, 0x08),
    "NAV-SBAS" : (0x01, 0x32),
    "NAV-SOL" : (0x01, 0x06),
    "NAV-STATUS" : (0x01, 0x03),
    "NAV-SVINFO" : (0x01, 0x30),
    "NAV-TIMEGPS" : (0x01, 0x20),
    "NAV-TIMEUTC" : (0x01, 0x21),
    "NAV-VELECEF" : (0x01, 0x11),
    "NAV-VELNED" : (0x01, 0x12),
    "RXM-ALM" : (0x02, 0x30),
    "RXM-EPH" : (0x02, 0x31),
    "RXM-POSREQ" : (0x02, 0x40),
    "RXM-RAW" : (0x02, 0x10),
    "RXM-SFRB" : (0x02, 0x11),
    "RXM-SVSI" : (0x02, 0x20),
    "TIM-SVIN" : (0x0d, 0x04),
    "TIM-TM" : (0x0d, 0x02),
    "TIM-TM2" : (0x0d, 0x03),
    "TIM-TP" : (0x0d, 0x01),
    "UPD-DOWNL" : (0x09, 0x01),
    "UPD-EXEC" : (0x09, 0x03),
    "UPD-MEMCPY" : (0x09, 0x04),
    "UPD-UPLOAD" : (0x09, 0x02)
}

CLIDPAIR_INV = dict( [ [v,k] for k,v in CLIDPAIR.items() ] )

MSGFMT = {
    ("NAV-POSECEF", 20) :
        ["<IiiiI", ["ITOW", "ECEF_X", "ECEF_Y", "ECEF_Z", "Pacc"]],
    ("NAV-POSLLH", 28) :
        ["<IiiiiII", ["ITOW", "LON", "LAT", "HEIGHT", "HMSL", "Hacc", "Vacc"]],
    ("NAV-POSUTM", 18) :
        ["<Iiiibb", ["ITOW", "EAST", "NORTH", "ALT", "ZONE", "HEM"]],
    ("NAV-DOP", 18) :
        ["<IHHHHHHH", ["ITOW", "GDOP", "PDOP", "TDOP", "VDOP", "HDOP", "NDOP", "EDOP"]],
    ("NAV-STATUS", 16) :
        ["<IBBBxII", ["ITOW", "GPSfix", "Flags", "DiffS", "TTFF", "MSSS"]],
    ("NAV-SOL", 52) :
        ["<IihBBiiiIiiiIHxBxxxx", ["ITOW", "Frac", "week", "GPSFix", "Flags", "ECEF_X", "ECEF_Y", "ECEF_Z", "Pacc",
         "ECEFVX", "ECEFVY", "ECEFVZ", "SAcc", "PDOP", "numSV"]],
    ("NAV-VELECEF", 20) :
        ["<IiiiI", ["ITOW", "ECEFVX", "ECEFVY", "ECEFVZ", "SAcc"]],
    ("NAV-VELNED", 36) :
        ["<IiiiIIiII", ["ITOW", "VEL_N", "VEL_E", "VEL_D", "Speed", "GSpeed", "Heading", "SAcc", "CAcc"]],
    ("NAV-TIMEGPS", 16) :
        ["<IihbBI", ["ITOW", "Frac", "week", "LeapS", "Valid", "TAcc"]],
    ("NAV-TIMEUTC", 20) :
        ["<IIiHBBBBBB", ["ITOW", "TAcc", "Nano", "Year", "Month", "Day", "Hour", "Min", "Sec", "Valid"]],
    ("NAV-CLOCK",  20) :
        ["<IiiII", ["ITOW", "CLKB", "CLKD", "TAcc", "FAcc"]],
    ("NAV-SVINFO", None) :
        [8, "<IBxxx", ["ITOW", "NCH"], 12, "<BBBbBbhi", ["chn", "SVID", "Flags", "QI", "CNO", "Elev", "Azim", "PRRes"]],
    ("NAV-DGPS", None) :
        [16, "<IihhBBxx", ["ITOW", "AGE", "BASEID", "BASEHLTH", "NCH", "STATUS"], 12, "<BBHff", ["SVID", "Flags", "AGECH", "PRC", "PRRC"]],
    ("NAV-SBAS", None) :
        [12, "<IBBbBBxxx", ["ITOW", "GEO", "MODE", "SYS", "SERVICE", "CNT"], 12, "<BBBBBxhxxh", ["SVID", "FLAGS", "UDRE", "SYSn", "SERVICEn", "PRC", "IC"]],
# NAV-EKFSTATUS - Dead reckoning
    ("RXM-RAW", None) :
        [8, "<ihBx", ["ITOW", "Week", "NSV"], 24, "<ddfBbbB", ["CPMes", "PRMes", "DOMes", "SV", "MesQI", "CNO", "LLI"]],
    ("RXM-SVSI", None) :
        [8, "<ihBB", ["ITOW", "Week", "NumVis", "NumSv"], 6, "<BBhbB", ["SVID", "SVFlag", "Azim", "Elev", "Age"]],
# RXM-SFRB - Subframe buffer
    ("RXM-ALM", 1) :
        ["<B", ["SVID"]],
    ("RXM-ALM", 8)  :
        ["<II", ["SVID", "WEEK"]],
    ("RXM-ALM", 40) :
        ["<" + "I"*10, ["SVID", "WEEK", "DWRD0", "DWRD1", "DWRD2", "DWRD3", "DWRD4", "DWRD5", "DWRD6", "DWRD7"]],
    ("RXM-EPH", 1) :
        ["<B", ["SVID"]],
    ("RXM-EPH", 8) :
        ["<II", ["SVID", "HOW"]],
    ("RXM-EPH", 104) :
        ["<" + "I"*26, ["SVID", "HOW", "SF1D0", "SF1D1", "SF1D2", "SF1D3", "SF1D4",
            "SF1D5", "SF1D6", "SF1D7", "SF2D0", "SF2D1", "SF2D2", "SF2D3", "SF2D4",
            "SF2D5", "SF2D6", "SF2D7", "SF3D0", "SF3D1", "SF3D2", "SF3D3", "SF3D4", "SF3D5", "SF3D6", "SF3D7"]],
    ("INF-ERROR", None) :
        [0, "", [], 1, "c", ["Char"]],
    ("INF-WARNING", None) :
        [0, "", [], 1, "c", ["Char"]],
    ("INF-NOTICE", None) :
        [0, "", [], 1, "c", ["Char"]],
    ("INF-TEST", None) :
        [0, "", [], 1, "c", ["Char"]],
    ("INF-DEBUG", None) :
       [0, "", [], 1, "c", ["Char"]],
    ("INF-USER", None) :
        [0, "", [], 1, "c", ["Char"]],
    ("ACK-ACK", 2) :
        ["<BB", ["ClsID", "MsgID"]],
    ("ACK-NACK", 2) :
        ["<BB", ["ClsID", "MsgID"]],
    ("CFG-PRT", 1) :
        ["<B", ["PortID"]],
    ("CFG-PRT", None) :
        [0, "", [], 20, "<BxxxIIHHHxx", ["PortID", "Mode", "Baudrate", "In_proto_mask", "Out_proto_mask", "Flags"]],
    ("CFG-USB", 108) :
        ["<HHxxHHH32s32s32s", ["VendorID", "ProductID", "reserved2", "PowerConsumption", "Flags", "VendorString", "ProductString", "SerialNumber"]],
    ("CFG-MSG", 2) :
        ["<BB", ["Class", "MsgID"]],
    ("CFG-MSG", 3) :
        ["<BBB", ["Class", "MsgID", "Rate"]],
    ("CFG-NMEA", 4) :
        ["<BBBB", ["Filter", "Version", "NumSV", "Flags"]],
    ("CFG-RATE", 6) :
        ["<HHH", ["Meas", "Nav", "Time"]],
    ("CFG-CFG", 12) :
        ["<III", ["Clear_mask", "Save_mask", "Load_mask"]],
    ("CFG-TP", 20) :
        ["<IIbBxxhhi", ["interval", "length", "status", "time_ref", "antenna_cable_delay", "RF_group_delay", "user_delay"]],
    ("CFG-NAV2", 40) :
        ["<BxxxBBBBiBBBBBBxxHHHHBxxxxxxxxxxx", ["Platform", "MinSVInitial", "MinSVs", "MaxSVs", "FixMode",
         "FixedAltitude", "MinCN0Initial", "MinCN0After", "MinELE", "DGPSTO", "MaxDR", "NAVOPT", "PDOP",
         "TDOP", "PACC", "TACC", "StaticThres"]],
# CFG DAT - Get/Set current Datum
    ("CFG-INF", 1) :
        ["<B", ["ProtocolID"]],
    ("CFG-INF", None) :
        [0, "", [], 8, "<BxxxBBBB", ["ProtocolID", "INFMSG_mask0", "INFMSG_mask1", "INFMSG_mask2", "INFMSG_mask3"]],
    ("CFG-RST", 4) :
        ["<HBx", ["nav_bbr", "Reset"]],
    ("CFG-RXM", 2) :
        ["<BB", ["gps_mode", "lp_mode"]],
    ("CFG-ANT", 4) :
        ["<HH", ["flags", "pins"]],
    ("CFG-FXN", 36) :
        ["<IIIIIIIxxxxI", ["flags", "t_reacq", "t_acq", "t_reacq_off", "t_acq_off", "t_on", "t_off", "base_tow"]],
    ("CFG-SBAS", 8) :
        ["<BBBxI", ["mode", "usage", "maxsbas", "scanmode"]],
    ("CFG-LIC", 12) :
        ["<HHHHHH", ["lic1", "lic2", "lic3", "lic4", "lic5", "lic6"]],
    ("CFG-TM", 12) :
        ["<III", ["INTID", "RATE", "FLAGS"]],
    ("CFG-TM2", 1) :
        ["<B", ["CH"]],
    ("CFG-TM2", 12) :
        ["<BxxxII", ["CH", "RATE", "FLAGS"]],
    ("CFG-TMODE", 28) :
        ["<IiiiIII", ["TimeMode", "FixedPosX", "FixedPosY", "FixedPosZ", "FixedPosVar", "SvinMinDur", "SvinVarLimit"]],
# CFG EKF - Dead Reckoning
# UPD - Lowlevel memory manipulation
    ("MON-SCHD", 24) :
        ["<IIIIHHHBB", ["TSKRUN", "TSKSCHD", "TSKOVRR", "TSKREG", "STACK", "STACKSIZE", "CPUIDLE", "FLYSLY", "PTLSLY"]],
# MON - GPS system statistics
    ("AID-INI", 48) :
        ["<iiiIHHIiIIiII", ["X", "Y", "Z", "POSACC", "TM_CFG", "WN", "TOW", "TOW_NS", "TACC_MS", "TACC_NS", "CLKD", "CLKDACC", "FLAGS"]],
    ("AID-DATA", 0) :
        ["", []],
    ("AID-HUI", 72) :
        ["<IddiHHHHHHffffffffI", ["HEALTH", "UTC_A1", "UTC_A0", "UTC_TOT", "UTC_WNT",
         "UTC_LS", "UTC_WNF", "UTC_DN", "UTC_LSF", "UTC_SPARE", "KLOB_A0", "KLOB_A1",
         "KLOB_A2", "KLOB_A3", "KLOB_B0", "KLOB_B1", "KLOB_B2", "KLOB_B3", "FLAGS"]],
    ("AID-ALM", 1) :
        ["<B", ["SVID"]],
    ("AID-ALM", 8)  :
        ["<II", ["SVID", "WEEK"]],
    ("AID-ALM", 40) :
        ["<" + "I"*10, ["SVID", "WEEK", "DWRD0", "DWRD1", "DWRD2", "DWRD3", "DWRD4", "DWRD5", "DWRD6", "DWRD7"]],
    ("AID-EPH", 1) :
        ["<B", ["SVID"]],
    ("AID-EPH", 8) :
        ["<II", ["SVID", "HOW"]],
    ("AID-EPH", 104) :
        ["<" + "I"*26, ["SVID", "HOW", "SF1D0", "SF1D1", "SF1D2", "SF1D3", "SF1D4",
            "SF1D5", "SF1D6", "SF1D7", "SF2D0", "SF2D1", "SF2D2", "SF2D3", "SF2D4",
            "SF2D5", "SF2D6", "SF2D7", "SF3D0", "SF3D1", "SF3D2", "SF3D3", "SF3D4", "SF3D5", "SF3D6", "SF3D7"]]
# TIM - Timekeeping
}

MSGFMT_INV = dict( [ [(CLIDPAIR[clid], le),v + [clid]] for (clid, le),v in MSGFMT.items() ] )

def checksum( msg ):
    ck_a = 0
    ck_b = 0
    for i in msg:
        ck_a = ck_a + ord(i)
        ck_b = ck_b + ck_a
    ck_a = ck_a % 256
    ck_b = ck_b % 256
    return (ck_a, ck_b)

