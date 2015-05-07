TARGET=bin/test

C=gcc
CC=g++
RM=rm -f
SRCDIR=src
BUILDDIR=build
CFLAGS=-g -Wall -std=c++11
INCS=-Iinclude -I/usr/include/freetype2 -Ivendor/freetype-gl
LIBS=-lGL -lGLEW -lglfw -lfreeimage -lAntTweakBar -lfreetype


GLSRC=$(wildcard $(SRCDIR)/gl/*.cc)
SCENESRC=$(wildcard $(SRCDIR)/scene/*.cc)
FTGLSRC=$(wildcard vendor/freetype-gl/*.c)
SRC=$(wildcard $(SRCDIR)/*.cc)

GLOBJ=$(GLSRC:$(SRCDIR)/gl/%.cc=$(BUILDDIR)/gl/%.o)
SCENEOBJ=$(SCENESRC:$(SRCDIR)/scene/%.cc=$(BUILDDIR)/scene/%.o)
FTGLOBJ=$(FTGLSRC:vendor/freetype-gl/%.c=$(BUILDDIR)/freetype-gl/%.o)
OBJ=$(SRC:$(SRCDIR)/%.cc=$(BUILDDIR)/%.o)

$(GLOBJ): $(BUILDDIR)/gl/%.o : $(SRCDIR)/gl/%.cc
	@echo "Compiling gl object files..."
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCS) -c $< -o $@

$(SCENEOBJ): $(BUILDDIR)/scene/%.o : $(SRCDIR)/scene/%.cc
	@echo "Compiling scene object files..."
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCS) -c $< -o $@

$(FTGLOBJ): $(BUILDDIR)/freetype-gl/%.o : vendor/freetype-gl/%.c
	@echo "Compiling freetype-gl object files..."
	@mkdir -p $(@D)
	$(C) $(CFLAGS) $(INCS) -c $< -o $@

$(OBJ): $(BUILDDIR)/%.o : $(SRCDIR)/%.cc
	@echo "Compiling object files..."
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCS) -c $< -o $@

$(TARGET): $(GLOBJ) $(SCENEOBJ) $(FTGLOBJ) $(OBJ)
	@echo "Linking..."
	$(CC) $(CFLAGS) $(INCS) $(LIBS) $(GLOBJ) $(SCENEOBJ) $(FTGLOBJ) $(OBJ) -o $(TARGET)

all:	$(TARGET)

clean:
	@echo "Cleaning..."
	$(RM) -r $(BUILDDIR) $(TARGET)
