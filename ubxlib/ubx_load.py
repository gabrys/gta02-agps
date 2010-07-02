
from ubx import CLIDPAIR
from datetime import datetime, timedelta
import logger

class UBXLoadHandler(object):
    def __init__(self, writer, aid_data):
        self.aid_data = aid_data
        self.writer = writer

    def handle_AID_DATA(self, data):
        pos = self.aid_data.get("position", {})

        # Let's just try some random values here and see how well this goes
        pacc = 100 * 1000 * 100 # in cm (100 km)

        # GPS week number
        # FIXME: The Global Positioning System (GPS) epoch is January 6, 1980 UTC.
        epoch = datetime(1980, 1, 6)
        now = datetime.utcnow()

        gpstime = (now - epoch)

        # Week number
        wn = gpstime.days / 7

        try:
            leapsecs = self.aid_data["hui"]["UTC_LS"]
        except:
            # If we don't have current leap seconds yet assume 14 seconds.
            leapsecs = 14

        # GPS time of week
        towdelta = gpstime - timedelta(weeks=wn)
        tow = (towdelta.days * 86400 + towdelta.seconds + leapsecs) * 1000

        # Time accuracy needs to be changed, because the RTC is imprecise
        tacc = 10 * 60 * 1000 # in ms (10 minutes)

        # We don't want the position to be valid if we don't know it
        if pos:
            flags = 0x03
        else:
            flags = 0x02

        # Feed gps with ephemeris
        if self.aid_data.get( "ephemeris", {} ):
            for k, a in self.aid_data["ephemeris"].iteritems():
                logger.debug("Loaded ephemeris for SV %d" % a["SVID"])
                self.writer.send("AID-EPH", 104, a);

        # Feed GPS with position and time
        self.writer.send("AID-INI", 48, {"X" : pos.get("x", 0) , "Y" : pos.get("y", 0) , "Z" : pos.get("z", 0), \
                  "POSACC" : pacc, "TM_CFG" : 0 , "WN" : wn , "TOW" : tow , "TOW_NS" : 0 , \
                  "TACC_MS" : tacc , "TACC_NS" : 0 , "CLKD" : 0 , "CLKDACC" : 0 , "FLAGS" : flags })

        if self.aid_data.get( "hui", {} ):
            self.writer.send("AID-HUI", 72, self.aid_data["hui"])

        # Feed gps with almanac
        if self.aid_data.get( "almanac", {} ):
            for k, a in self.aid_data["almanac"].iteritems():
                logger.debug("Loaded almanac for SV %d" % a["SVID"])
                self.writer.send("AID-ALM", 40, a);

        return True # ready

