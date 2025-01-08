# makefile

# ----------> Variable

# chemin
DINC = inc
DSRC = src
DOBJ = obj
DEXE = bin

# fichiers
FOBJ1 = $(DOBJ)/vessel.o $(DOBJ)/input.o $(DOBJ)/projectile.o  $(DOBJ)/star.o
FOBJ2 =  $(DOBJ)/gui.o $(DOBJ)/player.o  $(DOBJ)/image.o $(DOBJ)/gameplay.o 
FOBJ3 = $(DOBJ)/game.o $(DOBJ)/enemy.o $(DOBJ)/main.o
OBJ = $(FOBJ1) $(FOBJ2) $(FOBJ3)
EXE = prog

# variable designant le compilateur (generalement CC pour le C , CXX pour le C++)
CC = gcc

# variable regoroupant les options de compilation (generalement CFLAGS pour le C , CXXFLAGS pour le C++)
CLINK = -g  -Og
COPT = -W -Wall -Werror -ansi -pedantic -O -std=c99
CFLAGS = $(CLINK) $(COPT)

# regroupe dans ces dependances l'ensemble des executables à produire
all : $(EXE)

# permet de supprimer tous les fichiers intermediaires
clean : 
	rm -r $(DOBJ)/*.o
	rm -r $(DEXE)/$(EXE)

# supprime tous ce qui peut etre regenere et permet une reconstruction complet du projet
mrproper : clean
	rm -r $(DEXE)/$(EXE)

$(EXE) : $(OBJ)
	$(CC) $^ -o $(DEXE)/$(EXE) $(CFLAGS)

$(DOBJ)/%.o : $(DSRC)/%.c
	$(CC) -c $< -o $@ $(CFLAGS)

# $@ : Nom de la cible
# $< : Nom de la premiere dependence
# $^ : liste des dependances
# $? : liste des dependances plus recentes que la cible
# $* : Nom du fichier sans suffixe
