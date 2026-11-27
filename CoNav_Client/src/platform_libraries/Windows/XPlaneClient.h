
#ifndef XPLANECLIENT_H
#define XPLANECLIENT_H

#include <stdbool.h>

// VALUES

#define LOCAL_UDP_PORT 49005
#define XPLANE_UDP_PORT 49000
#define UDP_BUFFER_SIZE 2048

#define RREF_PACKET_SIZE 413
#define DREF_PACKET_SIZE 509
#define CMND_PACKET_SIZE 260

#define RREF_RATE_FAST  20
#define RREF_RATE_NORMAL 10
#define RREF_RATE_SLOW   1


typedef enum {
    RREFID_HEADING = 1,
    RREFID_COM1 = 2,
    RREFID_COM2 = 3,
    RREFID_OBS1_1 = 4,
    RREFID_OBS1_2 = 5
} RREFID;

#define HEADING_DATAREF "sim/cockpit2/autopilot/heading_dial_deg_mag_pilot"
#define CONFIRM_TIMEOUT 2000
#define HEADING_TOLERANCE 0.25f

#define OBS_NAV1_DATAREF1 "sim/cockpit/radios/nav1_obs_degm"
#define OBS_NAV1_DATAREF2 "sim/cockpit2/radios/actuators/nav1_course_deg_mag_pilot"

typedef enum {
    COM1_FINE_STDBY,
    NAV1_FINE_STBDY,
    COM1_COARSE_STDBY,
    NAV1_COARSE_STBDY,
    G530_FINE,
    G530_COARSE,
    BTTN_CV,
    BTTN_CRSR,
    BTTN_LNAV,
    BTTN_HDG,
    BTTN_VS,
    VS
} CMDS;

// FUNCTIONS

bool XPlaneClient_adjst_rref(char* dataref, int direction);
bool XPlaneClient_cmd_handler(CMDS cmd, int direction);

bool XPlaneClient_init(const char *ip, unsigned short port);
bool XPlaneClient_send_command(const char *command);

bool XPlaneClient_adjust_dataref_bearing(RREFID id, int direction);

void XPlaneClient_poll(void);
void XPlaneClient_close(void);

#endif
