/*------------------------------------------------------------
  Universal joystick reader for Gameduino
  
  http://www.artlum.com/gameduino/gameduino.html#joystick
------------------------------------------------------------*/
#include <SPI.h>
#include <GD.h>
#include "joystick.h"

// Change this line to enable an NES controller
// style joystick
// "1" = NES
// "0" = Other
//
#define NES_CONTROLLER 1


// Change this line to enable your customized
// joystick configuration
// "1" = custom configuration
// "0" = default (Sparkfun joystick)
//
#define CUSTOM_JOYSTICK 0


#if NES_CONTROLLER
/*------------------------------------------------------------
  NES controller
  
  See: http://www.artlum.com/gameduino/nes.html

  NES controllers need three Arduino pins: STROBE/CLOCK/DATA
------------------------------------------------------------*/
// eg. Control wires connected to analog pins 0/1/2
#define NES_DATA A0
#define NES_STROBE A1
#define NES_CLOCK A2



#elif CUSTOM_JOYSTICK
/*------------------------------------------------------------------
 Custom joystick configuration. This is what you need to
 hack to get your joystick working.

 The joystick layout can be customized by commenting
 or uncommenting the lines below.
 
 Notes
 -----
 To specify an analog pin for a button add "A0" to
 the pin number, eg. Analog pin 4 is "A0+4"

 When connecting a Joystick to an Arduino:
  * Digital pins 0/1 are used for system stuff.
  * Gameduino uses pins 2/9/10/11/13 for SPI interface
 
 That leaves digital pins 3/4/5/6/7/8/10 free, plus
 all the analog pins (which work as digital pins, too!)
------------------------------------------------------------------*/

// eg. The following layout is good for an NES controller

// Connect the four buttons to pins 3/4/5/6
#define BUTTON_A_PIN 3
#define BUTTON_B_PIN 4
//#define BUTTON_C_PIN ?
//#define BUTTON_X_PIN ?
//#define BUTTON_Y_PIN ?
//#define BUTTON_Z_PIN ?
#define BUTTON_ST_PIN 5
#define BUTTON_SEL_PIN 6

// Connect the DPAD L/R/U/D to analog pins 2/3/4/5
#define STICK_LEFT_PIN  (A0+2)
#define STICK_RIGHT_PIN (A0+3)
#define STICK_UP_PIN    (A0+4)
#define STICK_DOWN_PIN  (A0+5)

// Analog input is disabled...
//#define ANALOG_X_PIN ?
//#define ANALOG_Y_PIN ?

// Make the DPAD emulate an analog joystick
#define DIGITAL_EMULATE_ANALOG
//#define ANALOG_EMULATE_DIGITAL

#else
/*------------------------------------------------------------------
  Default - Sparkfun joystick
------------------------------------------------------------------*/
#define BUTTON_A_PIN 3
#define BUTTON_B_PIN 4
// #define BUTTON_C_PIN ?
// nb. Button X is pin 7 if you've hacked your board
#define BUTTON_X_PIN 2
// #define BUTTON_Y_PIN ?
// #define BUTTON_Z_PIN ?
#define BUTTON_ST_PIN 5
#define BUTTON_SEL_PIN 6

// Digital joystick/DPAD
//#define STICK_LEFT_PIN  11
//#define STICK_RIGHT_PIN 12
//#define STICK_UP_PIN    13
//#define STICK_DOWN_PIN  14

// Analog joystick is enabled
#define ANALOG_X_PIN 0
#define ANALOG_Y_PIN 1

// Enable digital joystick emulation
// #define DIGITAL_EMULATE_ANALOG
#define ANALOG_EMULATE_DIGITAL

#endif

/*------------------------------------------------------
  Stop hacking now...!
  
  There are no user-servicable parts below this line
------------------------------------------------------*/

// Some internal definitions
#define STICK_UP_BIT      0x01
#define STICK_DOWN_BIT    0x02
#define STICK_LEFT_BIT    0x04
#define STICK_RIGHT_BIT   0x08
#define ANALOG_STICK_BIT  0x80
#define STICK_INFO_MASK (ANALOG_STICK_BIT)


#if NES_CONTROLLER
/*---------------------------------------------
  NES controller
---------------------------------------------*/
Joystick::Joystick()
{
  pinMode(NES_DATA,INPUT);     digitalWrite(NES_DATA,HIGH);
  pinMode(NES_STROBE,OUTPUT);  digitalWrite(NES_STROBE,HIGH);
  pinMode(NES_CLOCK,OUTPUT);   digitalWrite(NES_CLOCK,HIGH);
}
static byte readNesBit(byte button)
{
  const bool b = (digitalRead(NES_DATA)==LOW);
  digitalWrite(NES_CLOCK,LOW);
  digitalWrite(NES_CLOCK,HIGH);
  return (b)? button:0;
}
void Joystick::read()
{
  prev = buttons;
  digitalWrite(NES_STROBE,LOW);
  buttons  = readNesBit(buttonA);
  buttons |= readNesBit(buttonB);
  buttons |= readNesBit(buttonSelect);
  buttons |= readNesBit(buttonStart);
  dpad  = readNesBit(0x01);
  dpad |= readNesBit(0x02);
  dpad |= readNesBit(0x04);
  dpad |= readNesBit(0x08);
  digitalWrite(NES_STROBE,HIGH);

  // Make the DPAD emulate an analog joystick
  // for games that need it...
  stickX = stickY = 0;
  if      (dpad&STICK_LEFT_BIT)  { stickX = -127; }
  else if (dpad&STICK_RIGHT_BIT) { stickX =  127; }
  if      (dpad&STICK_UP_BIT)    { stickY =  127; }
  else if (dpad&STICK_DOWN_BIT)  { stickY = -127; }
}

void Joystick::recalibrate()
{
}
bool Joystick::hasAnalogStick()
{
  return false;
}

#else

/*---------------------------------------------
  Sparkfun/custom joystick
---------------------------------------------*/

/*---------------------------------------------
  Initialise pins for joystick
---------------------------------------------*/
static PROGMEM prog_uchar joystickPinList[] = {
#ifdef BUTTON_A_PIN
  BUTTON_A_PIN
#endif
#ifdef BUTTON_B_PIN
  , BUTTON_B_PIN
#endif
#ifdef BUTTON_C_PIN
  , BUTTON_C_PIN
#endif
#ifdef BUTTON_X_PIN
  , BUTTON_X_PIN
#endif
#ifdef BUTTON_Y_PIN
  , BUTTON_Y_PIN
#endif
#ifdef BUTTON_Z_PIN
  , BUTTON_Z_PIN
#endif
#ifdef BUTTON_ST_PIN
  , BUTTON_ST_PIN
#endif
#ifdef BUTTON_SEL_PIN
  , BUTTON_SEL_PIN
#endif
#ifdef STICK_LEFT_PIN
  , STICK_LEFT_PIN,
    STICK_RIGHT_PIN,
    STICK_UP_PIN,
    STICK_DOWN_PIN
#endif
};
Joystick::Joystick()
{
  // set up digital pins for dpad and buttons
  prog_uchar *pin = joystickPinList;
  for (byte i=0; i<sizeof(joystickPinList); ++i) {
    byte p = pgm_read_byte(joystickPinList+i);
    pinMode(p,INPUT);
    digitalWrite(p,HIGH);
  }
  stickX = stickY = 0;
  xCal = yCal = 0;
  buttons = prev = dpad = 0;

#ifdef ANALOG_X_PIN
  dpad |= ANALOG_STICK_BIT;
#endif
}
bool Joystick::hasAnalogStick()
{
  return (dpad&ANALOG_STICK_BIT)!=0;
}
void Joystick::recalibrate()
{
  read();
  xCal = stickX;
  yCal = stickY;
}
static char stickCalc(int a, char cal)
{
  a = ((analogRead(a)-512)/4)-cal;
  if (a < -128) a = -128;
  if (a >  127) a =  127;
  return char(a);
}
void Joystick::read()
{
  // Joystick buttons
  prev = buttons;
  buttons = 0;
  #ifdef BUTTON_A_PIN
    if (digitalRead(BUTTON_A_PIN)==LOW) {  buttons |= buttonA;      }
  #endif
  #ifdef BUTTON_B_PIN
    if (digitalRead(BUTTON_B_PIN)==LOW) {  buttons |= buttonB;      }
  #endif
  #ifdef BUTTON_C_PIN
    if (digitalRead(BUTTON_C_PIN)==LOW) {  buttons |= buttonC;      }
  #endif
  #ifdef BUTTON_X_PIN
    if (digitalRead(BUTTON_X_PIN)==LOW) {  buttons |= buttonX;      }
  #endif
  #ifdef BUTTON_Y_PIN
    if (digitalRead(BUTTON_Y_PIN)==LOW) {  buttons |= buttonY;      }
  #endif
  #ifdef BUTTON_Z_PIN
    if (digitalRead(BUTTON_Z_PIN)==LOW) {  buttons |= buttonZ;      }
  #endif
  #ifdef BUTTON_SEL_PIN
    if (digitalRead(BUTTON_SEL_PIN)==LOW){ buttons |= buttonSelect; }
  #endif
  #ifdef BUTTON_ST_PIN
    if (digitalRead(BUTTON_ST_PIN)==LOW){  buttons |= buttonStart;  }
  #endif

  // Digital joystick/dpad
  dpad &= STICK_INFO_MASK;   // The top bits are informational...preserve them
  #ifdef STICK_LEFT_PIN
    if (!digitalRead(STICK_LEFT_PIN))  { dpad |= STICK_LEFT_BIT;  }
    if (!digitalRead(STICK_RIGHT_PIN)) { dpad |= STICK_RIGHT_BIT; }
    if (!digitalRead(STICK_UP_PIN))    { dpad |= STICK_UP_BIT;    }
    if (!digitalRead(STICK_DOWN_PIN))  { dpad |= STICK_DOWN_BIT;  }
    #ifdef DIGITAL_EMULATE_ANALOG
      stickX = 0;
      if      (dpad&STICK_LEFT_BIT)  { stickX = -127; }
      else if (dpad&STICK_RIGHT_BIT) { stickX =  127; }
      stickY = 0;
      if      (dpad&STICK_UP_BIT)    { stickY =  127; }
      else if (dpad&STICK_DOWN_BIT)  { stickY = -127; }
    #endif
  #endif

  // Analog stick
  #ifdef ANALOG_X_PIN
    stickX = stickCalc(ANALOG_X_PIN,xCal);
  #endif
  #ifdef ANALOG_Y_PIN
    stickY = stickCalc(ANALOG_Y_PIN,yCal);
  #endif
  #ifdef ANALOG_EMULATE_DIGITAL
    if      (stickX < -40) { dpad |= STICK_LEFT_BIT;  }
    else if (stickX >  40) { dpad |= STICK_RIGHT_BIT; }
    if      (stickY >  40) { dpad |= STICK_UP_BIT;    }
    else if (stickY < -40) { dpad |= STICK_DOWN_BIT;  }
  #endif
}

// Non-NES
#endif

/*--------------------------------------------------------
  Useful function to show joystick status on screen.

  Note: You won't make your program any smaller by
	removing this - the Arduino compiler is really,
	really good at discarding unused code.

	ie. If you're not calling Joystick::dump() then it
	doesn't take up any space. Try it and see...!
--------------------------------------------------------*/
// Formatted output of joystick position
static void jpos(int v, char *s, char t)
{
  *s++ = t;
  *s++ = ':';
  boolean neg = (v<0);
  if (neg) {
    v = -v;
  }
  char *o = s;
  int m = 1000;
  while (m != 0) {
    int d = v/m;
    *o++ = d+'0';
    v -= d*m;
    m /= 10;
  }
  *o-- = 0;
  // Remove leading zeros
  while ((s<o) and (*s=='0')) {
    *s++ = ' ';
  }
  if (neg) {
    s[-1] = '-';
  }
}
void Joystick::dump(int sx, int sy)
{
  char temp[32];
  if (hasAnalogStick()) {
    jpos(analogX(),temp,'X');
    GD.putstr(sx,sy++,temp);
    jpos(analogY(),temp,'Y');
    GD.putstr(sx,sy++,temp);
  }
  temp[0] = 'D';
  temp[1] = ':';
  char *s = temp+2;
  *s++ = up()    ?'U':'.';
  *s++ = down()  ?'D':'.';
  *s++ = left()  ?'L':'.';
  *s++ = right() ?'R':'.';
  *s = 0;
  GD.putstr(sx,sy++,temp);
  temp[0] = 'B';
  s = temp+2;
  *s++ = (buttons&buttonSelect)?'L':'.';
  *s++ = (buttons&buttonStart)?'S':'.';
  *s++ = (buttons&buttonZ)?'Z':'.';
  *s++ = (buttons&buttonY)?'Y':'.';
  *s++ = (buttons&buttonX)?'X':'.';
  *s++ = (buttons&buttonC)?'C':'.';
  *s++ = (buttons&buttonB)?'B':'.';
  *s++ = (buttons&buttonA)?'A':'.';
  *s = 0;
  GD.putstr(sx,sy++,temp);
}
