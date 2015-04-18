TARGET=bin/test

CC=g++
RM=rm -f
SRCDIR=src
BUILDDIR=build
CFLAGS=-g -Wall -std=c++11
INCS=-Iinclude
LIBS=-lGL -lGLEW -lglfw


GLSRC=$(wildcard $(SRCDIR)/gl/*.cc)
SCENESRC=$(wildcard $(SRCDIR)/scene/*.cc)
SRC=$(wildcard $(SRCDIR)/*.cc)

GLOBJ=$(GLSRC:$(SRCDIR)/gl/%.cc=$(BUILDDIR)/gl/%.o)
SCENEOBJ=$(SCENESRC:$(SRCDIR)/scene/%.cc=$(BUILDDIR)/scene/%.o)
OBJ=$(SRC:$(SRCDIR)/%.cc=$(BUILDDIR)/%.o)

$(GLOBJ): $(BUILDDIR)/gl/%.o : $(SRCDIR)/gl/%.cc
	@echo "Compiling gl object files..."
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCS) -c $< -o $@

$(SCENEOBJ): $(BUILDDIR)/scene/%.o : $(SRCDIR)/scene/%.cc
	@echo "Compiling scene object files..."
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCS) -c $< -o $@

$(OBJ): $(BUILDDIR)/%.o : $(SRCDIR)/%.cc
	@echo "Compiling object files..."
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCS) -c $< -o $@

$(TARGET): $(GLOBJ) $(SCENEOBJ) $(OBJ)
	@echo "Linking..."
	$(CC) $(CFLAGS) $(INCS) $(LIBS) $(GLOBJ) $(SCENEOBJ) $(OBJ) -o $(TARGET)

all:	$(TARGET)

clean:
	@echo "Cleaning..."
	$(RM) -r $(BUILDDIR) $(TARGET)
