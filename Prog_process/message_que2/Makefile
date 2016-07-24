.PHONY:all
all:client server
client:client.o comm.o
		gcc -o $@ $^
server:server.o comm.o
		gcc -o $@ $^
.PHONY:clean
clean:
	rm -f client.o server.o  comm.o client server

	

