CXX = g++
CXXFLAGS = -W -Wall -O2
OBJECTS = deauth-attack.o mac.o
TARGET = deauth-attack mac
LIBS = -lpcap


all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(@) $(^) $(LIBS)

clean:
	rm -f $(TARGET)
	rm -f *.o

PHONY: all clean
