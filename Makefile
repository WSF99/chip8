CC = gcc
CFLAGS = -O2 -Wall -I./src -std=c11
LDFLAGS =
SRCDIR = src
BINDIR = bin
OBJDIR = obj
TARGET = $(BINDIR)/chip8

SRCS = $(wildcard $(SRCDIR)/*.c)
OBJS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))

all: build

build: CFLAGS += -DHEADLESS
build: LDFLAGS +=
build: prepare $(TARGET)

sdl: CFLAGS += -DUSE_SDL
sdl: LDFLAGS += -lSDL2
sdl: prepare $(TARGET)

prepare:
	mkdir -p $(BINDIR) $(OBJDIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -rf $(OBJDIR) $(BINDIR)

.PHONY: all build sdl clean prepare
