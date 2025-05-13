# Makefile for GraphSketch Project

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++17 -O2 -Wall

# Include directories
INCLUDES = -Iinclude

# Source files
SRC = src/main.cpp src/GraphSketchTensor.cpp

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
