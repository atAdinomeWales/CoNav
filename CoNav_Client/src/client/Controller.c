#include "Controller.h"
#include "XPlaneClient.h"
#include <stdio.h>

static EncoderMode encoder1_mode;

void Controller_init(void){
    encoder1_mode = MODE_HDG;
    printf("Controller initialized.\n");
}

void Controller_handle_event(InputEvent event){

    if (event.type == INPUT_ENCODER) {
        if (event.id == ENCODER_HDG) {
            bool success = false;

            switch (encoder1_mode) {
                case MODE_HDG:
                    success = XPlaneClient_adjust_heading(event.value);
                    break;

                case MODE_MHZ:
                    success = XPlaneClient_adjust_com1(event.value);
                    break;
            }

            if (!success) {
                printf("Failed to send X-Plane command.\n");
            }
        }
    }

    else if (event.type == INPUT_BUTTON) {
        if (event.id == BUTTON_UNASSIGNED_1 && event.value == 1) {

            encoder1_mode = (encoder1_mode == MODE_HDG)
                          ? MODE_MHZ
                          : MODE_HDG;

            printf("ENC1 mode: %s\n",
                   encoder1_mode == MODE_HDG ? "HDG" : "COM1 MHz");
        }
    }
}
