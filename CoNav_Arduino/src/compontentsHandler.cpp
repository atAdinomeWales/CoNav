#include <Arduino.h>
#include "compontentsHandler.h"


encoder* encoderInit(int passId, uint8_t pinClk, uint8_t pinDt, uint8_t pinSw, bool Sw){
  encoder *retEnc = (encoder *)calloc(1, sizeof(encoder));
  retEnc->id = passId;
  retEnc->pinClk = pinClk;
  retEnc->pinDt = pinDt;
  pinMode(pinClk, INPUT_PULLUP);
  pinMode(pinDt, INPUT_PULLUP);
  if (Sw){
    retEnc->pinSw = pinSw;
    pinMode(pinSw, INPUT_PULLUP);
    retEnc->hasSw = true;
    retEnc->sw = HIGH;
    retEnc->lastSw = HIGH;
  } else {
    retEnc->pinSw = 0;
    retEnc->hasSw = false;
    retEnc->position = 0;
  }

  return retEnc;
}

void encoderRead(encoder *enc){
  if(!enc){
    return;
  }
  enc->clk = digitalRead(enc->pinClk);
  enc->dt = digitalRead(enc->pinDt);
  if (enc->hasSw){
    enc->sw = digitalRead(enc->pinSw);
  }
  return;
}

void encoderReport(encoder* enc, bool positive){
  if (!enc) return;

  unsigned long now = micros();
  bool fast = (enc->lastStep != 0 && now - enc->lastStep < 40000);

  Serial.print("ENC");
  Serial.print(enc->id);
  if (positive) Serial.println(fast ? "++" : "+");
  else          Serial.println(fast ? "--" : "-");

  enc->lastStep = now;
  return;
}

bool encoderProcess(encoder* enc){ // Logic for rotation as per a full cycle of 4 + switch logic
  if (!enc) return false;

  int state = (enc->clk << 1) | enc->dt;
  int lastState = (enc->lastClk << 1) | enc->lastDt;

  if (state != lastState){
    if ((lastState == 3 && state == 1) || // Clk fall, Data awaits
        (lastState == 1 && state == 0) || // Data falls after clk did
        (lastState == 0 && state == 2) || // Clk raises back
        (lastState == 2 && state == 3)) { // Data follows
      enc->position++;
    } else 
    if ((lastState == 3 && state == 2) ||
        (lastState == 2 && state == 0) ||
        (lastState == 0 && state == 1) ||
        (lastState == 1 && state == 3)) {
      enc->position--;
    }

    if (enc->position >= 4){
      encoderReport(enc, true);
      enc->position = 0;
    } else if (enc->position <= -4){
      encoderReport(enc, false);
      enc->position = 0;
    }
  }
  enc->lastClk = enc->clk;
  enc->lastDt = enc->dt;
  // Rotation logic block over
  
  if (enc->hasSw == false){
    return true;
  }

  if (enc->lastSw != enc->sw){
    digitalWrite(LED_BUILTIN, HIGH);
    digitalWrite(LED_BUILTIN, LOW);

    if (enc->sw == LOW){
      Serial.print("SW"); Serial.print(enc->id); Serial.println(", PRESS");
    } else if (enc->sw == HIGH){
      Serial.print("SW"); Serial.print(enc->id); Serial.println(", RELEASE");
    }

    enc->lastSw = enc->sw;
  }
  return true;
}

switch_t* switchInit(uint8_t id, uint8_t pin){
  switch_t* sw = (switch_t*)malloc(sizeof(switch_t));
  if (!sw) return NULL;

  sw->id = id;
  sw->pin = pin;
  pinMode(pin, INPUT_PULLUP);
  sw->state = digitalRead(id);
  sw->lastState = sw->state;

  return sw;
}

bool switchReport(switch_t* sw, bool forcedReport){
  if (!sw || sw->id == 0) return false;

  sw->state = digitalRead(sw->pin);

  if (forcedReport){
    Serial.print("SWT"); Serial.print(sw->id); Serial.print(", "); Serial.println(sw->state);
    sw->lastState = sw->state;
    return true;
  }

  if (sw->lastState != sw->state){
    Serial.print("SWT"); Serial.print(sw->id); Serial.print(", "); Serial.println(sw->state);
    sw->lastState = sw->state;
  }

  return true;
}