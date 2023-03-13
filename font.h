#ifndef __font_h__
#define __font_h__

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <strings.h>
#include "truetype.h"
#define color u32
#define getColor(a, b, c, d) (a|b<<8|c<<16|d<<24)

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef char s8;
typedef short s16;
typedef int s32;
typedef long long s64;

//该结构体在"trurtype.h"重复定义
//typedef struct stbtt_fontinfo
//{
//   void           * userdata;
//   unsigned char  * data;              // pointer to .ttf file
//   int              fontstart;         // offset of start of font
//
//   int numGlyphs;                     // number of glyphs, needed for range checking
//
//   int loca,head,glyf,hhea,hmtx,kern; // table locations as offset from start of .ttf
//   int index_map;                     // a cmap mapping for our chosen character encoding
//   int indexToLocFormat;              // format needed to map from glyph index to glyph
//} stbtt_fontinfo;


typedef struct{
	u32 height;// 高
	u32 width;// 长 
	u32 byteperpixel;// 色深
	u8 *map;//  指向RGB数据，没有上下颠倒，也没有一行4字节倍数的约束，就是纯粹的RGB
}bitmap;

typedef struct{
	stbtt_fontinfo *info;
	u8 *buffer;
	float scale;
}font;



//1.初始化字库 
font *fontLoad(char *fontPath);

//2.设置字体的大小 
void fontSetSize(font *f, s32 pixels);

//3.设置字体输出框的大小
bitmap *createBitmap(u32 width, u32 height, u32 byteperpixel);

//可以指定输出框的颜色(长width, 高height, 色深byteperpixel, 颜色c)
bitmap *createBitmapWithInit(u32 width, u32 height, u32 byteperpixel, color c);


//4.把字体输出到输出框中
void fontPrint(font *f, bitmap *screen, s32 x, s32 y, char *text, color c, s32 maxWidth);


//5.把输出框的所有信息显示到LCD屏幕中 
// bitmap.width
// bitmap.height
// bitmap.bytepixel
// bitmap.map


// 关闭字体库
void fontUnload(font *f);

// 关闭bitmap
void destroyBitmap(bitmap *bm);

#endif
