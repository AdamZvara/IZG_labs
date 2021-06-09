################################################################################
# IZG project, framework compilation
################################################################################

################################################################################
# promenne prekladu

ifeq ($(OS), Windows_NT)
    INCS = -I../SDL-1.2.15/include -IQ:/mingw/SDL/SDL
    LIBS = -lmingw32 -lSDLmain -lSDL -L../SDL-1.2.15/lib -LQ:/mingw/SDL
    CFLAGS = -m32
    REMOVE = del
    BIN = izg_lab_04.exe
else
    INCS = -I/usr/include/SDL
    LIBS = -lSDL
#    LIBS=-m32 -lSDL
#    LIBS=-m64 -lSDL
    REMOVE = rm -f
    BIN = izg_lab_04
endif

CC = gcc

CFLAGS += -std=c99 -g3 -Wall $(INCS) -Wno-unused-function -fmessage-length=0
#CFLAGS = -m32 -g3 -Wall $(INCS) -Wno-unused-function -fmessage-length=0
#CFLAGS = -m64 -g3 -Wall $(INCS) -Wno-unused-function -fmessage-length=0

################################################################################
# vyjmenovane obj soubory pro uziti implicitniho pravidla kompilace/linkovani
OBJ				= main.o student.o vector.o color.o

%.o : %.cpp
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

################################################################################
# obecny cil kompilace
all: $(BIN)

################################################################################
# kompilace a linkovani vysledneho souboru
$(BIN):	$(OBJ)
	$(CC) $(OBJ) -o $(BIN) $(LIBS) -lm

################################################################################
# cil vycisteni souboru prekladu
clean: clear
clear:
	$(REMOVE) $(OBJ) $(BIN)

################################################################################
################################################################################
