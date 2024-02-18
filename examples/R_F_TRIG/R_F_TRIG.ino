#include <standard.h>

standard TimerOff;

bool StartRTrig = true;
bool StartFTrig = false;

bool retornoRTrig = false;
bool retornoFTrig = false;

unsigned long CiclicTimer = 0;


void setup() {
  Serial.begin(9600);
  Serial.println("Exemplo Bloco R-TRIG and F_TRIG");
}

void loop() {

  retornoRTrig = TimerOff.r_trig(StartRTrig);
  retornoFTrig = TimerOff.f_trig(StartFTrig);

  if ((millis() - CiclicTimer) > 2500) {
    if (!retornoRTrig && !retornoFTrig) {
      CiclicTimer = millis();
      Serial.println("pulso");
      StartRTrig = true;
      StartFTrig = false;
    }
  }

  if (retornoFTrig) {
    Serial.println("Retorno F_TRIG true");
    StartRTrig = false;
  }


  if (retornoRTrig) {
    Serial.println("Retorno R_TRIG true");
    StartFTrig = true;
  }
}
