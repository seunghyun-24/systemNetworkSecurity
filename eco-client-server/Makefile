all: eco-client-server

eco-client-server: client.o server.o
	g++ -Wall -W -O2 -o eco-client-server client.o server.o

client.o: client.cpp
	g++ -Wall -W -O2 -o client.o client.cpp

server.o: server.cpp
	g++ -Wall -W -O2 -o server.o server.cpp	
	
clean:
	rm -f *.o
	rm -f eco-client-server

.PHONY: all clean
