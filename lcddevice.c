
#include "lcddevice.h"
#include "linux/fb.h"
//#define ARM   //如果是ARM要把这个宏打开
//初始化lcd
LcdDevice * lcd_init(const char *devname)
{
	//static LcdDevice lcd;
	//定义lcd结构体对象
	LcdDevice *lcd = (LcdDevice*)malloc(sizeof(LcdDevice));
	if(lcd == NULL)
	{
		perror("lcd init error");
		return NULL;
	}

	//打开lcd设备
	lcd->fd = open(devname, O_RDWR);
	if(lcd->fd < 0) 
	{
		perror("open error");
		free(lcd);
		return NULL;
	}

	//获取lcd设备信息	
	struct fb_var_screeninfo info;
	int ret  = ioctl(lcd->fd, FBIOGET_VSCREENINFO, &info);
	
#ifdef ARM
    lcd->width=info.xres;
    lcd->heigth=info.yres;
#else
    lcd->width=info.xres_virtual;
    lcd->heigth=info.yres_virtual;
#endif
	lcd->pixbyte = info.bits_per_pixel/8;
	lcd->screen_size = lcd->width*lcd->heigth*lcd->pixbyte;
	
	printf("x:y--%d:%d\n", info.xres, info.yres);
	printf("x:y--%d:%d\n", info.xres_virtual, info.yres_virtual);
	//映射
	lcd->mptr = mmap(NULL, lcd->screen_size, 
	                 PROT_READ|PROT_WRITE, MAP_SHARED, 
	                 lcd->fd, 0);

	if(lcd->mptr == MAP_FAILED)
	{
		perror("映射失败");
		close(lcd->fd);
		free(lcd);
		return NULL;
	}
	return lcd;
}
//销毁lcd
bool lcd_destroy(struct LcdDevice* lcd)
{
	if(lcd == NULL)return true;
	if(lcd->mptr != MAP_FAILED)
	{
		munmap(lcd->mptr, lcd->screen_size);
	}
	if(lcd->fd > 0)
	{
		close(lcd->fd);
	}
	free(lcd);
	return true;
}

//设置某一个像素点颜色
void  draw_point(struct LcdDevice *lcd,  int x, int y,  unsigned int color)
{
	//用ptr保存绘制的开始位置
	unsigned int *ptr =( (unsigned int*)lcd->mptr) + y*lcd->width+x;
	ptr[0] = color;
}


//清屏
void lcd_clear(struct LcdDevice *lcd, unsigned int color)
{
	int w=lcd->width; 
	int h=lcd->heigth;
	unsigned int *ptr = (unsigned int *)lcd->mptr;
	
	int i=0,j=0;
	for(i=0; i<h; i++)
	for(j=0; j<w; j++)
	{
		ptr[i*w+j] = color;
	}
}

void draw_rgb(struct LcdDevice* lcd, int x, int y, int w,int h,  const char *rgbData,int pixbyte)
{
	unsigned int *ptr = (unsigned int *)lcd->mptr + lcd->width*y+x;
	int i=0, j=0;
	for(i=0; i<h; i++)
	{
		for(j=0; j<w; j++)
		{
			memcpy(ptr+j+i*lcd->width, rgbData+(w*i+j)*pixbyte, 4);
		}
		
	}
}

