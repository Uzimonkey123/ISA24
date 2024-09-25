CXX = g++
CXXFLAGS = -std=gnu++17 -Wall -Wextra -pedantic -g
LDFLAGS = -lpcap -lncurses -pthread

TARGET = isa-top

SRCS = KeyGenerator.cpp Bandwidth.cpp ConnectionManager.cpp Packet.cpp Exception.cpp parser.cpp main.cpp
HEADERS = $(SRCS:.cpp=.hpp)
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@ $(LDFLAGS)

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	$(RM) $(TARGET) $(OBJS)