CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

SRC = src/main.cpp src/server.cpp src/database.cpp src/command_parser.cpp
OUT = build/mini-redis

all:
	mkdir -p build
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT)

run: all
	./$(OUT)

clean:
	rm -rf build
