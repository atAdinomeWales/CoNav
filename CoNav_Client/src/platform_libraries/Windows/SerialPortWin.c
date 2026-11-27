
#include "SerialPortWin.h"
#include <stdio.h>

static HANDLE serial_handle = INVALID_HANDLE_VALUE;

bool SerialPort_connect(const char *port, DWORD baud_rate){
    char device[32];

    snprintf(device, sizeof(device), "\\\\.\\%s", port);

    serial_handle = CreateFileA(
        device,
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        0,
        NULL
    );

    if (serial_handle == INVALID_HANDLE_VALUE)
        return false;

    DCB settings = {0};
    settings.DCBlength = sizeof(settings);

    bool success = GetCommState(serial_handle, &settings);

    if (success) {
        settings.BaudRate = baud_rate;
        settings.ByteSize = 8;
        settings.StopBits = ONESTOPBIT;
        settings.Parity = NOPARITY;

        success = SetCommState(serial_handle, &settings);
    }

    if (success) {
        COMMTIMEOUTS timeouts = {0};
        timeouts.ReadIntervalTimeout = MAXDWORD;

        success = SetCommTimeouts(serial_handle, &timeouts);
    }

    if (!success) {
        CloseHandle(serial_handle);
        serial_handle = INVALID_HANDLE_VALUE;
        return false;
    }

    return true;
}

bool SerialPort_is_connected(void){
    return serial_handle != INVALID_HANDLE_VALUE;
}

bool SerialPort_read_line(char *buffer, DWORD buffer_size){
    static DWORD position = 0;

    if (!SerialPort_is_connected() || buffer_size < 2)
        return false;

    char c;
    DWORD bytes_read;

    while (ReadFile(serial_handle, &c, 1, &bytes_read, NULL)) {
        if (bytes_read == 0)
            return false;

        if (c == '\n') {
            buffer[position] = '\0';
            position = 0;
            return true;
        }

        if (c != '\r' && position < buffer_size - 1)
            buffer[position++] = c;
    }

    return false;
}

void SerialPort_disconnect(void){
    if (SerialPort_is_connected()) {
        CloseHandle(serial_handle);
        serial_handle = INVALID_HANDLE_VALUE;
    }
}
