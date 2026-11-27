#include "Controller.h"
#include "XPlaneClient.h"
#include <stdio.h>

static PanelMode masterMode;

void Controller_init(void){
    masterMode = MODE_NAV;
    printf("Controller initialized.\n");
}

static void ModeComHandler(InputEvent event){
    bool success = false;
    if (event.type == INPUT_ENCODER){
        switch (event.id){
        case ENCODER_KHZ:
            success = XPlaneClient_adjust_com1(event.value);
            break;
        case ENCODER_MHZ:
            success = XPlaneClient_adjust_com1(event.value);
            break;
        case ENCODER_SNAV:
            
            break;
        case ENCODER_BNAV:
            
            break;
        default:
            printf("Encoder input ID not assigned.\n");
            return;
        }
    }
}

static void ModeNavHandler(InputEvent event){
    if (event.type == INPUT_ENCODER){
        switch (event.id){
        case ENCODER_KHZ:
            XPlaneClient_adjst_rref(HEADING_DATAREF, event.value);
            break;
        case ENCODER_MHZ:
            
            break;
        case ENCODER_SNAV:
            
            break;
        case ENCODER_BNAV:
            
            break;
        default:
            printf("Encoder input ID not assigned.\n");
            return;
        }
    }
}

void Controller_handle_event(InputEvent event){
    if (masterMode == MODE_COM){
        ModeComHandler(event);
    } else if (masterMode == MODE_NAV){
        ModeNavHandler(event);
    }

    if (event.type == INPUT_SWITCH) {
        if (event.id == SWITCH_MODE_MASTER) {
            if (event.value == MODE_COM){
                masterMode = MODE_COM;
                printf("Master Mode: COM\n");
            } else if (event.value == MODE_NAV){
                masterMode = MODE_NAV;
                printf("Master Mode: NAV\n");
            }
        }
    }
}