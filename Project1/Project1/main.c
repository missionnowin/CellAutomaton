#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <conio.h>
#include <mpi.h>

int evolute(int x1, int x2, int x3, int option);
inline int TorIt(int x, int count);
int initFirstGen(int* arr, int n);
void cellAutomaton(int* arr, int n, int steps, int rank, int range, int numProcesses, int option);
void fileOut(int* arr, int n);

void main(int argc, char* argv[]){
	int n, steps, numProcesses, rank;
	int* arr;

	n = atoi(argv[1]);
	steps = atoi(argv[2]);
	int option = atoi(argv[3]);

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	arr = (int*)malloc(n * sizeof(int));

	if (rank == 0) {
		int results = initFirstGen(arr, n);
		printf("\n");

		if (results == 0) {
			printf("\nAn error occurred!\n");
			return;
		}
	}
	
	int range = ceil((double)n / (numProcesses));
	
	cellAutomaton(arr, n, steps, rank, range, numProcesses, option);

	if (rank == 0) {
		fileOut(arr, n);
	}

	free(arr);

	MPI_Finalize();

	return 0;
}

void cellAutomaton(int* arr, int n, int steps, int rank, int range, int numProcesses, int option) {
	int* swap;
	swap = (int*)malloc(range * sizeof(int));

	for (int i = 0; i < steps; i++)
	{
		MPI_Bcast(arr, n, MPI_INT, 0, MPI_COMM_WORLD);

		int k = 0;
		for (int j = rank * range; j < n && j < (rank + 1) * range; j++)
		{
			swap[k++] = evolute(arr[TorIt(j - 1, n)], arr[TorIt(j, n)], arr[TorIt(j + 1, n)], option);
		}

		MPI_Gather(swap, range, MPI_INT, arr, range, MPI_INT, 0, MPI_COMM_WORLD);
		
	}

	free(swap);
}

int initFirstGen(int* arr, int n) {
	FILE* fin;
	fin = fopen("firstgen.txt", "r");

	if (!fin) {
		printf("An error occurred while reading firstgen.txt\n");
		return 0;
	}

	int i = 0;
	while (i < n)
	{
		fscanf(fin, "%d", &arr[i]);
		++i;
	}
	fclose(fin);

	printf("First generation: \n");

	for (i = 0; i < n; i++) {
		printf("%d ", arr[i]);
	}

	return 1;
}

int evolute(int x1, int x2, int x3, int option) {
	switch (option) {
		case 1:
			return x1 ^ x2 ^ x3;
		case 2: 
			return x1 | x2 | x3;
		case 3:
			return x1 ^ x3;
	}
	
}

inline int TorIt(int x, int count) {
	if (x < 0) {
		return x + count;
	}
	else {
		return x % count;
	}
}

void fileOut(int* arr, int n) {
	FILE* fOut;
	fOut = fopen("output.txt", "w");

	if (!fOut) {
		printf("An error occurred while writing output.txt\n");
		return;
	}

	for (int i = 0; i < n; i++) {
		fprintf(fOut, "%d ", arr[i]);
	}
}