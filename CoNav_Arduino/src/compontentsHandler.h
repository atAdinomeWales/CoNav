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

// FUNCTIONS
encoder* encoderInit(int passId, uint8_t pinClk, uint8_t pinDt, uint8_t pinSw, bool Sw);
void encoderRead(encoder *enc);
void encoderReport(bool positive, encoder* enc);
bool encoderProcess(encoder* enc);


#endif//COMPONENTS_HANDLER