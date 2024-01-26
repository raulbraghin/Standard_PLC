#ifndef standard_h
#define standard_h
#include <Arduino.h>

class TON
{
private:
    bool _IN;
    unsigned long _PT;
    String _unity;
    static unsigned long _TonMillimsAnt;
    
    int _TonMillisAnt;
    int _TonMillimAnt;
    int _TonMillihAnt;

public:
    bool ton(bool IN, unsigned long PT, String unity = "ms");
};

class TOF
{
private:
    bool _IN;
    unsigned long _PT;
    String _unity;
    static unsigned long _TofMillimsAnt;
    
    int _TofMillisAnt;
    int _TofMillimAnt;
    int _TofMillihAnt;

public:
    bool tof(bool IN, unsigned long PT, String unity = "ms");
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
    bool _IN;
    unsigned long _PT;
    String _unity;
    bool _Start;

    unsigned long _TPMillimsAnt;
    int _TPMillisAnt;
    int _TPMillimAnt;
    int _TPMillihAnt;

public:
    bool tp(bool IN, unsigned long PT, String unity = "ms");
};


#endif