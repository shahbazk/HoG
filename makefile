CC = g++
DEBUGFLAG = -DDEBUG
CFLAGS = -c -Wall
.DEFAULT_GOAL := run
BIN_DIR = bin
SRC_DIR = src
TEST_DIR = tests
TARGET = $(BIN_DIR)/testHOG
OBJECTS = $(BIN_DIR)/Aho-Cora.o $(BIN_DIR)/HOG-SSP.o $(BIN_DIR)/HOG-SK.o $(BIN_DIR)/testHOG.o $(BIN_DIR)/timer.o

$(TARGET) : $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)

$(BIN_DIR)/testHOG.o : $(TEST_DIR)/testHOG.cpp
	$(CC) $(CFLAGS) $< -o $@ $(DEBUGFLAG)

$(BIN_DIR)/%.o : $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) $< -o $@ $(DEBGUFLAG)

run : $(TARGET)
	./$(TARGET)

clean :
	rm -rf $(BIN_DIR)/*