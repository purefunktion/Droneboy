GBDK_HOME = path-to-gbdk

LCC = $(GBDK_HOME)bin/lcc

PROJECTNAME = droneboy

BINS        = $(PROJECTNAME).gb $(PROJECTNAME).pocket
CSOURCES   := $(wildcard src/*.c)

# include folder
CFLAGS  = -Wp-Iinclude
ifdef GBDK_DEBUG
	CFLAGS += -debug -v
endif

all:    $(BINS)

${PROJECTNAME}.gb: $(CSOURCES)
	$(LCC) $(CFLAGS) -o $@ $(CSOURCES)

${PROJECTNAME}.pocket: $(CSOURCES)
	$(LCC) $(CFLAGS) -sm83 -o $@ $(CSOURCES)

clean:
	rm -f *.o *.lst *.map *.gb *.ihx *.sym *.cdb *.adb *.asm *.pocket

