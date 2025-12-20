CC = clang
CFLAGS = -g -Wall -Wextra

SRCDIR = .
BUILDDIR = build

SRCS = main.c vector/vector.c
OBJS = $(patsubst %.c,$(BUILDDIR)/%.o,$(SRCS))

TARGET = $(BUILDDIR)/main

all: $(TARGET)

$(TARGET): $(OBJS) | $(BUILDDIR)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

$(BUILDDIR)/%.o: %.c | $(BUILDDIR)
	@mkdir -p $(dir $@)   # Ensure subdirectories exist
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

clean:
	rm -rf $(BUILDDIR) $(TARGET)