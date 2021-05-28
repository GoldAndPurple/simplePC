INC = header/
BIN = 
OBJ = obj/
SRC = src/
EX = extra/
OUT = $(BIN)console.exe

FILES =  $(wildcard $(SRC)*.cpp)
TEMP = $(FILES:.cpp=.o)
OBJECTS = $(patsubst $(SRC)%,$(OBJ)%,$(TEMP))

CFLAGS = -g
IFLAGS = -I$(INC)

.PHONY: clear folders

all: $(OUT) EXTRA
clear:
		rm -f -r $(OBJ) $(BIN)
folders:
		mkdir obj

$(OUT): $(OBJECTS)
		g++ $(CFLAGS) $^ -o $@

$(OBJ)%.o: $(SRC)%.cpp
		g++ $(CFLAGS) $(IFLAGS) $< -c -o $@

EXTRA:
		g++ $(CFLAGS) $(IFLAGS) $(EX)assembler.cpp $(SRC)l1.cpp -o sat.exe
		g++ $(CFLAGS) $(IFLAGS) $(EX)basic.cpp -o basic.exe