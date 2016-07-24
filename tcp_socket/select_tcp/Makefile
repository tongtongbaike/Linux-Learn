cc = gcc
tcp_client = tcp_client.c
tcp_server = tcp_server.c

.PHONY:all
all:client server
client:$(tcp_client)
	$(cc) -o $@ $^

server:$(tcp_server)
	$(cc) -o $@ $^


.PHONY:clean
clean:
	rm client server
