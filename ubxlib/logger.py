
from sys import stderr as logfile, exit

def debug(what):
    logfile.write("D: " + str(what) + "\n")
    logfile.flush()
    pass

def warning(what):
    logfile.write("W: " + str(what) + "\n")
    logfile.flush()
    pass

def error(what):
    logfile.write("E: " + str(what) + "\n")
    logfile.flush()
    exit()

