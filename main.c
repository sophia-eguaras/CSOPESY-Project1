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

/*	FUNCTION: computeAvgWaitTime ()
	
	Description: This function computes the average waiting time given the waiting time of each process
	@Parameter: processes[] - array of structures for processes
				Y - Number of elements (e.g. processes)
	@Return: returns a floating point number representing the average waiting time
*/
float computeAvgWaitTime (Process processes [], int Y) {
	
	float sum = 0.0;
	
	for (int j = 0; j < Y; j++)
		sum += processes[j].D;

	return sum / Y;
}

/*	FUNCTION: displayOutput()
	
	Description: This function displays the output for FCFS or SJF CPU scheduling algorithms
	@Parameter: processes[] - array of structures for processes
				Y - Number of elements (e.g. processes)
*/
void displayOutput (Process processes [], int Y) {
	for(int i = 0; i < Y; i++) {
        printf("P[%d] Start Time: %d End time: %d | Waiting time: %d\n", processes[i].A, processes[i].E, processes[i].F, processes[i].D);
    }
	printf ("Average waiting time:  %.2f", computeAvgWaitTime(processes, Y));
}

/*	FUNCTION: sortArrival()
	
	Description: This function sorts the array of processes according to arrival time in an ascending order using bubble sort
	@Parameter: processes[] - array of structures for processes
				Y - Number of elements (e.g. processes)
*/
void sortArrival (Process processes [], int Y) {
	
	Process temp;
	
	for (int j = 0; j < Y; j++)
		for (int k = j+1; k < Y; k++) {
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
	
	Process temp;
	
	for (int j = 0; j < Y-1; j++) {
        if (processes[j].C > processes[j+1].C) {
            temp = processes[j];
            processes[j] = processes[j+1];
            processes[j+1] = temp;
        }
    }
}	

/*	FUNCTION: FCFS_SJF()
	
	Description: This function shows the process scheduling using the FCFS or SJF method
	@Parameter: processes[] - array of structures for processes
				Y - Number of elements (e.g. processes)
                X - CPU Scheduling Algorithm indicator (0 - FCFS, 1 - SJF)
*/
void FCFS_SJF (Process processes [], int Y, int X) {
    int currTimeStamp = 0;
    sortArrival(processes, Y);
    if (X == 1)
        sortBurst(processes, Y);
    for(int i = 0; i < Y; i++) {
        processes[i].E = currTimeStamp;
        processes[i].F = currTimeStamp + processes[i].C;
        processes[i].D = computeWaitTime(0, processes[i].E, processes[i].B);
        currTimeStamp = processes[i].F;
    }
    displayOutput(processes, Y);
}

/*	FUNCTION: SRTF()
	
	Description: 
	@Parameter: processes[] - array of structures for processes
				Y - Number of elements (e.g. processes)
*/
// void SRTF (Process processes [], int Y) {
//    float burstSum = 0;
//    for(int j = 0;j<Y;j++)
//       burstSum+=processes[j].C;
//    int startEndSize = ceil(burstSum / Y);
//    int startEndTime [startEndSize][2];
// 
//     int currTimeStamp = 0;
//     sortArrival(processes, Y);
//     for(int i = 0; i < Y; i++) {
//         processes[i].E = currTimeStamp;
//         processes[i].F = currTimeStamp + processes[i].C;
//         processes[i].D = computeWaitTime(0, processes[i].E, processes[i].B);
//         currTimeStamp = processes[i].F;            
//     }
// }

/*
	int A; // process ID
    int B; // arrival time
    int C; // burst time
	int D; // waiting time
	int E; // start time
	int F; // end time
*/	

/*	FUNCTION: RR()
	
	Description: This function sorts the array of processes according to arrival time in an ascending order
	@Parameter: processes[] - array of structures for processes
				Y - Number of elements (e.g. processes)
                Z - Time Slice Value
*/
// void RR (Process processes [], int Y, int Z) {
	
// }

int main () {
    FILE *fptr;
    String20 sFile;
    int X, Y, Z, ctr = -1;
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
            case 1: FCFS_SJF(processes, Y, X); 
                break;
            case 2: //SRTF(processes, Y);
    			break;
    		case 3: //RR(processes, Y, Z);
                break;
            default: printf("Program terminating...\n"); 
    	}        
    }
    else printf("%s not found.\n\nProgram terminating...\n", sFile);    // 2.2. If the text file does not exists

    return 0;
}