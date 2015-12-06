INC=-I "./"

FLAGS=$(INC)

all:
	g++ main.cpp Essentials.cpp $(FLAGS)


clean:
	rm -f *.o RayTracing