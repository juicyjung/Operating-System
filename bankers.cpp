#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

int i, j;
int A, B, C;
int state=0, finish = 0;
int PidR, requestA, requestB, requestC;
int sequence[5];

struct process
{
	int pid, allocationA, allocationB, allocationC, maxA, maxB, maxC, needA, needB, needC, finish;
};

struct process p[5];

int main()
{
	FILE* fp;
	FILE* out;
	fp = fopen("scenario1.dat", "r");

	fscanf(fp, "%d %d %d", &A, &B, &C);

	for (i = 0; i < 5; i++)
		fscanf(fp, "%d %d %d %d %d %d %d", &p[i].pid, &p[i].allocationA, &p[i].allocationB, &p[i].allocationC,
			&p[i].maxA, &p[i].maxB, &p[i].maxC);

	fclose(fp);

	// input //

	for (i = 0; i < 5; i++) {
		A -= p[i].allocationA;
		B -= p[i].allocationB;
		C -= p[i].allocationC;

		p[i].needA = p[i].maxA - p[i].allocationA;
		p[i].needB = p[i].maxB - p[i].allocationB;
		p[i].needC = p[i].maxC - p[i].allocationC;

		p[i].finish = 0;
	}

	out = fopen("output.txt", "wt");

	fprintf(out, "scenario1.dat\n");

	while (1) // current state //
	{
		for (i = 0; i < 5; i++)
		{
			if (p[i].needA <= A && p[i].needB <= B && p[i].needC <= C && p[i].finish == 0)
			{
				A += p[i].allocationA;
				B += p[i].allocationB;
				C += p[i].allocationC;
				p[i].finish = -1;
				sequence[j] = p[i].pid;
				j++;
				break;
			}
			else
				state--;
		}

		finish = 0;

		for (i = 0; i < 5; i++)
			finish += p[i].finish;

		if (finish == -5)
		{
			fprintf(out, "current state: ");

			for (i = 0; i < 4; i++)
				fprintf(out, "p%d -> ", sequence[i]);
			fprintf(out, "p%d\n", sequence[4]);
			break;
		}

		if (state == -5)
		{
			fprintf(out, "current state: unsafe!");
			break;
		}
		state = 0;
	}

	fp = fopen("scenario1.dat", "r");

	fscanf(fp, "%d %d %d", &A, &B, &C);

	for (i = 0; i < 5; i++)
		fscanf(fp, "%d %d %d %d %d %d %d", &p[i].pid, &p[i].allocationA, &p[i].allocationB, &p[i].allocationC,
			&p[i].maxA, &p[i].maxB, &p[i].maxC);
	fscanf(fp, "%d %d %d %d", &PidR, &requestA, &requestB, &requestC);

	fclose(fp);

	for (i = 0; i < 5; i++) {
		A -= p[i].allocationA;
		B -= p[i].allocationB;
		C -= p[i].allocationC;

		p[i].needA = p[i].maxA - p[i].allocationA;
		p[i].needB = p[i].maxB - p[i].allocationB;
		p[i].needC = p[i].maxC - p[i].allocationC;

		p[i].finish = 0;
	}


	if (requestA <= p[PidR - 1].needA && requestB <= p[PidR - 1].needB && requestC <= p[PidR - 1].needC
		&& requestA <= A && requestB <= B && requestC <= C)
	{
		A -= requestA;
		B -= requestB;
		C -= requestC;
		p[PidR - 1].allocationA += requestA;
		p[PidR - 1].allocationB += requestB;
		p[PidR - 1].allocationC += requestC;
		p[PidR - 1].needA -= requestA;
		p[PidR - 1].needB -= requestB;
		p[PidR - 1].needC -= requestC;
		state = 0;

		while (state != -5) // after arriving //
		{
			for (i = 0; i < 5; i++)
			{
				if (p[i].needA <= A && p[i].needB <= B && p[i].needC <= C && p[i].finish == 0)
				{
					A += p[i].allocationA;
					B += p[i].allocationB;
					C += p[i].allocationC;
					p[i].finish = -1;
					sequence[j] = p[i].pid;
					j++;
					break;
				}
				else
					state--;
			}

			finish = 0;

			for (i = 0; i < 5; i++)
				finish += p[i].finish;

			if (finish == -5)
			{
				fprintf(out, "After arriving request: ");

				for (i = 0; i < 4; i++)
					fprintf(out, "p%d -> ", sequence[i]);
				fprintf(out, "p%d\n", sequence[4]);
				break;
			}

			if (state == -5)
			{
				fprintf(out, "After arriving request: unsafe!");
				break;
			}
			state = 0;
		}
	}
	else
		fprintf(out, "After arriving request: unsafe!");

	fclose(out);
}