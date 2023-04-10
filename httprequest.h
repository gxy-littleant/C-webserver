#ifdef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include<unordered_map>
#include<string>
#include<regex>
#include<errno.h>

class HttpRequest{

public:
    enum PARSE_STATE{REQUEST_LINE  = 0, //待解析整行
        HEADER,             
        BODY,
        FINISH};

    enum HTTP_CODE{NO_REQUEST = 0,
        GET_REQUEST,
        BAD_REQUEST,
        NO_RESOURSE,
        FORBIDDENT_REQUEST,
        FILE_REQUEST,
        INTERNAL_ERROR,
        CLOSED_CONNECTION,};


    // todo:
    HttpRequest(){};
    // 将该函数声明为显示默认构造函数。
    // 这就使得编译器为显示默认函数生成了默认实现，它比手动编程函数更加有效。
    ~HttpRequest()=default;

    bool parse(Buffer& buff);

    


}

#endif //HTTP_REQUEST_H