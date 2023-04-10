#include "buff.h"

buff::buff(int initBuffSize) : buffer_(initBuffSize), readPos_(0), writePos_(0) {}

size_t buff::ReadableBytes() const {
    return writePos_ - readPos_;
}
size_t buff::WritableBytes() const {
    return buffer_.size() - writePos_;
}

size_t buff::PrependableBytes() const {
    return readPos_;
}

const char* buff::Peek() const {
    return BeginPtr_() + readPos_;
}

void buff::Retrieve(size_t len) {
    assert(len <= ReadableBytes());
    readPos_ += len;
}

void buff::RetrieveUntil(const char* end) {
    assert(Peek() <= end );
    Retrieve(end - Peek());
}

void buff::RetrieveAll() {
    bzero(&buffer_[0], buffer_.size());
    readPos_ = 0;
    writePos_ = 0;
}

std::string buff::RetrieveAllToStr() {
    std::string str(Peek(), ReadableBytes());
    RetrieveAll();
    return str;
}

const char* buff::BeginWriteConst() const {
    return BeginPtr_() + writePos_;
}

char* buff::BeginWrite() {
    return BeginPtr_() + writePos_;
}

void buff::HasWritten(size_t len) {
    writePos_ += len;
}

void buff::Append(const std::string& str) {
    // string.c_str():生成一个const char*指针，指向以空字符终止的数组。
    // data():与c_str()类似，但是返回的数组不以空字符终止。
    Append(str.data(), str.length());
}

void buff::Append(const void* data, size_t len) {
    assert(data);
    // void* 转换成 char* 
    Append(static_cast<const char*>(data), len);
}

void buff::Append(const char* str, size_t len) {
    assert(str);
    EnsureWriteable(len);
    // 把当前字符串复制到写缓冲区 位置由writePos_指定
    std::copy(str, str + len, BeginWrite());
    HasWritten(len);
}

void buff::Append(const buff& buff) {
    Append(buff.Peek(), buff.ReadableBytes());
}

void buff::EnsureWriteable(size_t len) {
    if(WritableBytes() < len) {
        MakeSpace_(len);
    }
    assert(WritableBytes() >= len);
}

ssize_t buff::ReadFd(int fd, int* saveErrno) {
    char buff[65535];
    struct iovec iov[2];
    const size_t writable = WritableBytes();

    /* 分散读， 保证数据全部读完 */
    // iov[0]:存放到readbuffer中
    // iov[1]:存放到上述定义的buff中
    iov[0].iov_base = BeginPtr_() + writePos_; // 读入内存中
    iov[0].iov_len = writable;  // 读入数据长度
    iov[1].iov_base = buff; 
    iov[1].iov_len = sizeof(buff); 

    // readv返回读到的字节总数。如果遇到文件尾端，已无数据可读，则返回0。
    // 功能：将数据从文件描述符读到分散的内存块中，即分散读
    // ssize_t readv(int fd, const struct iovec *iov, int iovcnt);
    // iovcnt参数是vector数组的长度，即有多少块内存数据需要从fd读出或写到fd。
    // readv() 系统调用从关联的文件读取iovcnt缓冲区文件描述符fd
	// 放入iov描述的缓冲区（“分散输入”）

    // 通过writev函数可以将分散保存在多个buff的数据一并进行发送，
    // 通过readv可以由多个buff分别接受数据，适当的使用这两个函数可以减少I/O函数的调用次数：

    // readv ()代表分散读， 即将数据从文件描述符读到分散的内存块(iov中)中；
    const ssize_t len = readv(fd, iov, 2);
    if(len < 0) {
        *saveErrno = errno;
    }
    else if(static_cast<size_t>(len) <= writable) {
        writePos_ += len;
    }
    else {
        writePos_ = buffer_.size();
        Append(buff, len - writable); 
    }
    return len;
}

ssize_t buff::WriteFd(int fd, int* saveErrno) {
    size_t readSize = ReadableBytes();
    // 将读缓冲区写入到fd中
    ssize_t len = write(fd, Peek(), readSize);
    if(len < 0) {
        *saveErrno = errno;
        return len;
    } 
    readPos_ += len;
    return len;
}

char* buff::BeginPtr_() {
    return &*buffer_.begin();
}

const char* buff::BeginPtr_() const {
    return &*buffer_.begin();
}

void buff::MakeSpace_(size_t len) {
    if(WritableBytes() + PrependableBytes() < len) {
        buffer_.resize(writePos_ + len + 1);
    } 
    else {
        size_t readable = ReadableBytes();
        std::copy(BeginPtr_() + readPos_, BeginPtr_() + writePos_, BeginPtr_());
        readPos_ = 0;
        writePos_ = readPos_ + readable;
        assert(readable == ReadableBytes());
    }
}

