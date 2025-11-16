CC = gcc
CFLAGS = -O2 -Wall -I./src -std=c11
LDFLAGS =
SRCDIR = src
BINDIR = bin
OBJDIR = obj
TARGET = $(BINDIR)/chip8

SRCS = $(wildcard $(SRCDIR)/*.c)
OBJS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))

all: sdl

build: CFLAGS += -DHEADLESS
build: prepare clean_objs $(TARGET)

sdl: CFLAGS += -DUSE_SDL
sdl: LDFLAGS += -lSDL2 -lSDL2_ttf
sdl: prepare clean_objs $(TARGET)

prepare:
	mkdir -p $(BINDIR) $(OBJDIR)

clean_objs:
	rm -f $(OBJDIR)/*.o

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -rf $(OBJDIR) $(BINDIR)

.PHONY: all build sdl clean prepare clean_objs
