struct memEntry{
   struct memEntry *prev, *next;
   int isfree;
   unsigned int size;
   unsigned int sanity_check;
};

void
heapTraversal();

void *
mymalloc( unsigned int size, char * file, int line );

void
myfree( void * p, char * file, int line );

#define blocksize (1024*1024)
#define malloc( x )	mymalloc( x, __FILE__, __LINE__ )
#define free( x ) myfree( x, __FILE__, __LINE__ )
