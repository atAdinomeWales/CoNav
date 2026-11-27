#include <Arduino.h>
#include "compontentsHandler.h"
#include "pinMaster.h"

encoder *ENC1;
encoder *ENC2;
encoder *ENC3;
encoder *ENC4;

switch_t *SWT1;

void setup(){
  Serial.begin(9600);
  ENC1 = encoderInit(1, pENC1_CLK, pENC1_DT, pENC1_SW, true);
  ENC2 = encoderInit(2, pENC2_CLK, pENC2_DT, NO_SW, false);
  ENC3 = encoderInit(3, pENC3_CLK, pENC3_DT, NO_SW, false);
  ENC4 = encoderInit(4, pENC4_CLK, pENC4_DT, pENC4_SW, true);

  SWT1 = switchInit(1, pSWTCH_MODE);

  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  encoderRead(ENC1);
  encoderRead(ENC2);
  encoderRead(ENC3);
  encoderRead(ENC4);

  encoderProcess(ENC1);
  encoderProcess(ENC2);
  encoderProcess(ENC3);
  encoderProcess(ENC4);

  switchReport(SWT1, false);
}