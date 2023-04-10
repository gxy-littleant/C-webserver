//
// Created by gxy on 2023/01/21.
//

#ifndef LST_TIMER
#define LST_TIMER

#include <iostream>
#include <netinet/in.h>
#include <time.h>
#include <vector>
#include <algorithm>

using namespace std;

#define BUFFER_SIZE 64

struct client_data;


/* 定时器类 */
class heap_timer_node {
public:
    heap_timer_node(int delay) {
    //    expire = time(nullptr) + delay;
        expire = delay;
        validFlag = true;
    }

public:
    time_t expire;                      /* 定时器生效的绝对时间 */
    void (*cb_func)(client_data *);     /* 定时器的回调函数 */
    client_data *user_data;             /* 用户数据 */
    bool validFlag;                    /* 有效标志 */
};





/* 构建堆时的比较函数 */
bool cmp_heap_timer_node(heap_timer_node *node1, heap_timer_node *node2);



/* 时间堆类 */
class timer_heap {
public:
    timer_heap(int cap);

    void add_timer(heap_timer_node *timer); /* 添加目标定时器timer */
    void del_timer(heap_timer_node *timer); /* 删除目标定时器timer */
    heap_timer_node *get_top();             /* 获得堆顶部的定时器 */
    void del_top();                         /* 删除堆顶部的定时器 */
    void tick();                            /* 心博函数 */
    void disPlay();                         /* 输出堆中所有定时器 */
    bool is_empty();                        /* 堆是否为空 */
    void adjust_timer();                    /* 将堆重新排序*/


    ~timer_heap();                          /* 销毁时间堆 */
private:
    void resize();                          /* 将当前容量扩大一倍 */

private:
    vector<heap_timer_node *> array;        /* 堆数组 */
    int capacity;                           /* 堆数组的容量 */
    int cur_size;                           /* 堆数组中当前的元素个数 */
    int deleteNum;                          /* 已删除的定时器数量 */
};


/* 绑定socket和定时器 */
struct client_data {
    sockaddr_in address;                    // 客户端socket地址
    int sockfd;                             // socket文件描述符
    char buf[BUFFER_SIZE];                  // 读缓存
    heap_timer_node *timer;                 // 定时器

};


#endif //LST_TIMER

