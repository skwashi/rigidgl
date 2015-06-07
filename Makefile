TARGET=bin/gamma

C=gcc
CC=g++
RM=rm -f
SRCDIR=src
BUILDDIR=build
BINDIR=bin
CFLAGS=-g -Wall
#CCFLAGS=-g -Wall -std=c++11
CCFLAGS=-O3 -pipe -Wall -std=c++11
INCS=-Iinclude
LIBS=-lGL -lGLEW -lglfw -lfreeimage -lAntTweakBar -lnoise

GLSRC=$(wildcard $(SRCDIR)/gl/*.cc)
SCENESRC=$(wildcard $(SRCDIR)/scene/*.cc)
INPUTSRC=$(wildcard $(SRCDIR)/input/*.cpp)
SRC=$(wildcard $(SRCDIR)/*.cc)

GLOBJ=$(GLSRC:$(SRCDIR)/gl/%.cc=$(BUILDDIR)/gl/%.o)
SCENEOBJ=$(SCENESRC:$(SRCDIR)/scene/%.cc=$(BUILDDIR)/scene/%.o)
INPUTOBJ=$(INPUTSRC:$(SRCDIR)/input/%.cpp=$(BUILDDIR)/input/%.o)
OBJ=$(SRC:$(SRCDIR)/%.cc=$(BUILDDIR)/%.o)

$(BINDIR):
	@mkdir -p $(BINDIR)

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


$(OBJ): $(BUILDDIR)/%.o : $(SRCDIR)/%.cc
	@echo "Compiling object files..."
	@mkdir -p $(@D)
	$(CC) $(CCFLAGS) $(INCS) -c $< -o $@

$(TARGET): $(GLOBJ) $(SCENEOBJ) $(INPUTOBJ) $(OBJ)
	@echo "Linking..."
	$(CC) $(CCFLAGS) $(INCS) $(LIBS) $(GLOBJ) $(SCENEOBJ) $(INPUTOBJ) $(OBJ) -o $(TARGET)

all: $(BINDIR) $(TARGET)

clean:
	@echo "Cleaning..."
	$(RM) -r $(BUILDDIR) $(TARGET)
