#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <poll.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <limits.h>

#define BUFFER_LEN 4096

int testContentEqualsFd(int fd, char* content) {
	char buffer[BUFFER_LEN];
	size_t len = strlen(content);
	size_t l, read_count = 0;

	while((l = read(fd, buffer, BUFFER_LEN)) > 0){
		if(read_count + l > strlen(content) ||
		   memcmp(content+read_count, buffer, l))
			return 0;
		read_count += l;
	}
	
	return read_count == len;
}

int testContentEquals(char* file, char* content) {
	int fd;
	int ret;

	fd = open(file, O_RDONLY);
	ret = testContentEqualsFd(fd,  content);
	
	close(fd);
	return ret;
}


/*
 * Wait for an edge on the value file and return the new pin status.
 */
int waitForEdge(int fd) {
	struct pollfd polldata = {
		.events = POLLPRI,
		.fd = fd
	};

	int pollresult = poll(&polldata, 1, -1);

	if  (pollresult < 0) {
		return -1;
	}
	
	lseek(fd, 0, 0);
	return testContentEqualsFd(fd, "1\n");
}

int init(char* gpio_num)
{
	char filename[PATH_MAX];
	struct stat s;
	int fd;

	snprintf(filename, PATH_MAX, "/sys/class/gpio/gpio%s", gpio_num);
	if (stat(filename, &s) != 0 || !(s.st_mode & S_IFDIR)) {
		printf("GPIO pin %s seems not to be exported.\n", gpio_num);
                return -1;
	}
	
	snprintf(filename, PATH_MAX, "/sys/class/gpio/gpio%s/direction", gpio_num);
	if (!testContentEquals(filename, "in\n")) {
                printf("GPIO pin %s seems not to be an input pin.\n", gpio_num);
                return -1;
	}

	snprintf(filename, PATH_MAX, "/sys/class/gpio/gpio%s/edge", gpio_num);
        if (!testContentEquals(filename, "both\n")) {                
                printf("GPIO pin %s seems not to be configured to trigger on both edges.\n", gpio_num);
                return -1;
        }

	snprintf(filename, PATH_MAX, "/sys/class/gpio/gpio%s/value", gpio_num);
	fd = open(filename, O_RDONLY);
        if (fd < 0) {                
                printf("Could not open value file for GPIO pin %s.\n", gpio_num);
        }

	return fd;
}
