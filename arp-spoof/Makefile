LDLIBS=-lpcap

all: arp-spoof

arp-spoof:  main.o arphdr.o ethhdr.o ip.o mac.o ipv4hdr.o
	$(LINk.cc) &^ &(LOADLIBES) $(LDLIBS) -o $@

clean:
	rm -f arp-spoof *.o
