#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <conio.h>

int evolute(int x1, int x2, int x3);
inline int TorIt(int x, int count);

void main(int argc, char* argv[]){
	int n, i, j, steps,/* alive, num,*/* y, * y1;
	printf("Number of cells: ");
	scanf_s("%d", &n);

	printf("Number of steps: ");
	scanf_s("%d", &steps);
	y = malloc(n * sizeof(int));
	y1 = malloc(n * sizeof(int));
	FILE* fin = NULL;
	fin = fopen("firstgen.txt", "r");
	if (!fin)
		printf("Error!\n");
	else
	{
		i = 0;
		while (i < n)
		{
			fscanf(fin, "%d", &y[i]);
			++i;
			y1[i] = 0;
		}
		fclose(fin);
		for (i = 0; i < n; i++)
			printf("%d", y[i]);
	}
	for (j = 0; j < steps; j++)
	{
		printf("\n");

		for (i = 0; i < n; i++)
		{
			y1[i] = evolute(y[TorIt(i - 1, n)], y[TorIt(i, n)], y[TorIt(i + 1, n)]);
		}

		for (i = 0; i < n; i++)
			y[i] = y1[i];

		for (i = 0; i < n; i++)
			printf("%d", y[i]);

	}
	free(y);
	free(y1);
}

int evolute(int x1, int x2, int x3) {
	return x1 ^ x2 ^ x3;
}

inline int TorIt(int x, int count) {
	if (x < 0) {
		return x + count;
	}
	else {
		return x % count;
	}
}

