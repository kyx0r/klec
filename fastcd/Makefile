OBJS1 = fastcd-server.c data.c utils.c
OBJS2 = fastcd-client.c

all: fastcd-server fastcd-client

fastcd-server:
	$(CC) -o fastcd-server $(OBJS1) $(LDFLAGS) $(CFLAGS)
fastcd-client:
	$(CC) -o fastcd-client $(OBJS2) $(LDFLAGS) $(CFLAGS)
clean:
	rm -f *.o fastcd-client fastcd-server 
install: all
	cp -f fastcd-client /usr/bin
	cp -f fastcd-server /usr/bin

.PHONY: all clean install
