#include "Controller.h"
#include "XPlaneClient.h"
#include <stdio.h>

static PanelMode masterMode;
static bool CvBttn = false;

void Controller_init(void){
    masterMode = MODE_NAV;
    printf("Controller initialized.\n");
}

static bool ModeComHandler(InputEvent event){
    bool success = false;
    if (event.type == INPUT_ENCODER){
        switch (event.id){
        case ENCODER_KHZ:
            if (!CvBttn){
                success = XPlaneClient_cmd_handler(COM1_FINE_STDBY ,event.value);
            } else {
                success = XPlaneClient_cmd_handler(NAV1_FINE_STBDY ,event.value);
            }
            break;
        case ENCODER_MHZ:
            if (!CvBttn){
                success = XPlaneClient_cmd_handler(COM1_COARSE_STDBY ,event.value);
            } else {
                success = XPlaneClient_cmd_handler(NAV1_COARSE_STBDY ,event.value);
            }
            break;
        case ENCODER_SNAV:
            success = XPlaneClient_cmd_handler(G530_FINE, event.value);
            break;
        case ENCODER_BNAV:
            success = XPlaneClient_cmd_handler(G530_COARSE, event.value);
            break;
        
        default:
            printf("Encoder input ID not assigned.\n");
            return false;
        }
    }
    else if ((event.type == INPUT_BUTTON) && (event.value != 0)){
        switch (event.id){
        case BUTTON_PUSH_CV:
            success = XPlaneClient_cmd_handler(BTTN_CV, 1);
            if (success) { 
                CvBttn = !CvBttn;
            }
            break;
        case BUTTON_PUSH_CRSR:
            success = XPlaneClient_cmd_handler(BTTN_CRSR, 1);
            break;
        
        default:
            printf("Button input ID not assigned.\n");
            return false;
        }
    }
    return success;
}

static bool ModeNavHandler(InputEvent event){
    bool success = false;
    if (event.type == INPUT_ENCODER){
        switch (event.id){
        case ENCODER_VSP:
            XPlaneClient_cmd_handler(VS, event.value);
            break;
        case ENCODER_BAR:
            
            break;
        case ENCODER_TRK:
            XPlaneClient_adjst_rref(OBS_NAV1_DATAREF1, event.value);
            XPlaneClient_adjst_rref(OBS_NAV1_DATAREF2, event.value);
            break;
        case ENCODER_HDG:
            XPlaneClient_adjst_rref(HEADING_DATAREF, event.value);
            break;

        default:
            printf("Encoder input ID not assigned.\n");
            return false;
        }
    }
    else if ((event.type == INPUT_BUTTON) && (event.value != 0)){
        switch (event.id){
        case BUTTON_VS:
            success = XPlaneClient_cmd_handler(BTTN_VS, 1);
            break;
        case BUTTON_HDG:
            success = XPlaneClient_cmd_handler(BTTN_HDG, 1);
            break;
        
        default:
            printf("Button input ID not assigned.\n");
            return false;
        }
    return success;
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