#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include <stdlib.h>

int i;
int A, B, C;
int PidR, requestA, requestB, requestC;

struct process
{
	int pid, allocationA, allocationB, allocationC, maxA, maxB, maxC;
};

struct process p[5];

int main()
{
	FILE* fp;
	fp = fopen("scenario1.dat", "r");

	fscanf(fp, "%d %d %d", &A, &B, &C);

	for (i = 0; i < 5; i++)
		fscanf(fp, "%d %d %d %d %d %d %d", &p[i].pid, &p[i].allocationA, &p[i].allocationB, &p[i].allocationC,
			&p[i].maxA, &p[i].maxB, &p[i].maxC);
	fscanf(fp, "%d %d %d %d", &PidR, &requestA, &requestB, &requestC);

	fclose(fp);

	printf("%d %d %d\n", A, B, C);
	for (i = 0; i < 5; i++)
		printf("%d\t%d %d %d   %d %d %d\n", p[i].pid, p[i].allocationA, p[i].allocationB, p[i].allocationC,
			p[i].maxA, p[i].maxB, p[i].maxC);
	printf("%d\t%d %d %d\n", PidR, requestA, requestB, requestC);


	fp = fopen("output.txt", "wt");

	fclose(fp);
}