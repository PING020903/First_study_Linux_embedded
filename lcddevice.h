#ifndef _LCDDEDVICE_H_
#define _LCDDEDVICE_H_ 

#include <stdio.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/stat.h>

typedef  struct  LcdDevice{
	int fd  ;//文件描述符
	void  *mptr ;//映射空间首地址
	int width, heigth;//宽,高(该参数为虚拟内存中的高度，与实际大小可能有所出入)
	int pixbyte;//单个像素占用的字节数
	unsigned int screen_size;
} LcdDevice;

//初始化lcd
LcdDevice * lcd_init(const char *devname);

//销毁lcd
bool lcd_destroy(struct LcdDevice* lcd);

//设置某一个像素点颜色
void  draw_point(struct LcdDevice *lcd,  int x, int y,  unsigned int color);

//清屏
void lcd_clear(struct LcdDevice* lcd, unsigned int color);

//绘制RGB数据(lcd设备文件，起始位置x，起始位置y，区域宽w，区域高h，rgbData文件，像素字节pixbyte )
void draw_rgb(struct LcdDevice* lcd, int x, int y, int w,int h,  const char *rgbData,int pixbyte);


#endif//_LCDDEDVICE_H_ 
