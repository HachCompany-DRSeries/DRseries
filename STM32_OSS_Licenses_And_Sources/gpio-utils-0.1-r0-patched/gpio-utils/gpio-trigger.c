#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <poll.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "gpio-utils-lib.h"

void usage(void){
	printf("gpio-trigger <gpio_num>\n");
}	

int main(int argc, char *argv[]) {

	int fd, edge;

	if(argc < 2 || argc > 2){
		usage();
		return 1;
	}

	if((fd = init(argv[1])) < 0)
	  exit(EXIT_FAILURE);

	/* first one always returns */
	waitForEdge(fd);

	edge = waitForEdge(fd);

	if (edge <= -1) {
		printf("Poll error!\n");
                exit(EXIT_FAILURE);
	}

	return 0;
}

