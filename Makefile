CC := g++
SRCDIR := src
BUILDDIR := build
TARGET := bin/DX

SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -g -Wall -Wextra
LIB := $$(pkg-config --libs allegro-5 allegro_primitives-5 allegro_acodec-5 allegro_image-5 allegro_ttf-5)
INC := -I include

$(TARGET): $(OBJECTS)
	@mkdir -p bin
	@echo " Linking..."
	@echo " $(CC) $^ -o $(TARGET) $(LIB)"; $(CC) $^ -o $(TARGET) $(LIB)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	@echo " Cleaning...";
	@echo " $(RM) -rv $(BUILDDIR) $(TARGET)"; $(RM) -rv $(BUILDDIR) $(TARGET)

run:
	./bin/DX

.PHONY: clean
