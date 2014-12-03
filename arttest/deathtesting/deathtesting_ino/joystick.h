#ifndef JOYSTICK_INCLUDED
#define JOYSTICK_INCLUDED
/*------------------------------------------------------------
  Universal joystick driver for Gameduino
  
  http://www.artlum.com/gameduino/gameduino.html#joystick

  Important: Read the file "read_me_first.txt" *before*
  writing any code. Do it NOW!
	
  Thanks go to 'Guy" for his contributions to this
------------------------------------------------------------*/

#include "Arduino.h"

class Joystick {
  byte buttons;      // State of buttons, packed into a byte
  byte prev;         // State of the buttons on previous read
  byte dpad;         // State of Up/Down/Left/Right buttons plus some control flags
  char stickX,stickY;// Analogue x/y position
  char xCal,yCal;    // Calibration
public:
  // The constructor sets up the Arduino pins for input
  // and calibrates the joystick using the current position
  Joystick();

  // Read the current state
  void read();

  // Return "true" if analog X/Y position is available
  bool hasAnalogStick();

// Digital joystick functions
  enum button_name {
    buttonA      = 0x01,
    buttonB      = 0x02,  
    buttonC      = 0x04,
    buttonX      = 0x08,  
    buttonY      = 0x10,  
    buttonZ      = 0x20,  
    buttonStart  = 0x40,
    buttonSelect = 0x80
  };
  // Test a named button
  // nb. You can combine button names to test multiple buttons at the same time
  bool isPressed(byte n)  { return (buttons&n)!=0;  }
 
  // This tells you if a button changed between the last two calls to "read()"
  bool changed(byte n) { return bool((buttons^prev)&n); }
  
  // Joystick up/down/left/right (or analog stick)
  bool up()    { return (dpad&0x01)!=0; }
  bool down()  { return (dpad&0x02)!=0; }
  bool left()  { return (dpad&0x04)!=0; }
  bool right() { return (dpad&0x08)!=0; }


// Analog joystick functions

   // Force the analog joystick to recalibrate itself
  void recalibrate();

  // Current joystick position in range [-128..127]
  char analogX()  { return stickX; }
  char analogY()  { return stickY; }
 
// For debugging - show all joystick state onscreen at (x,y)
  void dump(int x, int y);
};


// JOYSTICK_INCLUDED
#endif


