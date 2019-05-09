CC = g++
LIBS = -lglfw -lGL -lm -lX11 -lglut -lGLU
CFLAGS = -g -Wall -Wno-deprecated -Wextra
OBJ = main.o Point.o Pecas.o

%.o: %.cpp
	$(CC) $(CFLAGS) -c $<

main: $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

clean:
	-rm -f *.o

install-deps:
	sudo apt install build-essential cmake xorg-dev libglu1-mesa-dev libglfw3 libglfw3-dev freeglut3 freeglut3-dev libglew-dev mesa-utils mesa-common-dev
