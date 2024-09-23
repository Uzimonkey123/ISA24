CXX = g++
CXXFLAGS = -std=gnu++20 -Wall -Wextra -Werror -pedantic -g

TARGET = isa-top

SRCS = $(wildcard *.cpp)
HEADERS = $(wildcard *.h)
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	$(RM) $(TARGET) $(OBJS)