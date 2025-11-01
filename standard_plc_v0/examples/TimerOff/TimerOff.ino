#include <standard.h>

standard TimerOff;

bool RetrunTof1;  //return Ton
bool StartTof1 = false;   //Start Ton
unsigned long CVTof1; //Current Value
unsigned long IntervaloTof1;  //Time on start count time


void setup() {
  Serial.begin(9600);
  Serial.println("Exemplo Bloco Tof");

}

void loop() {
  RetrunTof1 = TimerOff.tof(!RetrunTof1, 2500, CVTof1, IntervaloTof1);

  if (!RetrunTof1) {
    Serial.println("Retorno Tof1 false");
    StartTof1 = true;
  }



}
