#include <standard.h>
#include <time.h>


/******************************************************************************
IN          = when true, start time
PT          = Preset Time in milliseconds
CV          = Current Value of time since IN=true
Interval    = Comparative Value since IN=true

if IN=true then the time start, when CV=PT then return is true
*/
typedef struct
{
    struct timespec start_time;
    int Duration;
    bool finish;
} sTON;

void timer_task(void *arg) {
    sTON *timer = (sTON *)arg;
    struct timespec current_time;

    long StartTime = timer->start_time.tv_sec;

    if (timer->finish) {
        long elapsed_time = (current_time.tv_sec - StartTime) * 1000 + (current_time.tv_nsec / 1000000);

        if (elapsed_time >= timer->Duration) {
            timer->finish = false;

        }
    }
}
/*
bool TON::ton(bool IN, unsigned long PT, unsigned long &CV, unsigned long &Interval)
{

         static bool Start;
        CV = millis() - Interval;

        if (!IN)
        {
            return false;
        }

        if (IN && !Start)
        {
            Interval = millis();
            Start = true;
        }

        if (IN && (millis() - Interval) >= PT)
        {
            Interval = millis();
            Start = false;
            return true;
        }
        else
        {
            return false;
        }

    return false;
} // FUNÇÃO TON
*/

/******************************************************************************
IN          = when false, start time
PT          = Preset Time in milliseconds
CV          = Current Value of time since IN=true
Interval    = Comparative Value since IN=true

if IN=true then the time start, when CV=PT then return is true
*/
bool standard::tof(bool IN, unsigned long PT, unsigned long &CV, unsigned long &Interval)
{

        static bool Start;
        CV = millis() - Interval;

        if (IN)
        {
            Start = false;
            return true;
        }

        if (!IN && !Start)
        {
            Start = true;
            Interval = millis();
        }

        if (!IN && (millis() - Interval) >= PT)
        {
            Interval = millis();
            return false;
        }
        else
        {
            return true;
        }
    

    return false;
} // função TOF

/******************************************************************************
return true only 1 scan when IN change from false to true
*/
bool standard::r_trig(bool IN)
{

    static bool Start;

    if (IN && !Start)
    {
        Start = true;
        return true;
    }

    if (IN && Start)
    {
        return false;
    }

    if (!IN)
    {
        Start = false;
    }

    return false;

} // r_trig


/******************************************************************************
return true only 1 scan when IN change from true to false
*/
bool standard::f_trig(bool IN)
{

    static bool Start;

    if (!IN && !Start)
    {
        Start = true;
        return true;
    }

    if (!IN && Start)
    {
        return false;
    }

    if (IN)
    {
        Start = false;
    }

    return false;

} // f_trig

/******************************************************************************
IN          = when true, retrun true and start time
PT          = Preset Time in milliseconds
CV          = Current Value of time since IN=true
Interval    = Comparative Value since IN=true

if IN=true and CV=PT then return change to false
*/
bool standard::tp(bool IN, unsigned long PT, unsigned long &CV, unsigned long &Interval)
{

  
        static bool Start;
        CV = millis() - Interval;

        if (IN && !Start)
        {
            Interval = millis();
            Start = true;
            return true;
        }

        if (!IN)
        {
            Start = false;
            return false;
        }

        if (IN && (millis() - Interval) >= PT)
        {
            Start = false;
            return false;
        }
        else
        {
            return true;
        }

    return false;
} // FUNÇÃO TP

/******************************************************************************
CU    = when true, increase CV
RESET = make CV=0
PV    = Preset Value
CV    = Comparative Value with PV

if CU=true then increase CV. if CV=PV then return true. if RESET=TRUE CV=0 and return=false
*/
bool standard::ctu(bool CU, bool RESET, int PV, int &CV){

    static bool FS = false;

    if (!FS) {
        CV = 0;
        FS = true;
    }

    if (CU) {
        CV += 1;
    }

    if (RESET) {
        CV = 0;
    }

    if (CV >= PV) {
        return true;
    }

    return false;
}   //FIM CTU


/******************************************************************************
CD    = when true, decrease CV
RESET = make CV=PV
PV    = Preset Value
CV    = Comparative Value with 0

if CD=true then decrease CV. if CV=0 then return true. if RESET=TRUE CV=PV and return=false
*/
bool standard::ctd(bool CD, bool RESET, int PV, int &CV){
    
    static bool FS = false;

    if (!FS) {
        CV = PV;
        FS = true;
    }

    if (CD) {
        CV -= 1;
    }

    if (RESET) {
        CV = PV;
    }

    if (CV < 0) {
        CV = 0;
    }

    if (CV == 0) {
        return true;
    }

    return false;
}   //FIM CTD