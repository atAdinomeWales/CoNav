#ifndef SERIAL_H
#define SERIAL_H

#include <windows.h>
#include <stdbool.h>

bool SerialPort_connect(const char *port, DWORD baud_rate);
bool SerialPort_is_connected(void);
bool SerialPort_read_line(char *buffer, DWORD buffer_size);
void SerialPort_disconnect(void);

#endif
