#include <Standard_PLC.h>

// TP_Block fields: PT (ms), ET (ms), Q, start_time, IN_old
TP_Block pulse = { 500, 0, false, 0, false };

unsigned long lastTrigger = 0;

void setup() {
    Serial.begin(9600);
    Serial.println("Standard_PLC - TP (Timer Pulse) example");
}

void loop() {
    unsigned long now = millis();

    // Trigger a single-scan pulse every 1500 ms.
    bool trigger = false;
    if (now - lastTrigger >= 1500) {
        trigger = true;
        lastTrigger = now;
    }

    processTP(&pulse, trigger, now);

    Serial.print("Q = ");
    Serial.print(pulse.Q);
    Serial.print("  ET = ");
    Serial.println(pulse.ET);
    delay(50);
}
