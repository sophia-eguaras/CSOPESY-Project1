#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef char String20 [21];
typedef struct ProcessInfo {
    int A; // process ID
    int B; // arrival time
    int C; // burst time
	int D; // waiting time
	int E; // start time
	int F; // end time
} Process;

/*	FUNCTION: computeWaitTime ()
	
	Description: This function computes the waiting time for each process
	@Parameter: currWaitTime - current waiting time of a processs
				a - time the process started (first time). Otherwise, time the process continued
				b - time the process arrived (first time). Otherwise, time the process is interrupted
	@Return: returns an integer representing the waiting time
*/
int computeWaitTime (int currWaitTime, int a, int b) {
	return currWaitTime + (a - b);
}
/*	FUNCTION: computeTotalWaitTime ()
	
	Description: This function computes the total waiting time for each process
	@Parameter: processes[] - array of structures for processes
				Y - Number of elements (e.g. processes)
                startEndTime[][3] - array of the start-end time pairs of the processes
                totalBurst - the sum of all burst time of the processes
*/
void computeTotalWaitTime (Process processes [], int Y, int startEndTime [][3], int totalBurst) {
    int i, j;
    // Computing total waiting time per process
    for(i = 0; i < Y; i++)
        for(j = 0; j < totalBurst; j++)
            if(processes[i].A == startEndTime[j][0]) {
                processes[i].D = computeWaitTime(processes[i].D, startEndTime[j][1], processes[i].B);
                processes[i].E = startEndTime[j][1];
                processes[i].F = startEndTime[j][1];                
                j = totalBurst;
            }
    for(i = 0; i < Y; i++)
        for(j = 0; j < totalBurst; j++)
            if(processes[i].A == startEndTime[j][0]) {
                processes[i].E = startEndTime[j][1];
                processes[i].D = computeWaitTime(processes[i].D, processes[i].E, processes[i].F);  
                processes[i].F = startEndTime[j][2];
            }
}
/*	FUNCTION: computeAvgWaitTime ()
	
	Description: This function computes the average waiting time given the waiting time of each process
	@Parameter: processes[] - array of structures for processes
				Y - Number of elements (e.g. processes)
	@Return: returns a floating point number representing the average waiting time
*/
float computeAvgWaitTime (Process processes [], int Y) {
	int j;
	float sum = 0.0;
	
	for (j = 0; j < Y; j++)
		sum += processes[j].D;

	return sum / Y;
}

/*	FUNCTION: printMultipleStartEndTime ()
	
	Description: This function prints all of start and end time of a process
	@Parameter: process - specific process
                startEndTime[][3] - array of the start-end time pairs of the processes
                totalBurst - the sum of all burst time of the processes
*/
void printMultipleStartEndTime (Process process, int startEndTime[][3], int totalBurst) {
    int i;
    for(i = 0; i < totalBurst; i++) {
        if(process.A == startEndTime[i][0])
            printf(" Start Time: %d End Time: %d |", startEndTime[i][1], startEndTime[i][2]);
    }
}
/*	FUNCTION: displayOutput()
	
	Description: This function displays the output for the scheduling algorithms
	@Parameter: processes[] - array of structures for processes
				Y - Number of elements (e.g. processes)
                X - CPU scheduling algorithm indicator
                startEndTime[][3] - array of the start-end time pairs of the processes
                totalBurst - the sum of all burst time of the processes
*/
void displayOutput (Process processes [], int Y, int X, int startEndTime[][3], int totalBurst) {
    int i, j, k;
    Process temp;
	// Sorting according to Process ID
	for (j = 0; j < Y; j++)
		for (k = j+1; k < Y; k++) {
			if (processes[j].A > processes[k].A) {
				temp = processes[j];
				processes[j] = processes[k];
				processes[k] = temp;
			}
		}
    // Displaying Results
    for(i = 0; i < Y; i++) {
        printf("P[%d]", processes[i].A);
        if(X < 2)
            printf(" Start Time: %d End Time: %d |", processes[i].E, processes[i].F); 
        else printMultipleStartEndTime(processes[i],startEndTime,totalBurst);
        printf(" Waiting Time: %d\n", processes[i].D); 
    }
	printf ("Average Waiting Time: %.2f \n", computeAvgWaitTime(processes, Y));
}

/*	FUNCTION: sortArrival()
	
	Description: This function sorts the array of processes according to arrival time in an ascending order using bubble sort
	@Parameter: processes[] - array of structures for processes
				Y - Number of elements (e.g. processes)
*/
void sortArrival (Process processes [], int Y) {
	int j, k;
	Process temp;
	
	for (j = 0; j < Y; j++)
		for (k = j+1; k < Y; k++) {
			if (processes[j].B > processes[k].B) {
				temp = processes[j];
				processes[j] = processes[k];
				processes[k] = temp;
			}
		}
}
/*	FUNCTION: sortBurstTime()
	
	Description: This function sorts the array of processes according to burst time in an ascending order using bubble sort
	@Parameter: processes[] - array of structures for processes
				Y - Number of elements (e.g. processes)
*/
void sortBurst (Process processes [], int Y) {
	int j;
	Process temp;
	
	for (j = 0; j < Y-1; j++) {
        if (processes[j].C > processes[j+1].C) {
            temp = processes[j];
            processes[j] = processes[j+1];
            processes[j+1] = temp;
        }
    }
}	

/*	FUNCTION: computeTotalBurstTime()
	
	Description: This function computes the total burst time of all processes
	@Parameter: processes[] - array of structures for processes
				Y - Number of elements (e.g. processes)
    @Return: returns an integer representing the total burst time
*/
int computeTotalBurstTime (Process processes [], int Y) {
    float burstSum = 0;
    int j;
    for(j = 0; j < Y; j++)
        burstSum+=processes[j].C;
    return burstSum;
}

/*	FUNCTION: FCFS_SJF()
	
	Description: This function performs the First-Come First-Served (FCFS) or the Shortest-Job-First Scheduling (SJF), depending on the X value.
	@Parameter: processes[] - array of structures for processes
                X - CPU Scheduling Algorithm indicator (0 - FCFS, 1 - SJF)
				Y - Number of elements (e.g. processes)
*/
void FCFS_SJF (Process processes [], int X, int Y) {
    int startEndTime[computeTotalBurstTime(processes, Y)][3]; 
    int i, currTimeStamp = 0;
    sortArrival(processes, Y);
    if (X == 1)
        sortBurst(processes, Y);
    for(i = 0; i < Y; i++) {
        processes[i].E = currTimeStamp;
        processes[i].F = currTimeStamp + processes[i].C;
        processes[i].D = computeWaitTime(0, processes[i].E, processes[i].B);
        currTimeStamp = processes[i].F;
    }
    displayOutput(processes, Y, X, startEndTime, 0);
}

/*	FUNCTION: SRTF()
	
	Description: This function performs the Shortest-Remaining-Time-First (SRTF) Scheduling.
	@Parameter: processes[] - array of structures for processes
                X - CPU Scheduling Algorithm indicator (0 - FCFS, 1 - SJF)
				Y - Number of elements (e.g. processes)
*/
void SRTF (Process processes [], int X, int Y) {
    int i, j, index, min, seCtr = 0; 
    int totalBurst = computeTotalBurstTime(processes, Y);
    int execution[totalBurst];
    int startEndTime[totalBurst][3]; 
    // Initializing startEndTime Array
    for(i = 0; i < totalBurst; i++) {
        startEndTime[i][0] = -1;
        startEndTime[i][1] = -1;
        startEndTime[i][2] = -1;
    }

    // Sorting Processes according to their arrival time
    sortArrival(processes, Y);

    // Execution of processes
    for(i = 0; i < totalBurst; i++) { 
        min = totalBurst; 
        for(j = 0; j < Y; j++) 
            if (processes[j].C != 0 && processes[j].B <= i && processes[j].C < min) { 
                min = processes[j].C; 
                index = j; 
            }  
        processes[index].C--; 
        execution[i] = processes[index].A; 
    }
        
    // Getting the start and end time pairs per process
    for(i = 0; i < totalBurst; i++) {
        startEndTime[seCtr][0] = execution[i];
        startEndTime[seCtr][1] = i;
        for(j = i; j < totalBurst; j++) {
            if(execution[i] == execution[j]) {
                startEndTime[seCtr][2] = j+1; 
                if(j == totalBurst-1)
                    i = j;
            }
            else {
                i = j-1;
                j = totalBurst;
            }
        }
        seCtr++;
    }
    
    // Generating Results
    computeTotalWaitTime(processes,Y,startEndTime,totalBurst);
    displayOutput(processes, Y, X, startEndTime, totalBurst);
}

/*
	int A; // process ID
    int B; // arrival time
    int C; // burst time
	int D; // waiting time
	int E; // start time
	int F; // end time
*/	

/*	FUNCTION: RR()
	
	Description: This function performs the Round Robin (RR) Scheduling.
	@Parameter: processes[] - array of structures for processes
                X - CPU Scheduling Algorithm indicator (0 - FCFS, 1 - SJF)
				Y - Number of elements (e.g. processes)
                Z - Time Slice Value
*/
// void RR (Process processes [], int X, int Y, int Z) {
	
// }

int main () {
    FILE *fptr;
    String20 sFile;
    int X = 0, Y = 0, Z = 0, ctr = -1;
    Process processes[100];
    
    // 1. The program will just ask the user to input the name of the input text file
    printf("File name (e.g. Sample.txt): ");
    scanf("%s", sFile);   
    fptr = fopen (sFile, "r");
    
    // 2. Checking if the text file exists
    if(fptr != NULL) { // 2.1. If the text file exists
        // 3. Processing the contents of the text file
        while(!feof(fptr)) {
            if (ctr == -1) {   // 3.1. Reading X(sched algo), Y(processes #), Z(time quantum)
                fscanf(fptr, "%d %d %d", &X, &Y, &Z);
                if (X < 0 || X > 3) {
                    printf("The indicated scheduling algorithm does not exist.\n");
                    fseek(fptr, 0, SEEK_END);
                }
                else if(Y < 3 || Y > 100) {
                    X = -1;
                    printf("The number of processes is not in range (3 <= Y <= 100).\n"); 
                    fseek(fptr, 0, SEEK_END);
                }
                else if(X == 3 && (Z < 1 || Z > 100)) {
                    X = -1;
                    printf("The indicated time slice value is not in range (1 <= Z <= 100).\n");   
                    fseek(fptr, 0, SEEK_END);
                } 
            }
            else {               // 3.2. Reading each process
                fscanf(fptr, "%d %d %d",  &processes[ctr].A, &processes[ctr].B, &processes[ctr].C);
                processes[ctr].D = 0;
                processes[ctr].E = 0;
                processes[ctr].F = 0;
            }
            ctr++;
        }
        fclose(fptr);

        // 4. Performing the CPU Scheduling Algo
    	switch (X) {
            case 0:
            case 1: FCFS_SJF(processes, X, Y); 
                break;
            case 2: SRTF(processes, X, Y);
    			break;
    		case 3: //RR(processes, X, Y, Z);
                break;
            default: printf("Program terminating...\n"); 
    	}          
    }
    else printf("%s not found.\n\nProgram terminating...\n", sFile);    // 2.2. If the text file does not exists

    return 0;
}