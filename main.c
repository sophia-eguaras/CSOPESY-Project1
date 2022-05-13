#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
    X denotes:
    0 - FCFS
    1 - SJF
    2 - SRTF
    3 - RR
*/

typedef char String20 [21];

struct ProcessesEntry
{
    int position;
    int X;
    int Y;
    int Z;
};

int main () {

    FILE * fptr;
    String20 sFile;
    char cLet;

    printf("File name (e.g. Sample.txt): ");
    scanf("%s", sFile);

    do {
        if ((fptr = fopen(sFile, "r")) == NULL)
        printf ("File does not exist \n");
    } while ((fptr = fopen(sFile, "r")) == NULL);
    
    // do {
    //     X = cLet;
    // } while (fscanf(fptr, "%c", cLet) == 1);

    fclose(fptr);

    return 0;
    
}