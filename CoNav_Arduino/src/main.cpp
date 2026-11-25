
#include <Arduino.h>

int lastClk = 1, lastDt = 1, lastSw = 1;
int encoderPosition = 0;


typedef struct {
  unsigned long lastStep = 0;
  uint8_t pinClk;
  uint8_t pinDt; 
  uint8_t pinSw;
  int clk;
  int dt;
  int sw;
} encoder;


encoder* encoderInit(uint8_t pinClk, uint8_t pinDt, uint8_t pinSw){
  encoder *retEnc = (encoder *)malloc(sizeof(encoder));
  retEnc->pinClk = pinClk;
  retEnc->pinDt = pinDt;
  retEnc->pinSw = pinSw;
  pinMode(pinClk, INPUT_PULLUP);
  pinMode(pinDt, INPUT_PULLUP);
  pinMode(pinSw, INPUT_PULLUP);

  return retEnc;
}

void encoderRead(encoder *enc){
  if(!enc){
    return;
  }
  enc->clk = digitalRead(enc->pinClk);
  enc->dt = digitalRead(enc->pinDt);
  enc->sw = digitalRead(enc->pinSw);
  
  return;
}

void encoderReport(bool positive, encoder* enc){
  unsigned long now = micros();
  int step = (now - enc->lastStep < 40000 && enc->lastStep != 0) ? 5 : 1;
  if (!positive) step = -step;

  switch (step) {
    case 5:
      Serial.println("ENC1++");
      break;
    case 1:
      Serial.println("ENC1+");
      break;
    case -1:
      Serial.println("ENC1-");
      break;
    case -5:
      Serial.println("ENC1--");
      break;
}

  enc->lastStep = now;
  return;
}


encoder *ENC1;
void setup() {
  Serial.begin(9600);
  ENC1 = encoderInit(2, 3, 4);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  encoderRead(ENC1);

  int state = (ENC1->clk << 1) | ENC1->dt;
  int lastState = (lastClk << 1) | lastDt;

  if (state != lastState) {
    if ((lastState == 3 && state == 1) ||
        (lastState == 1 && state == 0) ||
        (lastState == 0 && state == 2) ||
        (lastState == 2 && state == 3)) {
      encoderPosition++;
    }
    else if ((lastState == 3 && state == 2) ||
             (lastState == 2 && state == 0) ||
             (lastState == 0 && state == 1) ||
             (lastState == 1 && state == 3)) {
      encoderPosition--;
    }

    if (encoderPosition >= 4) {
      encoderReport(true, ENC1);
      encoderPosition = 0;
    } else if (encoderPosition <= -4) {
      encoderReport(false, ENC1);
      encoderPosition = 0;
    }
  }

  lastClk = ENC1->clk;
  lastDt = ENC1->dt;

  if (lastSw != ENC1->sw) {
    digitalWrite(LED_BUILTIN, HIGH);
    digitalWrite(LED_BUILTIN, LOW);

    if (!ENC1->sw) {
      Serial.println("SW1, PRESS");
    } else {
      Serial.println("SW1, RELEASE");
    }

    lastSw = ENC1->sw;
  }
}