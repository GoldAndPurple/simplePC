INC = header/
BIN = 
OBJ = obj/
SRC = src/
OUT = $(BIN)program.exe

FILES =  $(wildcard $(SRC)*.cpp)
TEMP = $(FILES:.cpp=.o)
OBJECTS = $(patsubst $(SRC)%,$(OBJ)%,$(TEMP))

CFLAGS = -Werror
IFLAGS = -I$(INC)

.PHONY: clear folders

all: $(OUT)
clear:
		rm -f -r $(OBJ) $(BIN)
folders:
		mkdir bin obj

$(OUT): $(OBJECTS)
		g++ $(CFLAGS) $^ -o $@

$(OBJ)%.o: $(SRC)%.cpp
		g++ $(CFLAGS) $(IFLAGS) $< -c -o $@