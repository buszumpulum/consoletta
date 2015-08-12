#include "consoletta.h"
#define JOY_SCALER 84

const int P_B_MENU = 8;
const int P_B_PAUSE = 1;
const int P_B_UP = 3;
const int P_B_DOWN = 2;
const int P_B_LEFT = 4;
const int P_B_RIGHT = 12;
const int P_B_JOY = 0;
const int P_X_JOY = A1;
const int P_Y_JOY = A0;

int bt_last_state[7];
int bt_before[7];
int bt_state[7];
long last_debounce[7];

int joy_state[2];
int joy_centre[2];

void read_button(const int button_port, const int id) 
{
  int reading = digitalRead(button_port);
  bt_before[id]=bt_state[id];
  if (reading != bt_last_state[id]) 
  {
    last_debounce[id] = millis();
  }

  if ((millis() - last_debounce[id]) > 50) 
  {
    if (reading != bt_state[id]) 
    {
      bt_state[id] = reading;
    }
  }

  bt_last_state[id] = reading; 
}

void init_buttons()
{
  pinMode(P_B_MENU, INPUT);
  pinMode(P_B_PAUSE, INPUT);
  pinMode(P_B_UP, INPUT);
  pinMode(P_B_DOWN, INPUT);
  pinMode(P_B_LEFT, INPUT);
  pinMode(P_B_RIGHT, INPUT);
  pinMode(P_B_JOY, INPUT);
  int i;
  for(i=0;i<7;i++)
  {
    bt_last_state[i]=LOW;
    bt_state[i]=LOW;
    bt_before[i]=LOW;
    last_debounce[i]=millis();
  }
}

int joy_to_direction(const int id)
{
  int reference = ((1023-joy_centre[id])/JOY_SCALER)/2;
  if(joy_state[id]>reference)
    return 1;
  else if(joy_state[id]<-reference)
    return -1;
  else
    return 0;
}
    

void read_joy_ax(const int joy_port, const int id)
{
  int temp=analogRead(joy_port);
  joy_state[id]=(temp-joy_centre[id])/JOY_SCALER;
}

void read_joy_axes()
{
  read_joy_ax(P_X_JOY, X_JOY);
  read_joy_ax(P_Y_JOY, Y_JOY);
}

void init_joy()
{
  joy_centre[X_JOY] = analogRead(X_JOY);
  joy_centre[Y_JOY] = analogRead(Y_JOY);
  joy_state[X_JOY]=0;
  joy_state[Y_JOY]=0;
}

void read_function_buttons()
{
  read_button(P_B_UP, B_UP);
  read_button(P_B_DOWN, B_DOWN);
  read_button(P_B_LEFT, B_LEFT);
  read_button(P_B_RIGHT, B_RIGHT);
  read_button(P_B_JOY, B_JOY);
}

void read_special_buttons()
{
  read_button(P_B_MENU, B_MENU);
  read_button(P_B_PAUSE, B_PAUSE);
}

boolean button_activated(int btn)
{
  return bt_before[btn]==LOW && bt_state[btn]==HIGH;
}

boolean button_deactivated(int btn)
{
  return bt_before[btn]==HIGH && bt_state[btn]==LOW;
}

boolean on_button(int btn)
{
  return bt_state[btn] == HIGH;
}
int read_ax(int ax)
{
  return joy_state[ax];
}

