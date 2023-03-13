
#include "tsevent.h"
#include "linux/input.h"
static struct TsDevice *instance = NULL;
struct Point get_xy()
{	
	struct Point point={0,0};
	if(instance == NULL) return point;
	struct input_event evt;
	while(1)
	{
		int ret = read(instance->fd, &evt, sizeof(evt));
		if(evt.type == EV_ABS && evt.code == ABS_X)
		{
			point.x = evt.value;
		}
		if(evt.type == EV_ABS && evt.code == ABS_Y)
		{
			point.y = evt.value;
		}

		if(point.x != 0  && point.y != 0) break;

	}
	return point;
}

struct TsDevice* ts_init(const char *devname)
{
	instance = malloc(sizeof(struct TsDevice));
	if(instance == NULL) return NULL;

	instance->fd = open(devname, O_RDONLY);
	if(instance->fd < 0)
	{
		free(instance);
		return NULL;
	}
	instance->getXY = get_xy;
	return instance;
}

//关闭触摸文件
bool ts_destroy(struct TsDevice* ts)
{
	if(ts == NULL) return NULL;

	close(ts->fd);

	free(ts);
	return true;
}


