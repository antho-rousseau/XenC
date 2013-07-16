CC=g++
MACHINE:=$(shell sbin/machine-type)
BITS:=$(shell sbin/machine-bits)
# ----
BOOST=/usr
INCBOOST=$(BOOST)/include/boost
ifeq ($(BITS),64)
	ifneq "$(wildcard $(BOOST)/lib64)" ""
        	LIBBOOST=$(subst include/boost,lib64,$(INCBOOST))
	else
		LIBBOOST=$(subst include/boost,lib,$(INCBOOST))
	endif
else
        LIBBOOST=$(subst include/boost,lib,$(INCBOOST))
endif
SRILM=/usr/local
INCSRILM=$(SRILM)/include
LIBSRILM=$(subst include,lib/$(MACHINE),$(INCSRILM))
# ----

# ----
CFLAGS=-Wall -O2 -Iinclude -Iinclude/modes -Iinclude/utils -I$(INCBOOST) -I$(INCSRILM)
LDFLAGS=-L$(LIBBOOST) -L$(LIBSRILM)
LDLIBS=-lz -lboost_regex-mt -lboost_system-mt -lboost_filesystem-mt -lboost_iostreams-mt -lboost_program_options-mt -lboost_thread-mt -loolm -ldstruct -lmisc -lpthread
EXEC=bin/XenC
# ----

# ----
EXECDIR=bin
MODDIR=obj/modes
UTIDIR=obj/utils
# ----

# ----
SRC=$(wildcard src/*.cpp)
MODSRC=$(wildcard src/modes/*.cpp)
UTISRC=$(wildcard src/utils/*.cpp)
OBJ=$(patsubst src/%.cpp,obj/%.o,$(SRC))
MODOBJ=$(patsubst src/modes/%.cpp,obj/modes/%.o,$(MODSRC))
UTIOBJ=$(patsubst src/utils/%.cpp,obj/utils/%.o,$(UTISRC))
# ----

all: makedir $(EXEC)

debug: CFLAGS += -g
debug: makedir $(EXEC)

$(EXEC): $(MODOBJ) $(UTIOBJ) $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS) $(LDLIBS)

obj/modes/%.o: src/modes/%.cpp
	$(CC) -o $@ -c $< $(CFLAGS)

obj/utils/%.o: src/utils/%.cpp
	$(CC) -o $@ -c $< $(CFLAGS)

obj/%.o: src/%.cpp
	$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean mrproper

clean:
	rm -rf obj/*

mrproper: clean
	rm $(EXEC)

makedir:
	mkdir -p $(EXECDIR) $(MODDIR) $(UTIDIR)
