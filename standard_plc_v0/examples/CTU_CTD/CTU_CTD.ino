#include <standard.h>

standard TimerOff;

bool xCountUP = false;
int iCountUpPv = 10;
int iCountUPCV;

bool xCountDW = false;
int iCountDWPv = 7;
int iCountDWCV;

unsigned long CiclicTimer = 0;
bool StartPulso = true;


void setup() {
  Serial.begin(9600);
  Serial.println("Exemplo Bloco CTU CTD");
}

void loop() {

  xCountUP = TimerOff.ctu(StartPulso, xCountUP, iCountUpPv, iCountUPCV);

  if (StartPulso) {
    Serial.print("iCountUPCV =");
    Serial.println(iCountUPCV);
  }

  if (xCountUP) {
    Serial.println("Retorno CountUP true");
  }


  xCountDW = TimerOff.ctd(StartPulso, xCountDW, iCountDWPv, iCountDWCV);

  if (StartPulso) {
    Serial.print("iCountDWCV =");
    Serial.println(iCountDWCV);
  }

  if (xCountDW) {
    Serial.println("Retorno CountDW true");
  }

  StartPulso = false;

    if ((millis() - CiclicTimer) > 2500) {
    
      CiclicTimer = millis();
      StartPulso = true;
    
  }

}
