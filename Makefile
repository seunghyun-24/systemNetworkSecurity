CXX = g++
CXXFLAGS = -W -Wall -O2
OBJECTS = deauth-attck.o
TARGET = deauth-attack
LDLIBS = -lpcap


all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(@) $(^) $(LDLIBS)

clean:
	rm -f $(TARGET)
	rm -f *.o

PHONY: all clean