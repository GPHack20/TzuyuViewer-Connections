#include "cportservicenames.h"

#include <netdb.h>
#include <cstring>

CPortServiceNames::CPortServiceNames()
{
    // *Thread safety MT-Unsafe
    //
    struct servent *sentry;
    setservent(0);
    while ( (sentry = getservent()) != nullptr )
    {
        if(std::strcmp(sentry->s_proto,"tcp") == 0)
            m_PortServiceNamesTcp.insert(std::pair<int, std::string>(ntohs(sentry->s_port), std::string(sentry->s_name)));
        else if(std::strcmp(sentry->s_proto,"udp") == 0)
            m_PortServiceNamesUdp.insert(std::pair<int, std::string>(ntohs(sentry->s_port), std::string(sentry->s_name)));
    }
    endservent();

}

void CPortServiceNames::GetServiceName(int port, char *buff, size_t buffLength, bool IsTcp)
{

    auto pNames = IsTcp ? &m_PortServiceNamesTcp : &m_PortServiceNamesUdp;

    auto it = pNames->find(port);
    if (it == pNames->end())
        std::snprintf(buff, buffLength, "%d", port);
    else
        std::snprintf(buff, buffLength, "%d (%s)", port, it->second.c_str());
}
