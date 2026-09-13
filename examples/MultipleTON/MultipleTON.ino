#include <Standard_PLC.h>

// Several TON blocks running at the same time, sharing the same time base.
// Using an array makes it easy to scale to any number of timers.
const int TIMER_COUNT = 4;

// Fields: PT (ms), ET (ms), Q, start_time
TON_Block timers[TIMER_COUNT] = {
    {  500, 0, false, 0 },
    { 1000, 0, false, 0 },
    { 1500, 0, false, 0 },
    { 2000, 0, false, 0 }
};

// Flag used to report each timer only once when it finishes.
bool reported[TIMER_COUNT];

unsigned long cycleStart = 0;

void setup() {
    Serial.begin(9600);
    Serial.println("Standard_PLC - Multiple simultaneous TON blocks");
    Serial.println("4 TONs with different PT, all counting at the same time.");
    cycleStart = millis();
}

void loop() {
    unsigned long now = millis();

    // All blocks are evaluated every scan. Each one keeps its own
    // state, so they count independently and never interfere.
    for (int i = 0; i < TIMER_COUNT; i++) {
        processTON(&timers[i], true, now);
    }

    // Report each timer once, in the order it completes.
    for (int i = 0; i < TIMER_COUNT; i++) {
        if (timers[i].Q && !reported[i]) {
            reported[i] = true;
            Serial.print("Timer ");
            Serial.print(i);
            Serial.print(" (PT = ");
            Serial.print(timers[i].PT);
            Serial.print(" ms) finished at ");
            Serial.print(now);
            Serial.println(" ms");
        }
    }

    // Restart the whole group every 3 s to repeat the demonstration.
    if (now - cycleStart >= 3000) {
        for (int i = 0; i < TIMER_COUNT; i++) {
            processTON(&timers[i], false, now); // IN = false resets the block
            reported[i] = false;
        }
        cycleStart = now;
        Serial.println("--- restart all timers ---");
    }
}
