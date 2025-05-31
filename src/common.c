#include "common.h"

// Common functions between control pages

void placeMacroMarker(void) {
	switch(current_state)
  {
  	case 0: { // volume
  		placeVolumeMacroMarker();
      break;
    }
    case 1: { // duty
    	placeDutyMacroMarker();
    	break;
    }
    case 2: { // freq
    	placeFreqMacroMarker();
    }
  }
}

// Place volume macro markers
void placeVolumeMacroMarker(void) {
	switch(current_channel)
	{
		case SWEEP: { // sweep
			if (volumeMacroStatus.sweep == 2) {
  			volumeMacroStatus.sweep = 0;
  		} else {
  			volumeMacroStatus.sweep++;
  		}
	    if (volumeMacroStatus.sweep == 0) {
	    	set_bkg_tiles(0x03, 0x10, 1, 1, blankTileBkg);
	    } else if (volumeMacroStatus.sweep == 1) {
	    	set_bkg_tiles(0x03, 0x10, 1, 1, iconMacroMarkerBkg);
	    } else {
	      set_bkg_tiles(0x03, 0x10, 1, 1, iconInvertedMacroMarkerBkg);
	    }
	    break;
		}
		case SQUARE: { // square
			if (volumeMacroStatus.square == 2) {
  			volumeMacroStatus.square = 0;
  		} else {
  			volumeMacroStatus.square++;
  		}
	    if (volumeMacroStatus.square == 0) {
	    	set_bkg_tiles(0x08, 0x10, 1, 1, blankTileBkg);
	    } else if (volumeMacroStatus.square == 1) {
	      set_bkg_tiles(0x08, 0x10, 1, 1, iconMacroMarkerBkg);
	    } else {	      
	    	set_bkg_tiles(0x08, 0x10, 1, 1, iconInvertedMacroMarkerBkg);
	    }
	    break;
		}
		case WAVE: { // wave
			if (volumeMacroStatus.wave == 2) {
  			volumeMacroStatus.wave = 0;
  		} else {
  			volumeMacroStatus.wave++;
  		}
	    if (volumeMacroStatus.wave == 0) {
	    	set_bkg_tiles(0x0D, 0x10, 1, 1, blankTileBkg);
	    } else if (volumeMacroStatus.wave == 1) {
	      set_bkg_tiles(0x0D, 0x10, 1, 1, iconMacroMarkerBkg);
	    } else {
	    	set_bkg_tiles(0x0D, 0x10, 1, 1, iconInvertedMacroMarkerBkg);
	    }
	    break;
		}
		case NOISE: { // noise
			if (volumeMacroStatus.noise == 2) {
  			volumeMacroStatus.noise = 0;
  		} else {
  			volumeMacroStatus.noise++;
  		}
	    if (volumeMacroStatus.noise == 0) {
	    	set_bkg_tiles(0x12, 0x10, 1, 1, blankTileBkg);
	    } else if (volumeMacroStatus.noise == 1) {
	      set_bkg_tiles(0x12, 0x10, 1, 1, iconMacroMarkerBkg);
	    } else {
	    	set_bkg_tiles(0x12, 0x10, 1, 1, iconInvertedMacroMarkerBkg);
	    }
	    break;
		}
	}
}

/*
This is used when coming back to the volume control page to set macro markers
*/
void setAllVolumeMacroMarkers(void) {
  if (volumeMacroStatus.sweep == 0) {
  	set_bkg_tiles(0x03, 0x10, 1, 1, blankTileBkg);
  } else if (volumeMacroStatus.sweep == 1) {
    set_bkg_tiles(0x03, 0x10, 1, 1, iconMacroMarkerBkg);
  } else {
  	set_bkg_tiles(0x03, 0x10, 1, 1, iconInvertedMacroMarkerBkg);
  }
  if (volumeMacroStatus.square == 0) {
  	set_bkg_tiles(0x08, 0x10, 1, 1, blankTileBkg);
  } else if (volumeMacroStatus.square == 1) {
    set_bkg_tiles(0x08, 0x10, 1, 1, iconMacroMarkerBkg);
  } else {
  	set_bkg_tiles(0x08, 0x10, 1, 1, iconInvertedMacroMarkerBkg);
  }
  if (volumeMacroStatus.wave == 0) {
  	set_bkg_tiles(0x0D, 0x10, 1, 1, blankTileBkg);
  } else if (volumeMacroStatus.wave == 1) {
    set_bkg_tiles(0x0D, 0x10, 1, 1, iconMacroMarkerBkg);
  } else  {
  	set_bkg_tiles(0x0D, 0x10, 1, 1, iconInvertedMacroMarkerBkg);
  }
	if (volumeMacroStatus.noise == 0) {
		set_bkg_tiles(0x12, 0x10, 1, 1, blankTileBkg);
  } else if (volumeMacroStatus.noise == 1) {
    set_bkg_tiles(0x12, 0x10, 1, 1, iconMacroMarkerBkg);
  } else {
  	set_bkg_tiles(0x12, 0x10, 1, 1, iconInvertedMacroMarkerBkg);
  }
}

// Place the duty macro markers
void placeDutyMacroMarker(void) {
  switch(current_channel)
  {
    case SWEEP: { // sweep
      if (dutyMacroStatus.sweep == 2) {
        dutyMacroStatus.sweep = 0;
      } else {
        dutyMacroStatus.sweep++;
      }
      if (dutyMacroStatus.sweep == 0) {
        set_bkg_tiles(0x03, 0x10, 1, 1, blankTileBkg);
      } else if (dutyMacroStatus.sweep == 1) {
        set_bkg_tiles(0x03, 0x10, 1, 1, iconMacroMarkerBkg);
      } else {
        set_bkg_tiles(0x03, 0x10, 1, 1, iconInvertedMacroMarkerBkg);
      }
      break;
    }
    case SQUARE: { // square
      if (dutyMacroStatus.square == 2) {
        dutyMacroStatus.square = 0;
      } else {
        dutyMacroStatus.square++;
      }
      if (dutyMacroStatus.square == 0) {
        set_bkg_tiles(0x08, 0x10, 1, 1, blankTileBkg);
      } else if (dutyMacroStatus.square == 1) {
        set_bkg_tiles(0x08, 0x10, 1, 1, iconMacroMarkerBkg);
      } else {        
        set_bkg_tiles(0x08, 0x10, 1, 1, iconInvertedMacroMarkerBkg);
      }
      break;
    }
    case WAVE: { // wave
      if (dutyMacroStatus.wave == 2) {
        dutyMacroStatus.wave = 0;
      } else {
        dutyMacroStatus.wave++;
      }
      if (dutyMacroStatus.wave == 0) {
        set_bkg_tiles(0x0D, 0x10, 1, 1, blankTileBkg);
      } else if (dutyMacroStatus.wave == 1) {
        set_bkg_tiles(0x0D, 0x10, 1, 1, iconMacroMarkerBkg);
      } else {
        set_bkg_tiles(0x0D, 0x10, 1, 1, iconInvertedMacroMarkerBkg);
      }
      break;
    }
    case NOISE: { // noise
      if (dutyMacroStatus.noise == 2) {
        dutyMacroStatus.noise = 0;
      } else {
        dutyMacroStatus.noise++;
      }
      if (dutyMacroStatus.noise == 0) {
        set_bkg_tiles(0x12, 0x10, 1, 1, blankTileBkg);
      } else if (dutyMacroStatus.noise == 1) {
        set_bkg_tiles(0x12, 0x10, 1, 1, iconMacroMarkerBkg);
      } else {
        set_bkg_tiles(0x12, 0x10, 1, 1, iconInvertedMacroMarkerBkg);
      }
      break;
    }
  }
}

/*
This is used when coming back to the duty control page 
to set already set macro markers
*/
void setAllDutyMacroMarkers(void) {
  if (dutyMacroStatus.sweep == 0) {
    set_bkg_tiles(0x03, 0x10, 1, 1, blankTileBkg);
  } else if (dutyMacroStatus.sweep == 1) {
    set_bkg_tiles(0x03, 0x10, 1, 1, iconMacroMarkerBkg);
  } else {
    set_bkg_tiles(0x03, 0x10, 1, 1, iconInvertedMacroMarkerBkg);
  }
  if (dutyMacroStatus.square == 0) {
    set_bkg_tiles(0x08, 0x10, 1, 1, blankTileBkg);
  } else if (dutyMacroStatus.square == 1) {
    set_bkg_tiles(0x08, 0x10, 1, 1, iconMacroMarkerBkg);
  } else {
    set_bkg_tiles(0x08, 0x10, 1, 1, iconInvertedMacroMarkerBkg);
  }
  if (dutyMacroStatus.wave == 0) {
    set_bkg_tiles(0x0D, 0x10, 1, 1, blankTileBkg);
  } else if (dutyMacroStatus.wave == 1) {
    set_bkg_tiles(0x0D, 0x10, 1, 1, iconMacroMarkerBkg);
  } else  {
    set_bkg_tiles(0x0D, 0x10, 1, 1, iconInvertedMacroMarkerBkg);
  }
  if (dutyMacroStatus.noise == 0) {
    set_bkg_tiles(0x12, 0x10, 1, 1, blankTileBkg);
  } else if (dutyMacroStatus.noise == 1) {
    set_bkg_tiles(0x12, 0x10, 1, 1, iconMacroMarkerBkg);
  } else {
    set_bkg_tiles(0x12, 0x10, 1, 1, iconInvertedMacroMarkerBkg);
  }
}

// Place the duty macro markers
void placeFreqMacroMarker(void) {
  switch(current_channel)
  {
    case SWEEP: { // sweep
      if (freqMacroStatus.sweep == 2) {
        freqMacroStatus.sweep = 0;
      } else {
        freqMacroStatus.sweep++;
      }
      if (freqMacroStatus.sweep == 0) {
        set_bkg_tiles(0x05, 0x08, 1, 1, blankTileBkg);
      } else if (freqMacroStatus.sweep == 1) {
        set_bkg_tiles(0x05, 0x08, 1, 1, iconMacroMarkerBkg);
      } else {
        set_bkg_tiles(0x05, 0x08, 1, 1, iconInvertedMacroMarkerBkg);
      }
      break;
    }
    case SQUARE: { // square
      if (freqMacroStatus.square == 2) {
        freqMacroStatus.square = 0;
      } else {
        freqMacroStatus.square++;
      }
      if (freqMacroStatus.square == 0) {
        set_bkg_tiles(0x0F, 0x08, 1, 1, blankTileBkg);
      } else if (freqMacroStatus.square == 1) {
        set_bkg_tiles(0x0F, 0x08, 1, 1, iconMacroMarkerBkg);
      } else {
        set_bkg_tiles(0x0F, 0x08, 1, 1, iconInvertedMacroMarkerBkg);
      }
      break;
    }
    case WAVE: { // wave
      if (freqMacroStatus.wave == 2) {
        freqMacroStatus.wave = 0;
      } else {
        freqMacroStatus.wave++;
      }
      if (freqMacroStatus.wave == 0) {
        set_bkg_tiles(0x05, 0x10, 1, 1, blankTileBkg);
      } else if (freqMacroStatus.wave == 1) {
        set_bkg_tiles(0x05, 0x10, 1, 1, iconMacroMarkerBkg);
      } else {
        set_bkg_tiles(0x05, 0x10, 1, 1, iconInvertedMacroMarkerBkg);
      }
      break;
    }
    case NOISE: { // noise
      if (freqMacroStatus.noise == 2) {
        freqMacroStatus.noise = 0;
      } else {
        freqMacroStatus.noise++;
      }
      if (freqMacroStatus.noise == 0) {
        set_bkg_tiles(0x0F, 0x10, 1, 1, blankTileBkg);
      } else if (freqMacroStatus.noise == 1) {
        set_bkg_tiles(0x0F, 0x10, 1, 1, iconMacroMarkerBkg);
      } else {
        set_bkg_tiles(0x0F, 0x10, 1, 1, iconInvertedMacroMarkerBkg);
      }
      break;
    }
 	}
}

/*
This is used when coming back to the duty control page 
to set already set macro markers
*/
void setAllFreqMacroMarkers(void) {
  if (freqMacroStatus.sweep == 0) {
    set_bkg_tiles(0x05, 0x08, 1, 1, blankTileBkg);
  } else if (freqMacroStatus.sweep == 1) {
    set_bkg_tiles(0x05, 0x08, 1, 1, iconMacroMarkerBkg);
  } else {
    set_bkg_tiles(0x05, 0x08, 1, 1, iconInvertedMacroMarkerBkg);
  }
  if (freqMacroStatus.square == 0) {
    set_bkg_tiles(0x0F, 0x08, 1, 1, blankTileBkg);
  } else if (freqMacroStatus.square == 1) {
    set_bkg_tiles(0x0F, 0x08, 1, 1, iconMacroMarkerBkg);
  } else {
    set_bkg_tiles(0x0F, 0x08, 1, 1, iconInvertedMacroMarkerBkg);
  }
  if (freqMacroStatus.wave == 0) {
    set_bkg_tiles(0x05, 0x10, 1, 1, blankTileBkg);
  } else if (freqMacroStatus.wave == 1) {
    set_bkg_tiles(0x05, 0x10, 1, 1, iconMacroMarkerBkg);
  } else  {
    set_bkg_tiles(0x05, 0x10, 1, 1, iconInvertedMacroMarkerBkg);
  }
  if (freqMacroStatus.noise == 0) {
    set_bkg_tiles(0x0F, 0x10, 1, 1, blankTileBkg);
  } else if (freqMacroStatus.noise == 1) {
    set_bkg_tiles(0x0F, 0x10, 1, 1, iconMacroMarkerBkg);
  } else {
    set_bkg_tiles(0x0F, 0x10, 1, 1, iconInvertedMacroMarkerBkg);
  }
}

// print a icon to indicate if we use the high or low waves
// on freq page
void printLowHighFreqIcon(void) {
    if (low_or_high_wave_freq == 0) { // low
        set_bkg_tile_xy(0x03, 0x0B, 0x07); // L
    } else {
        set_bkg_tile_xy(0x03, 0x0B, 0x30); // H
    }
}