#include <SPI.h>
#include <GD.h>

#include "test1.h"

void setup()
{
  GD.begin();
  for (byte y = 0; y < 38; y++)
    GD.copy(RAM_PIC + y * 64, test1_pic + y * 50, 50);
  GD.copy(RAM_CHR, test1_chr, sizeof(test1_chr));
  GD.copy(RAM_PAL, test1_pal, sizeof(test1_pal));
}

void loop()
{
}
