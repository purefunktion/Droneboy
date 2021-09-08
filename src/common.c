#include "common.h"

// Common functions between control pages

void placeMacroMarker() {
	switch(active_control_page)
  {
  	case 0: { // volume
  		placeVolumeMacroMarker();
      break;
    }
    case 1: {
    	placeDutyMacroMarker();
    	break;
    }
  }
}

// Place volume macro markers
void placeVolumeMacroMarker() {
	switch(current_channel)
	{
		case 0: { // sweep
			if (volumeMacroStatus.sweep == 2) {
  			volumeMacroStatus.sweep = 0;
  		} else {
  			volumeMacroStatus.sweep++;	
  		}
			DISPLAY_OFF;
	    if (volumeMacroStatus.sweep == 0) {
	    	set_bkg_tiles(0x03, 0x10, 1, 1, blankTileBkg);        
	    } else if (volumeMacroStatus.sweep == 1) {
	    	set_bkg_tiles(0x03, 0x10, 1, 1, iconMacroMarkerBkg);
	    } else {
	      set_bkg_tiles(0x03, 0x10, 1, 1, iconInvertedMacroMarkerBkg);
	    }
	    SHOW_BKG;
	    DISPLAY_ON;
	    break;
		}
		case 1: { // square
			if (volumeMacroStatus.square == 2) {
  			volumeMacroStatus.square = 0;
  		} else {
  			volumeMacroStatus.square++;	
  		}
			DISPLAY_OFF;
	    if (volumeMacroStatus.square == 0) {
	    	set_bkg_tiles(0x08, 0x10, 1, 1, blankTileBkg);        
	    } else if (volumeMacroStatus.square == 1) {
	      set_bkg_tiles(0x08, 0x10, 1, 1, iconMacroMarkerBkg);
	    } else {	      
	    	set_bkg_tiles(0x08, 0x10, 1, 1, iconInvertedMacroMarkerBkg);
	    }
	    SHOW_BKG;
	    DISPLAY_ON;
	    break;
		}
		case 2: { // wave
			if (volumeMacroStatus.wave == 2) {
  			volumeMacroStatus.wave = 0;
  		} else {
  			volumeMacroStatus.wave++;	
  		}
			DISPLAY_OFF;
	    if (volumeMacroStatus.wave == 0) {
	    	set_bkg_tiles(0x0D, 0x10, 1, 1, blankTileBkg);        
	    } else if (volumeMacroStatus.wave == 1) {
	      set_bkg_tiles(0x0D, 0x10, 1, 1, iconMacroMarkerBkg);
	    } else {
	    	set_bkg_tiles(0x0D, 0x10, 1, 1, iconInvertedMacroMarkerBkg);
	    }
	    SHOW_BKG;
	    DISPLAY_ON;
	    break;
		}
		case 3: { // noise
			if (volumeMacroStatus.noise == 2) {
  			volumeMacroStatus.noise = 0;
  		} else {
  			volumeMacroStatus.noise++;	
  		}
			DISPLAY_OFF;
	    if (volumeMacroStatus.noise == 0) {
	    	set_bkg_tiles(0x12, 0x10, 1, 1, blankTileBkg);        
	    } else if (volumeMacroStatus.noise == 1) {
	      set_bkg_tiles(0x12, 0x10, 1, 1, iconMacroMarkerBkg);
	    } else {
	    	set_bkg_tiles(0x12, 0x10, 1, 1, iconInvertedMacroMarkerBkg);
	    }
	    SHOW_BKG;
	    DISPLAY_ON;
	    break;
		}
	}
}

/*
This is used when coming back to the volume control page to set macro markers
*/
void setAllVolumeMacroMarkers() {
	DISPLAY_OFF;
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
  SHOW_BKG;
  DISPLAY_ON;
}

// Place the duty macro markers
void placeDutyMacroMarker() {
  switch(current_channel)
  {
    case 0: { // sweep
      if (dutyMacroStatus.sweep == 2) {
        dutyMacroStatus.sweep = 0;
      } else {
        dutyMacroStatus.sweep++;  
      }
      DISPLAY_OFF;
      if (dutyMacroStatus.sweep == 0) {
        set_bkg_tiles(0x03, 0x10, 1, 1, blankTileBkg);        
      } else if (dutyMacroStatus.sweep == 1) {
        set_bkg_tiles(0x03, 0x10, 1, 1, iconMacroMarkerBkg);
      } else {
        set_bkg_tiles(0x03, 0x10, 1, 1, iconInvertedMacroMarkerBkg);
      }
      SHOW_BKG;
      DISPLAY_ON;
      break;
    }
    case 1: { // square
      if (dutyMacroStatus.square == 2) {
        dutyMacroStatus.square = 0;
      } else {
        dutyMacroStatus.square++; 
      }
      DISPLAY_OFF;
      if (dutyMacroStatus.square == 0) {
        set_bkg_tiles(0x08, 0x10, 1, 1, blankTileBkg);        
      } else if (dutyMacroStatus.square == 1) {
        set_bkg_tiles(0x08, 0x10, 1, 1, iconMacroMarkerBkg);
      } else {        
        set_bkg_tiles(0x08, 0x10, 1, 1, iconInvertedMacroMarkerBkg);
      }
      SHOW_BKG;
      DISPLAY_ON;
      break;
    }
    case 2: { // wave
      if (dutyMacroStatus.wave == 2) {
        dutyMacroStatus.wave = 0;
      } else {
        dutyMacroStatus.wave++; 
      }
      DISPLAY_OFF;
      if (dutyMacroStatus.wave == 0) {
        set_bkg_tiles(0x0D, 0x10, 1, 1, blankTileBkg);        
      } else if (dutyMacroStatus.wave == 1) {
        set_bkg_tiles(0x0D, 0x10, 1, 1, iconMacroMarkerBkg);
      } else {
        set_bkg_tiles(0x0D, 0x10, 1, 1, iconInvertedMacroMarkerBkg);
      }
      SHOW_BKG;
      DISPLAY_ON;
      break;
    }
    case 3: { // noise
      if (dutyMacroStatus.noise == 2) {
        dutyMacroStatus.noise = 0;
      } else {
        dutyMacroStatus.noise++;  
      }
      DISPLAY_OFF;
      if (dutyMacroStatus.noise == 0) {
        set_bkg_tiles(0x12, 0x10, 1, 1, blankTileBkg);        
      } else if (dutyMacroStatus.noise == 1) {
        set_bkg_tiles(0x12, 0x10, 1, 1, iconMacroMarkerBkg);
      } else {
        set_bkg_tiles(0x12, 0x10, 1, 1, iconInvertedMacroMarkerBkg);
      }
      SHOW_BKG;
      DISPLAY_ON;
      break;
    }
  }
}

/*
This is used when coming back to the duty control page 
to set already set macro markers
*/
void setAllDutyMacroMarkers() {
  DISPLAY_OFF;
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
  SHOW_BKG;
  DISPLAY_ON;
}