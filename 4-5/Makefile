all: server first second

server: server.c utils.c
	gcc server.c utils.c -o server -lpthread -g3

first: first.c utils.c
	gcc first.c utils.c -o first -g3

second: second.c utils.c
	gcc second.c utils.c -o second

clean:
	rm server first second
