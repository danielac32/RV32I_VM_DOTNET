

#include <string.h>
#include <stdlib.h>
#include <stdarg.h>   
#include <stdio.h>
#include <memory.h>
#include <syscall.h>
#include <math.h>
struct  memblk  memlist;    /* List of free memory blocks       */
extern  void meminit(void);
 
 
int main(int argc, char const *argv[])
{
    struct  memblk  *memptr;    /* Ptr to memory block      */
    uint32  free_mem;       /* Total amount of free memory  */
    meminit();
    free_mem = 0;
    for (memptr = memlist.mnext; memptr != NULL;
                        memptr = memptr->mnext) {
        free_mem += memptr->mlength;
    }
    printf("rv32i\n");
    printf ("Build date: %s %s\n\n", __DATE__, __TIME__);
    printf("%10d bytes of free memory.  Free list:\n", free_mem);
    for (memptr=memlist.mnext; memptr!=NULL;memptr = memptr->mnext) {
        printf("           [0x%08x to 0x%08x]\n",
        (uint32)memptr, ((uint32)memptr) + memptr->mlength - 1);
    }

    printf("%10d bytes of Xinu code.\n",
        (uint32)&_etext - (uint32)&_text);
    printf("           [0x%08x to 0x%08x]\n",
        (uint32)&_text, (uint32)&_etext - 1);
    printf("%10d bytes of data.\n",
        (uint32)&_ebss - (uint32)&_sdata);
    printf("           [0x%08x to 0x%08x]\n\n",
        (uint32)&_sdata, (uint32)&_ebss - 1);


    return 0;
}