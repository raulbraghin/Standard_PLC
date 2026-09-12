#include <Standard_PLC.h>

// CTU_Block fields: PV, CV, Q, CU_old
CTU_Block upCounter = { 5, 0, false, false };

// CTD_Block fields: PV, CV, Q, CD_old
// CV is pre-loaded with PV so the count down starts from 5.
CTD_Block downCounter = { 5, 5, false, false };

// CTUD_Block fields: PV, CV, QU, QD, CU_old, CD_old
CTUD_Block upDown = { 5, 0, false, false, false, false };

unsigned long lastPulse = 0;

void setup() {
    Serial.begin(9600);
    Serial.println("Standard_PLC - Counters example");
}

void loop() {
    unsigned long now = millis();

    // One count pulse (single scan) every 500 ms.
    bool pulse = (now - lastPulse) >= 500;
    if (pulse) {
        lastPulse = now;
    }

    processCTU(&upCounter, pulse, false);
    processCTD(&downCounter, pulse, false);
    processCTUD(&upDown, pulse, false, false, false);

    Serial.print("CTU CV = ");
    Serial.print(upCounter.CV);
    Serial.print(" (Q = ");
    Serial.print(upCounter.Q);
    Serial.print(")  CTD CV = ");
    Serial.print(downCounter.CV);
    Serial.print(" (Q = ");
    Serial.print(downCounter.Q);
    Serial.print(")  CTUD CV = ");
    Serial.print(upDown.CV);
    Serial.print(" (QU = ");
    Serial.print(upDown.QU);
    Serial.print(")");
    Serial.println();

    delay(50);
}
