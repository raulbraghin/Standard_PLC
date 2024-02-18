#include <standard.h>

//standard.TON TimerOn;
standard TimerOn;

bool RetrunTon1;  //return Ton
bool StartTon1 = true;   //Start Ton
unsigned long CVTon1; //Current Value
unsigned long IntervaloTon1;  //Time on start count time

bool ReturnTon2;
bool StartTon2 = true;
unsigned long TempoTon2 = 5000;  //count 5000 ms
unsigned long CVTon2;
unsigned long IntervaloTon2;

void setup() {
  Serial.begin(9600);
  Serial.println("Exemplo Bloco Ton");

}

void loop() {
  RetrunTon1 = TimerOn.ton(StartTon1, 1000, CVTon1, IntervaloTon1);

  if (RetrunTon1) {
    Serial.println("Retorno Ton1 true");
    StartTon1 = false;
  }

  ReturnTon2 = TimerOn.ton(!ReturnTon2, TempoTon2, CVTon2, IntervaloTon2);  //Ciclic TimerOn

  if (ReturnTon2) {
    StartTon1 = true;
    Serial.println("Retorno Ton2 True");

  }


}
