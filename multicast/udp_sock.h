#ifndef _UDP_SOCK_H_
#define _UDP_SOCK_H_

#include "socket.h"
class UDPSocket: public Socket
{
public:
    UDPSocket() = default;
    UDPSocket(const Address& addr, short port);
    virtual int connect();

    int setLocalAddr(const Address& addr, short port);
    int setRemoteAddr(const Address& addr, short port);
    int sendTo(const char *data, size_t len, const Address& addr, short port);
    int recvFrom(char* buf, size_t len, Address& addr, short& port); //this function returns the remote address as `addr` parameter
    int joinMultiCastGroup(const Address& addr, int if_index, short port, const Address& source);
    int leaveMultiCastGroup(const Address& addr, int if_index, const Address& source);
private:
    int bindSocket();//bind socket to local address and port
    short m_localPort;
    short m_remotePort;
};

#endif
