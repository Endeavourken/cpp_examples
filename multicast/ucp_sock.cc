#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <cstring>
#include <errno.h>
#include <stdexcept>
#include <memory>
#include <iostream>
#include "errors.h"
#include "udp_sock.h"

int convertReq(const Address& addr, int if_index, const Address& source, std::shared_ptr<group_req>& group, std::shared_ptr<group_source_req>& group_src);
void convertIPv4Addr(const Address& addr, short port, sockaddr_in* sock_addr);
void convertIPv6Addr(const Address& addr, short port, sockaddr_in6* sock_addr);

UDPSocket::UDPSocket(const Address& addr, short port):
    Socket(addr),
    m_localPort(port)
{
    if (bindSocket() != 0)
    {
        throw std::runtime_error(strerror(errno));
    }
}

int UDPSocket::bindSocket()
{
    auto af = m_localAddr.Type() == Address::IPv4 ? AF_INET : AF_INET6;
    m_sockfd = socket(af, SOCK_DGRAM, 0);
    if (m_sockfd < 0)
    {
        std::cerr << "Failed to create UDP socket, " << strerror(errno) << std::endl;
        return -1;
    }

    if (m_localAddr.isMulticastAddress())
    {
        int on = 1, ret = 0;
        ret = ::setsockopt(m_sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
        if (ret != 0)
        {
            std::cerr << "Failed to create UDP socket, " << strerror(errno) << std::endl;
            return ret;
        }
    }
    switch (m_localAddr.Type())
    {
    case Address::IPv4:
        struct sockaddr_in inAddr;
        convertIPv4Addr(m_localAddr, m_localPort, &inAddr);
        return bind(m_sockfd, (const sockaddr*)&inAddr, sizeof(inAddr));
    case Address::IPv6:
        struct sockaddr_in6 in6Addr;
        convertIPv6Addr(m_localAddr, m_localPort, &in6Addr);
        return bind(m_sockfd, (const sockaddr*)&in6Addr, sizeof(in6Addr));
    default:
        return ERR_UNSUPPORTED_ADDR_TYPE;
    }
    return 0;
}

int UDPSocket::setLocalAddr(const Address& addr, short port)
{
    Socket::setLocalAddr(addr);
    m_localPort = port;

    return bindSocket();
}


int UDPSocket::setRemoteAddr(const Address& addr, short port)
{
    Socket::setRemoteAddr(addr);
    m_remotePort = port;
    return connect();
}

int UDPSocket::connect()
{
    int ret = 0;
    if (m_remoteAddr.Type() == Address::IPv4)
    {
        struct sockaddr_in sock_addr = {0};
        convertIPv4Addr(m_remoteAddr, m_remotePort, &sock_addr);
        ret = ::connect(m_sockfd, (const struct sockaddr*)&sock_addr, sizeof(sock_addr));
    }
    else
    {
        struct sockaddr_in6 sock_addr = {0};
        convertIPv6Addr(m_remoteAddr, m_remotePort, &sock_addr);
        ret = ::connect(m_sockfd, (const struct sockaddr*)&sock_addr, sizeof(sock_addr));
        if (ret != 0)
        {
            std::cerr << "Failed to set remote address to " << m_remoteAddr.toString() << " at port " << m_remotePort << std::endl;
            std::cerr << "error is " << strerror(errno) << std::endl;
            return ret;
        }
    }
    return 0;
}

int UDPSocket::sendTo(const char *data, size_t len, const Address& addr, short port)
{
    auto ret = setRemoteAddr(addr, port);
    if (ret != 0)
    {
        std::cerr << "Failed to set remote address due to " << strerror(errno) << std::endl;
        return ret;
    }
    return Socket::send(data, len);
}


int UDPSocket::recvFrom(char* buf, size_t len, Address& addr, short& port)
{
    struct sockaddr* remoteAddr;
    socklen_t addrLen = 0;
    if (m_localAddr.Type() == Address::IPv4)
    {
        remoteAddr =  (sockaddr*)(new sockaddr_in);
        addrLen = sizeof(sockaddr_in);
    }
    else
    {
        remoteAddr = (sockaddr*)(new sockaddr_in6);
        addrLen = sizeof(sockaddr_in6);
    }
    int ret = ::recvfrom(m_sockfd, buf, len, 0, remoteAddr, &addrLen);
    if (ret == -1)
    {
        std::cerr << "Failed to receive data, error is " << strerror(errno) << std::endl;
    }
    else
    {
        if (m_localAddr.Type() == Address::IPv4)
        {
            auto t = (sockaddr_in*)remoteAddr;
            addr = Address(Address::IPv4, static_cast<void *>(&t->sin_addr));
            port = ntohs(t->sin_port);
        }
        else
        {
            auto t = (sockaddr_in6*)remoteAddr;
            addr = Address(Address::IPv6, static_cast<void *>(&t->sin6_addr));
            port = ntohs(t->sin6_port);
        }
    }
    delete remoteAddr;
    return ret;
}

int UDPSocket::joinMultiCastGroup(const Address& addr, int if_index, short port, const Address& source)
{
    if (addr.Type() != source.Type())
    {
        std::cerr << "group address and source address do not belong to the same address class" << std::endl;
        return -1;
    }

    int ret = 0;
    if (m_sockfd == -1)
    {
        ret = setLocalAddr(addr, port);
        if (ret != 0)
        {
            std::cerr << "Failed to set local address and port, " << strerror(errno) << std::endl;
            return ret;
        }
    }
    std::shared_ptr<group_req> group_addr;
    std::shared_ptr<group_source_req> src_group_addr;
    ret = convertReq(addr, if_index, source, group_addr, src_group_addr);
    if (ret != 0)
    {
        std::cerr << "Failed to convert to group (source) request" << std::endl;
        return ret;
    }

    int level = (addr.Type() == Address::IPv4) ? IPPROTO_IP : IPPROTO_IPV6;

    if (src_group_addr)
    {
        if (0 != setsockopt(m_sockfd, level, MCAST_JOIN_SOURCE_GROUP, src_group_addr.get(), sizeof(group_source_req)))
        {
            std::cerr << "failed to join SSM group " << strerror(errno) << std::endl;
            return -1;
        }
    }
    else
    {
        if (0 != setsockopt(m_sockfd, level, MCAST_JOIN_GROUP, group_addr.get(), sizeof(group_req)))
        {
            std::cerr << "failed to join multicast group " << strerror(errno) << std::endl;
            return -1;
        }
    }

    return 0;
}

int UDPSocket::leaveMultiCastGroup(const Address& addr, int if_index, const Address& source)
{
    if (addr.Type() != source.Type())
    {
        std::cerr << "group address and source address do not belong to the same address class" << std::endl;
        return -1;
    }

    std::shared_ptr<group_req> group_addr;
    std::shared_ptr<group_source_req> src_group_addr;
    auto ret = convertReq(addr, if_index, source, group_addr, src_group_addr);
    if (ret != 0)
    {
        std::cerr << "Failed to convert to group (source) request" << std::endl;
        return ret;
    }
    int level = (addr.Type() == Address::IPv4) ? IPPROTO_IP : IPPROTO_IPV6;

    if (src_group_addr)
    {
        if (0 != setsockopt(m_sockfd, level, MCAST_LEAVE_SOURCE_GROUP, src_group_addr.get(), sizeof(group_source_req)))
        {
            std::cerr << "failed to leave SSM group " << strerror(errno) << std::endl;
            return -1;
        }
    }
    else
    {
        if (0 != setsockopt(m_sockfd, level, MCAST_LEAVE_GROUP, group_addr.get(), sizeof(group_req)))
        {
            std::cerr << "failed to leave multicast group " << strerror(errno) << std::endl;
            return -1;
        }
    }
    return 0;
}
