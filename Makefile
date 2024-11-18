CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -g
LDFLAGS = -lpcap -lncurses -pthread

TARGET = isa-top

SRCS = Display.cpp Connection.cpp KeyGenerator.cpp Bandwidth.cpp ConnectionManager.cpp Packet.cpp Exception.cpp parser.cpp main.cpp
HEADERS = $(SRCS:.cpp=.hpp)
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@ $(LDFLAGS)

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

pack:
	tar -cf xbabia01.tar $(SRCS) $(HEADERS) Makefile isa-top.1 manual.pdf ./tests

clean:
	$(RM) $(TARGET) $(OBJS)