#include "error/error.hpp"

namespace sysy::error{
//BaseError
BaseError::BaseError(){}
BaseError::~BaseError(){}
std::string BaseError::what()
{
    return this->message;
}
//ReDefineVar
RedefineVar::RedefineVar(){
    message = str;
}
std::string RedefineVar::what()
{
    return this->message;
}
//UnDefineVar
UndefineVar::UndefineVar()
{
    message = str;
}
std::string UndefineVar::what()
{
    return this->message;
}
//Undefine of Function
UndefineFunc::UndefineFunc()
{
    message = str;
}
std::string UndefineFunc::what()
{
    return this->message;
}
}