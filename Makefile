include include.mk

# put the path to your gbdk installation in as include.mk file
GBDK_HOME = $(GBDK_PATH)

LCC = $(GBDK_HOME)bin/lcc

PROJECTNAME = droneboy

SRCDIR      = src
OBJDIR      = obj
RESDIR      = res
BINS        = $(OBJDIR)/$(PROJECTNAME).gb
CSOURCES    = $(foreach dir,$(SRCDIR),$(notdir $(wildcard $(dir)/*.c))) $(foreach dir,$(RESDIR),$(notdir $(wildcard $(dir)/*.c)))
ASMSOURCES  = $(foreach dir,$(SRCDIR),$(notdir $(wildcard $(dir)/*.s)))
OBJS       = $(CSOURCES:%.c=$(OBJDIR)/%.o) $(ASMSOURCES:%.s=$(OBJDIR)/%.o)

all:	prepare $(BINS)

# include folder
LCCFLAGS  += -Wp-Iinclude
ifdef GBDK_DEBUG
	LCCFLAGS += -debug -v
endif

all: prepare $(BINS)

# if you are on windows
compile.bat: Makefile
	@echo "REM Automatically generated from Makefile" > compile.bat
	@make -sn | sed y/\\//\\\\/ | sed s/mkdir\ -p\/mkdir\/ | grep -v make >> compile.bat

# Compile .c files in "src/" to .o object files
$(OBJDIR)/%.o:	$(SRCDIR)/%.c
	$(LCC) $(LCCFLAGS) -c -o $@ $<

# Compile .c files in "res/" to .o object files
$(OBJDIR)/%.o:	$(RESDIR)/%.c
	$(LCC) $(LCCFLAGS) -c -o $@ $<

# Compile .s assembly files in "src/" to .o object files
$(OBJDIR)/%.o:	$(SRCDIR)/%.s
	$(LCC) $(LCCFLAGS) -c -o $@ $<

# If needed, compile .c files in "src/" to .s assembly files
# (not required if .c is compiled directly to .o)
$(OBJDIR)/%.s:	$(SRCDIR)/%.c
	$(LCC) $(LCCFLAGS) -S -o $@ $<

# Link the compiled object files into a .gb ROM file
$(BINS):	$(OBJS)
	$(LCC) $(LCCFLAGS) -o $(BINS) $(OBJS)

prepare:
	mkdir -p $(OBJDIR)

clean:
#	rm -f  *.gb *.ihx *.cdb *.adb *.noi *.map
	rm -f  $(OBJDIR)/*.*
