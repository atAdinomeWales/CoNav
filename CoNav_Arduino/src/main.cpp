#include <Arduino.h>
#include "compontentsHandler.h"
#include "pinMaster.h"

encoder *ENC1;
encoder *ENC2;

void setup(){
  Serial.begin(9600);
  ENC1 = encoderInit(1, pENC1_CLK, pENC1_DT, pENC1_SW, true);
  ENC2 = encoderInit(2, pENC2_CLK, pENC2_DT, NO_SW, false);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  encoderRead(ENC1);
  encoderRead(ENC2);

  encoderProcess(ENC1);
  encoderProcess(ENC2);
}