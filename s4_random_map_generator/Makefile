GDPINCLUDES:=../include
GDPLIBS:=../lib

#$(info $$WSGDPINCLUDES is [${WSGDPINCLUDES}])
#$(info $$WSGDPLIBS is [${WSGDPLIBS}])

WSGDPINCLUDES?=.
WSGDPLIBS?=.

CC=g++
CFLAGS=-Wall -W -Wextra -std=c++14 -pedantic -O3
INCLUDES=-I include/ -I $(GDPINCLUDES) -I $(WSGDPINCLUDES)
LIBS=-L $(GDPLIBS) -L $(WSGDPLIBS)
LIBS+=-lgdptools -lgdpc -lX11 -lpthread -lpng -lz -lm

ifdef SystemRoot
	UNAME=C:\MinGW\msys\1.0\bin\uname.exe
else
	UNAME=uname
endif

# Mac OS X
ifneq ($(findstring Darwin, $(shell $(UNAME) -s)),)
INCLUDES+=-I /usr/X11/include
LIBS+=-L /usr/X11/lib
else
# MinGW
ifneq ($(findstring MINGW, $(shell $(UNAME) -s)),)
LIBS+=-lXau -lws2_32
endif
endif

HEADERS=$(wildcard include/*.h)
SOURCES=$(wildcard src/*.cpp)
OBJECTS=$(patsubst %.cpp, %.o, $(SOURCES))
DRIVERSSOURCES=$(wildcard drivers/*.cpp)
DRIVERS=$(basename $(DRIVERSSOURCES))

default: $(DRIVERS)

debug: CFLAGS+=-O0 -ggdb # -Og overwrites previous -O3
debug: $(DRIVERS)

all: $(DRIVERS)

$(OBJECTS): %.o: %.cpp $(HEADERS)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(DRIVERS): %: %.cpp $(HEADERS) $(OBJECTS)
	-$(CC) $(CFLAGS) $(INCLUDES) $< $(OBJECTS) $(LIBS) -o $@

doc: $(SOURCES) $(HEADERS)
	@doxygen

clean:
ifdef SystemRoot
	del /S/Q doc $(subst /,\,$(addsuffix .exe,$(DRIVERS))) $(subst /,\,$(OBJECTS))
else
	rm -rf doc $(DRIVERS) $(OBJECTS)
endif
