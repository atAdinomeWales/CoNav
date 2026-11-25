
#include <stdio.h>
#include <conio.h>
#include <windows.h>

#include "SerialPortWin.h"
#include "InputParser.h"
#include "Controller.h"
#include "XPlaneClient.h"

int main(void)
{
    char message[128];
    InputEvent event;

    if (!SerialPort_connect("COM5", 9600)) {
        printf("Failed to connect to Arduino.\n");
        return 1;
    }

    if (!XPlaneClient_init("127.0.0.1", 49000)) {
        printf("Failed to initialize X-Plane UDP client.\n");
        SerialPort_disconnect();
        return 1;
    }

    Controller_init();

    printf("CoNav connected. Press ESC to exit.\n");

    while (SerialPort_is_connected()) {

        if (_kbhit() && _getch() == 27) {
            break;
        }

        XPlaneClient_poll();

        if (SerialPort_read_line(message, sizeof(message))) {

            if (InputParser_parse(message, &event)) {
                Controller_handle_event(event);
            }
            else {
                printf("Unknown input: %s\n", message);
            }
        }

        Sleep(1);
    }

    XPlaneClient_close();
    SerialPort_disconnect();

    return 0;
}
