#include <stdio.h>
#include <stdlib.h>

struct FrmNode
{
    int pNo, counter;
} frames[20];
int n;

int pageFound(int pNo)
{
    int fNo;
    for (fNo = 0; fNo < n; fNo++)
    {
        if (frames[fNo].pNo == pNo)
            return fNo;
    }
    return -1;
}

int getFreeFrame()
{
    int fNo;
    for (fNo = 0; fNo < n; fNo++)
    {
        if (frames[fNo].pNo == -1)
            return fNo;
    }
    return -1;
}

int getLruFrame()
{
    int lruFNo = 0, fNo;
    for (fNo = 0; fNo < n; fNo++)
    {
        if (frames[fNo].counter < frames[lruFNo].counter)
            lruFNo = fNo;
    }
    return lruFNo;
}

void main()
{
    int pages[] = {3,5,7,2,5,1,2,3,1,3,5,3,1,6,2};
    int size = sizeof(pages) / sizeof(pages[0]);
    int pageFault = 0, i, j, fNo;

    printf("\nEnter the number of frames: ");
    scanf("%d", &n);
    
    for (i = 0; i < n; i++)
    {
        frames[i].pNo = -1;
        frames[i].counter = 0;
    }
    
    printf("\nPageNo      PageFrame       PageFault ");
    printf("\n---------------------------------------");

    for (i = 0; i < size; i++)
    {
        for (fNo = 0; fNo < n; fNo++)
        {
            if (frames[fNo].pNo != -1)
                frames[fNo].counter++;
        }

        j = pageFound(pages[i]);
        if (j == -1)
        {
            pageFault++;
            j = getFreeFrame();
            if (j == -1)
                j = getLruFrame();

            frames[j].pNo = pages[i];
            frames[j].counter = 0;
            printf("\n%4d\t", pages[i]);
            for (fNo = 0; fNo < n; fNo++)
                printf("%4d", frames[fNo].pNo);
            printf(":\t\tYES");
        }
        else
        {
            printf("\n%4d\t", pages[i]);
            for (fNo = 0; fNo < n; fNo++)
                printf("%4d", frames[fNo].pNo);
            printf(":\t\tNO");
        }
    }
    printf("\n---------------------------------------");
    printf("\nTotal page faults: %d\n", pageFault);
}
