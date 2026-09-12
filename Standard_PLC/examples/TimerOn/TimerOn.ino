#include <Standard_PLC.h>

// TON_Block fields: PT (ms), ET (ms), Q, start_time
TON_Block startDelay = { 1000, 0, false, 0 };

void setup() {
    Serial.begin(9600);
    Serial.println("Standard_PLC - TON (Timer ON delay) example");
}

void loop() {
    // IN = true starts the count. Q turns true after PT milliseconds.
    processTON(&startDelay, true, millis());

    if (startDelay.Q) {
        Serial.print("TON finished. ET = ");
        Serial.println(startDelay.ET);
        delay(500);
    }
}
