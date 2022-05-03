
CFLAGS := -O3 -Wall -Wextra $(shell sdl2-config --cflags)
LIBS := $(shell sdl2-config --libs) -lSDL2_gfx
OBJECTS := e6809.o e8910.o osint.o vecx.o
TARGET := vecx
CLEANFILES := $(TARGET) $(OBJECTS)
BUILDDIR := build

$(TARGET): $(OBJECTS)
	mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)
	mv $(TARGET) $(OBJECTS) $(BUILDDIR)

clean:
	$(RM) $(CLEANFILES:%=$(BUILDDIR)/%) 
