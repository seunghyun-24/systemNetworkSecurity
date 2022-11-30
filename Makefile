CXX = g++
CXXFLAGS = -W -Wall -O2
OBJECTS = airodump.o
TARGET = airodump
LIBS = -lpcap

all: $(TARGET)

$(TARGET): $(OBJECTS)
    $(CXX) $(CXXFLAGS) -o $(@) $(^) $(LIBS)

$(OBJECTS): airodump.cpp

clean:
    rm -f $(TARGET)
    rm -f *.o

.PHONY: all clean

