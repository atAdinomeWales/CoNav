#include "InputParser.h"
#include <stdio.h>
#include <string.h>

bool InputParser_parse(const char *line, InputEvent *event) {
    int id;
    char action[8];

    if (sscanf(line, "ENC%d%2s", &id, action) == 2) {
        event->type = INPUT_ENCODER;
        event->id = id;

        if      (strcmp(action, "+")  == 0) event->value = 1;
        else if (strcmp(action, "++") == 0) event->value = 5;
        else if (strcmp(action, "-")  == 0) event->value = -1;
        else if (strcmp(action, "--") == 0) event->value = -5;
        else return false;

        return true;
    }

    if (sscanf(line, "SW%d, %7s", &id, action) == 2) {
        event->type = INPUT_BUTTON;
        event->id = id;

        if      (strcmp(action, "PRESS")   == 0) event->value = 1;
        else if (strcmp(action, "RELEASE") == 0) event->value = 0;
        else return false;

        return true;
    }

    return false;
}