#ifndef standard_h
#define standard_h
#include <Arduino.h>
/*
class TON
{
private:

public:
    bool ton(bool IN, unsigned long PT, unsigned long &CV, unsigned long &interval);
};

class TOF
{
private:

public:
    bool tof(bool IN, unsigned long PT, unsigned long &CV, unsigned long &interval);
};

class R_TRIG
{
private:
    bool _IN;
public:
    bool r_trig(bool IN);
};

class F_TRIG
{
private:
    bool _IN;
public:
    bool f_trig(bool IN);
};

class TP
{
private:

public:
    bool tp(bool IN, unsigned long PT, unsigned long &CV, unsigned long &Interval);
};

class CTU
{
private:
    
public:
    bool ctu(bool CU, bool RESET, int PV, int &CV);
};

class CTD
{
private:
    
public:
    bool ctd(bool CD, bool RESET, int PV, int &CV);
};




#endif
*/


class standard
{
private:

public:
    bool ton(bool IN, unsigned long PT, unsigned long &CV, unsigned long &interval);
    bool tof(bool IN, unsigned long PT, unsigned long &CV, unsigned long &interval);
    bool r_trig(bool IN);
    bool f_trig(bool IN);
    bool tp(bool IN, unsigned long PT, unsigned long &CV, unsigned long &Interval);
    bool ctu(bool CU, bool RESET, int PV, int &CV);
    bool ctd(bool CD, bool RESET, int PV, int &CV);
};

#endif