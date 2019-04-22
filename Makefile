CC = g++
LIBS = -lglfw -lGL -lm -lX11 -lglut -lGLU
CFLAGS = -g -Wall -Wno-deprecated

main: main.o
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $<

clean:
	-rm -f *.o

install-deps:
	sudo apt install build-essential cmake xorg-dev libglu1-mesa-dev libglfw3 libglfw3-dev freeglut3 freeglut3-dev libglew-dev mesa-utils mesa-common-dev
