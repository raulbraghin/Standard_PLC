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

// =================================================================
// BISTABLE IMPLEMENTATIONS
// =================================================================

void processRS(RS_Block *bistable, bool SET, bool RESET1) {
    bistable->Q1 = !RESET1 && (SET || bistable->Q1);
}

void processSR(SR_Block *bistable, bool SET1, bool RESET) {
    bistable->Q1 = SET1 || (!RESET && bistable->Q1);
}

// =================================================================
// STRING FUNCTION IMPLEMENTATIONS (IEC 61131-3 / CODESYS Standard)
// =================================================================

size_t plc_len(const char *str) {
    if (!str) return 0;
    return strlen(str);
}

char *plc_left(char *dst, size_t dst_size, const char *str, int size) {
    if (!dst || dst_size == 0) return dst;
    if (!str || size <= 0) {
        dst[0] = '\0';
        return dst;
    }
    size_t s_len = strlen(str);
    size_t count = (size_t)size < s_len ? (size_t)size : s_len;
    if (count >= dst_size) {
        count = dst_size - 1;
    }
    if (dst != str) {
        memcpy(dst, str, count);
    }
    dst[count] = '\0';
    return dst;
}

char *plc_right(char *dst, size_t dst_size, const char *str, int size) {
    if (!dst || dst_size == 0) return dst;
    if (!str || size <= 0) {
        dst[0] = '\0';
        return dst;
    }
    size_t s_len = strlen(str);
    size_t count = (size_t)size < s_len ? (size_t)size : s_len;
    const char *start = str + (s_len - count);
    if (count >= dst_size) {
        count = dst_size - 1;
        start = str + (s_len - count);
    }
    memmove(dst, start, count);
    dst[count] = '\0';
    return dst;
}

char *plc_mid(char *dst, size_t dst_size, const char *str, int len, int pos) {
    if (!dst || dst_size == 0) return dst;
    if (!str || len <= 0 || pos <= 0) {
        dst[0] = '\0';
        return dst;
    }
    size_t s_len = strlen(str);
    if ((size_t)pos > s_len) {
        dst[0] = '\0';
        return dst;
    }
    const char *start = str + (pos - 1);
    size_t rem = s_len - (size_t)(pos - 1);
    size_t count = (size_t)len < rem ? (size_t)len : rem;
    if (count >= dst_size) {
        count = dst_size - 1;
    }
    memmove(dst, start, count);
    dst[count] = '\0';
    return dst;
}

char *plc_concat(char *dst, size_t dst_size, const char *str1, const char *str2) {
    if (!dst || dst_size == 0) return dst;
    if (!str1) str1 = "";
    if (!str2) str2 = "";

    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);

    if (dst == str1) {
        if (len1 >= dst_size - 1) {
            dst[dst_size - 1] = '\0';
            return dst;
        }
        size_t avail = dst_size - 1 - len1;
        size_t to_copy = len2 < avail ? len2 : avail;
        memcpy(dst + len1, str2, to_copy);
        dst[len1 + to_copy] = '\0';
    } else {
        size_t c1 = len1 < (dst_size - 1) ? len1 : (dst_size - 1);
        memcpy(dst, str1, c1);
        size_t avail = dst_size - 1 - c1;
        size_t c2 = len2 < avail ? len2 : avail;
        memcpy(dst + c1, str2, c2);
        dst[c1 + c2] = '\0';
    }
    return dst;
}

char *plc_insert(char *dst, size_t dst_size, const char *str1, const char *str2, int pos) {
    if (!dst || dst_size == 0) return dst;
    if (!str1) str1 = "";
    if (!str2) str2 = "";

    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);
    size_t split_pos;

    if (pos <= 0) {
        split_pos = 0;
    } else if ((size_t)pos >= len1) {
        split_pos = len1;
    } else {
        split_pos = (size_t)pos;
    }

    size_t out_idx = 0;

    size_t p1 = split_pos < (dst_size - 1) ? split_pos : (dst_size - 1);
    if (p1 > 0) {
        memmove(dst + out_idx, str1, p1);
        out_idx += p1;
    }

    if (out_idx < dst_size - 1 && len2 > 0) {
        size_t avail = dst_size - 1 - out_idx;
        size_t p2 = len2 < avail ? len2 : avail;
        memmove(dst + out_idx, str2, p2);
        out_idx += p2;
    }

    size_t rem1 = len1 - split_pos;
    if (out_idx < dst_size - 1 && rem1 > 0) {
        size_t avail = dst_size - 1 - out_idx;
        size_t p3 = rem1 < avail ? rem1 : avail;
        memmove(dst + out_idx, str1 + split_pos, p3);
        out_idx += p3;
    }

    dst[out_idx] = '\0';
    return dst;
}

char *plc_delete(char *dst, size_t dst_size, const char *str, int len, int pos) {
    if (!dst || dst_size == 0) return dst;
    if (!str) {
        dst[0] = '\0';
        return dst;
    }

    size_t s_len = strlen(str);
    if (len <= 0 || pos <= 0 || (size_t)pos > s_len) {
        size_t to_copy = s_len < dst_size - 1 ? s_len : dst_size - 1;
        memmove(dst, str, to_copy);
        dst[to_copy] = '\0';
        return dst;
    }

    size_t del_start = (size_t)(pos - 1);
    size_t del_len = (size_t)len;
    size_t del_end = del_start + del_len;
    if (del_end > s_len) {
        del_end = s_len;
    }

    size_t out_idx = 0;

    size_t prefix_len = del_start < dst_size - 1 ? del_start : dst_size - 1;
    if (prefix_len > 0) {
        memmove(dst + out_idx, str, prefix_len);
        out_idx += prefix_len;
    }

    size_t suffix_len = s_len - del_end;
    if (out_idx < dst_size - 1 && suffix_len > 0) {
        size_t avail = dst_size - 1 - out_idx;
        size_t to_copy = suffix_len < avail ? suffix_len : avail;
        memmove(dst + out_idx, str + del_end, to_copy);
        out_idx += to_copy;
    }

    dst[out_idx] = '\0';
    return dst;
}

char *plc_replace(char *dst, size_t dst_size, const char *str1, const char *str2, int len, int pos) {
    if (!dst || dst_size == 0) return dst;
    if (!str1) str1 = "";
    if (!str2) str2 = "";

    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);

    if (pos <= 0 || (size_t)pos > len1 || len <= 0) {
        size_t to_copy = len1 < dst_size - 1 ? len1 : dst_size - 1;
        memmove(dst, str1, to_copy);
        dst[to_copy] = '\0';
        return dst;
    }

    size_t rep_start = (size_t)(pos - 1);
    size_t rep_len = (size_t)len;
    size_t rep_end = rep_start + rep_len;
    if (rep_end > len1) {
        rep_end = len1;
    }

    size_t out_idx = 0;

    size_t p1 = rep_start < dst_size - 1 ? rep_start : dst_size - 1;
    if (p1 > 0) {
        memmove(dst + out_idx, str1, p1);
        out_idx += p1;
    }

    if (out_idx < dst_size - 1 && len2 > 0) {
        size_t avail = dst_size - 1 - out_idx;
        size_t p2 = len2 < avail ? len2 : avail;
        memmove(dst + out_idx, str2, p2);
        out_idx += p2;
    }

    size_t rem1 = len1 - rep_end;
    if (out_idx < dst_size - 1 && rem1 > 0) {
        size_t avail = dst_size - 1 - out_idx;
        size_t p3 = rem1 < avail ? rem1 : avail;
        memmove(dst + out_idx, str1 + rep_end, p3);
        out_idx += p3;
    }

    dst[out_idx] = '\0';
    return dst;
}

int plc_find(const char *str1, const char *str2) {
    if (!str1 || !str2) return 0;
    if (str2[0] == '\0') return 0;
    const char *found = strstr(str1, str2);
    if (!found) return 0;
    return (int)(found - str1) + 1;
}

#if defined(ARDUINO)
int plc_len(const String &str) {
    return (int)str.length();
}

String plc_left(const String &str, int size) {
    if (size <= 0) return "";
    if ((size_t)size >= str.length()) return str;
    return str.substring(0, size);
}

String plc_right(const String &str, int size) {
    if (size <= 0) return "";
    if ((size_t)size >= str.length()) return str;
    return str.substring(str.length() - size);
}

String plc_mid(const String &str, int len, int pos) {
    if (len <= 0 || pos <= 0 || (size_t)pos > str.length()) return "";
    int start_idx = pos - 1;
    int end_idx = start_idx + len;
    if ((size_t)end_idx > str.length()) end_idx = (int)str.length();
    return str.substring(start_idx, end_idx);
}

String plc_concat(const String &str1, const String &str2) {
    return str1 + str2;
}

String plc_insert(const String &str1, const String &str2, int pos) {
    if (pos <= 0) {
        return str2 + str1;
    }
    if ((size_t)pos >= str1.length()) {
        return str1 + str2;
    }
    return str1.substring(0, pos) + str2 + str1.substring(pos);
}

String plc_delete(const String &str, int len, int pos) {
    if (len <= 0 || pos <= 0 || (size_t)pos > str.length()) return str;
    int del_start = pos - 1;
    int del_end = del_start + len;
    if ((size_t)del_end > str.length()) del_end = (int)str.length();
    return str.substring(0, del_start) + str.substring(del_end);
}

String plc_replace(const String &str1, const String &str2, int len, int pos) {
    if (len <= 0 || pos <= 0 || (size_t)pos > str1.length()) return str1;
    int rep_start = pos - 1;
    int rep_end = rep_start + len;
    if ((size_t)rep_end > str1.length()) rep_end = (int)str1.length();
    return str1.substring(0, rep_start) + str2 + str1.substring(rep_end);
}

int plc_find(const String &str1, const String &str2) {
    if (str2.length() == 0) return 0;
    int idx = str1.indexOf(str2);
    if (idx < 0) return 0;
    return idx + 1;
}
#endif
