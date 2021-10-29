# Les différents répertoires
HEADDIR := ./include
BINDIR  := ./bin
OBJDIR	:= ./obj
SRCDIR 	:= ./src

# Options de compilation
CFLAGS = -g -Wall -Wextra -O2 -std=c99
CC 	= gcc
EXEC = $(BINDIR)/lzw

# Emplacement des fichiers
SOURCES := $(wildcard ${SRCDIR}/*.c)
HEADERS := $(wildcard ${HEADDIR}/*.h)
OBJ 		:= $(SOURCES:src$(EXEC)/%.c=obj/%.o)

# Makefile
all: $(EXEC)

# Création de l'exécutable
$(EXEC): $(OBJ)
	mkdir -p $(BINDIR)
	$(CC) -o $(EXEC) $^ $(CFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	mkdir -p $(OBJDIR)
	$(CC) -o $@ -c $< $(CFLAGS)

# Verifier fuites de memoire et autres
valgrind: 
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind-out.txt $(EXEC) 

# Nettoyage complet
clean:
	rm -rf $(OBJDIR) $(BINDIR) $(DOCDIR) ../$(DIST).tar.xz $(EXEC)
