
int AID_DATA_TIMEOUT_S = 10;                       
/* 10 seconds to wait for AID_DATA request */
/* the same timeout applies for collecting AID data */

/*
    NOTE: some testing is needed to better understand how much 
    accuracy parameters are important for UBX chip.
*/

unsigned int POSITION_ACCURACY_MIN_CM = 5 * 1000 * 100; /* 5 km, in cm */
/*
    if last recorded position accuracy was less that POSITION_ACCURACY_MIN_CM,
    send POSITION_ACCURACY_MIN_CM as position accuracy.
    
    This is how far you can go between turning off and on the GPS chip.
*/

unsigned int POSITION_ACCURACY_MAX_CM = 200 * 1000 * 100; /* 200 km in cm */
/*
    if position accuracy is more than POSITION_ACCURACY_MAX_CM,
    don't send the last position to GPS at all.
*/

int TIME_ACCURACY_MS = 10 * 60 * 1000; /* 10 minutes in ms */
/*
    system time accuracy in ms.
    
    If you have your time desynchronized by 10 minutes, you should
    definitely notice it and set the time.
    
    If you have your system time synchronized via NTP (recommenended)
    you can set TIME_ACCURACY_NS to 10000 (which is 10 seconds)
    or even less.
*/
