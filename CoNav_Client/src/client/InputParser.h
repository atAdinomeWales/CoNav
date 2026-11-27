#ifndef INPUTPARSER_H
#define INPUTPARSER_H

#include <stdbool.h>

typedef enum {
    INPUT_ENCODER,
    INPUT_BUTTON,
    INPUT_SWITCH
} InputType;

typedef struct {
    InputType type;
    int id;
    int value;
} InputEvent;

bool InputParser_parse(const char *line, InputEvent *event);

#endif