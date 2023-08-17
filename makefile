CXX = g++
CXXFLAGS = -std=c++11 -Wall -I./include

# Source files
SRCS = src/main.cpp src/helpers.cpp

# Output binary name
OUTPUT = main

all: $(OUTPUT)

$(OUTPUT): $(SRCS)
	$(CXX) $(CXXFLAGS) $^ -o ./build/$@
run:
	./build/$(OUTPUT)

clean:
	rm -f $(OUTPUT)

