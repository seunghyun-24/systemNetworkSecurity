CXX = g++
CXXFLAGS = -W -Wall -O2
OBJECTS = deauth-attack.o
TARGET = deauth-attack
LIBS = -lpcap


all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(@) $(^) $(LIBS)

clean:
	rm -f $(TARGET)
	rm -f *.o

PHONY: all clean
