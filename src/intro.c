#include <gb/gb.h>
#include "tilesandbackgrounds.h"

// Intro logo roll
void intro() {
  wait_vbl_done();
  NR52_REG = 0x80;
  NR11_REG = 0x80;
  NR12_REG = 0xF3;
  NR51_REG = 0xF3;
  NR50_REG = 0x77;

  DISPLAY_OFF;
  set_bkg_data(0, 31, splash);
  set_bkg_tiles(0x00, 0x00, 20, 29, splashbackground);

  SHOW_BKG;
  DISPLAY_ON;

  int lol = 80;
  while(lol) {
    scroll_bkg(0,1);
    delay(50);
    lol--;
  }
  NR13_REG = 0xC1;
  NR14_REG = 0x87;  
  delay(150);
  NR13_REG = 0x83;
  NR14_REG = 0x87;
  delay(400);
  NR52_REG = 0x00;
  delay(1000);
  move_bkg(0, 0);
}
