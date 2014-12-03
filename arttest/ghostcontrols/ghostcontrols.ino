//************************************************************************************************************************************************
// Header files
//************************************************************************************************************************************************
#include <SPI.h>
#include <GD.h>

#include "image.h"
#include "sprite.h"
#include "joystick.h"

//************************************************************************************************************************************************
// set up counters and variables
//************************************************************************************************************************************************
static unsigned int t; // used as the in game timer
static int ghostx = 50;  //ghost x axis
static int ghosty = 256; //ghosts y axis
static int dead = 0; //is the ghost dead...from being a ghost
static int jumping = 0;//used to tell if ghost is jumping
byte ghostsprt = GD.spr; //why is this here?
//************************************************************************************************************************************************
// arrays and initialilazations
//************************************************************************************************************************************************
Joystick joystick;




static uint16_t atxy(byte x, byte y)
{
  return RAM_PIC + 64 * y + x;
}
//************************************************************************************************************************************************
// setup loop
//************************************************************************************************************************************************
void setup()
{
  delay(250); //suggested to help boot up the gameduino
  GD.begin();
  GD.ascii();
  
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
            joystick.read();
             joystick.dump(0,0);
              
            GD.__wstartspr(0);
             draw_sprite(ghostx, ghosty,50+((t/32)%2), 0);
             if (joystick.left()) {
                    movePlayerLeft();
             }
             if (joystick.right()){
                    movePlayerRight();
             }
             if (joystick.isPressed(Joystick::buttonA)){
               if (jumping == 0){ 
               jumping = 1;
             }
             }
             jump();
            draw_sprite(-t%300, 259, 25+((t/32)%2), 0);
            draw_sprite((-t/5)%300,(-t*2)%300 , 75+((t/32)%2), 0);
            draw_sprite((-t/2)%300, 50, 50+((t/32)%2), 0); //remember that third variable is the sprite

            GD.__end();
            t++;
            
}
//****************************************************************************************************************************************************************
// control functions
//****************************************************************************************************************************************************************

void movePlayerLeft(){
  ghostx --;
}

void movePlayerRight(){
  ghostx ++;
}
void jump (){
  if (jumping == 1) {
    if (ghosty <= 226){
      jumping = 2;
    }
    ghosty--;
    ghosty--;
  }
  if (jumping == 2) {
    if (ghosty >= 257){
      jumping =0;
    }
    ghosty++;
    ghosty++;
  }
  
}
void shoot(){
}


