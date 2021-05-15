#include<stdio.h>
#include <stdlib.h>

int i;
int time, idleCount = 0;
int allLock = 0;
int numberOfready = 0;
int numberOfExit = 0;
int contextSwitch = 0;

struct process
{
	int pid, priority, arrivalTime, burstTime, finishTime, firstTime, lock, waitTime;
};

struct process p[10];

void sort();
void run(FILE*, int);
void newArrival(FILE*, int);
void idle(FILE*);
void calculate(FILE*);

int main(int argc, char* argv[])
{
	FILE* fp;
	fp = fopen(argv[1], "r");

	for (i = 0; i < 10; i++)
	{
		fscanf(fp, "%d %d %d %d", &p[i].pid, &p[i].priority, &p[i].arrivalTime, &p[i].burstTime);
		p[i].firstTime = 50000;
	}
	fclose(fp);

	int quantum = atoi(argv[3]);

	sort();

	fp = fopen(argv[2], "wt");

	while (1)
	{
		for (i = 0; i < 10; i++)
		{
			if (time == p[i].arrivalTime)
				newArrival(fp, i);
		}
		for (i = 0; i < 10; i++)
		{
			if (time >= p[i].arrivalTime)
			{
				if (p[i].burstTime >= 1)
				{
					if (p[i].lock < quantum)
					{
						run(fp, i);
						break;
					}				
				}
			}
			else
			{
				if (i!=0 && p[i-1].lock == quantum && p[i-1].burstTime >= 1 && time >= p[i-1].arrivalTime)
				{
					break;
				}
				else
				{
					idle(fp);
					break;
				}
			}
		}

		numberOfready = 0;
		allLock = 0;
		for (i = 0; i < 10; i++)
		{
			if (time >= p[i].arrivalTime && p[i].burstTime >= 1)
			{
				numberOfready++;
				if (p[i].lock >= quantum)
					allLock++;
			}
		}
		if (numberOfready == allLock)
		{
			for (i = 0; i < 10; i++)
				p[i].lock = 0;
		}

		if (numberOfExit == 10)
		{
			fprintf(fp, "<time %d> all processes finish\n", time);
			printf("<time %d> all processes finish\n", time);
			break;
		}
		time++;
	}
	calculate(fp);
	fclose(fp);
}

void sort()
{
	struct process temp;
	for (int i = 9; i > 0; i--)
	{
		for (int j = 0; j < i; j++)
		{
			if (p[j].arrivalTime > p[j + 1].arrivalTime)
			{
				temp = p[j];
				p[j] = p[j + 1];
				p[j + 1] = temp;
			}
		}
	}
}

void run(FILE* fp, int which)
{
	if (contextSwitch != p[which].pid)
	{
		fprintf(fp, "-------------------------------- (Context-Switch)\n");
		printf("-------------------------------- (Context-Switch)\n");
	}

	if (p[which].burstTime == 1)
	{
		fprintf(fp, "<time %d> process %d is finished\n", time, p[which].pid);
		printf("<time %d> process %d is finished\n", time, p[which].pid);
		p[which].finishTime = time + 1;
		numberOfExit++;
	}
	else
	{
		fprintf(fp, "<time %d> process %d is running\n", time, p[which].pid);
		printf("<time %d> process %d is running\n", time, p[which].pid);
	}
	p[which].burstTime -= 1;
	p[which].lock++;

	for (i = 0; i < 10; i++)
	{
		if (time >= p[i].arrivalTime && p[i].burstTime >= 1 && i != which)
			p[i].waitTime++;
	}

	if (p[which].firstTime > time)
		p[which].firstTime = time;

	contextSwitch = p[which].pid;
}

void newArrival(FILE* fp, int which)
{
	fprintf(fp, "<time %d> [new arrival] process %d\n", time, p[which].pid);
	printf("<time %d> [new arrival] process %d\n", time, p[which].pid);
}

void idle(FILE* fp)
{
	fprintf(fp, "<time %d> ---- systiem is idle ----\n", time);
	printf("<time %d> ---- systiem is idle ----\n", time);
	idleCount++;
}

void calculate(FILE* fp)
{

	double cpuUsage;
	cpuUsage = 100 * (time - idleCount) / (double)time;

	double waitingTime = 0;
	double responseTime = 0;
	double turnaroundTime = 0;
	for (i = 0; i < 10; i++)
	{
		waitingTime += p[i].waitTime;
		responseTime += p[i].firstTime - p[i].arrivalTime;
		turnaroundTime += p[i].finishTime - p[i].arrivalTime;
	}
	waitingTime /= (double)10;
	responseTime /= (double)10;
	turnaroundTime /= (double)10;

	printf("===================================================\n");
	printf("Average cpu usage : %f\n", cpuUsage);
	printf("Average waiting time : %f\n", waitingTime);
	printf("Average response time : %f\n", responseTime);
	printf("Average turnaround time : %f\n", turnaroundTime);

	fprintf(fp, "===================================================\n");
	fprintf(fp, "Average cpu usage : %f\n", cpuUsage);
	fprintf(fp, "Average waiting time : %f\n", waitingTime);
	fprintf(fp, "Average response time : %f\n", responseTime);
	fprintf(fp, "Average turnaround time : %f\n", turnaroundTime);
}
