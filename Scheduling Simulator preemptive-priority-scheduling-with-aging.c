#include<stdio.h>
#include <stdlib.h>

int i;
int time, idleCount = 0;
int notArrive = 0;
int numberOfExit = 0;
int contextSwitch = 0;

struct process
{
	int pid, arrivalTime, burstTime, finishTime, firstTime, waitTime;
	double priority, basePriority;
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
		fscanf(fp, "%d %lf %d %d", &p[i].pid, &p[i].priority, &p[i].arrivalTime, &p[i].burstTime);
		p[i].basePriority = p[i].priority;
		p[i].firstTime = 50000;
	}
	fclose(fp);
	double alpha = atof(argv[4]);

	sort();

	fp = fopen(argv[2], "wt");

	while (1)
	{
		sort();
		for (i = 0; i < 10; i++)
		{
			if (time == p[i].arrivalTime)
				newArrival(fp, i);
		}

		notArrive = 0;
		for (i = 0; i < 10; i++)
		{
			if (time >= p[i].arrivalTime && p[i].burstTime >= 1)
			{
				run(fp, i);
				break;
			}
			else
				notArrive++;
		}

		if(notArrive==10 && numberOfExit != 10)
			idle(fp);

		if (numberOfExit == 10)
		{
			fprintf(fp, "<time %d> all processes finish\n", time);
			printf("<time %d> all processes finish\n", time);
			break;
		}
		time++;
		
		for (i = 0; i < 10; i++)
		{
			if (time >= p[i].arrivalTime && p[i].burstTime >= 1)
				p[i].priority = p[i].basePriority + alpha * p[i].waitTime;
		}
	}
	calculate(fp);
	fclose(fp);
}

void sort()
{
	struct process temp1;
	for (int i = 9; i > 0; i--)
	{
		for (int j = 0; j < i; j++)
		{
			if (p[j].priority < p[j + 1].priority)
			{
				temp1 = p[j];
				p[j] = p[j + 1];
				p[j + 1] = temp1;
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
		p[which].finishTime = time+1;
		numberOfExit++;
	}
	else
	{
		fprintf(fp, "<time %d> process %d is running\n", time, p[which].pid);
		printf("<time %d> process %d is running\n", time, p[which].pid);
	}
	p[which].burstTime -= 1;

	for (i = 0; i < 10; i++)
	{
		if (time >= p[i].arrivalTime && p[i].burstTime >= 1 && i != which)
			p[i].waitTime++;
	}

	if (p[which].firstTime >= time)
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
