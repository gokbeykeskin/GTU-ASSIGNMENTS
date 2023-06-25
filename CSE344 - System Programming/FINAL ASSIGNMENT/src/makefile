
.PHONY: all
all: server servant client

server: server.o utils.o queue.o networking.o
	gcc -o $@ $^ -pthread -Wall
servant: servant.o utils.o networking.o
	gcc -o $@ $^ -pthread -Wall
client: client.o utils.o networking.o
	gcc -o $@ $^ -pthread -Wall

%.o: code/%.c
	gcc -c $< -o $@


.PHONY: clean
clean:
	rm	-f *.o server servant client

.PHONY: killsv
killsv:
	killall -2 server
