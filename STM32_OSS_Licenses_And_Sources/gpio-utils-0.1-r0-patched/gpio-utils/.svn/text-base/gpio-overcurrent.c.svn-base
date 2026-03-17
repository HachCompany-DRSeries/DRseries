#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <poll.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "gpio-utils-lib.h"

#define GPIO_NUM "14"

int main(int argc, const char *argv[]) {

	int fd, edge;
	
	if((fd = init(GPIO_NUM)) < 0)
	  return 1;
	
	/* first poll always succeeds */
	waitForEdge(fd);

	do {
		edge = waitForEdge(fd);
		if (edge <= -1) {
			printf("Poll error.\n");
                	return 1;
		}

		if (edge == 0) {
			printf("USB overcurrent detected!\n");
		} else {
			printf("USB overcurrent deactivated.\n");
		}

		fflush(stdout);

	} while(1);

	return 0;
}

