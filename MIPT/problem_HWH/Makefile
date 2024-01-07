NODEBUG ?= 1
ifeq ($(NODEBUG), 1)
    CFLAGS = -std=gnu99 -Wall -O2 -DNDEBUG
else
    CFLAGS = -std=gnu99 -Wall -fsanitize=undefined -fsanitize=address -fsanitize=leak -O2
endif

all: hash

hashtable.o: hashtable.c hashtable.h

%.o: %.c
	cc $(CFLAGS) -o $@ -c $<

hash: hashtable.o main.c
	cc $(CFLAGS) hashtable.o main.c -o hash

clean:
	rm -f hash
