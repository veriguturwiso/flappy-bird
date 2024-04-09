CC = g++
CFLAGS = -Wall

VPATH = src

SOURCE_FILES = $(wildcard src/*.cpp) $(wildcard src/rendering/*.cpp) $(wildcard src/io/*.cpp) $(wildcard src/math/*.cpp)
DEFAULT_LIBRARIES = -lgdi32 -lpng -lz

all: main 

main: main.cpp 
	$(CC) $(CFLAGS) -o main $(SOURCE_FILES) dependencies/glad.c -Isrc -Idependencies/includes -Ldependencies/libs -lopengl32 -lglfw3 -lfreetype $(DEFAULT_LIBRARIES) 

clean: 
	rm -f main
