#include <SPI.h> // biblioteka do komunikacji z nokiaLCD
#include "nokiaLCD.h" // biblioteka do obslugi wyswietlacza
#include "consoletta.h" // biblioteka do obslugi przyciskow i joysticka
#include "images.h" // plik z obrazami

#define ENTRIES 2 // liczba pozycji menu
#define PONG  0 // id ponga
#define DEMO  1 // id demo

#define WHITE       0  // biale punkty
#define BLACK       1  // czarne punkty

//wyswietla intro
void intro()
{
  updateDisplay();
  delay(1000);
  
  setBitmap(arduino_logo);
  updateDisplay();
  delay(1000);
  
  setBitmap(logo);
  updateDisplay();
  delay(2000);
  //plynne przejscie z koloru do ciemnosci, trwa 2s
  backlightAnimate(255, 0, 2000);
}

//wyswietlanie i logika menu
int menu()
{
  clearDisplay(WHITE);
  setBacklight(255);
  byte pos = 0;//pozycja menu
  boolean redraw = true;
  boolean locked = false;//blokada joysticka
  while(true)
  {
    read_function_buttons();
    if(redraw)
    {//przerysowanie, tylko przy zmianie zawartosci
      if(pos==PONG)
        setBitmap(pong_logo);
      if(pos==DEMO)
        setBitmap(demo_logo);
      updateDisplay();
      redraw = false;
    }
    read_joy_axes();
    read_function_buttons();
    int action = joy_to_direction(X_JOY);
    if(!locked)
    {//reakcja na ruch joystickiem
      if(action!=0)
      {
        pos = pos+action < 0 ? ENTRIES-1 : pos+action;
        pos = pos > ENTRIES-1 ? 0 : pos;
        redraw=true;
        locked=true;
      } 
    }
    else
    {//odblokowanie joysticka
      if(action==0)
        locked=false;
    }
    //aktywacja wybranej pozycji
    if(button_activated(B_UP))
      break;
  }
  return pos;//zwrot identyfikatora pozycji
}

//wyswietlanie i logika ponga
void pong()
{
  boolean paused=false;
  long timer=millis();
  randomSeed(timer);
  int p1_position = 2400;//pozycja paletki gracza 1
  int p2_position = 2400;//pozycja paletki gracza 2
  byte p1_result=0;//wynik gracza 1
  byte p2_result=0;//wynik gracza 2
  int ball_x=4200;//wspolrzedna x pilki
  int ball_y=2400;//wspolrzedna y pilki
  int ball_V_x=random(-5,5);//losowa skladowa x predkosci
  if(ball_V_x==0)
    ball_V_x=5;
  int ball_V_y=random(-3,3);//losowa skladowa y predkosci
  if(ball_V_y==0)
    ball_V_y=-4;
  boolean redraw=true;//odswiezenie ekranu
  clearDisplay(WHITE);
  while(true)
  {
    read_special_buttons();
    if(button_activated(B_PAUSE))
    {//wlaczenie/wylaczenie pauzy
      paused=!paused;
      if(paused)
      {
        setStr(" PAUSE ", 22, 20, WHITE);
        updateDisplay();
      }
    }
    if(button_activated(B_MENU))
      return;//wyjscie do menu
    if(!paused)
    {//logika gry
      long now = millis();
      long diff = now-timer;//delta t
      timer=now;
      read_function_buttons();
      read_joy_axes();
      
      //ruch gracza 1
      //gracz 1 korzysta z joysticka
      int j_dir = -joy_to_direction(Y_JOY);//kierunek ruchu paletki
      int old_pos = p1_position;
      //ograniczenia na maksymalna i minimalna pozycje paletki
      p1_position = p1_position + j_dir* 5*diff > 4299 ? p1_position : p1_position + j_dir* 5*diff;
      p1_position = p1_position < 500 ? 500 : p1_position;
      if(old_pos/100 != p1_position/100)//sprawdzenie warunku odswiezenia ekranu
        redraw=true;
      
      //ruch gracza 2
      //gracz 2 korzysta z przyciskow
      old_pos=p2_position;
      int b_dir = on_button(B_UP) ? -1 : (on_button(B_DOWN) ? 1: 0);
      p2_position = p2_position + b_dir* 5*diff > 4299 ? p2_position : p2_position + b_dir* 5*diff;
      p2_position = p2_position < 500 ? 500 : p2_position;
      if(old_pos/100 != p2_position/100)
        redraw=true;
      
      //ruch pilki
      old_pos = ball_x;
      ball_x+= ball_V_x*diff;//nowa pozycja x pilki
      if(old_pos/100 != ball_x/100)
        redraw=true;//warunek odswiezenia ekranu
      old_pos=ball_y;
      ball_y+= ball_V_y*diff;//nowa pozycja y pilki
      if(old_pos/100 != ball_y/100)
        redraw=true;
        
      if(ball_y > 4499 || ball_y < 300)
        ball_V_y=-ball_V_y;//odbicie pilki od gornej/dolnej krawedzi
      
      if(ball_x > 8199||ball_x < 200)
      {//trafienie gracza 1
        if(ball+x > 8199)
          p1_result++;//zwiekszenie wyniku
        else
          p2_result++;
        ball_y=2400;//pozycja startowa pilki
        ball_x=4200;
        //losowa predkosc pilki
        ball_V_x=random(-5,6);
        if(ball_V_x==0)
          ball_V_x=3;
        ball_V_y=random(-3,4);
        if(ball_V_y==0)
          ball_V_y=-3;
      }
      
      //kolizja z paletka
      //odleglosc od blizszej pionowej krawedzi
      int temp = ball_x - 100 < 8200 - ball_x ? ball_x - 100 : 8200 - ball_x;
      boolean collision=false;
      boolean edge=false;
      if(temp<=300)
     { //jezeli blizej do krawedzi niz wartosc progowa
        if(ball_y> p1_position-400 && ball_y < p1_position+400 && ball_x < 4200) 
        {//jezeli pilka uderzyla w paletke po lewej stronie
          if((ball_y==p1_position-400 || ball_y == p1_position+400) && ball_V_y*j_dir<0)
            edge=true;
          collision=true;
        }
        else if(ball_y> p2_position-400 && ball_y < p2_position+400 && ball_x > 4200)
        {//jezeli pilka uderzyla w palekte po prawej stronie
          if((ball_y==p2_position-400 || ball_y == p2_position+400) && ball_V_y*b_dir<0)
            edge=true;
          collision=true;
        }
        if(collision)
          ball_V_x=-ball_V_x;//odbicie pilki od paletki
        if(edge)
        {//odbicie pilki od krawedzi paletki - zmiana obu skladowych predkosci pilki
          ball_V_y=-ball_V_y;
          int old_V = ball_V_y;
          ball_V_y= random((ball_V_y>0 ? ball_V_y-1 : ball_V_y+1), (ball_V_y>0 ? ball_V_y+1 : ball_V_y-1));
          ball_V_y= ball_V_y==0 ? old_V : ball_V_y;
        }
      }
      
      if(redraw)
      {
        clearDisplay(WHITE);
        
        //wypisz wynik
        char result[6];
        result[0]= p1_result>9 ? (p1_result/10) + '0' : ' ';
        result[1]= p1_result%10 + '0';
        result[2]=':';
        result[3]= p2_result>9 ? (p2_result/10) + '0' : ' ';
        result[4]= p2_result%10 + '0';
        result[5]=0x00; 
        setStr(result, 27, 0, BLACK);
        
        //linia srodkowa
        int i;
        for(i=0;i<48;i+=2)
          setPixel(41,i,BLACK);
        
        
        //paletka gracza 1
        setLine(0, p1_position/100-4, 0, p1_position/100+4, BLACK);
        setLine(1, p1_position/100-4, 1, p1_position/100+4, BLACK);
        
        //paletka gracza 2
        setLine(82, p2_position/100-4, 82, p2_position/100+4, BLACK);
        setLine(83, p2_position/100-4, 83, p2_position/100+4, BLACK);
        
        //pilka
        setCircle(ball_x/100, ball_y/100, 2, BLACK, 3);
        setRect(ball_x/100-1, ball_y/100-1, ball_x/100+2, ball_y/100+1, true, BLACK);
        updateDisplay();
        redraw=false;
      }
      if(p1_result > 99 || p2_result > 99)
      {//warunek konca gry
        clearDisplay(WHITE);
        setStr(" ENOUGH! ", 22, 20, WHITE);
        delay(3000);
        return;
      }
    }
    else
      timer = millis();
  }
}

void setup()
{
  init_buttons();
  init_joy();
  lcdBegin();
  setContrast(60);
  intro();
  delay(500);
  backlightAnimate(0, 255, 1000);
  int state = -1;
  while(true)
  {//glowna petla
    state = menu();
    if(state==PONG)
      pong();
    if(state==DEMO)
    {
      lcdFunTime();
      backlightAnimate(255, 0, 1000);
    }
  }
}

void loop()
{
  
}
