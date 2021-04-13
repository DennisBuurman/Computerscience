#ifndef util_h
#define util_h

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct{
	bool drop_packets;
	bool bit_flips;
	int fixed_quality; 
}optionset;

void pdie(char* msg);

#endif
