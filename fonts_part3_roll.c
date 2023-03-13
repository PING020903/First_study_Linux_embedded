#include "head.h"
#include "font.h"
#include "tsevent.h"

//LCD显示的位置
extern void showbitmap(bitmap* bm, int x, int y, char* p);
//触摸位置判断
extern int mytouch(void);
extern unsigned int touch_x;
extern unsigned int touch_y;

void Show_Text(int lcd)
{
    unsigned int my_x = 0, my_y = 0;

    struct fb_var_screeninfo fixinfo; // 固定属性
    ioctl(lcd,FBIOGET_VSCREENINFO,&fixinfo); //// 获取固定属性
    unsigned long VWIDTH  = fixinfo.xres; //可见区宽度（单位：像素）
    unsigned long VHEIGHT = fixinfo.yres; //可见区高度（单位：像素）
    unsigned long BPP = fixinfo.bits_per_pixel;  //色深  
    // 申请一块虚拟区大小的映射内存
    char  *p = mmap(NULL, VWIDTH * VHEIGHT * BPP/8, PROT_WRITE,
                   MAP_SHARED, lcd, 0); 
     //清屏  
    bzero(p,VWIDTH*VHEIGHT*BPP/8);
  
    FILE *fp = fopen("text.txt","r");
    if(fp == NULL)
    {
        perror("打开失败");
        exit(0);
    }
    fseek(fp,0,SEEK_END);
    long Sizelit = ftell(fp);
    fseek(fp,0,SEEK_SET);
    char *Buf=calloc(1,Sizelit);
    bzero(Buf,Sizelit);
    fread(Buf,Sizelit/1000,1000,fp);
  
    int roll ,roll2;
    while (1)
    { 
        //坐标判断
        if (0 == touch_x && 0 == touch_y || touch_x < 200 && (touch_y > 100 && touch_y < 400))
        {
            my_x = 600;
            my_y = 100;
        }
        if (touch_x > 600 && (touch_y > 100 && touch_y < 400))
        {
            my_x = 0;
            my_y = 100;
        }

        bitmap* bm3;
        bm3 = createBitmapWithInit(200, 300, 4, 0x00CCAA00);
        font* f3 = fontLoad("simfang.ttf");
        fontSetSize(f3, 30);
  
        int i;
        fontPrint(f3, bm3, 3, i -= 33, Buf, 0xffd70000, 750);
        showbitmap(bm3, my_x, my_y, p);

        fontUnload(f3);
        destroyBitmap(bm3);    
    } 
    return;
} 
