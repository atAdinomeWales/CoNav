
#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "InputParser.h"

typedef enum {
    ENCODER_VSP = 1,
    ENCODER_BAR = 2,
    ENCODER_TRK = 3,
    ENCODER_HDG = 4,
} NavEncoderID;

typedef enum {
    ENCODER_KHZ = 1,
    ENCODER_MHZ = 2,
    ENCODER_SNAV = 3,
    ENCODER_BNAV = 4,
} ComEncoderID;

typedef enum {
    BUTTON_PUSH_CV = 1,
    BUTTON_PUSH_CRSR = 4
} NavButtonID;

typedef enum {
    BUTTON_LNAV = 1,
    BUTTON_HDG = 4
} ComButtonID;

typedef enum {
    SWITCH_MODE_MASTER = 1
} SwitchID;

typedef enum {
    MODE_NAV,
    MODE_COM
} PanelMode;


// FUNCTIONS

void Controller_init(void);
void Controller_handle_event(InputEvent event);

#endif
