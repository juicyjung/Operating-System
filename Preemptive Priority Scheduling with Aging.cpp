#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>

int i;
int time, idleCount = 0;
int notArrive = 0;
int numberOfExit = 0;
int contextSwitch = 0;

struct process
{
	int pid, arrivalTime, burstTime;
	double priority;
};

struct info
{
	int finishTime, firstTime, waitTime;
	double basePriority;
};
struct process p[10];
struct info t[10];

void sort();
void run(int);
void newArrival(int);
void idle();
void calculate();

int main()
{
	FILE* fp;
	fp = fopen("input.txt", "r");

	for (i = 0; i < 10; i++)
	{
		fscanf(fp, "%d %lf %d %d", &p[i].pid, &p[i].priority, &p[i].arrivalTime, &p[i].burstTime);
		t[i].basePriority = p[i].priority;
		t[i].firstTime = 50000;
	}
	fclose(fp);

	sort();

	for (i = 0; i < 10; i++)
		printf("%d %.3f %d %d \n", p[i].pid, p[i].priority, p[i].arrivalTime, p[i].burstTime);

	while (1)
	{
		sort();
		for (i = 0; i < 10; i++)
		{
			if (time == p[i].arrivalTime)
				newArrival(i);
		}

		notArrive = 0;
		for (i = 0; i < 10; i++)
		{
			if (time >= p[i].arrivalTime && p[i].burstTime >= 1)
			{
				run(i);
				break;
			}
			else
				notArrive++;
		}

		if(notArrive==10 && numberOfExit != 10)
			idle();

		if (numberOfExit == 10)
		{
			printf("<time %d> all processes finish\n", time);
			break;
		}
		time++;
		
		for (i = 0; i < 10; i++)
		{
			if (time >= p[i].arrivalTime && p[i].burstTime >= 1)
				p[i].priority = t[i].basePriority + 0.2 * t[i].waitTime;
		}
	}
	calculate();
}

void sort()
{
	struct process temp1;
	struct info temp2;
	for (int i = 9; i > 0; i--)
	{
		for (int j = 0; j < i; j++)
		{
			if (p[j].priority < p[j + 1].priority)
			{
				temp1 = p[j];
				p[j] = p[j + 1];
				p[j + 1] = temp1;

				temp2 = t[j];
				t[j] = t[j + 1];
				t[j + 1] = temp2;
			}
		}
	}
}

void run(int which)
{
	if(contextSwitch != p[which].pid)
		printf("-------------------------------- (Context-Switch)\n");

	if (p[which].burstTime == 1)
	{
		printf("<time %d> process %d is finished\n", time, p[which].pid);
		t[which].finishTime = time+1;
		numberOfExit++;
	}
	else
	{
		printf("<time %d> process %d is running\n", time, p[which].pid);
	}
	p[which].burstTime -= 1;

	for (i = 0; i < 10; i++)
	{
		if (time >= p[i].arrivalTime && p[i].burstTime >= 1 && i != which)
			t[i].waitTime++;
	}

	if (t[which].firstTime >= time)
		t[which].firstTime = time;

	contextSwitch = p[which].pid;
}

void newArrival(int which)
{
	printf("<time %d> [new arrival] process %d\n", time, p[which].pid);
}

void idle()
{
	printf("<time %d> ---- systiem is idle ----\n", time);
	idleCount++;
}

void calculate()
{

	double cpuUsage;
	cpuUsage = 100 * (time - idleCount) / (double)time;

	double waitingTime = 0;
	double responseTime = 0;
	double turnaroundTime = 0;
	for (i = 0; i < 10; i++)
	{
		waitingTime += t[i].waitTime;
		responseTime += t[i].firstTime - p[i].arrivalTime;
		turnaroundTime += t[i].finishTime - p[i].arrivalTime;
	}
	waitingTime /= (double)10;
	responseTime /= (double)10;
	turnaroundTime /= (double)10;

	printf("===================================================\n");
	printf("Average cpu usage : %f\n", cpuUsage);
	printf("Average waiting time : %f\n", waitingTime);
	printf("Average response time : %f\n", responseTime);
	printf("Average turnaround time : %f\n", turnaroundTime);
}