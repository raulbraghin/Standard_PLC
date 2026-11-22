#ifndef STANDARD_PLC_H
#define STANDARD_PLC_H

#if defined(ARDUINO)
#include <Arduino.h>
#endif
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

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

// =================================================================
// 4. BISTABLE BLOCKS (RS, SR)
// =================================================================

/**
 * @brief RS (Reset-dominant bistable / latch) function block.
 * Output Q1 is reset to false if RESET1 is true, regardless of SET.
 */
typedef struct {
    bool Q1;    // Output state
} RS_Block;

/**
 * @brief Runs the RS (reset-dominant bistable) logic.
 * Logic: Q1 = !RESET1 && (SET || Q1)
 * @param bistable Pointer to the RS_Block instance.
 * @param SET Set input (sets Q1 to true if RESET1 is false).
 * @param RESET1 Reset-dominant input (forces Q1 to false).
 */
void processRS(RS_Block *bistable, bool SET, bool RESET1);

/**
 * @brief SR (Set-dominant bistable / latch) function block.
 * Output Q1 is set to true if SET1 is true, regardless of RESET.
 */
typedef struct {
    bool Q1;    // Output state
} SR_Block;

/**
 * @brief Runs the SR (set-dominant bistable) logic.
 * Logic: Q1 = SET1 || (!RESET && Q1)
 * @param bistable Pointer to the SR_Block instance.
 * @param SET1 Set-dominant input (forces Q1 to true).
 * @param RESET Reset input (resets Q1 to false if SET1 is false).
 */
void processSR(SR_Block *bistable, bool SET1, bool RESET);

// =================================================================
// 5. STRING FUNCTIONS (IEC 61131-3 / CODESYS Standard.lib)
// =================================================================

/**
 * @brief Returns the length of the string (character count).
 * Equivalent to CODESYS LEN(STR).
 * @param str Input null-terminated string.
 * @return Number of characters in str, or 0 if str is NULL.
 */
size_t plc_len(const char *str);

/**
 * @brief Extracts the leftmost 'size' characters from a string.
 * Equivalent to CODESYS LEFT(STR, SIZE).
 * @param dst Destination buffer.
 * @param dst_size Size of destination buffer in bytes.
 * @param str Source string.
 * @param size Number of characters to extract.
 * @return Pointer to dst.
 */
char *plc_left(char *dst, size_t dst_size, const char *str, int size);

/**
 * @brief Extracts the rightmost 'size' characters from a string.
 * Equivalent to CODESYS RIGHT(STR, SIZE).
 * @param dst Destination buffer.
 * @param dst_size Size of destination buffer in bytes.
 * @param str Source string.
 * @param size Number of characters to extract.
 * @return Pointer to dst.
 */
char *plc_right(char *dst, size_t dst_size, const char *str, int size);

/**
 * @brief Extracts a substring of length 'len' starting at position 'pos' (1-based).
 * Equivalent to CODESYS MID(STR, LEN, POS).
 * @param dst Destination buffer.
 * @param dst_size Size of destination buffer in bytes.
 * @param str Source string.
 * @param len Number of characters to extract.
 * @param pos 1-based start position.
 * @return Pointer to dst.
 */
char *plc_mid(char *dst, size_t dst_size, const char *str, int len, int pos);

/**
 * @brief Concatenates str1 and str2 into dst.
 * Equivalent to CODESYS CONCAT(STR1, STR2).
 * @param dst Destination buffer.
 * @param dst_size Size of destination buffer in bytes.
 * @param str1 First string.
 * @param str2 Second string.
 * @return Pointer to dst.
 */
char *plc_concat(char *dst, size_t dst_size, const char *str1, const char *str2);

/**
 * @brief Inserts str2 into str1 at position 'pos'.
 * Equivalent to CODESYS INSERT(STR1, STR2, POS).
 * pos = 0 inserts before the 1st character; pos = 1 inserts after the 1st character.
 * @param dst Destination buffer.
 * @param dst_size Size of destination buffer in bytes.
 * @param str1 Base string.
 * @param str2 String to insert.
 * @param pos Position index.
 * @return Pointer to dst.
 */
char *plc_insert(char *dst, size_t dst_size, const char *str1, const char *str2, int pos);

/**
 * @brief Deletes 'len' characters from 'str' starting at position 'pos' (1-based).
 * Equivalent to CODESYS DELETE(STR, LEN, POS).
 * @param dst Destination buffer.
 * @param dst_size Size of destination buffer in bytes.
 * @param str Source string.
 * @param len Number of characters to delete.
 * @param pos 1-based start position.
 * @return Pointer to dst.
 */
char *plc_delete(char *dst, size_t dst_size, const char *str, int len, int pos);

/**
 * @brief Replaces 'len' characters of str1 starting at position 'pos' (1-based) with str2.
 * Equivalent to CODESYS REPLACE(STR1, STR2, LEN, POS).
 * @param dst Destination buffer.
 * @param dst_size Size of destination buffer in bytes.
 * @param str1 Original string.
 * @param str2 Replacement string.
 * @param len Number of characters to replace.
 * @param pos 1-based start position.
 * @return Pointer to dst.
 */
char *plc_replace(char *dst, size_t dst_size, const char *str1, const char *str2, int len, int pos);

/**
 * @brief Searches for the first occurrence of str2 in str1.
 * Equivalent to CODESYS FIND(STR1, STR2).
 * @param str1 String to search within.
 * @param str2 Substring to look for.
 * @return 1-based position of the first character, or 0 if not found.
 */
int plc_find(const char *str1, const char *str2);

#if defined(ARDUINO)
// Overloaded functions using Arduino String objects
int plc_len(const String &str);
String plc_left(const String &str, int size);
String plc_right(const String &str, int size);
String plc_mid(const String &str, int len, int pos);
String plc_concat(const String &str1, const String &str2);
String plc_insert(const String &str1, const String &str2, int pos);
String plc_delete(const String &str, int len, int pos);
String plc_replace(const String &str1, const String &str2, int len, int pos);
int plc_find(const String &str1, const String &str2);
#endif

#endif // STANDARD_PLC_H
