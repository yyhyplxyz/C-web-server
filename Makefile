CC = gcc
LD = gcc
CFLAGS = -g -Wall -std=gnu99
LDFLAGS = -g
RM = rm -f

SRCS = server.c connection.c http_header.c request.c response.c stringutils.c config.c log.c
OBJS = $(addsuffix .o, $(basename $(SRCS)))
PROG = web

all: $(PROG)

$(PROG): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $(PROG)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

.PHONY: clean
clean:
	$(RM) $(PROG) $(OBJS)
