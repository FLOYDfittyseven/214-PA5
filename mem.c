#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "mem.h"

static char myblock[blocksize];

void
heapTraversal()
{
	struct memEntry		*ptr;
	int					count;
	
	ptr = (struct memEntry *) myblock;
	count = 0;
	while( ptr )
	{
		if( !ptr->isfree )
		{
			count++;
		}
		ptr = ptr->next;
	}
	
	if( count > 0 )
	{
		fprintf( stderr, "\x1b[1;33mWARNING: %d dynamically allocated chunks of memory left unfreed.\x1b[0m\n", count );
	}
}

void *
mymalloc( unsigned int size, char * file, int line )
{
	static int				initialized = 0;
	static struct memEntry	*root;
	struct memEntry			*p, *next, *smallest;
	
	/*
	 * Initialize the block with a memEntry struct
	 */
	if( !initialized )
	{
		root = (struct memEntry *) myblock;
		root->prev = 0;
		root->next = 0;
		root->size = blocksize - sizeof (struct memEntry);
		root->isfree = 1;
		root->sanity_check = 0xAAAAAAAA;
		initialized = 1;
		atexit(heapTraversal);
	}
	
	p = root;
	smallest = NULL;
	
	/*
	 * Best Fit Method
	 */
	do
	{
		/* 
		 * If chunk is not big enough or is already taken, move on.
		 */
		if( p->size < size || !p->isfree )
		{
			p = p->next;
		}
		else
		{
			if( !smallest )
			{
				smallest = p;
			}
			else if( p->size < smallest->size )
			{
				smallest = p;
			}
			p = p->next;
		}
		
	}while( p != 0 );
	
	if( !smallest )
	{
	fprintf(stderr, "\x1b[1;33mNo chunk of size %d is available. mymalloc failed in %s line %d.\x1b[0m\n", size, file, line);
	return (char *)0;
	}
	/*
	* Chunk not big enough for an additional memEntry
	*/
	else if( smallest->size < ( size + sizeof(struct memEntry) ) )
	{
		smallest->isfree = 0;
		return (char *)smallest + sizeof(struct memEntry);
	}
	/*
	* Chunk big enough for an additional memEntry
	*/
	else
	{
		/*
		* Moving pointers accordingly
		*/
		next = (struct memEntry *)( (char *)smallest + sizeof(struct memEntry) + size );
		next->prev = smallest;
		next->next = smallest->next;
		if( smallest->next != 0 )
		{
			smallest->next->prev = next;
		}
		smallest->next = next;
		
		/*
		* Setting values
		*/
		next->size = smallest->size - sizeof(struct memEntry) - size;
		next->isfree = 1;
		next->sanity_check = 0xAAAAAAAA;
		smallest->size = size;
		smallest->isfree = 0;
			 
		return (char *)smallest + sizeof(struct memEntry);
	}
}

void
myfree( void * p, char * file, int line )
{
	struct memEntry *ptr, *prev, *next;
	char *blockbeg, *blockend;
	
	blockbeg = &myblock[0];
	blockend = &myblock[blocksize - 1];
	
	if( !p )
	{
		fprintf( stderr, "\x1b[1;31mError: Attempting to free a null pointer in %s line %d.\x1b[0m\n", file, line );
		return;
	}
	else if( (ptr = (struct memEntry *) ( (char *)p - sizeof(struct memEntry) )), (char *)ptr < blockbeg || (char *)p > blockend )
	{
		fprintf( stderr, "\x1b[1;31mError: Attempting to free pointer outside of the heap in %s line %d.\x1b[0m\n", file, line );
		return;
	}
	else if( ptr->sanity_check != 0xAAAAAAAA )
	{
		fprintf( stderr, "\x1b[1;31mError: Attempting to free a pointer that has not been returned by mymalloc in %s line %d.\x1b[0m\n", file, line );
		return;
	}
	else if( ptr->isfree )
	{
		fprintf( stderr, "\x1b[1;31mError: Attempting to free a pointer to memory that is not currently allocated in %s line %d.\x1b[0m\n", file, line );
		return;
	}
	
	/*
	 * If chunk being freed is not the first chunk in the block and
	 * the previous chunk is free, combine chunks
	 */
	if( (prev = ptr->prev) != 0 && prev->isfree )
	{
		prev->size += sizeof(struct memEntry) + ptr->size;
		prev->next = ptr->next;
		if( ptr->next != 0 )
		{
			ptr->next->prev = prev;
		}
	}
	/*
	 * Don't combine, just mark the chunk as free
	 */
	else
	{
		ptr->isfree = 1;
		prev = ptr;
	}
	/*
	 * If chunk being freed is not the last chunk in the block and the
	 * next chunk is free, combine chunks
	 */
	if( (next = ptr->next) != 0 && (next->isfree) )
	{
		prev->size += sizeof(struct memEntry) + next->size;
		if( next->next != 0 )
		{
			next->next->prev = prev;
		}
	}

}
