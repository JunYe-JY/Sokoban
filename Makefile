CC = g++
CFLAGS = --std=c++17 -Wall -Werror -pedantic -g
LIB = -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system -lboost_unit_test_framework

DEPS = Sokoban.hpp

OBJECTS = Sokoban.o

PROGRAM = Sokoban test

STATIC_LIB = Sokoban.a

.PHONY: all clean lint

all: $(PROGRAM) $(STATIC_LIB)

%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c $<

test: test.o $(STATIC_LIB)
	$(CC) $(CFLAGS) -o $@ $^ $(LIB)

Sokoban: main.o $(STATIC_LIB)
	$(CC) $(CFLAGS) -o $@ $^ $(LIB)

$(STATIC_LIB): $(OBJECTS)
	ar rcs $@ $^

clean:
	rm *.o $(PROGRAM) $(STATIC_LIB)

lint:
	cpplint *.cpp *.hpp
