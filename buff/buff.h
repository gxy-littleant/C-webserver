#ifndef BUFFER_H
#define BUFFER_H
#include <cstring>   //perror
#include <iostream>
#include <unistd.h>  // write
#include <sys/uio.h> //readv
#include <vector> //readv
#include <atomic>
#include <assert.h>

/*
读缓冲区：将读缓冲区中的数据写入fd中
写缓冲区：将fd中的数据读出到写缓冲区中
*/

class buff{

public:

    buff(int initBuffSize = 1024){};
    ~buff(){};

    size_t WritableBytes() const; // 可写字节数
    size_t ReadableBytes() const ; // 可读字节数
    size_t PrependableBytes() const; // 获取当前该读位置(index)

    const char* Peek() const; // 可读的第一个位置(可读首地址) 由readPos_指定
        
    void EnsureWriteable(size_t len); // 确认空间是否够写 不够则申请空间
    void HasWritten(size_t len); // 更新 writePos_

    void Retrieve(size_t len);  //  读取len长度的数据
    void RetrieveUntil(const char* end); // 读取地址为end处的数据

    void RetrieveAll() ; // 重置缓冲区
    std::string RetrieveAllToStr(); // 把所有可读的数据都读出来 并重置缓冲区

    const char* BeginWriteConst() const;
    char* BeginWrite(); // 可写的第一个位置 由writePos_指定

    void Append(const std::string& str);    // 将str添加进缓冲区
    void Append(const char* str, size_t len);
    void Append(const void* data, size_t len);
    void Append(const buff& buff);

    ssize_t ReadFd(int fd, int* Errno);
    ssize_t WriteFd(int fd, int* Errno);


private:
    std::vector<char> buffer_;

    char* BeginPtr_();  // 缓存起始位置
    const char* BeginPtr_() const; // 缓存起始位置
    void MakeSpace_(size_t len); // 申请空间


    std::vector<char> buffer_;

    // size_t:简单理解为 unsigned int就可以
    // todo:理解为什么需要size_t

    std::atomic<std::size_t> readPos_; // 写缓冲区 当前可读位置
    std::atomic<std::size_t> writePos_; // 读缓冲区 当前可写位置

};



#endif // BUFFER_H