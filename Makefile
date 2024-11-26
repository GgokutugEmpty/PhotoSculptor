CXX = g++
CXXFLAGS = -Wall -std=c++17

TARGET = PhotoSculptor

SRCS = main.cpp bmpops.cpp

OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

clean:
	rm -f $(OBJS) $(TARGET)
