TARGET=bin/test

CC=g++
RM=rm -f
SRCDIR=src
BUILDDIR=build
CFLAGS=-g -Wall
INCS=-Iinclude
LIBS=-lGL -lglfw

SRC=$(wildcard $(SRCDIR)/*.cc)
OBJ=$(SRC:$(SRCDIR)/%.cc=$(BUILDDIR)/%.o)

$(OBJ): $(BUILDDIR)/%.o : $(SRCDIR)/%.cc
	@echo "Compiling object files..."
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

$(TARGET): $(OBJ)
	@echo "Linking..."
	$(CC) $(CFLAGS) $(INCS) $(LIBS) $(OBJ) -o $(TARGET)

all:	$(TARGET)

clean:
	@echo "Cleaning..."
	$(RM) -r $(BUILDDIR) $(TARGET)
