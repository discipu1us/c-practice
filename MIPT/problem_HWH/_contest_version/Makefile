NODEBUG ?= 1
ifeq ($(NODEBUG), 1)
    CFLAGS = -std=gnu99 -Wall -O2 -DNDEBUG
else
    CFLAGS = -std=gnu99 -Wall -fsanitize=undefined -fsanitize=address -fsanitize=leak -O2
endif

hash: main.c
	cc $(CFLAGS) main.c -o hash

clean:
	rm -f hash
