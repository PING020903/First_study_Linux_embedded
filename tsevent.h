#ifndef _TSEVENT_H_
#define _TSEVENT_H_


#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <stdbool.h>

struct Point{
	int x, y;
};

struct TsDevice{
	int fd ;//文件描述符
	struct Point (*getXY)();
};

struct TsDevice* ts_init(const char *devname);
bool ts_destroy(struct TsDevice* ts);

#endif//_TSEVENT_H_
