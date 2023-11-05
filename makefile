CC = g++
CFLAGS = -c -Iinclude
MODE_FLAG = -g -Wall -DDEBUG
# MODE_FLAG = -03
.DEFAULT_GOAL := all
INCLUDE_DIR = include
BIN_DIR = bin
SRC_DIR = src
TEST_DIR = tests
TARGET = $(BIN_DIR)/testHOG
OBJECTS = $(BIN_DIR)/HOG-SSP.o $(BIN_DIR)/HOG-SK.o $(BIN_DIR)/timer.o $(BIN_DIR)/Aho-Cora.o

all : sk run

sk : $(OBJECTS) $(TEST_DIR)/testHOG.cpp
	$(CC) $(MODE_FLAG) $^ -o $(TARGET)

ssp : $(OBJECTS) $(TEST_DIR)/testHOG.cpp
	$(CC) $(MODE_FLAG) $^ -DSSP -o $(TARGET)

$(BIN_DIR)/%.o : $(SRC_DIR)/%.cpp $(INCLUDE_DIR)/%.h
	$(CC) $(CFLAGS) $(MODE_FLAG) $< -o $@

run : $(TARGET)
	./$(TARGET)

clean :
	rm -rf $(BIN_DIR)/*

.PHONY : clean sk ssp