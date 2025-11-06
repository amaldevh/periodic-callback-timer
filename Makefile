CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic -O2
LDFLAGS = -pthread

TARGET = timer_example
SOURCES = example.cpp
HEADERS = HighPrecisionTimer.h

all: $(TARGET)

$(TARGET): $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
