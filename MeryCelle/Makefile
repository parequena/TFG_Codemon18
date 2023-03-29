CC=g++
EXIT=exe
OPTIONS=`pkg-config --cflags --libs sdl2`
LINK=SDL2_image

all: exe

exe:
	$(CC) -o $(EXIT) main.cpp $(OPTIONS) -l$(LINK)

clean:
	rm -rf $(EXIT)