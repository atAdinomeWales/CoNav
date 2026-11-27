#ifndef COMPONENTS_HANDLER
#define COMPONENTS_HANDLER

typedef struct {
  int id;
  unsigned long lastStep;
  bool hasSw;
  uint8_t pinClk, pinDt, pinSw;
  int clk, dt, sw;
  int lastClk, lastDt, lastSw;
  int position;
} encoder;

typedef struct {
  uint8_t pin;
  uint8_t state;
  uint8_t lastState;
  uint8_t id;
} switch_t;


encoder* encoderInit(int passId, uint8_t pinClk, uint8_t pinDt, uint8_t pinSw, bool Sw);
void encoderRead(encoder *enc);
void encoderReport(encoder* enc, bool positive);
bool encoderProcess(encoder* enc);

switch_t* switchInit(uint8_t id, uint8_t pin);
bool switchReport(switch_t* sw, bool forcedReport);

#endif//COMPONENTS_HANDLER