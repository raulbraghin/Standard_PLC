#include <Standard_PLC.h>

// TOF_Block fields: PT (ms), ET (ms), Q, start_time
TOF_Block stopDelay = { 2000, 0, false, 0 };

void setup() {
    Serial.begin(9600);
    Serial.println("Standard_PLC - TOF (Timer OFF delay) example");
}

void loop() {
    // IN is true for the first 2 s, then goes false. Q stays true
    // for PT milliseconds after IN goes false.
    bool input = (millis() < 2000);

    processTOF(&stopDelay, input, millis());

    Serial.print("IN = ");
    Serial.print(input);
    Serial.print("  Q = ");
    Serial.print(stopDelay.Q);
    Serial.print("  ET = ");
    Serial.println(stopDelay.ET);
    delay(200);
}
