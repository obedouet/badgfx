# Makefile pour un Hello World !
# Cree le : 02/04/2002
# Par Bad Max

# Emplacement de la BadGFX
BADGFXDIR=../..

# Macros diverses
CC=g++
OPT=-std=c++17 -Wall -pipe -O
DIRECTIVES=-I$(BADGFXDIR)

# Specification des librairie a utiliser
LDFLAGS=-lX11

PROJET=helloworld

OBJECTS=\
 main.o\
 $(BADGFXDIR)/fenetre.o\
 $(BADGFXDIR)/bouton.o\
 $(BADGFXDIR)/message.o\
 $(BADGFXDIR)/composant.o

HEADERS=\
 $(BADGFXDIR)/badgfx.hpp\
 $(BADGFXDIR)/composant.hpp\
 $(BADGFXDIR)/fenetre.hpp\
 $(BADGFXDIR)/bouton.hpp\
 $(BADGFXDIR)/message.hpp

%.o: %.cpp $(HEADERS)
	$(CC) $(OPT) $(DIRECTIVES) -c $< -o $@

.PHONY: all clean menage

all: $(OBJECTS) $(HEADERS)
	$(CC) $(DIRECTIVES) -o $(PROJET) $(OBJECTS) $(LDFLAGS)

clean:
	rm -f $(OBJECTS) $(PROJET)

menage: clean
