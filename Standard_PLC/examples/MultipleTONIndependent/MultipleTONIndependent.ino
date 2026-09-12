#include <Standard_PLC.h>

// Three independent TON blocks, each with its own input, preset and state.
// Fields: PT (ms), ET (ms), Q, start_time
TON_Block motorStart = { 1000, 0, false, 0 };
TON_Block valveOpen  = { 2500, 0, false, 0 };
TON_Block alarmDelay = { 4000, 0, false, 0 };

// Simulated inputs, enabled one after the other.
bool motorIn = false;
bool valveIn = false;
bool alarmIn = false;

unsigned long lastInput = 0;
unsigned long lastStatus = 0;

void setup() {
    Serial.begin(9600);
    Serial.println("Standard_PLC - Independent TON blocks");
    Serial.println("motor = 1s   valve = 2.5s   alarm = 4s");
    lastInput = millis();
}

void loop() {
    unsigned long now = millis();

    // Turn on one more input every second.
    if (now - lastInput >= 1000) {
        lastInput = now;
        if (!motorIn)      motorIn = true;
        else if (!valveIn) valveIn = true;
        else if (!alarmIn) alarmIn = true;
    }

    // Each block is updated with its own IN; states never interfere.
    processTON(&motorStart, motorIn, now);
    processTON(&valveOpen,  valveIn,  now);
    processTON(&alarmDelay, alarmIn,  now);
    

    // Periodic status table showing the three timers progressing together.
    if (now - lastStatus >= 500) {
        lastStatus = now;

        Serial.print("motor IN=");  Serial.print(motorIn);
        Serial.print(" ET=");       Serial.print(motorStart.ET);
        Serial.print(" Q=");        Serial.print(motorStart.Q);
        Serial.print(" | valve IN="); Serial.print(valveIn);
        Serial.print(" ET=");       Serial.print(valveOpen.ET);
        Serial.print(" Q=");        Serial.print(valveOpen.Q);
        Serial.print(" | alarm IN="); Serial.print(alarmIn);
        Serial.print(" ET=");       Serial.print(alarmDelay.ET);
        Serial.print(" Q=");        Serial.println(alarmDelay.Q);
    }
}
