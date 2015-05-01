#include "mem.h"
#include <unistd.h>
#include <stdlib.h>

static char myblock[blocksize];

void *
mymalloc( unsigned int size, char * file, int line )
{
	static int				initialized = 0;
	static struct memEntry	*root;
	struct memEntry			*p, *next;
	
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
		initialized = 1;
	}
	
	p = root;
	do
	{
		/* 
		 * If chunk is not big enough or is already taken, move on.
		 */
		if( p->size < size || !p->isfree )
		{
			p = p->next;
		}
		/*
		 * Chunk not big enough for an additional memEntry
		 */
		else if( p->size < ( size + sizeof(struct memEntry) ) )
		{
			p->isfree = 0;
			return (char *)p + sizeof(struct memEntry);
		}
		/*
		 * Chunk big enough for an additional memEntry
		 */
		else
		{
			/*
			 * Moving pointers accordingly
			 */
			next = (struct memEntry *)( (char *)p + sizeof(struct memEntry) + size )
			next->prev = p;
			next->next = p->next;
			if( p->next != 0 )
			{
				p->next->prev = next;
			}
			p->next = next;
			
			/*
			 * Setting values
			 */
			 next->size = p->size - sizeof(struct memEntry) - size;
			 next->isfree = 1;
			 p->size = size;
			 p->isfree = 0;
			 
			 return (char *)p + sizeof(struct memEntry);
		}
	}while( p != 0 );
	
	fprintf(stderr, "No chunk of size %d is available. mymalloc failed in %s line %d.\n", size, file, line);
	return (char *)0;
}

void *
mymalloc2( unsigned int size, char * file, int line )
{
	static struct memEntry		*root = 0, *last = 0;
	struct memEntry				*p, *next;
	
	p = root;
	
	while( p != 0 )
	{
		if( p->size < size )
		{
			p = p->next;
		}
		else if( !p->isfree )
		{
			p = p->next;
		}
		else if( p->size < (size + sizeof(struct memEntry)) )
		{
			p->isfree = 0;
			return (char *)p + sizeof(struct memEntry);
		}
		else
		{
			next = (struct memEntry *)( (char *)p + sizeof(struct memEntry) + size );
			next->prev = p;
			next->next = p->next;
			if( p->next != 0 )
			{
				p->next->prev = next;
			}
			p->next = next;
			next->size = p->size - sizeof(struct memEntry) - size;
			next->isfree = 1;
			p->size = size;
			p->isfree = 0;
			return (char *)p + sizeof(struct memEntry);
		}
	}
	
	if( (p = (struct memEntry *) sbrk(sizeof(struct memEntry) + size)) == (void *)-1 )
	{
		return 0;		//also, complain
	}
	else if( last == 0 )
	{
		p->prev = p->next = 0;
		p->size = 0;
		p->isfree = 0;
		root = last = p;
		return (char *)p + sizeof(struct memEntry);
	}
	else
	{
		p->prev = last;
		p->next = last->next;		//0 (?)
		p->size = size;
		p->isfree = 0;
		last->next = p;
		last = p;
		return p+1;
	}

}

void
myfree( void * p, char * file, int line )
{
	struct memEntry *ptr, *prev, *next;
	
	ptr = (struct memEntry *) ( (char *)p - sizeof(struct memEntry) );
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
