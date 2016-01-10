OBJ = main.o
INC = -I "./"

RayTracing: $(OBJ)
	g++ $(OBJ) -o RayTracing.exe -fopenmp
	rm -f $(OBJ)

main.o:
	g++ -c main.cpp $(INC) -fopenmp

clean:
	rm -f $(OBJ) RayTracing