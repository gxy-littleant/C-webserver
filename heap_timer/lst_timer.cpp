//
// Created by gxy on 2023/01/21.
//

#include "lst_timer.h"

/* 用于构造堆 */
bool cmp_heap_timer_node(heap_timer_node *node1, heap_timer_node *node2) {
    // 从大到小排序
    return node1->expire > node2->expire;
}

// 构建最大堆
// make_heap(nums.begin(), nums.end(), less<int>());
// 构建最小堆
// make_heap(nums.begin(), nums.end(), greater<int>());

/* 构造函数 */
timer_heap::timer_heap(int cap) {
    this->capacity = cap;
    this->array.resize(cap);
    this->cur_size = 0;
    this->deleteNum = 0;
    for (int i = 0; i < cap; i++) {
        this->array[i] = nullptr;
    }
    make_heap(array.begin(), array.begin() + cur_size, cmp_heap_timer_node);
}

/* 添加一个定时器 */
void timer_heap::add_timer(heap_timer_node *timer) {
    if (timer == nullptr) {
        return;
    }
    if (cur_size >= capacity) {     /* 无空间，则将array扩大一倍 */
        resize();
    }

    /* 插入新元素 */
    array[cur_size] = timer;
    cur_size++;

    /* 调整堆 */
    push_heap(array.begin(), array.begin() + cur_size, cmp_heap_timer_node);
}

/* 删除一个定时器 */
void timer_heap::del_timer(heap_timer_node *timer) {
    if (timer == nullptr) {
        return;
    }
    /* 仅仅将目标定时器的回调函数设置为空，即所谓的延迟销毁，
     * 将节省真正删除该定时器造成的开销，但这样做容易造成堆数组膨胀 */
    timer->cb_func = nullptr;
    timer->validFlag = false;   /* 将定时器标记为无效 */
    deleteNum++;
    /* 判断删除数是否达到当前容量的一半，达到了则删除并重新建堆 */
    if (deleteNum >= cur_size / 2) {
        int vaildIndex = 0;
        for (int i = 0; i < cur_size; i++) {
            if (array[i]->validFlag) {
                array[vaildIndex] = array[i];
                vaildIndex++;
                array[i] = nullptr;
            } else {
                delete array[i];
                array[i] = nullptr;
            }
        }
        cur_size = vaildIndex;
        make_heap(array.begin(), array.begin() + cur_size, cmp_heap_timer_node);
    }
}

/* 获取堆顶定时器 */
heap_timer_node *timer_heap::get_top() {
    if (is_empty()) {
        return nullptr;
    } else {
        return array.front();
    }
}

/* 删除堆顶定时器 */
void timer_heap::del_top() {
    /* pop_heap 将堆顶元素移至当前堆的最末尾位置，即：array[array.begin() + cur_size-1]处*/
    pop_heap(array.begin(), array.begin() + cur_size, cmp_heap_timer_node);
    /* 删除该定时器 */
    delete array[cur_size - 1];
    array[cur_size - 1] = nullptr;
    cur_size--;
}

/* 心博函数 */
void timer_heap::tick() {
    heap_timer_node *topTimer = get_top();
    time_t cur = time(nullptr);
    cout<< "timer tick"<<endl;
    while (!is_empty()) {
        if (topTimer == nullptr) {
            break;
        }
        /* 如果堆顶元素还没有到期，则退出循环 */
        if (topTimer->expire > cur) {
            break;
        }
        /* 否则执行堆顶定时器中的任务 */
        if (topTimer->validFlag && topTimer->cb_func != nullptr) {
            topTimer->cb_func(topTimer->user_data);
        }
        /* 删除堆顶元素 */
        del_top();
        topTimer = get_top();
    }
}

/* 将array的容量扩大一倍 */
void timer_heap::resize() {
    array.resize(capacity * 2);
    capacity = capacity * 2;
}

void timer_heap::disPlay() {
    cout << "disPlay: "<<endl;
    for (int i = 0; i < capacity; i++) {
        if (array[i] == nullptr) {
            break;
        }
        cout << array[i]->expire << " ";
    }
    cout << endl;
}

void timer_heap::adjust_timer(){
    make_heap(array.begin(), array.begin() + cur_size, cmp_heap_timer_node);
}

/* 判断堆是否为空 */
bool timer_heap::is_empty() {
    return cur_size == 0;
}

/* 析构函数，销毁堆 */
timer_heap::~timer_heap() {
    for (int i = 0; i < cur_size; i++) {
        delete array[i];
        array[i] = nullptr;
    }
}
