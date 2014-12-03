#include <SPI.h>
#include <GD.h>

#include "image.h"
#include "sprite.h"
 
static uint16_t atxy(byte x, byte y)
{
  return RAM_PIC + 64 * y + x;
}

void setup()
{
  GD.begin();
  Serial.begin(9600);
  for (byte y = 0; y < 38; y++)
    GD.copy(RAM_PIC + y * 64, image_pic + y * 50, 50);
  GD.copy(RAM_CHR, image_chr, sizeof(image_chr));
  GD.copy(RAM_PAL, image_pal, sizeof(image_pal));
  
  GD.copy(RAM_SPRPAL, sprite_sprpal, sizeof(sprite_sprpal));
  GD.copy(RAM_SPRIMG, sprite_sprimg, sizeof(sprite_sprimg));

  // For show, randomly scatter the frames on the screen
  //GD.__wstartspr(0);
  //for (int anim = 0; anim < SPRITE_FRAMES; anim++)
   //draw_sprite(random(400), random(300), anim, 0);
  //GD.__end();
}
static unsigned int t;
void loop()
{
            GD.waitvblank();
            GD.__wstartspr(0);
            draw_sprite(-t%300, 259, 25+((t/32)%2), 0);
            draw_sprite((-t/5)%300,(-t*2)%300 , 75+((t/32)%2), 0);
            draw_sprite((-t/2)%300, 50, 50+((t/32)%2), 0); //remember that third variable is the sprite
            GD.__end();
            Serial.println (t);
            t++;
            
}
