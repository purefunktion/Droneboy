# Simple Makefile for DroneBoy
#

# If you move this project you can change the directory
# to match your GBDK root directory (ex: GBDK_HOME = "C:/GBDK/"
GBDK_HOME = /opt/gbdk/

LCC = $(GBDK_HOME)bin/lcc

# You can uncomment the line below to turn on debug output
# LCC = $(LCC) -debug

PROJECTNAME = droneboy

BINS        = $(PROJECTNAME).gb $(PROJECTNAME).pocket
CSOURCES   := $(wildcard src/*.c)

CFLAGS     := -Wp-Iinclude

all:    $(BINS)

compile.bat: Makefile
	@echo "REM Automatically generated from Makefile" > compile.bat
	@make -sn | sed y/\\//\\\\/ | grep -v make >> compile.bat

# Compile and link all source files in a single call to LCC
${PROJECTNAME}.gb: $(CSOURCES)
	$(LCC) $(CFLAGS) -o $@ $(CSOURCES)

${PROJECTNAME}.pocket: $(CSOURCES)
	$(LCC) $(CFLAGS) -mgbz80:ap -o $@ $(CSOURCES)

clean:
	rm -f *.o *.lst *.map *.gb *.ihx *.sym *.cdb *.adb *.asm *.pocket

