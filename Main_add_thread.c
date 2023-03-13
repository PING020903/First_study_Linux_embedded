#include "head.h"
#include "font.h"
#include "bmp.h"
#include "lcddevice.h"
#include "tsevent.h"


extern void showbitmap(bitmap* bm, int x, int y, char* p);
extern void Bottom_Roll(int lcd);
extern void Show_Time(int lcd);
extern void Show_Text(int lcd);
extern int mytouch(void);

//全局触摸变量x，y
unsigned int touch_x = 0, touch_y = 0;


//图片显示( 路径pathname, 图片尺寸width, 图片尺寸heigth, 显示位置x, 显示位置y )
void showBmp(struct LcdDevice* lcd, char* pathname, unsigned int width, unsigned int heigth, unsigned int x, unsigned int y)
{

    // //打开一个图片
    struct Bmp* bmp = create_bmp(pathname);
    //把图片尺寸修改与屏幕一样大
    zoom_bmp(bmp,  /*lcd->width */width, /*lcd->heigth*/heigth); //缩放的时候尺寸会有影响？？？
    // //反转图片
    reversal_bmp(bmp);
    // //绘制
    draw_rgb(lcd, x, y, bmp->width, bmp->height, bmp->data, 3);
    // //释放图片空间
    destroy_bmp(bmp);
}

//记录总线程数， 睡眠线程数， 运行线程数， 任务数
int anumber = 0;
int sleepnumber = 0;
int runnumber = 0; 
int tasknumber = 0;

pthread_cond_t v; //条件变量
pthread_mutex_t m; //互斥锁


//任务链表结构体
typedef struct Task{
    struct Task *next;
    struct Task *prev;
    void (*handler)(void *arg); //要执行的任务
    void *arg;
}Task;

//创建任务节点
Task * createTask(void (*handler)(void *arg), void *arg){
    Task* node = (Task*)malloc(sizeof(Task));
    node->next = node->prev = node;
    node->handler = handler;
    node->arg = arg;
    return node;
}

//把任务节点添加到任务链表
bool insertTask(Task *head,  Task *newTask){
    //把newTask添加到head后面,双向循环链表的头插法
    if(newTask == NULL || head==NULL) return false;
    newTask->next = head->next;
    newTask->prev = head;
    newTask->next->prev = newTask;
    head->next = newTask;
    return true;
}

//从任务链表中提取一个任务
Task* outTask(Task* head){
    if(head == NULL||head->next == NULL) return NULL;

    Task* node = head->next;
    head->next = node->next;
    node->next->prev = head;
    return node;
}


void * run(void *arg)
{
    //任务链表头，通过线程参数传递
    Task* thead = (Task*)arg;
    anumber+=1;
    while(1){
        sleepnumber+=1;
        pthread_mutex_lock(&m);
        pthread_cond_wait(&v, &m);
        sleepnumber-=1;
        runnumber+=1;
        //解锁
        pthread_mutex_unlock(&m);

        while(1){
            pthread_mutex_lock(&m); //上锁
            Task* node = outTask(thead); //提取一个任务准备执行
            pthread_mutex_unlock(&m); //解锁
            if(node == NULL || node->handler==NULL) break; //任务链表为NULL，所有任务已经执行完毕，线程准备进入到睡眠
            node->handler(node->arg); //执行任务
        }
        runnumber -= 1;
    }
}


//设计任务函数
void taskrun(void *arg){

    //打开lcd文件
    int lcd = open("/dev/fb0", O_RDWR);

    //创建lcd
    struct LcdDevice* lcd_bmp = lcd_init("/dev/fb0");
    
    //触摸准备
    struct TsDevice* ts = ts_init("/dev/input/event0");
    if (ts == NULL)
    {
        perror("init");
        return;
    }

    if (lcd < 0)
    {
        perror("fuck!!!open the file error!!!\n");
    }
    int  num= (int)arg;// 把参数直接强制转换为int
    if(num == 0)
    {
        printf("\ndo it(0) now!!!\n");

        //底部滚动字幕
        Bottom_Roll(lcd);
    }
    if(num == 1)
    {   
        printf("\ndo it(1) now!!!\n");

        //时间显示
        Show_Time(lcd);
    }
    if(num == 2)
    {
        printf("\ndo it(2) now!!!\n");

        //上下滚动文字
        Show_Text(lcd);
    }
    if (num == 3)
    {      
        printf("\ndo it(3) now!!!\n");
        while (1)
        {
            unsigned int my_x = 0, my_y = 0;

            //./Elysia.bmp
            for (int i = 0; i < 50; i++)
            {
                if (0 == touch_x && 0 == touch_y || touch_x < 200 && (touch_y > 100 && touch_y < 400))
                {
                    my_x = 0;
                    my_y = 0;
                }
                if (touch_x > 600 && (touch_y > 100 && touch_y < 400))
                {
                    my_x = 200;
                    my_y = 0;
                }
                showBmp(lcd_bmp, "./Elysia.bmp", 600, 400, my_x, my_y);
            }
                
            //./Elysia2.bmp
            for (int i = 0; i < 50; i++)
            {
                if (0 == touch_x && 0 == touch_y || touch_x < 200 && (touch_y > 100 && touch_y < 400))
                {
                    my_x = 0;
                    my_y = 0;
                }
                if (touch_x > 600 && (touch_y > 100 && touch_y < 400))
                {
                    my_x = 200;
                    my_y = 0;
                }
                showBmp(lcd_bmp, "./Elysia2.bmp", 600, 400, my_x, my_y);
            }
                
            //./Elysia3.bmp
            for (int i = 0; i < 50; i++)
            {
                if (0 == touch_x && 0 == touch_y || touch_x < 200 && (touch_y > 100 && touch_y < 400))
                {
                    my_x = 0;
                    my_y = 0;
                }
                if (touch_x > 600 && (touch_y > 100 && touch_y < 400))
                {
                    my_x = 200;
                    my_y = 0;
                }
                showBmp(lcd_bmp, "./Elysia3.bmp", 600, 400, my_x, my_y);
            }
                
            //./Elysia4.bmp
            for (int i = 0; i < 50; i++)
            {
                if (0 == touch_x && 0 == touch_y || touch_x < 200 && (touch_y > 100 && touch_y < 400))
                {
                    my_x = 0;
                    my_y = 0;
                }
                if (touch_x > 600 && (touch_y > 100 && touch_y < 400))
                {
                    my_x = 200;
                    my_y = 0;
                }
                showBmp(lcd_bmp, "./Elysia4.bmp", 600, 400, my_x, my_y);
            }
                
            //./Elysia5.bmp
            for (int i = 0; i < 20; i++)
            {
                if (0 == touch_x && 0 == touch_y || touch_x < 200 && (touch_y > 100 && touch_y < 400))
                {
                    my_x = 0;
                    my_y = 0;
                }
                if (touch_x > 600 && (touch_y > 100 && touch_y < 400))
                {
                    my_x = 200;
                    my_y = 0;
                }
                showBmp(lcd_bmp, "./Elysia5.bmp", 600, 400, my_x, my_y);
            }
                
            //./Elysia6.bmp
            for (int i = 0; i < 50; i++)
            {

                if (0 == touch_x && 0 == touch_y || touch_x < 200 && (touch_y > 100 && touch_y < 400))
                {
                    my_x = 0;
                    my_y = 0;
                }
                if (touch_x > 600 && (touch_y > 100 && touch_y < 400))
                {
                    my_x = 200;
                    my_y = 0;
                }
                showBmp(lcd_bmp, "./Elysia6.bmp", 600, 400, my_x, my_y);
            }               
        }       
    }
    if (num == 4)
    {
        while (1)
        {
            struct Point p = ts->getXY();

            printf("%d:%d\n", p.x, p.y);
            touch_x = p.x;
            touch_y = p.y;

            if (p.x > 760 && p.y > 440)
                system("kill -9 -1");
        }
    }
    ts_destroy(ts);
}


int main(void)
{
    //初始化条件变量和互斥锁
    pthread_cond_init(&v,NULL);
    pthread_mutex_init(&m, NULL);

    //创建任务头
    Task* thead = createTask(NULL, 0);




    pthread_t id = 0;
    for(int i=0; i<5; i++)
    {
        int ret  = pthread_create(&id, NULL, run, (void *)thead);
        pthread_detach(id);
    }

    while(1)
    {

        printf("All:%d - Sleep:%d - run:%d - task:%d\n",anumber, sleepnumber, runnumber, tasknumber);
        int n=0;
        printf("input a count(0,1,2,3):");
        scanf("%d",&n);
        //创建任务
        Task* node = createTask(taskrun, (void*)(n));
        
        pthread_mutex_lock(&m);
        //把任务添加到链表中
        insertTask(thead, node);
        pthread_cond_signal(&v);
        pthread_mutex_unlock(&m);
    
    }
}