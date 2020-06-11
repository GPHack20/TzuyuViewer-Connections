#include "cusername.h"

#include <pwd.h>

CUserName::CUserName()
{

}

QString CUserName::GetUserNameString(__uid_t euid, bool bErrorUser)
{

    if(bErrorUser)
        return QString("- error userid -");

    struct passwd *ptr = getpwuid(euid);
    if(ptr)
        return QString(ptr->pw_name);

    return QString::number(euid);

}
