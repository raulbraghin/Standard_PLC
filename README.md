# Standard_PLC

Standard PLC function blocks (timers, counters, bistables, edge triggers) and string functions for Arduino.

Portable IEC 61131-3 style function blocks: `TON`, `TOF`, `TP` timers, `CTU`, `CTD`, `CTUD` counters, `RS`, `SR` bistables, `R_TRIG`, `F_TRIG` edge detectors, and IEC string manipulation functions (`plc_concat`, `plc_find`, `plc_left`, `plc_right`, `plc_mid`, `plc_insert`, `plc_delete`, `plc_replace`, `plc_len`). Each block keeps its own state, so you can use as many instances as needed. Pass `millis()` as the current time.

## Installation

### Arduino Library Manager (once published)

1. Open Arduino IDE → **Sketch → Include Library → Manage Libraries…**
2. Search for **Standard_PLC**
3. Click **Install**

### Manual (ZIP)

1. Download this repository as ZIP (`Code → Download ZIP`)
2. Arduino IDE → **Sketch → Include Library → Add .ZIP Library…**
3. Select the ZIP file

## Quick start

```cpp
#include <Standard_PLC.h>

// TON_Block fields: PT (ms), ET (ms), Q, start_time
TON_Block startDelay = {1000, 0, false, 0};

void setup() {
  Serial.begin(9600);
}

void loop() {
  // IN = true starts the count. Q turns true after PT milliseconds.
  processTON(&startDelay, true, millis());

  if (startDelay.Q) {
    Serial.println("Timer finished!");
    delay(500);
  }
}
```

See `examples/` for full sketches:

- `TimerOn` — TON (ON delay)
- `TimerOff` — TOF (OFF delay)
- `TimerPulse` — TP (pulse)
- `Counters` — CTU / CTD / CTUD
- `Bistables` — RS (reset-dominant) / SR (set-dominant) bistable latches
- `EdgeTriggers` — R_TRIG / F_TRIG
- `StringFunctions` — Standard IEC 61131-3 string manipulation functions (LEN, LEFT, RIGHT, MID, CONCAT, INSERT, DELETE, REPLACE, FIND)
- `MultipleTON` / `MultipleTONIndependent` — multiple independent instances

## API

### Timers

Pass `millis()` as `current_time`. Unsigned subtraction handles `millis()` wrap-around.

| Block | Struct | Function |
|-------|--------|----------|
| TON (ON delay) | `TON_Block { PT, ET, Q, start_time }` | `processTON(&blk, IN, millis())` |
| TOF (OFF delay) | `TOF_Block { PT, ET, Q, start_time }` | `processTOF(&blk, IN, millis())` |
| TP (pulse) | `TP_Block { PT, ET, Q, start_time, IN_old }` | `processTP(&blk, IN, millis())` |

### Counters

Edge-triggered on rising edge of `CU` / `CD`.

| Block | Struct | Function |
|-------|--------|----------|
| CTU (count up) | `CTU_Block { PV, CV, Q, CU_old }` | `processCTU(&c, CU, R)` |
| CTD (count down) | `CTD_Block { PV, CV, Q, CD_old }` | `processCTD(&c, CD, LD)` |
| CTUD (up/down) | `CTUD_Block { PV, CV, QU, QD, CU_old, CD_old }` | `processCTUD(&c, CU, CD, R, LD)` |

### Edge detection

| Block | Struct | Function |
|-------|--------|----------|
| R_TRIG (rising) | `R_TRIG_Block { Q, M }` | `processR_TRIG(&t, CLK)` |
| F_TRIG (falling) | `F_TRIG_Block { Q, M }` | `processF_TRIG(&t, CLK)` |

### Bistables (Latches / Flip-Flops)

Bistable memory blocks conforming to IEC 61131-3 and CODESYS.

| Block | Struct | Function |
|-------|--------|----------|
| RS (reset-dominant) | `RS_Block { Q1 }` | `processRS(&blk, SET, RESET1)` |
| SR (set-dominant) | `SR_Block { Q1 }` | `processSR(&blk, SET1, RESET)` |

### String Functions (IEC 61131-3 / CODESYS Standard.lib)

Standard string manipulation functions with 1-based indexing conforming to CODESYS:

| Function | C Buffer API (Zero Heap Allocation) | Arduino String API | Description |
|----------|-------------------------------------|--------------------|-------------|
| LEN | `plc_len(str)` | `plc_len(str)` | Character length of string |
| LEFT | `plc_left(dst, sizeof(dst), str, size)` | `plc_left(str, size)` | Extract leftmost `size` characters |
| RIGHT | `plc_right(dst, sizeof(dst), str, size)` | `plc_right(str, size)` | Extract rightmost `size` characters |
| MID | `plc_mid(dst, sizeof(dst), str, len, pos)` | `plc_mid(str, len, pos)` | Extract `len` chars starting at 1-based `pos` |
| CONCAT | `plc_concat(dst, sizeof(dst), str1, str2)` | `plc_concat(str1, str2)` | Concatenate `str1` and `str2` |
| INSERT | `plc_insert(dst, sizeof(dst), str1, str2, pos)` | `plc_insert(str1, str2, pos)` | Insert `str2` at `pos` (`pos=0` before 1st char) |
| DELETE | `plc_delete(dst, sizeof(dst), str, len, pos)` | `plc_delete(str, len, pos)` | Delete `len` characters at 1-based `pos` |
| REPLACE | `plc_replace(dst, sizeof(dst), str1, str2, len, pos)` | `plc_replace(str1, str2, len, pos)` | Replace `len` chars at 1-based `pos` with `str2` |
| FIND | `plc_find(str1, str2)` | `plc_find(str1, str2)` | Find 1-based position of `str2` in `str1` (0 if not found) |

## Requirements

- Any Arduino-compatible board (`architectures=*`)
- No external dependencies

## License

MIT — see [LICENSE](LICENSE).
