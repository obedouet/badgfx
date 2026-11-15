# Makefile pour un Hello World !
# Cree le : 02/04/2002
# Par Bad Max

# Emplacement de la BadGFX
BADGFXDIR=../BadGFX

# Repertoires des includes 
PUBLIC=/usr/include -I/usr/X11R6/include -I. -I$(BADGFXDIR)

# Repertoires des bibliotheques necessaires au link
LIBDIR=/usr/lib -L/usr/X11R6/lib

# Macros diverses
CC=g++
OPT=-Wall -pipe -DUNIX -O
DIRECTIVES=-I$(PUBLIC) -L$(LIBDIR)

# Specification des librairie a utiliser
LDFLAGS=-lX11

.SUFFIXES: .c .o

PROJET=helloworld

# Pour tout module ajoute au projet, le rajouter
# dans la liste ci-dessous
OBJECTS=\
 main.o\
 $(BADGFXDIR)/fenetre.o\
 $(BADGFXDIR)/bouton.o

HEADERS=\
 $(BADGFXDIR)/composant.hpp\
 $(BADGFXDIR)/fenetre.hpp\
 $(BADGFXDIR)/bouton.hpp

############ COMPILATION ET EDITION DE LIENS ########
.cpp.o: $(HEADERS)
	$(CC) $(OPT) $(DIRECTIVES) -c $< -o $@

all: $(OBJECTS) $(HEADERS)
	$(CC) $(DIRECTIVES) -o $(PROJET) $(OBJECTS) $(LDFLAGS)

menage:
	rm -f *.o
	rm -f $(PROJET)

