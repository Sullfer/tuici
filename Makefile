cc = gcc
cflags = -Wall -Wextra -O3 -std=c11 -Iinclude -Ithird_party
ldlibs = -lm

src = src/main.c src/image.c src/tui.c src/render.c
obj = $(src:.c=.o)
bin = tiv

all: $(bin)

$(bin): $(obj)
	$(cc) $(cflags) -o $@ $^ $(ldlibs)

clean:
	rm -f $(obj) $(bin)