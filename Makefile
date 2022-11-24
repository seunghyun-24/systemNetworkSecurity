all: eco-client-server
	  g++ -Wall -W -O2 -o $(@) client.cpp
	  g++ -Wall -W -O2 -o $(@) server.cpp
	
clean:
	  rm -f *.o
	  rm -f eco-client-server

.PHONY: all clean
