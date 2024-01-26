#include <Arduino.h>
#include <standard.h>

bool TON::ton(bool IN, unsigned long PT, String unity)
{

    if (unity == "ms") // millisegundos
    {

        static bool Start;
        static unsigned long TonMillimsAnt = 0;

        if (!IN)
        {
            Start = false;
        }

        if (IN && !Start)
        {
            TonMillimsAnt = millis();
            Start = true;
        }

        if (Start && (millis() - TonMillimsAnt) >= PT)
        {
            // Serial.println("return true");
            return true;
        }
        else
        {
            // Serial.println("return false");
            return false;
        }
    }

    if (unity == "s") // segundos
    {
        static bool Start;
        static int TonMillisAnt = 0;

        if (!IN)
        {
            Start = false;
        }

        if (IN && !Start)
        {
            TonMillisAnt = millis() / 1000;
            Start = true;
        }

        if (Start && ((millis() / 1000) - TonMillisAnt) >= PT)
        {

            return true;
        }
        else
        {
            return false;
        }
    }

    if (unity == "m") // minutos
    {
        static bool Start;
        static int TonMillimAnt = 0;

        if (!IN)
        {
            Start = false;
        }

        if (IN && !Start)
        {
            TonMillimAnt = millis() / 60000;
            Start = true;
        }

        if (Start && ((millis() / 60000) - TonMillimAnt) >= PT)
        {

            return true;
        }
        else
        {
            return false;
        }
    }

    if (unity == "h")
    {
        static bool Start;
        static int TonMillihAnt = 0;

        if (!IN)
        {
            Start = false;
        }

        if (IN && !Start)
        {
            TonMillihAnt = millis() / 36000000;
            Start = true;
        }

        if (Start && ((millis() / 36000000) - TonMillihAnt) >= PT)
        {

            return true;
        }
        else
        {
            return false;
        }
    }

    return false;
} // FUNÇÃO TON

bool TOF::tof(bool IN, unsigned long PT, String unity)
{

    if (unity == "ms") // millisegundos
    {
        static bool Start;
        static unsigned long TofMillimsAnt = 0;

        if (IN)
        {
            Start = false;
            return true;
        }

        if (!IN && !Start)
        {
            Start = true;
            TofMillimsAnt = millis();
        }

        if (Start && (millis() - TofMillimsAnt) >= PT)
        {
            return false;
        }
        else
        {
            return true;
        }
    }

    if (unity == "s") // Segundos
    {
        static bool Start;
        static int TofMillisAnt = 0;

        if (IN)
        {
            Start = false;
            return true;
        }

        if (!IN && !Start)
        {
            Start = true;
            TofMillisAnt = millis() / 1000;
        }

        if (Start && ((millis() / 1000) - TofMillisAnt) >= PT)
        {
            return false;
        }
        else
        {
            return true;
        }
    }

    if (unity == "m") // Minutos
    {
        static bool Start;
        static int TofMillimAnt = 0;

        if (IN)
        {
            Start = false;
            return true;
        }

        if (!IN && !Start)
        {
            Start = true;
            TofMillimAnt = millis() / 60000;
        }

        if (Start && ((millis() / 60000) - TofMillimAnt) >= PT)
        {
            return false;
        }
        else
        {
            return true;
        }
    }

    if (unity == "h") // Horas
    {
        static bool Start;
        static int TofMillihAnt = 0;

        if (IN)
        {
            Start = false;
            return true;
        }

        if (!IN && !Start)
        {
            Start = true;
            TofMillihAnt = millis() / 36000000;
        }

        if (Start && ((millis() / 36000000) - TofMillihAnt) >= PT)
        {
            return false;
        }
        else
        {
            return true;
        }
    }

    return false;
} // função TOF

bool R_TRIG::r_trig(bool IN)
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

bool F_TRIG::f_trig(bool IN)
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

bool TP::tp(bool IN, unsigned long PT, String unity)
{

    if (unity == "ms") // millisegundos
    {

        static bool Start;
        static unsigned long TPMillimsAnt = 0;

        if (IN && !Start)
        {
            TPMillimsAnt = millis();
            Start = true;
            return true;
        }

        if (!IN)
        {
            Start = false;
            return false;
        }

        if (Start && (millis() - TPMillimsAnt) >= PT)
        {
            // Serial.println("return true");
            Start = false;
            return false;
        }
        else
        {
            // Serial.println("return false");
            return true;
        }
    }

    if (unity == "s") // segundos
    {
        static bool Start;
        static int TPMillisAnt = 0;

        if (IN && !Start)
        {
            TPMillisAnt = millis() / 1000;
            Start = true;
            return true;
        }

        if (!IN)
        {
            Start = false;
            return false;
        }

        if (Start && ((millis() / 1000) - TPMillisAnt) >= PT)
        {

            Start = false;
            return false;
        }
        else
        {
            return true;
        }
    }

    if (unity == "m") // minutos
    {
        static bool Start;
        static int TPMillimAnt = 0;

        if (IN && !Start)
        {
            TPMillimAnt = millis() / 60000;
            Start = true;
            return true;
        }

        if (!IN)
        {
            Start = false;
            return false;
        }

        if (Start && ((millis() / 60000) - TPMillimAnt) >= PT)
        {

            Start = false;
            return false;
        }
        else
        {
            return true;
        }
    }

    if (unity == "h")
    {
        static bool Start;
        static int TPMillihAnt = 0;

        if (IN && !Start)
        {
            TPMillihAnt = millis() / 36000000;
            Start = true;
            return true;
        }

        if (!IN)
        {
            Start = false;
            return false;
        }

        if (Start && ((millis() / 36000000) - TPMillihAnt) >= PT)
        {

            Start = false;
            return false;
        }
        else
        {
            return true;
        }
    }

    return false;
} // FUNÇÃO TP