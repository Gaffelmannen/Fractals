#CC=clang++
CXXFLAGS=-g -Wall -std=c++17
SRC_DIR=./src
LIB_DIR=./lib
OBJ_DIR=./obj
SRC_FILES=$(wildcard $(SRC_DIR)/*.cpp $(LIB_DIR)/*.cpp)
OBJ_FILES=$(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
EXECUTABLE=fractals

ifeq ($(TARGET),mac)
	CC = clang++
else
	CC = g++
endif

.PHONY : clean cleanall cleanobj all

default: clean all

clean : cleanall

cleanall : cleanobj
	    rm -f $(EXECUTABLE)

cleanobj :
	    rm -f *.o
	    rm -rf ${EXECUTABLE}.dSYM/

$(OBJ_DIR):
	  mkdir $(OBJ_DIR)

all :
	    $(CC) $(CXXFLAGS) -o $(EXECUTABLE) $(SRC_FILES)

   
