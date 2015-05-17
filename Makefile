TARGET=bin/test

C=gcc
CC=g++
RM=rm -f
SRCDIR=src
BUILDDIR=build
CFLAGS=-g -Wall
CCFLAGS=-g -Wall -std=c++11
INCS=-Iinclude -I/usr/include/freetype2 -Ivendor/freetype-gl
LIBS=-lGL -lGLEW -lglfw -lfreeimage -lAntTweakBar -lfreetype -lnoise

GLSRC=$(wildcard $(SRCDIR)/gl/*.cc)
SCENESRC=$(wildcard $(SRCDIR)/scene/*.cc)
INPUTSRC=$(wildcard $(SRCDIR)/input/*.cpp)
FTGLSRC=$(wildcard vendor/freetype-gl/*.c)
SRC=$(wildcard $(SRCDIR)/*.cc)

GLOBJ=$(GLSRC:$(SRCDIR)/gl/%.cc=$(BUILDDIR)/gl/%.o)
SCENEOBJ=$(SCENESRC:$(SRCDIR)/scene/%.cc=$(BUILDDIR)/scene/%.o)
INPUTOBJ=$(INPUTSRC:$(SRCDIR)/input/%.cpp=$(BUILDDIR)/input/%.o)
FTGLOBJ=$(FTGLSRC:vendor/freetype-gl/%.c=$(BUILDDIR)/freetype-gl/%.o)
OBJ=$(SRC:$(SRCDIR)/%.cc=$(BUILDDIR)/%.o)

$(GLOBJ): $(BUILDDIR)/gl/%.o : $(SRCDIR)/gl/%.cc
	@echo "Compiling gl object files..."
	@mkdir -p $(@D)
	$(CC) $(CCFLAGS) $(INCS) -c $< -o $@

$(SCENEOBJ): $(BUILDDIR)/scene/%.o : $(SRCDIR)/scene/%.cc
	@echo "Compiling scene object files..."
	@mkdir -p $(@D)
	$(CC) $(CCFLAGS) $(INCS) -c $< -o $@

$(INPUTOBJ): $(BUILDDIR)/input/%.o : $(SRCDIR)/input/%.cpp
	@echo "Compiling input object files..."
	@mkdir -p $(@D)
	$(CC) $(CCFLAGS) $(INCS) -c $< -o $@

$(FTGLOBJ): $(BUILDDIR)/freetype-gl/%.o : vendor/freetype-gl/%.c
	@echo "Compiling freetype-gl object files..."
	@mkdir -p $(@D)
	$(C) $(CFLAGS) $(INCS) -c $< -o $@

$(OBJ): $(BUILDDIR)/%.o : $(SRCDIR)/%.cc
	@echo "Compiling object files..."
	@mkdir -p $(@D)
	$(CC) $(CCFLAGS) $(INCS) -c $< -o $@

$(TARGET): $(GLOBJ) $(SCENEOBJ) $(INPUTOBJ) $(FTGLOBJ) $(OBJ)
	@echo "Linking..."
	$(CC) $(CCFLAGS) $(INCS) $(LIBS) $(GLOBJ) $(SCENEOBJ) $(INPUTOBJ) $(FTGLOBJ) $(OBJ) -o $(TARGET)

all:	$(TARGET)

clean:
	@echo "Cleaning..."
	$(RM) -r $(BUILDDIR) $(TARGET)
