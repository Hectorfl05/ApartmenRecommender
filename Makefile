CXX ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -O2 -Iinclude -Isrc
LDFLAGS ?= -ljsoncpp

SRCS = src/main.cpp src/Apartment.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = ApartmentRecommender

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
