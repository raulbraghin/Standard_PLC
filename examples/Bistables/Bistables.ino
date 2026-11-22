#include <Standard_PLC.h>

// RS_Block fields: Q1
// RS is reset-dominant: if SET and RESET1 are both true, Q1 will be false.
RS_Block latchRS = { false };

// SR_Block fields: Q1
// SR is set-dominant: if SET1 and RESET are both true, Q1 will be true.
SR_Block latchSR = { false };

unsigned long lastStep = 0;
int step = 0;

void setup() {
    Serial.begin(9600);
    Serial.println("Standard_PLC - Bistables (RS / SR) example");
}

void loop() {
    unsigned long now = millis();

    // Advance demonstration sequence every 1000 ms:
    // Step 0: Idle (inputs false) -> Latches maintain current state
    // Step 1: SET pulse -> Both Q1 become true
    // Step 2: Idle (inputs false) -> Both Q1 remain true (latched)
    // Step 3: Conflict: Set & Reset simultaneously active:
    //         - RS turns false (reset-dominant)
    //         - SR stays true (set-dominant)
    // Step 4: RESET pulse -> Both Q1 turn false
    if (now - lastStep >= 1000) {
        lastStep = now;
        step = (step + 1) % 5;

        bool setSignal = false;
        bool resetSignal = false;

        switch (step) {
            case 0:
                Serial.println("\n--- Step 0: Idle (no signals) ---");
                break;
            case 1:
                Serial.println("\n--- Step 1: SET pulse ---");
                setSignal = true;
                break;
            case 2:
                Serial.println("\n--- Step 2: Idle (maintains latched state) ---");
                break;
            case 3:
                Serial.println("\n--- Step 3: Conflict (SET and RESET both TRUE) ---");
                setSignal = true;
                resetSignal = true;
                break;
            case 4:
                Serial.println("\n--- Step 4: RESET pulse ---");
                resetSignal = true;
                break;
        }

        processRS(&latchRS, setSignal, resetSignal);
        processSR(&latchSR, setSignal, resetSignal);

        Serial.print("RS (Reset-Dominant): Q1 = ");
        Serial.print(latchRS.Q1);
        Serial.print(" | SR (Set-Dominant): Q1 = ");
        Serial.println(latchSR.Q1);
    }
}
