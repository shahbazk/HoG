CC = g++
DEBUGFLAG = -DDEBUG
CFLAGS = -c -Wall
.DEFAULT_GOAL := run
BIN_DIR = bin
SRC_DIR = src
TEST_DIR = tests
TARGET = $(BIN_DIR)/testAho-Cora
OBJECTS = $(BIN_DIR)/Aho-Cora.o $(BIN_DIR)/HOG-SSP.o $(BIN_DIR)/testAho-Cora.o

$(TARGET) : $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)

$(BIN_DIR)/testAho-Cora.o : $(TEST_DIR)/testAho-Cora.cpp
	$(CC) $(CFLAGS) $< -o $@ $(DEBGUFLAG)

$(BIN_DIR)/%.o : $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) $< -o $@ $(DEBGUFLAG)

run : $(TARGET)
	./$(TARGET)

clean :
	rm -rf $(BIN_DIR)/*.o $(TARGET)