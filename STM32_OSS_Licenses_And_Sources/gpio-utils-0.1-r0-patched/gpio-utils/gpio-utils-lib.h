#ifndef GPIO_UTILS_LIBS_H
#define GPIO_UTILS_LIBS_H

#include <limits.h>
#include <errno.h>

int testContentEqualsFd(int fd, char* content);
int testContentEquals(char* file, char* content);
int waitForEdge(int fd);
int init(char* gpio_num);

#endif
