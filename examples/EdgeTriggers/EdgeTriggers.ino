#include <Standard_PLC.h>

// R_TRIG_Block fields: Q, M
R_TRIG_Block risingEdge = { false, false };

// F_TRIG_Block fields: Q, M
F_TRIG_Block fallingEdge = { false, false };

unsigned long lastToggle = 0;
bool signal = false;

void setup() {
    Serial.begin(9600);
    Serial.println("Standard_PLC - R_TRIG / F_TRIG example");
}

void loop() {
    unsigned long now = millis();

    // Toggle the signal every 1000 ms.
    if (now - lastToggle >= 1000) {
        signal = !signal;
        lastToggle = now;
    }

    processR_TRIG(&risingEdge, signal);
    processF_TRIG(&fallingEdge, signal);

    if (risingEdge.Q) {
        Serial.println("R_TRIG: rising edge detected");
    }
    if (fallingEdge.Q) {
        Serial.println("F_TRIG: falling edge detected");
    }
}
