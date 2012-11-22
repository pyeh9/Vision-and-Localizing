Project4: Project4.o image.o Robot.o
	g++ -o Project4 Project4.o image.o Robot.o
Project4.o: Project4.cpp
	g++ -c Project4.cpp
image.o: image.cpp
	g++ -c image.cpp
Robot.o: Robot.cpp
	g++ -c Robot.cpp
clean:
	rm Project4 Project4.o image.o Robot.o
