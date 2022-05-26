/*
错误信息类
*/
#pragma once
#include <exception>
#include <string>
#include <string_view>

namespace sysy::error{
//标准错误信息，调用使用catch(const BaseError &e){cerr<<e.what()}
class BaseError{
    protected:
    std::string message;

    public:
    BaseError(string_view str="A Problem"): message{str}{};
    virtual ~BaseError()=default;
    virtual string_view what() const {return message};
};
//变量重复定义
class RedefineVar:public BaseError{
    public:
    RedefineVar(string_view str="Duplicate definitions of Var"): message{str}{};
};
//变量未定义
class UndefineVar:public BaseError{
    public:
    UndefineVar(string_view str="Undefine of a Var"):message{str}{};
};
}