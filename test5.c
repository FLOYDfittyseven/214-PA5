#include <stdlib.h>
#include <stdio.h>
#include "mem.h"

int main()
{
	void		*ptr;
	
	ptr = malloc( 500 );
	free( ptr );
	free( ptr );
	
	exit(EXIT_SUCCESS);
}
