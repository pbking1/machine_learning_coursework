FILES=main.cpp
OBJ=${FILES:.cpp=.o}
FLAGS=-Wall -g -o0 -pedantic `pkg-config --cflags --libs opencv`

EXE=pov-semantic-segmentation

CC=g++
RM=rm -rf

all: main.o 
	$(CC) $(FLAGS) $(LIBS) $(OBJ) -o $(EXE)

run:
	./$(EXE)

clean:
	$(RM) *.o $(EXE)
