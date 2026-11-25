
#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "InputParser.h"

typedef enum {
    ENCODER_HDG = 1
} EncoderID;

typedef enum {
    BUTTON_UNASSIGNED_1 = 1
} ButtonID;

typedef enum {
    MODE_HDG,
    MODE_MHZ
} EncoderMode;

void Controller_init(void);
void Controller_handle_event(InputEvent event);

#endif
