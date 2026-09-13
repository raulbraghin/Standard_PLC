# Standard_PLC

Standard PLC function blocks (timers, counters and edge triggers) for Arduino.

Portable IEC 61131-3 style function blocks: `TON`, `TOF`, `TP` timers, `CTU`, `CTD`, `CTUD` counters and `R_TRIG`, `F_TRIG` edge detectors. Each block keeps its own state, so you can use as many instances as needed. Pass `millis()` as the current time.

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
- `EdgeTriggers` — R_TRIG / F_TRIG
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

## Requirements

- Any Arduino-compatible board (`architectures=*`)
- No external dependencies

## License

MIT — see [LICENSE](LICENSE).
