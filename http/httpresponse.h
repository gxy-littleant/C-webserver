#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

#include <unordered_map>
#include <fcntl.h>       // open
#include <unistd.h>      // close
#include <sys/stat.h>    // stat
#include <sys/mman.h>    // mmap, munmap

#include "../buff/buff.h"
// #include "../log/log.h"

class HttpResponse {
public:
    HttpResponse();
    ~HttpResponse();

    void Init(const std::string& srcDir, std::string& path, bool isKeepAlive = false, int code = -1);
    void MakeResponse(buff& buff);
    void UnmapFile();
    char* File();
    size_t FileLen() const;
    void ErrorContent(buff& buff, std::string message);
    int Code() const { return code_; }

private:
    void AddStateLine_(buff &buff);
    void AddHeader_(buff &buff);
    void AddContent_(buff &buff);

    void ErrorHtml_();
    std::string GetFileType_();

    int code_;
    bool isKeepAlive_;

    std::string path_;
    std::string srcDir_;
    
    char* mmFile_; 

    // struct stat这个结构体是用来描述一个linux系统文件系统中的文件属性的结构。
    struct stat mmFileStat_;

    static const std::unordered_map<std::string, std::string> SUFFIX_TYPE;
    static const std::unordered_map<int, std::string> CODE_STATUS;
    static const std::unordered_map<int, std::string> CODE_PATH;
};


#endif //HTTP_RESPONSE_H

// struct stat {
//         mode_t     st_mode;       //文件对应的模式，文件，目录等
//         ino_t      st_ino;       //inode节点号
//         dev_t      st_dev;        //设备号码
//         dev_t      st_rdev;       //特殊设备号码
//         nlink_t    st_nlink;      //文件的连接数
//         uid_t      st_uid;        //文件所有者
//         gid_t      st_gid;        //文件所有者对应的组
//         off_t      st_size;       //普通文件，对应的文件字节数
//         time_t     st_atime;      //文件最后被访问的时间
//         time_t     st_mtime;      //文件内容最后被修改的时间
//         time_t     st_ctime;      //文件状态改变时间
//         blksize_t st_blksize;    //文件内容对应的块大小
//         blkcnt_t   st_blocks;     //伟建内容对应的块数量
//       };