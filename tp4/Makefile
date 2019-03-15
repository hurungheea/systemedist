CC = gcc
exec = client-td4.out
exes = server-td4.out

all: $(exes) $(exec)

$(exes): server.o
	$(CC) -g -o $(exes) server.c common.c calculs.c

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
