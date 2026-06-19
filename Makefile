CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude
SRC = src/Book.cpp src/Member.cpp src/Transaction.cpp src/FileManager.cpp src/Library.cpp src/Auth.cpp src/main.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = library_system

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
