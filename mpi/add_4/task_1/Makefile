CC = mpicc

SRC = ./src/

all: client server

server: $(SRC)server.c
	$(CC) -o server.out $^

client: $(SRC)client.c
	$(CC) -o client.out $^

master: $(SRC)master.c
	$(CC) -o master.out $^
	
run: master server client
	mpirun -np 1 ./master.out
	
clean:
	rm -rf server.out client.out master.out