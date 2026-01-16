CC = clang
CFLAGS = -g -Wall -Wextra -Werror -I.

BUILDDIR = build

SRCS = main.c vector/vector.c hashmap/hashmap.c
OBJS = $(patsubst %.c,$(BUILDDIR)/%.o,$(SRCS))

TARGET = $(BUILDDIR)/main

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

$(BUILDDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJS): | $(BUILDDIR)

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

clean:
	rm -rf $(BUILDDIR)
