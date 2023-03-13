#include "bmp.h"

//创建一个bmp对象，从文件导出bmp图片数据
struct Bmp* create_bmp(const char *bmpfile)
{
	//创建BMP结构体对象
	struct  Bmp *bm = (struct Bmp*)malloc(sizeof(struct Bmp));
	if(bm == NULL) 
	{
		perror("create: malloc");
		return NULL;
	}
	//定义bmp文件头
	BitHeader  header;
	FILE *file = fopen(bmpfile,"r+");
	if(file == NULL) 
	{
		perror("create:fopen");
		goto delbm;
	}

	//读取图片头
	size_t size = fread(&header,1,sizeof(header),file);
	if(size != sizeof(header))
	{
		perror("create:fread header");
		goto delfile;	
	}
	bm->width =  header.biWidth;
	bm->height =  header.biHeight;
	bm->pixbyte =  header.biBitCount/8;

	//show_bmpheader(&header);
	//定义空间存储图片像素
	unsigned int  rgbLen = header.bfSize-54;
	bm->data =(uint8*) malloc(rgbLen);
	if(bm->data == NULL)
	{
		perror("craete:calloc");
	}

	if(header.biSizeImage == 0)
	{
		header.biSizeImage = header.biSize - 54;
	}
	//一行占用的字节数
	unsigned int rowsize = header.biSizeImage/bm->height;//(包含补齐的数据)
	//读取像素数据
	int n=0;
	unsigned int rsize = rowsize-(bm->width%4);
	for(n=0; n<bm->height; n++)
	{
		size  =  fread(bm->data+n*rsize,1, rowsize, file);
		if(size < 0)
		{
			perror("create:fread context");
			free(bm->data);
			fclose(file);
			goto delbm;
		}
	}
	
	fclose(file);
	return bm;
delfile:
	fclose(file);
delbm:
	free(bm);
	return NULL;
}
//销毁bmp对象
bool destroy_bmp(struct Bmp* bm)
{
	if(bm == NULL) return false;
	if(bm->data != NULL)free(bm->data);
	free(bm);
	return true;
}
//缩放
bool zoom_bmp(struct Bmp *src,  int w, int h)
{
	if(src == NULL) 
	{
		perror("zoom:src empty");
		return false;
	}
	//申请目标空间
	struct Bmp dest;
	dest.width = w;
	dest.height = h;
	dest.pixbyte = src->pixbyte;
	dest.data = malloc(dest.width*dest.height*dest.pixbyte);
	if(dest.data == NULL)
	{
		perror("zoom_bmp malloc ");
		return false;
	}
	int bw = src->width*100/dest.width;
	int bh = src->height*100/dest.height;
	
	int i=0,j=0;
	for(j=0; j<h; j++)
	{
		for(i=0; i<w; i++)
		{
			dest.data[j*w*3+i*3+0] = src->data[(j*bh/100)*src->width*3 + (i*bw/100)*3+0];
			dest.data[j*w*3+i*3+1] = src->data[(j*bh/100)*src->width*3 + (i*bw/100)*3+1];
			dest.data[j*w*3+i*3+2] = src->data[(j*bh/100)*src->width*3 + (i*bw/100)*3+2];
		}
	}
	//把原图的data释放
	free(src->data);
	src->data = dest.data;
	src->width = w;
	src->height = h;
	
	return true;
}

//反转
void reversal_bmp(struct Bmp* src)
{
	if(src == NULL) return;
	
	//printf("i=%d j=%d\n", 1,2);
	int rowsize = src->width*src->pixbyte; //一行字节数
	unsigned char tmpbuffer[rowsize]; //保存
	int i=0;//第一行
	int j=src->height-1;//最后一行

	for(i=0; i<j; i++,j--)
	{
		
		memcpy(tmpbuffer,src->data+i*rowsize, rowsize);
		memcpy(src->data+i*rowsize, src->data+j*rowsize,rowsize);
		memcpy(src->data+j*rowsize, tmpbuffer,rowsize);
		
	}
}

void show_bmpheader(struct BitFileHeader *header)
{
	
	printf("%s\n", (char*)&header->bfType);
	printf("%d\n", header->bfSize);
	printf("%d\n", header->bfReserved1);
	printf("%d\n", header->bfReserved2);
	printf("%d\n", header->bfOffBits);
	printf("%d\n", header->biSize); 
	printf("%d\n", header->biWidth);
	printf("%d\n", header->biHeight); 
	printf("%d\n", header->biPlanes); 
	printf("%d\n", header->biBitCount);
	printf("%d\n", header->biCompression);
	printf("%d\n", header->biSizeImage); 
	printf("%d\n", header->biXPelsPerMeter);
	printf("%d\n", header->biYPelsPerMeter);
	printf("%d\n", header->biClrUsed); 
	printf("%d\n", header->biClrImportant);
}


bool save_bmp(struct Bmp* bm , const char *bmpfile)
{
	if(bm==NULL)return false;
	
	//每一行占用的字节数（包括补齐）
	int rowbyte = bm->width*bm->pixbyte;
	if(rowbyte % 4 != 0)
	{
		rowbyte += bm->width%4;
	}
	//定义bmp文件头
	BitHeader  header;
	//初始化头
	header.bfType = 0x4D42;
	header.bfSize=rowbyte*bm->height+54;
	header.bfReserved1=0;
	header.bfReserved2=0;
	header.bfOffBits=54;
	header.biSize=40; 
	header.biWidth=bm->width;
	header.biHeight=bm->height; 
	header.biPlanes=1; 
	header.biBitCount=bm->pixbyte*8;
	header.biCompression=0;
	header.biSizeImage=rowbyte*bm->height;
	header.biXPelsPerMeter=2835;
	header.biYPelsPerMeter=2835;
	header.biClrUsed=0; 
	header.biClrImportant=0;
	
	//打开文件
	FILE *file = fopen(bmpfile, "w");
	if(file == NULL) return false;
	//写文件头
	fwrite(&header, sizeof(header), 1, file);
	//写图片数据
	int i=0;
	
	int rgbrowByte = bm->width*bm->pixbyte;
	for(i=0; i<bm->height; i++)
	{
		fwrite(bm->data+i*rgbrowByte, 1,rgbrowByte, file);
		#if 1
		if(rgbrowByte % 4 != 0)
		{
			int  p = 0;
			fwrite(&p, 1,bm->width%4, file);
		}
		#endif
	}
	//关闭文件
	fclose(file);
	return true;
}







