CPPFLAGS = -std=c++11 -Wall -Werror -g3

CC=g++

SOURCES=$(wildcard *.cpp)
OBJECTS=$(SOURCES:.cpp=.o)

TARGET=main

clean:
	rm -f *.o $(TARGET) *.out
	rm -rf ./test/*.o ./test/tests 

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) -o $@ $^ $(CPPFLAGS)

%.o: %.cpp
	$(CC) $(CPPFLAGS) -c $< -o $@