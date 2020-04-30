CPP=clang++
CXXFLAGS=-g -Wall -std=c++17
SRC_DIR=./src
OBJ_DIR=./obj
SRC_FILES=$(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES=$(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
EXECUTABLE=fractals

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
	    $(CPP) $(CXXFLAGS) -o $(EXECUTABLE) $(SRC_FILES)

   
