
#ifndef XPLANECLIENT_H
#define XPLANECLIENT_H

#include <stdbool.h>

bool XPlaneClient_init(const char *ip, unsigned short port);
bool XPlaneClient_send_command(const char *command);

bool XPlaneClient_adjust_heading(int direction);
bool XPlaneClient_adjust_com1(int direction);

void XPlaneClient_poll(void);
void XPlaneClient_close(void);

#endif
