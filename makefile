CC = g++
DEBUGFLAG = -DDEBUG
CFLAGS = -c -Wall
.DEFAULT_GOAL := SK
BIN_DIR = bin
SRC_DIR = src
TEST_DIR = tests
TARGET = $(BIN_DIR)/testHOG
OBJECTS= $(BIN_DIR)/HOG-SSP.o $(BIN_DIR)/HOG-SK.o $(BIN_DIR)/testHOG.o $(BIN_DIR)/timer.o $(BIN_DIR)/Aho-Cora.o
FLAG1 = -DSSP

MODIFY_FLAG:
	$(eval FLAG1 = )

$(TARGET) : $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)

$(BIN_DIR)/testHOG.o : $(TEST_DIR)/testHOG.cpp
	$(CC) $(CFLAGS) $< -o $@ $(FLAG1) $(DEBUGFLAG)

$(BIN_DIR)/%.o : $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) $< -o $@ $(DEBUGFLAG)

SK : MODIFY_FLAG run
SSP : run
run : $(TARGET)
	./$(TARGET)

clean :
	rm -rf $(BIN_DIR)/*