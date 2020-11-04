#include <stdexcept>
#include <iostream>
#include <memory>
#include <cstring>
#include <errno.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include "socket.h"

const Address AnyIPv4 = Address(Address::IPv4, "0.0.0.0");
const Address AnyIPv6 = Address(Address::IPv6, "0::0");

Address::Address(AddressType type, void* rawAddr, int if_index):
    m_type(type),
    m_index(if_index)
{
    memset(m_rawAddr, 0, sizeof(m_rawAddr));
    if (rawAddr == nullptr)
    {
        return;
    }
    if (m_type == IPv4)
    {
        memcpy(m_rawAddr, rawAddr, 4);
    }
    else
    {
        memcpy(m_rawAddr, rawAddr, 16);
    }
}


Address::Address(AddressType type, const std::string& addrString, int if_index):
    m_type(type),
    m_index(if_index)
{
    if (type != IPv4 && type != IPv6)
        throw std::invalid_argument("Address type only suports IPv4 and IPv6");

    memset(m_rawAddr, 0, sizeof(m_rawAddr));
    if (addrString == "")
        return;

    int ret = 0;
    if (type == IPv4)
        ret =inet_pton(AF_INET, addrString.c_str(), &m_rawAddr[0]);
    else
        ret = inet_pton(AF_INET6, addrString.c_str(), &m_rawAddr[0]);
    if (ret != 1)
    {
        throw std::invalid_argument(strerror(errno));
    }
}

std::string Address::toString() const
{
    size_t len;
    if (m_type == IPv4)
        len = INET_ADDRSTRLEN;
    else
        len = INET6_ADDRSTRLEN;
    std::shared_ptr<char> str(new char[len], [](char* p)
    {
        delete []p;
    });
    memset(str.get(), 0, len);
    auto af = m_type == Address::IPv4 ? AF_INET : AF_INET6;
    if (nullptr == inet_ntop(af, &m_rawAddr[0], str.get(), len))
    {
        std::cerr << "Invalid address" << std::endl;
        return "";
    }
    std::string addr(str.get());
    return addr;
}

bool Address::operator==(const Address& addr) const
{
    if (this->m_type != addr.m_type)
    {
        return false;
    }
    size_t len = this->m_type == IPv4 ? 4 : 16;
    if (0 != memcmp(this->m_rawAddr, addr.m_rawAddr, len))
    {
        return false;
    }

    if (this->m_index != addr.m_index)
    {
        return false;
    }
    return true;
}

bool Address::isMulticastAddress() const
{
    if (m_type == IPv4)
    {
        if (m_rawAddr[0] >= 0xe0 && m_rawAddr[0] <= 0xef)
        {
            return true;
        }
    }
    if (m_type == IPv6)
    {
        if (m_rawAddr[0] == 0xff)
        {
            return true;
        }
    }
    return false;
}

//most simplistic wrapping of `send` system call
int Socket::send(const char *data, size_t len)
{
    return ::send(m_sockfd, data, len, 0);
}

//most simplistic wrapping of `receive` system call
int Socket::receive(char* buf, size_t len)
{
    return ::recv(m_sockfd, buf, len, 0);
}

int Socket::disconnect()
{
    return close(m_sockfd);
}
