
#include <syscall.h>
 #include <stddef.h>
 #include <kernel.h>
 #include <string.h>
#include <memory.h>

extern syscall	freemem(
	  char		*blkaddr,	/* Pointer to memory block	*/
	  uint32	nbytes		/* Size of block in bytes	*/
	);
 
extern  char  	*getmem(
	  uint32	nbytes		/* Size of memory requested	*/
	);

void *malloc(uint32 nbytes)
{
    struct memblk *pmem;

    /* we don't allocate 0 bytes. */
    if (0 == nbytes)
    {
        return NULL;
    }

    /* make room for accounting info */
    nbytes += sizeof(struct memblk);

    /* acquire memory from kernel */
    pmem = (struct memblk *)getmem(nbytes);
    if (SYSERR == (uint32)pmem)
    {
        return NULL;
    }

    /* set accounting info */
    pmem->mnext = pmem;
    pmem->mlength = nbytes;

    return (void *)(pmem + 1);  /* +1 to skip accounting info */
}

void free(void *pmem)
{
    struct memblk *block;

     if (pmem == NULL) {
        return; // safe: free(NULL) is legal in C
    }
    
    /* block points at the memblock we want to free */
    block = (struct memblk *)pmem;

    /* back up to accounting information */
    block--;

    /* don't memfree if we fail basic checks */
    if (block->mnext != block)
    {
        return;
    }

    freemem((char *)block, block->mlength);
}

 

void* realloc(void* ptr, size_t size)
{
    void* new_data = NULL;

    if(size)
    {
        if(!ptr)
        {
            return malloc(size);
        }

        new_data = malloc(size);
        if(new_data)
        {
            memcpy(new_data, ptr, size); // TODO: unsafe copy...
            free(ptr); // we always move the data. free.
        }
    }

    return new_data;
}

void            *calloc(size_t nmemb, size_t size)
{
  unsigned int      s;
  char          *ptr;

  s = nmemb * size;
  if ((ptr = malloc(s)) == NULL)
    return (NULL);
  memset(ptr, 0, s);
  return (ptr);
}