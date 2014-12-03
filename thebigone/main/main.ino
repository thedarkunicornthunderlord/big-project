#include <SPI.h>
#include <GD.h>

#include "test1.h"
#include "sprite.h"















void setup()
{
  GD.begin();
  for (byte y = 0; y < 38; y++)
    GD.copy(RAM_PIC + y * 64, test1_pic + y * 50, 50);
  GD.copy(RAM_CHR, test1_chr, sizeof(test1_chr));
  GD.copy(RAM_PAL, test1_pal, sizeof(test1_pal));
  GD.copy(RAM_SPRPAL, sprite_sprpal, sizeof(sprite_sprpal));
  GD.copy(RAM_SPRIMG, sprite_sprimg, sizeof(sprite_sprimg));

  // For show, randomly scatter the frames on the screen
  GD.__wstartspr(0);
  for (int anim = 0; anim < SPRITE_FRAMES; anim++)
    draw_sprite(random(400), random(300), anim, 0);
  GD.__end();
}

void loop()
{
}
