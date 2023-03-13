#include "tsevent.h"

int mytouch(void)/*1024*600*/
{
	struct TsDevice *ts = ts_init("/dev/input/event0");
	if(ts == NULL)
	{
		perror("init");
		return -1;
	}
	while(1)
	{
		struct Point p = ts->getXY();
		printf("%d:%d\n", p.x , p.y);
		//if(p.x >760 && p.y>440) break;
		/*x > 600 && (y > 100 && y < 400)上下滚动区域*/
	}

	ts_destroy(ts);
	return 0;

}
