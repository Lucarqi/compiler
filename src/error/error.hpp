/*
错误信息类
*/
#pragma once
#include <exception>
#include <string>

namespace sysy::error{
//标准错误信息，调用使用catch(const BaseError &e){cerr<<e.what()}
class BaseError{
    public:
    std::string message = "Error";
    BaseError();
    virtual ~BaseError();
    virtual std::string what();
};
//变量重复定义
class RedefineVar:public BaseError{
    public:
    std::string str = "Redefine of Var";
    RedefineVar();
    virtual std::string what();
};
//变量未定义
class UndefineVar:public BaseError{
    public:
    std::string str = "Undefine of Var";
    UndefineVar();
    virtual std::string what();
};
}