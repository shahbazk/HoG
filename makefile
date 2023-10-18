CC = g++
DEBUGFLAG = -DDEBUG
CFLAGS = -c -Wall
.DEFAULT_GOAL := run
TARGET = bin/testAho-Cora
OBJECTS = tests/testAho-Cora.o src/Aho-Cora.o src/HOG-SSP.o

$(TARGET) : $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)

%.o : %.cpp
	$(CC) $(CFLAGS) $< -o $@ $(DEBGUFLAG)

run : $(TARGET)
	./$(TARGET)