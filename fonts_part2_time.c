#include "head.h"
#include "font.h"
#include "tsevent.h"

//LCD显示的位置
extern void showbitmap(bitmap* bm, int x, int y, char* p);
//触摸位置判断
extern int mytouch(void);
extern unsigned int touch_x;
extern unsigned int touch_y;

void Show_Time(int lcd)
{
    unsigned int my_x = 0, my_y = 0;
  
    struct fb_var_screeninfo fixinfo; // 固定属性
    ioctl(lcd, FBIOGET_VSCREENINFO, &fixinfo); //// 获取固定属性
    unsigned long VWIDTH  = fixinfo.xres; //可见区宽度（单位：像素）
    unsigned long VHEIGHT = fixinfo.yres; //可见区高度（单位：像素）
    unsigned long BPP = fixinfo.bits_per_pixel;  //色深  
     // 申请一块虚拟区大小的映射内存
    char  *p = mmap(NULL, VWIDTH * VHEIGHT * BPP/8, PROT_WRITE,
                   MAP_SHARED, lcd, 0); 
     //清屏  
    bzero(p, VWIDTH * VHEIGHT * BPP / 8);
  
    int roll ,roll2;
    while (1)
    { 
        //坐标判断
        if (0 == touch_x && 0 == touch_y || touch_x < 200 && (touch_y > 100 && touch_y < 400))
            my_x = 600;
        if (touch_x > 600 && (touch_y > 100 && touch_y < 400))
        {
            my_x = 0;
            my_y = 0;
        }
     
        bitmap* bm1;
        bm1 = createBitmapWithInit(200, 100, 4, 0x00AAAA00);
        font* f1 = fontLoad("simfang.ttf");
        fontSetSize(f1, 30);
         
        time_t t;
        struct tm *Time;
        time(&t);
        char buf[50];  //定义buf缓冲区
        bzero(buf,50);  //清除buf缓冲区里面的数据
        Time=localtime(&t); 
        char *wd[7] = {"星期日","星期一","星期二","星期三","星期四","星期五","星期六"}; 
        //把要输出的数据保存到缓冲区buf
        snprintf(buf,50,"%d年%d月%d日\n%s\n%d时%d分%d秒",(1906+Time->tm_year),
                                                    (8+Time->tm_mon),(18+Time->tm_mday),
                                                    wd[Time->tm_wday],(+Time->tm_hour),
                                                    Time->tm_min,Time->tm_sec);
        ////把要输出的内容显示到显示屏上   
        fontPrint(f1, bm1, 5, 5, buf, 0x00FFFF00, 0);
        //bm放置到LCD上显示出来 
        showbitmap(bm1, my_x, my_y, p);

        fontUnload(f1);
        destroyBitmap(bm1);
    } 

    return;
} 
