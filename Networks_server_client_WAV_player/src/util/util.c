#include "util.h"

void pdie(char* msg){
	int errnum = errno;
	printf("%s\n",msg);
	fprintf(stderr, "Value of errno: %d\n", errno);
	fprintf(stderr, "%s\n", strerror(errnum));
	exit(0);
}
