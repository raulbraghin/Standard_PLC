# Standard_PLC

IEC 61131-3-style function blocks for Arduino. The library provides timers,
counters, and edge detectors with independent state for each block instance.

## Installation

### Arduino IDE

1. Download or clone this repository into your Arduino libraries directory.
2. Restart the Arduino IDE.
3. Include the library in a sketch:

```cpp
#include <Standard_PLC.h>
```

Once the library is accepted by the Arduino Library Manager, it can also be
installed through **Sketch > Include Library > Manage Libraries**.

## Usage

Timer preset times are expressed in milliseconds. Pass the current value of
`millis()` to timer processing functions on every loop iteration.

```cpp
#include <Standard_PLC.h>

TON_Block startDelay = {1000, 0, false, 0};

void setup() {
    Serial.begin(9600);
}

void loop() {
    processTON(&startDelay, true, millis());

    if (startDelay.Q) {
        Serial.println("Timer finished");
    }
}
```

Each block keeps its own state, so multiple instances can be used
independently.

## Available function blocks

### Timers

- `TON_Block` and `processTON`: on-delay timer.
- `TOF_Block` and `processTOF`: off-delay timer.
- `TP_Block` and `processTP`: pulse timer triggered by a rising edge.

### Counters

- `CTU_Block` and `processCTU`: up counter.
- `CTD_Block` and `processCTD`: down counter.
- `CTUD_Block` and `processCTUD`: up/down counter.

Counter inputs count rising edges. The `PV` field is the preset value and `CV`
is the current value.

### Edge detectors

- `R_TRIG_Block` and `processR_TRIG`: rising-edge detector.
- `F_TRIG_Block` and `processF_TRIG`: falling-edge detector.

## Examples

The `examples/` directory contains sketches demonstrating:

- Timers: `TimerOn`, `TimerOff`, and `TimerPulse`.
- Counters: `Counters`.
- Edge detection: `EdgeTriggers`.
- Independent timer instances: `MultipleTON` and `MultipleTONIndependent`.

Open an example from **File > Examples > Standard_PLC** after installing the
library.

## Library layout

```text
library.properties
keywords.txt
src/
  Standard_PLC.h
  Standard_PLC.cpp
examples/
```

## License

See the repository for license information.
