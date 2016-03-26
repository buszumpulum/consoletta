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

void register_interrupt(byte pin)
{
  *digitalPinToPCMSK(pin) |= bit(digitalPinToPCMSKbit(pin));
  PCIFR |= bit(digitalPinToPCICRbit(pin));
  PCICR |= bit(digitalPinToPCICRbit(pin));
}

void initButton(const int button)
{
  pinMode(button, INPUT);
  register_interrupt((byte)button);
}

ISR(PCINT0_vect) // D8 - D13
{
  bt_before[B_MENU]=bt_state[B_MENU];
  bt_state[B_MENU]=digitalRead(P_B_MENU);
  
  bt_before[B_RIGHT]=bt_state[B_RIGHT];
  bt_state[B_RIGHT]=digitalRead(P_B_RIGHT);
}

ISR(PCINT1_vect) // A0 - A5
{
  
}

ISR(PCINT2_vect) // D0 - D7
{
  bt_before[B_JOY]=bt_state[B_JOY];
  bt_state[B_JOY]=digitalRead(P_B_JOY);
  
  bt_before[B_PAUSE]=bt_state[B_PAUSE];
  bt_state[B_PAUSE]=digitalRead(P_B_PAUSE);
  
  bt_before[B_DOWN]=bt_state[B_DOWN];
  bt_state[B_DOWN]=digitalRead(P_B_DOWN);
  
  bt_before[B_UP]=bt_state[B_UP];
  bt_state[B_UP]=digitalRead(P_B_UP);
  
  bt_before[B_LEFT]=bt_state[B_LEFT];
  bt_state[B_LEFT]=digitalRead(P_B_LEFT);
}

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
  //pinMode(P_B_MENU, INPUT);
  initButton(P_B_MENU);
  //pinMode(P_B_PAUSE, INPUT);
  initButton(P_B_PAUSE);
  //pinMode(P_B_UP, INPUT);
  initButton(P_B_UP);
  //pinMode(P_B_DOWN, INPUT);
  initButton(P_B_DOWN);
  //pinMode(P_B_LEFT, INPUT);
  initButton(P_B_LEFT);
  //pinMode(P_B_RIGHT, INPUT);
  initButton(P_B_RIGHT);
  //pinMode(P_B_JOY, INPUT);
  initButton(P_B_JOY);
  int i;
  for(i=0;i<7;i++)
  {
    bt_state[i]=LOW;
    bt_before[i]=LOW;
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
  
}

void read_special_buttons()
{
    
}

boolean button_activated(int btn)
{
  boolean ret = bt_before[btn]==LOW && bt_state[btn]==HIGH;
  bt_before[btn]=bt_state[btn];
  return ret;
}

boolean button_deactivated(int btn)
{
  boolean ret = bt_before[btn]==HIGH && bt_state[btn]==LOW;
  bt_before[btn]=bt_state[btn];
  return ret;
}

boolean on_button(int btn)
{
  return bt_state[btn] == HIGH;
}
int read_ax(int ax)
{
  return joy_state[ax];
}



