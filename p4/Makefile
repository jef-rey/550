CC = g++

CCFLAGS =  -Wall -Werror -std=c++11 --pedantic -g

TARGET = proj4
FULL_TARGET = $(TARGET)

all: $(TARGET) $(NEW_TARGET)

$(TARGET): $(TARGET).cpp
	$(CC) $(CCFLAGS) -o $(FULL_TARGET) $(TARGET).cpp

clean:
	-rm -f $(FULL_TARGET)
