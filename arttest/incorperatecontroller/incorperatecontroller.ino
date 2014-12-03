//************************************************************************************************************************************************
// Header files
//************************************************************************************************************************************************
#include <SPI.h>
#include <GD.h>

#include "image.h"
#include "sprite.h"

//************************************************************************************************************************************************
// initializing pins
//************************************************************************************************************************************************

//************************************************************************************************************************************************
// arrays and initialilazations
//************************************************************************************************************************************************

    
int latchPin = 6;
int dataPin = 7;
int clockPin = 5;
byte switchVar1 = 72;  
int gamearray[8] = {
    0,0,0,0,0,0,0,0};
static unsigned int t; // used as the in game timer


static uint16_t atxy(byte x, byte y)
{
  return RAM_PIC + 64 * y + x;
}
//************************************************************************************************************************************************
// setup loop
//************************************************************************************************************************************************
void setup()
{
  GD.begin();
  Serial.begin(9600);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT); 
  pinMode(dataPin, INPUT);

  
  for (byte y = 0; y < 38; y++)
    GD.copy(RAM_PIC + y * 64, image_pic + y * 50, 50); //set up initial background
    GD.copy(RAM_CHR, image_chr, sizeof(image_chr));
    GD.copy(RAM_PAL, image_pal, sizeof(image_pal));
  
    GD.copy(RAM_SPRPAL, sprite_sprpal, sizeof(sprite_sprpal));//ready up the foreground sprites
    GD.copy(RAM_SPRIMG, sprite_sprimg, sizeof(sprite_sprimg));
  

  // For show, randomly scatter the frames on the screen   //used to test new sprites
  //GD.__wstartspr(0);
  //for (int anim = 0; anim < SPRITE_FRAMES; anim++)
   //draw_sprite(random(400), random(300), anim, 0);
  //GD.__end();
}

void loop()
{
            GD.waitvblank();
            controller_get();
            GD.__wstartspr(0);
            draw_sprite(-t%300, 259, 25+((t/32)%2), 0);
            draw_sprite((-t/5)%300,(-t*2)%300 , 75+((t/32)%2), 0);
            draw_sprite((-t/2)%300, 50, 50+((t/32)%2), 0); //remember that third variable is the sprite
            GD.__end();
            Serial.println (t);
            t++;
            
}

void controller_get (){
  

  digitalWrite(latchPin,1);
  delayMicroseconds(20);
  digitalWrite(latchPin,0);
  switchVar1 = shiftIn(dataPin, clockPin);

  for (int n=0; n<=7; n++)
  {

    if (switchVar1 & (1 << n) ){
     gamearray [n] = 0; //print the value of the array location
    }
      else
      {
        gamearray [n] = 1;
      }
    }
  

for (int n=0; n<=7; n++)
{
Serial.println (gamearray [n]);
}//white space
Serial.println("-------------------");

//delay so all these print satements can keep up. 
delay(100);

}







byte shiftIn(int myDataPin, int myClockPin) {   //inner workings of the shift in
  int i;
  int temp = 0;
  int pinState;
  byte myDataIn = 0;

  pinMode(myClockPin, OUTPUT);
  pinMode(myDataPin, INPUT);

  for (i=7; i>=0; i--)
  {
    digitalWrite(myClockPin, 0);
    delayMicroseconds(0.2);
    temp = digitalRead(myDataPin);
    if (temp) {
      pinState = 1;
      //set the bit to 0 no matter what
      myDataIn = myDataIn | (1 << i);
    }
    else {
      //turn it off -- only necessary for debuging
     //print statement since myDataIn starts as 0
      pinState = 0;
    }

    //Debuging print statements
    //Serial.print(pinState);
    //Serial.print("     ");
    //Serial.println (dataIn, BIN);

    digitalWrite(myClockPin, 1);

  }
  //debuging print statements whitespace
  //Serial.println();
  //Serial.println(myDataIn, BIN);
  return myDataIn;
}


