CFLAGS += -Wall -Werror

EXES = status

SRCS = ${EXES:=.c}
OBJS = ${EXES:=.o}

all: $(EXES)

status: status.c
	$(CC) $(CFLAGS) $< -o $@ -lcurses

clean:
	rm -f $(EXES) $(OBJS)
