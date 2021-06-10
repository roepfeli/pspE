TARGET = snake
CFLAGS = -g -G0 -Wall -O2
CXXFLAGS = $(CLFAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

INCDIR := $(wildcard include/)

LIBS := -lpspvram -lpspgum -lpspgu -lstdc++ -lm

BUILD_PRX = 1
EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = snake

SRC 	:= $(wildcard src/*.cpp) $(wildcard src/*/*.cpp) # if subfolder exist in src
SRC 	+= $(wildcard src/*.c) $(wildcard src/*/*.c) # if subfolder exist in src
OBJS	:= $(SRC:%.cpp=%.o)

.PHONY: clean print

# print:
# 	$(info $$OBJS is [${OBJS}])

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak