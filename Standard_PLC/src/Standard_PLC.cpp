#include "Standard_PLC.h"

// Elapsed time between two millis() readings. Unsigned subtraction
// handles the millis() wrap-around automatically.
static inline uint32_t elapsedTime(uint32_t current_time, uint32_t start_time) {
    return current_time - start_time;
}

// =================================================================
// TIMER IMPLEMENTATIONS
// =================================================================

void processTON(TON_Block *timer, bool IN, uint32_t current_time) {
    if (IN) {
        if (!timer->Q) {
            if (timer->start_time == 0) {
                timer->start_time = current_time;
                timer->ET = 0;
            } else {
                timer->ET = elapsedTime(current_time, timer->start_time);

                if (timer->ET >= timer->PT) {
                    timer->Q = true;
                    timer->ET = timer->PT;
                }
            }
        } else {
            timer->ET = timer->PT; // Locked at PT while Q is true and IN is true
        }
    } else {
        // Reset
        timer->start_time = 0;
        timer->ET = 0;
        timer->Q = false;
    }
}

void processTOF(TOF_Block *timer, bool IN, uint32_t current_time) {
    if (IN) {
        // Immediate activation and reset of the count
        timer->Q = true;
        timer->ET = 0;
        timer->start_time = 0;
    } else {
        // Counting (OFF delay)
        if (timer->Q) {
            if (timer->start_time == 0) {
                // Start counting on the falling edge of IN
                timer->start_time = current_time;
                timer->ET = 0;
            } else {
                timer->ET = elapsedTime(current_time, timer->start_time);

                if (timer->ET >= timer->PT) {
                    timer->Q = false; // Turn the output off
                    timer->ET = timer->PT;
                }
            }
        } else {
            // Idle state (Q is already false)
            timer->Q = false;
            timer->ET = timer->PT;
            timer->start_time = 0;
        }
    }
}

void processTP(TP_Block *timer, bool IN, uint32_t current_time) {
    bool rising_edge = (IN && !timer->IN_old);

    if (rising_edge) {
        if (!timer->Q) {
            // Start the pulse only if no pulse is in progress
            timer->Q = true;
            timer->start_time = current_time;
            timer->ET = 0;
        }
    }

    if (timer->Q) {
        timer->ET = elapsedTime(current_time, timer->start_time);

        if (timer->ET >= timer->PT) {
            timer->Q = false; // End of pulse
            timer->ET = timer->PT;
            timer->start_time = 0;
        }
    } else {
        timer->ET = 0;
    }

    timer->IN_old = IN; // Update for edge detection on the next cycle
}

// =================================================================
// COUNTER IMPLEMENTATIONS
// =================================================================

void processCTU(CTU_Block *counter, bool CU, bool R) {
    bool rising_edge_CU = (CU && !counter->CU_old);

    if (R) {
        counter->CV = 0;
    } else {
        if (rising_edge_CU) {
            if (counter->CV < UINT32_MAX) {
                counter->CV++;
            }
        }
    }

    counter->Q = (counter->CV >= counter->PV);
    counter->CU_old = CU;
}

void processCTD(CTD_Block *counter, bool CD, bool LD) {
    bool rising_edge_CD = (CD && !counter->CD_old);

    if (LD) {
        counter->CV = counter->PV;
    } else {
        if (rising_edge_CD) {
            if (counter->CV > 0) {
                counter->CV--;
            }
        }
    }

    counter->Q = (counter->CV == 0);
    counter->CD_old = CD;
}

void processCTUD(CTUD_Block *counter, bool CU, bool CD, bool R, bool LD) {
    bool rising_edge_CU = (CU && !counter->CU_old);
    bool rising_edge_CD = (CD && !counter->CD_old);

    if (R) {
        counter->CV = 0;
    } else if (LD) {
        counter->CV = counter->PV;
    } else {
        // Ignore simultaneous CU and CD rising edges
        if (rising_edge_CU && !rising_edge_CD) {
            if (counter->CV < UINT32_MAX) {
                counter->CV++;
            }
        } else if (rising_edge_CD && !rising_edge_CU) {
            if (counter->CV > 0) {
                counter->CV--;
            }
        }
    }

    counter->QU = (counter->CV >= counter->PV);
    counter->QD = (counter->CV == 0);

    counter->CU_old = CU;
    counter->CD_old = CD;
}

// =================================================================
// EDGE DETECTION IMPLEMENTATIONS
// =================================================================

void processR_TRIG(R_TRIG_Block *trigger, bool CLK) {
    trigger->Q = (CLK && !trigger->M);
    trigger->M = CLK;
}

void processF_TRIG(F_TRIG_Block *trigger, bool CLK) {
    trigger->Q = (!CLK && trigger->M);
    trigger->M = CLK;
}
