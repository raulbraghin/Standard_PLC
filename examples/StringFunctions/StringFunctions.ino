#include <Standard_PLC.h>

void setup() {
    Serial.begin(9600);
    while (!Serial) { delay(10); }

    Serial.println("=================================================");
    Serial.println("Standard_PLC - String Functions Example (IEC 61131-3 / CODESYS)");
    Serial.println("=================================================\n");

    // -------------------------------------------------------------
    // 1. C-Buffer API (Embedded Safe, Zero Dynamic Heap Allocation)
    // -------------------------------------------------------------
    Serial.println("--- 1. C-Buffer API (Safe Static Memory) ---");
    char buffer[64];

    // LEN: length of string
    Serial.print("LEN('CODESYS') = ");
    Serial.println((unsigned int)plc_len("CODESYS")); // 7

    // LEFT: leftmost characters
    plc_left(buffer, sizeof(buffer), "CODESYS", 4);
    Serial.print("LEFT('CODESYS', 4) = '");
    Serial.print(buffer);
    Serial.println("'"); // 'CODE'

    // RIGHT: rightmost characters
    plc_right(buffer, sizeof(buffer), "CODESYS", 3);
    Serial.print("RIGHT('CODESYS', 3) = '");
    Serial.print(buffer);
    Serial.println("'"); // 'SYS'

    // MID: substring of length 4 starting at 1-based pos 2
    plc_mid(buffer, sizeof(buffer), "CODESYS", 4, 2);
    Serial.print("MID('CODESYS', 4, 2) = '");
    Serial.print(buffer);
    Serial.println("'"); // 'ODES'

    // CONCAT: combine two strings
    plc_concat(buffer, sizeof(buffer), "PLC_", "Standard");
    Serial.print("CONCAT('PLC_', 'Standard') = '");
    Serial.print(buffer);
    Serial.println("'"); // 'PLC_Standard'

    // INSERT: insert 'XY' into 'SUSI' after pos 2
    plc_insert(buffer, sizeof(buffer), "SUSI", "XY", 2);
    Serial.print("INSERT('SUSI', 'XY', 2) = '");
    Serial.print(buffer);
    Serial.println("'"); // 'SUXYSI'

    // DELETE: delete 3 chars from 'CODESYS' starting at pos 4
    plc_delete(buffer, sizeof(buffer), "CODESYS", 3, 4);
    Serial.print("DELETE('CODESYS', 3, 4) = '");
    Serial.print(buffer);
    Serial.println("'"); // 'CODS'

    // REPLACE: replace 2 chars from 'SUXYSI' at pos 2 with 'K'
    plc_replace(buffer, sizeof(buffer), "SUXYSI", "K", 2, 2);
    Serial.print("REPLACE('SUXYSI', 'K', 2, 2) = '");
    Serial.print(buffer);
    Serial.println("'"); // 'SKYSI'

    // FIND: locate ':' in command string (1-based, 0 if not found)
    int pos = plc_find("CMD:START", ":");
    Serial.print("FIND('CMD:START', ':') = ");
    Serial.println(pos); // 4

    // -------------------------------------------------------------
    // 2. Arduino String API (High-level Ergonomics)
    // -------------------------------------------------------------
    Serial.println("\n--- 2. Arduino String API (Overloaded Objects) ---");
    String sCommand = "SET_SPEED:1500";
    int colonIdx = plc_find(sCommand, ":");

    if (colonIdx > 0) {
        String action = plc_left(sCommand, colonIdx - 1);
        String value = plc_mid(sCommand, plc_len(sCommand) - colonIdx, colonIdx + 1);

        Serial.print("Parsed Action: '");
        Serial.print(action);
        Serial.print("', Value: '");
        Serial.print(value);
        Serial.println("'");
    }

    Serial.println("\nAll string functions demonstrated successfully.");
}

void loop() {
    // Nothing in loop for this demonstration
    delay(1000);
}
