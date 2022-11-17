all: 1m-block
	g++ -Wall -W -O2 -o $(@) 1m-block.cpp -lnetfilter_queue

clean:
	rm -f *.o
	rm -f 1m-block

.PHONY: all clean
