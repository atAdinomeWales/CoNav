
#include <stdio.h>
#include <conio.h>
#include <windows.h>

#include "SerialPortWin.h"
#include "InputParser.h"
#include "Controller.h"
#include "XPlaneClient.h"

static int wait_for_retry_or_exit(void)
{
    int key;

    printf("\n[R] Retry connection\n");
    printf("[ESC] Exit\n");

    while (1) {
        key = _getch();

        if (key == 'r' || key == 'R') {
            return 1;
        }

        if (key == 27) {
            return 0;
        }
    }
}

int main(void)
{
    char message[128];
    InputEvent event;

    /* Arduino connection */
    while (!SerialPort_connect("COM5", 9600)) {

        printf("\nFailed to connect to Arduino on COM5.\n");

        if (!wait_for_retry_or_exit()) {
            return 0;
        }

        printf("\nRetrying Arduino connection...\n");
    }

    printf("Arduino connected.\n");

    /* X-Plane UDP initialization */
    while (!XPlaneClient_init("127.0.0.1", 49000)) {

        printf("\nFailed to initialize X-Plane UDP client.\n");

        if (!wait_for_retry_or_exit()) {
            SerialPort_disconnect();
            return 0;
        }

        printf("\nRetrying X-Plane connection...\n");
    }

    Controller_init();

    printf("\nCoNav connected. Press ESC to exit.\n");

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

    printf("\nCoNav shut down.\n");

    return 0;
}
