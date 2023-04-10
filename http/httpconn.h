#ifndef HTTP_CONN_H
#define HTTP_CONN_H

#include <sys/types.h>
#include <sys/uio.h>     // readv/writev
#include <arpa/inet.h>   // sockaddr_in
#include <stdlib.h>      // atoi()
#include <errno.h>      

// #include "../log/log.h"
// #include "../pool/sqlconnRAII.h"
#include "../buff/buff.h"
#include "httprequest.h"
#include "httpresponse.h"

class HttpConn {
public:
    HttpConn();

    ~HttpConn();

    void init(int sockFd, const sockaddr_in& addr);

    ssize_t read(int* saveErrno);

    ssize_t write(int* saveErrno);

    void Close();

    int GetFd() const;

    int GetPort() const;

    const char* GetIP() const;
    
    sockaddr_in GetAddr() const;
    
    bool process();

    int ToWriteBytes() {    //当前缓冲区中已有数据长度
        return iov_[0].iov_len + iov_[1].iov_len; 
    }

    bool IsKeepAlive() const {
        return request_.IsKeepAlive();
    }

    static bool isET;
    static const char* srcDir;
    static std::atomic<int> userCount;
    
private:
   
    int fd_;
    struct  sockaddr_in addr_;

    bool isClose_;
    
    int iovCnt_;
    struct iovec iov_[2];
    
    buff readBuff_; // 读缓冲区:从fd中读出的数据先放入readBuff_中
    buff writeBuff_; // 写缓冲区：待写入fd的数据先写入writeBuff_中

    HttpRequest request_;
    HttpResponse response_;
};


#endif //HTTP_CONN_H

// struct sockaddr_in { 
// 　　 short int sin_family;
// 　　 unsigned short int sin_port; 
//      struct in_addr sin_addr;
/*       
        struct in_addr { 
            unsigned long s_addr;
        }
*/
                
//      unsigned char sin_zero[8];
// }   