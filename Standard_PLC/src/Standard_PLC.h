#ifndef STANDARD_PLC_H
#define STANDARD_PLC_H

#include <Arduino.h>
#include <stdint.h>
#include <stdbool.h>

// =================================================================
// 1. TIMER BLOCKS (TON, TOF, TP)
// =================================================================

/**
 * @brief TON (Timer ON Delay) function block.
 */
typedef struct {
    uint32_t PT;           // Preset Time in milliseconds
    uint32_t ET;           // Elapsed Time in milliseconds
    bool Q;                // Timer output
    uint32_t start_time;   // Internal time stamp used to start counting
} TON_Block;


/**
 * @brief Runs the TON (ON Delay) timer logic.
 * @param timer Pointer to the TON_Block instance.
 * @param IN Input state (true to start/keep counting).
 * @param current_time Current system time in milliseconds (millis()).
 */
void processTON(TON_Block *timer, bool IN, uint32_t current_time);

/**
 * @brief TOF (Timer OFF Delay) function block.
 */
typedef struct {
    uint32_t PT;           // Preset Time in milliseconds
    uint32_t ET;           // Elapsed Time in milliseconds
    bool Q;                // Timer output
    uint32_t start_time;   // Internal time stamp used to start counting
} TOF_Block;

/**
 * @brief Runs the TOF (OFF Delay) timer logic.
 * @param timer Pointer to the TOF_Block instance.
 * @param IN Input state (true to keep Q active).
 * @param current_time Current system time in milliseconds (millis()).
 */
void processTOF(TOF_Block *timer, bool IN, uint32_t current_time);

/**
 * @brief TP (Timer Pulse) function block.
 */
typedef struct {
    uint32_t PT;           // Pulse duration (Preset Time) in milliseconds
    uint32_t ET;           // Elapsed Time in milliseconds
    bool Q;                // Timer output
    uint32_t start_time;   // Internal time stamp used to start the pulse
    bool IN_old;           // Previous IN state (for rising edge detection)
} TP_Block;

/**
 * @brief Runs the TP (Pulse) timer logic.
 * @param timer Pointer to the TP_Block instance.
 * @param IN Input state (trigger to start the pulse).
 * @param current_time Current system time in milliseconds (millis()).
 */
void processTP(TP_Block *timer, bool IN, uint32_t current_time);

// =================================================================
// 2. COUNTER BLOCKS (CTU, CTD, CTUD)
// =================================================================

/**
 * @brief CTU (Count Up) function block.
 */
typedef struct {
    uint32_t PV;           // Preset Value
    uint32_t CV;           // Current Value
    bool Q;                // Output
    bool CU_old;           // Previous CU state
} CTU_Block;

/**
 * @brief Runs the CTU (Count Up) counter logic.
 * @param counter Pointer to the CTU_Block instance.
 * @param CU Count Up input (active on rising edge).
 * @param R Reset input (true to clear the counter).
 */
void processCTU(CTU_Block *counter, bool CU, bool R);

/**
 * @brief CTD (Count Down) function block.
 */
typedef struct {
    uint32_t PV;           // Preset Value (initial load value)
    uint32_t CV;           // Current Value
    bool Q;                // Output
    bool CD_old;           // Previous CD state
} CTD_Block;

/**
 * @brief Runs the CTD (Count Down) counter logic.
 * @param counter Pointer to the CTD_Block instance.
 * @param CD Count Down input (active on rising edge).
 * @param LD Load input (true to load CV with PV).
 */
void processCTD(CTD_Block *counter, bool CD, bool LD);

/**
 * @brief CTUD (Count Up/Down) function block.
 */
typedef struct {
    uint32_t PV;           // Preset Value
    uint32_t CV;           // Current Value
    bool QU;               // Count Up output
    bool QD;               // Count Down output
    bool CU_old;           // Previous CU state
    bool CD_old;           // Previous CD state
} CTUD_Block;

/**
 * @brief Runs the CTUD (Count Up/Down) counter logic.
 * @param counter Pointer to the CTUD_Block instance.
 * @param CU Count Up input (active on rising edge).
 * @param CD Count Down input (active on rising edge).
 * @param R Reset input (true to clear the counter).
 * @param LD Load input (true to load CV with PV).
 */
void processCTUD(CTUD_Block *counter, bool CU, bool CD, bool R, bool LD);

// =================================================================
// 3. EDGE DETECTION BLOCKS (R_TRIG, F_TRIG)
// =================================================================

/**
 * @brief R_TRIG (Rising Edge Trigger) function block.
 */
typedef struct {
    bool Q;     // Output (true for one execution cycle after the edge)
    bool M;     // Memory of the previous CLK state
} R_TRIG_Block;

/**
 * @brief Runs the rising edge detection (R_TRIG) logic.
 * @param trigger Pointer to the R_TRIG_Block instance.
 * @param CLK Input clock/signal to monitor.
 */
void processR_TRIG(R_TRIG_Block *trigger, bool CLK);

/**
 * @brief F_TRIG (Falling Edge Trigger) function block.
 */
typedef struct {
    bool Q;     // Output (true for one execution cycle after the edge)
    bool M;     // Memory of the previous CLK state
} F_TRIG_Block;

/**
 * @brief Runs the falling edge detection (F_TRIG) logic.
 * @param trigger Pointer to the F_TRIG_Block instance.
 * @param CLK Input clock/signal to monitor.
 */
void processF_TRIG(F_TRIG_Block *trigger, bool CLK);

#endif // STANDARD_PLC_H
