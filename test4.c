#include <stdlib.h>
#include <stdio.h>
#include "mem.h"

int main()
{
	void		*ptr;
	int			x;
	
	x = 5;
	ptr = malloc( 500 );
	
	free( ((char *)ptr) + 10 );
	free( ptr );
	free( &x );
	
	exit(EXIT_SUCCESS);
}
