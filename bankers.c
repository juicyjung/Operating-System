#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	int i, j, k, count;
	int resource[atoi(argv[3])];
	int state = 0, finish = 0;
	int PidR, request[atoi(argv[3])];
	int sequence[atoi(argv[4])];

	struct resource
	{
		int allocation, max, need;
	};

	struct process
	{
		int pid, finish;
		struct resource r[atoi(argv[3])];
	};

	struct process p[atoi(argv[4])];

	FILE* fp;
	FILE* out;
	fp = fopen(argv[1], "r");

	for (k = 0; k < atoi(argv[3]); k++)
		fscanf(fp, "%d", &resource[k]);

	for (i = 0; i < atoi(argv[4]); i++)
	{
		fscanf(fp, "%d", &p[i].pid);
		for (k = 0; k < atoi(argv[3]); k++)
			fscanf(fp, "%d", &p[i].r[k].allocation);
		for (k = 0; k < atoi(argv[3]); k++)
			fscanf(fp, "%d", &p[i].r[k].max);
	}

	fclose(fp);

	// input //

	for (i = 0; i < atoi(argv[4]); i++) {
		for (k = 0; k < atoi(argv[3]); k++)
		{
			resource[k] -= p[i].r[k].allocation;
			p[i].r[k].need = p[i].r[k].max - p[i].r[k].allocation;
		}
		p[i].finish = 0;
	}

	out = fopen(argv[2], "wt");

	fprintf(out, "%s\n", argv[2]);

	while (1) // current state //
	{
		for (i = 0; i < atoi(argv[4]); i++)
		{
			count = 0;
			for (k = 0; k < atoi(argv[3]); k++)
			{
				if (p[i].r[k].need <= resource[k])
					count++;
			}
			if (count == atoi(argv[3]) && p[i].finish == 0)
			{
				for (k = 0; k < atoi(argv[3]); k++)
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

		for (i = 0; i < atoi(argv[4]); i++)
			finish += p[i].finish;

		if (finish == -atoi(argv[4]))
		{
			fprintf(out, "current state: ");

			for (i = 0; i < atoi(argv[4])-1; i++)
				fprintf(out, "p%d -> ", sequence[i]);
			fprintf(out, "p%d\n", sequence[atoi(argv[4]) - 1]);
			break;
		}

		if (state == -atoi(argv[4]))
		{
			fprintf(out, "current state: unsafe!\n");
			break;
		}
		state = 0;
	}


	fp = fopen(argv[1], "r");

	for (k = 0; k < atoi(argv[3]); k++)
		fscanf(fp, "%d", &resource[k]);

	for (i = 0; i < atoi(argv[4]); i++)
	{
		fscanf(fp, "%d", &p[i].pid);
		for (k = 0; k < atoi(argv[3]); k++)
			fscanf(fp, "%d", &p[i].r[k].allocation);
		for (k = 0; k < atoi(argv[3]); k++)
			fscanf(fp, "%d", &p[i].r[k].max);
	}

	fscanf(fp, "%d", &PidR);
	for (k = 0; k < atoi(argv[3]); k++)
		fscanf(fp, "%d", &request[k]);

	fclose(fp);

	for (i = 0; i < atoi(argv[4]); i++) {
		for (k = 0; k < atoi(argv[3]); k++)
		{
			resource[k] -= p[i].r[k].allocation;
			p[i].r[k].need = p[i].r[k].max - p[i].r[k].allocation;
		}
		p[i].finish = 0;
	}

	count = 0;
	for (k = 0; k < atoi(argv[3]); k++)
	{
		if (request[k] <= p[PidR - 1].r[k].need && request[k] <= resource[k])
			count++;
	}

	if (count == atoi(argv[3]))
	{
		for (k = 0; k < atoi(argv[3]); k++)
		{
			resource[k] -= request[k];
			p[PidR - 1].r[k].allocation += request[k];
			p[PidR - 1].r[k].need -= request[k];
		}
		state = 0;
		j = 0;

		while (state != -atoi(argv[4])) // after arriving //
		{
			for (i = 0; i < atoi(argv[4]); i++)
			{
				count = 0;
				for (k = 0; k < atoi(argv[3]); k++)
				{
					if (p[i].r[k].need <= resource[k])
						count++;
				}
				if (count == atoi(argv[3]) && p[i].finish == 0)
				{
					for (k = 0; k < atoi(argv[3]); k++)
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

			for (i = 0; i < atoi(argv[4]); i++)
				finish += p[i].finish;

			if (finish == -atoi(argv[4]))
			{
				fprintf(out, "After arriving request: ");

				for (i = 0; i < 4; i++)
					fprintf(out, "p%d -> ", sequence[i]);
				fprintf(out, "p%d\n", sequence[4]);
				break;
			}

			if (state == -atoi(argv[4]))
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

