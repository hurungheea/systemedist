CC = gcc
exec = client-td1.out
exes = server-td1.out

all: $(exes) $(exec)

$(exes): server.o
	$(CC) -g -o $(exes) server.c common.c

$(exec): client.o
	$(CC) -g -o $(exec) client.c common.c

client.o:
	$(CC) -g -o $(exec) -c client.c

server.o:
	$(CC) -g -o $(exec) -c server.c

clean:
	rm -f *.o

mrproper:
	rm -fr $(exec) $(exes)
	rm -f *.o
