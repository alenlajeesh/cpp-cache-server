CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -pthread -Iinclude

SRC = src/main.cpp \
      src/server.cpp \
      src/database.cpp \
      src/command_parser.cpp \
      src/persistence.cpp

TARGET = build/cache-server

all: $(TARGET)

$(TARGET): $(SRC)
	mkdir -p build
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

run: all
	./$(TARGET)

clean:
	rm -rf build
