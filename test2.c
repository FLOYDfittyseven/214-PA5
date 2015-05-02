#include <stdlib.h>
#include <stdio.h>
#include "mem.h"

int main()
{
	void		*ptr, *ptr2;
	
	ptr = malloc( (blocksize - sizeof(struct memEntry)) );
	ptr2 = malloc( 50 );
	free( ptr );
	free( ptr2 );
	
	ptr2 = malloc( 50 );
	
	exit(EXIT_SUCCESS);
}
