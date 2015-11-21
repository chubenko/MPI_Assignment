all: compile 

compile:
	mpicc MPI_Assignment.c -o run_MPI

