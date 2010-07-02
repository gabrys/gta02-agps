
from ubx import CLIDPAIR
import logger

class UBXDumpHandler():
    def __init__(self, writer):
        self.aid_data = { "almanac": {}, "ephemeris": {}, "position": {}, "hui": {} }
        self.writer = writer

    def check_aid_data(self):
        "Check if aid data is complete and if so return it"
        if self.aid_data["almanac"] and self.aid_data["ephemeris"]:
            if self.aid_data["position"] and self.aid_data["hui"]:
                return self.aid_data

    def handle_NAV_POSECEF(self, data):
        data = data[0]
        self.aid_data["position"]["accuracy"] = data["Pacc"]
        self.aid_data["position"]["x"] = data["ECEF_X"]
        self.aid_data["position"]["y"] = data["ECEF_Y"]
        self.aid_data["position"]["z"] = data["ECEF_Z"]
        self.writer.send("CFG-MSG", 3, {"Class" : CLIDPAIR["NAV-POSECEF"][0] , "MsgID" : CLIDPAIR["NAV-POSECEF"][1] , "Rate" : 0 })
        return self.check_aid_data()

    def handle_AID_ALM( self, data ):
        data = data[0]
        # Save only, if there are values
        if "DWRD0" in data:
            logger.debug("AID_ALM -- " + str(data["SVID"]))
            self.aid_data["almanac"][ data["SVID"] ] = data
        return self.check_aid_data()

    def handle_AID_EPH( self, data ):
        data = data[0]
        # Save only, if there are values
        if "SF1D0" in data:
            logger.debug("AID_EPH -- " + str(data["SVID"]))
            self.aid_data["ephemeris"][ data["SVID"] ] = data
        return self.check_aid_data()

    def handle_AID_HUI( self, data ):
        logger.debug("AID_HUI")
        data = data[0]
        self.aid_data["hui"] = data
        return self.check_aid_data()

