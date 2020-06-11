#ifndef CPORTSERVICENAMES_H
#define CPORTSERVICENAMES_H

#include <map>

class CPortServiceNames
{
public:
    CPortServiceNames();
    void GetServiceName(int port, char *buff, size_t buffLength, bool IsTcp);

    std::map<int, std::string>                  m_PortServiceNamesTcp;
    std::map<int, std::string>                  m_PortServiceNamesUdp;

};

#endif // CPORTSERVICENAMES_H
