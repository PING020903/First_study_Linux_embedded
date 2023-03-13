#include "head.h"
#include "font.h"
#include "tsevent.h"

//触摸位置判断
extern int mytouch(void);
//LCD显示的位置
void showbitmap(bitmap *bm, int x, int y,char *p)
{ 
     
     char *q = p + (y*800 + x)*4;
    for(int j=0;j<bm->height && j < 480 ;j++)
    {
     for(int i=0;i<bm->width && i< 800 ;i++)   
         memcpy(q+(800*4*j)+i*4,bm->map+(bm->width*bm->byteperpixel*j)+
                                         bm->byteperpixel*i,bm->byteperpixel);
    } 
    
    bzero(bm->map,bm->width*bm->height*bm->byteperpixel);   
    //  字体的信息：
    // 长 bm->width
    // 高:bm->height
    // 色深:bm->byteperpixel);
    //  bm->map; 指向RGB数据，没有上下颠倒，也没有一行4字节倍数的约束，就是纯粹的RGB
}

void Bottom_Roll(int lcd)
{
    printf("oh shit!\n");

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

    int roll ,roll2;
    while (1)
    { 
        bitmap* bm2;
        bm2 = createBitmapWithInit(800, 79, 4, 0xFFFFFFF);
        font* f2 = fontLoad("simfang.ttf");
        fontSetSize(f2, 50);
          
      //把要输出的内容显示到显示屏上
        fontPrint(f2, bm2,5,30, "出门戴口罩 室内注意通风 勤洗手", 0xFF000000, 0); 
        roll -= 2;
        while (roll <=0) //字幕右移动
        {
            roll = 800;

        }
        roll2 += 1;
        while (roll2 >=800) //字幕左移动
        {
            roll2 =0;
        }         
       //bm妥善地放置到LCD上显示出来 
        showbitmap(bm2,    roll, 400,  p); 
        fontUnload(f2);
        destroyBitmap(bm2);
    } 
    
    return;
} 
