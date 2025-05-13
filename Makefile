# Makefile for GraphSketch Project

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++17 -O2 -Wall

# Include directories
INCLUDES = -Iinclude

# Source files
<<<<<<< HEAD
SRC = src/main.cpp src/GraphSketchTensor.cpp
=======
<<<<<<< HEAD
SRC = src/main.cpp
=======
SRC = src/main.cpp src/GraphSketchTensor.cpp
>>>>>>> e0f993a (local changes before pulling)
>>>>>>> 724517a (adding darpa data)

# Output executable name
TARGET = graphsketch

# Default target
all: $(TARGET)

# Build target
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(TARGET) $(SRC)

# Clean target
clean:
	rm -f $(TARGET)
