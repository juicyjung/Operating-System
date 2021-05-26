#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

int i, j, k, count;
int resource[3];
int state = 0, finish = 0;
int PidR, request[3];
int sequence[5];

struct resource
{
	int allocation, max, need;
};

struct process
{
	int pid, finish;
	struct resource r[3];
};

struct process p[5];


int main()
{
	FILE* fp;
	FILE* out;
	fp = fopen("scenario1.dat", "r");

	for(k=0; k<3; k++)
		fscanf(fp, "%d", &resource[k]);

	for (i = 0; i < 5; i++)
	{
		fscanf(fp, "%d", &p[i].pid);
		for (k = 0; k < 3; k++)
			fscanf(fp, "%d", &p[i].r[k].allocation);
		for (k = 0; k < 3; k++)
			fscanf(fp, "%d", &p[i].r[k].max);
	}

	fclose(fp);

	// input //

	for (i = 0; i < 5; i++) {
		for (k = 0; k < 3; k++)
		{
			resource[k] -= p[i].r[k].allocation;
			p[i].r[k].need = p[i].r[k].max - p[i].r[k].allocation;
		}
		p[i].finish = 0;
	}

	out = fopen("output.txt", "wt");

	fprintf(out, "scenario1.dat\n");

	while (1) // current state //
	{
		for (i = 0; i < 5; i++)
		{
			count = 0;
			for (k = 0; k < 3; k++)
			{
				if (p[i].r[k].need <= resource[k])
					count++;
			}
			if (count == 3 && p[i].finish == 0)
			{
				for(k=0; k<3; k++)
					resource[k] += p[i].r[k].allocation;
				
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
			fprintf(out, "current state: unsafe!\n");
			break;
		}
		state = 0;
	}
	

	fp = fopen("scenario1.dat", "r");

	for (k = 0; k < 3; k++)
		fscanf(fp, "%d", &resource[k]);

	for (i = 0; i < 5; i++)
	{
		fscanf(fp, "%d", &p[i].pid);
		for (k = 0; k < 3; k++)
			fscanf(fp, "%d", &p[i].r[k].allocation);
		for (k = 0; k < 3; k++)
			fscanf(fp, "%d", &p[i].r[k].max);
	}

	fscanf(fp, "%d", &PidR);
	for (k = 0; k < 3; k++)
		fscanf(fp, "%d", &request[k]);

	fclose(fp);

	for (i = 0; i < 5; i++) {
		for (k = 0; k < 3; k++)
		{
			resource[k] -= p[i].r[k].allocation;
			p[i].r[k].need = p[i].r[k].max - p[i].r[k].allocation;
		}
		p[i].finish = 0;
	}

	count = 0;
	for (k = 0; k < 3; k++)
	{
		if (request[k] <= p[PidR - 1].r[k].need && request[k] <= resource[k])
			count++;
	}

	if (count==3)
	{
		for (k = 0; k < 3; k++)
		{
			resource[k] -= request[k];
			p[PidR - 1].r[k].allocation += request[k];
			p[PidR - 1].r[k].need -= request[k];
		}
		state = 0;
		j = 0;

		while (state != -5) // after arriving //
		{
			for (i = 0; i < 5; i++)
			{
				count = 0;
				for (k = 0; k < 3; k++)
				{
					if (p[i].r[k].need <= resource[k])
						count++;
				}
				if (count == 3 && p[i].finish == 0)
				{
					for (k = 0; k < 3; k++)
						resource[k] += p[i].r[k].allocation;

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
