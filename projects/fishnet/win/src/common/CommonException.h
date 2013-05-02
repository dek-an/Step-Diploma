#ifndef COMMON_COMMONEXCEPTION_H_
#define COMMON_COMMONEXCEPTION_H_

#include <exception>
#include <string>

namespace common
{

class CommonException
    : public std::exception
{
public:
    CommonException(const std::string& strMessage = "")
        : m_strMessage(strMessage)
    {}

    virtual ~CommonException() {}

public:
    virtual const char* what() const { return m_strMessage.c_str(); }

private:
    std::string m_strMessage;
};

} // namespace common

#endif // COMMON_COMMONEXCEPTION_H_