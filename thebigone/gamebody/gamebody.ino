#include <font8x8.h>
#include <GD.h>

#include <font8x8.h>
#include <GD.h>
#include <SPI.h>

#include "test1.h"
#include "sprite.h"
  const int buttonpin4 = 4;
  const int buttonpin5 = 5;
  int button4 = 0;
  int button5 = 0;

void setup()
{
  GD.begin();
       
  pinMode(buttonpin4, INPUT); //set up the buttons
  pinMode(buttonpin5, INPUT);
  GD.copy(RAM_SPRPAL, sprite_sprpal, sizeof(sprite_sprpal));
  GD.copy(RAM_SPRIMG, sprite_sprimg, sizeof(sprite_sprimg));



  for (byte y = 0; y < 38; y++)
  GD.copy(RAM_PIC + y * 64, test1_pic + y * 50, 50); //display background
  GD.copy(RAM_CHR, test1_chr, sizeof(test1_chr));
  GD.copy(RAM_PAL, test1_pal, sizeof(test1_pal));
    
  
}

void loop()
{     
       GD.__wstartspr(25);
            draw_sprite(t%256, 216, 3, 0);
            GD.__end();
  
       
}
