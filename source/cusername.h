#ifndef CUSERNAME_H
#define CUSERNAME_H

#include <QString>

class CUserName
{
public:
    CUserName();
    static QString GetUserNameString(__uid_t euid, bool bErrorUser);
};

#endif // CUSERNAME_H
