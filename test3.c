#include <stdlib.h>
#include <stdio.h>
#include "mem.h"

int main()
{
	void		*ptr;
	
	ptr = malloc( blocksize );		//There are only (blocksize - sizeof(struct memEntry)) bytes left in heap
	
	exit(EXIT_SUCCESS);
}
