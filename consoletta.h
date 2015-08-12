#ifndef _CONSOLETTA_H
#define _CONSOLETTA_H
#include <Arduino.h>

const int B_MENU = 0;
const int B_PAUSE = 1;
const int B_UP = 2;
const int B_DOWN = 3;
const int B_LEFT = 4;
const int B_RIGHT = 5;
const int B_JOY = 6;
const int X_JOY = 1;
const int Y_JOY = 0;

void init_buttons();
int joy_to_direction(const int id);
void read_joy_axes();
void init_joy();
void read_function_buttons();
void read_special_buttons();
boolean on_button(int btn);
int read_ax(int ax);
boolean button_activated(int btn);
boolean button_deactivated(int btn);

#endif
