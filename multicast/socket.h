#ifndef _SOCKET_H_
#define _SOCKET_H_
//socket C++ wrapper for ease of use, which hides the difference between IPv4 and IPv6
#include <string>

class Address
{
public:
    enum AddressType
    {
        IPv4 = 1,
        IPv6 = 2
    };

    //constructor
    Address(AddressType type, const std::string& addrString, int if_index = 0);
    Address(AddressType type, void* rawAddr, int if_index = 0);

    Address(const Address& addr) = default;
    AddressType Type() const
    {
        return m_type;
    }
    std::string toString() const;
    const unsigned char* raw() const
    {
        return &m_rawAddr[0];
    }
    int index() const
    {
        return m_index;
    }
    bool operator==(const Address& addr) const;
    bool operator!=(const Address& addr) const
    {
        return !this->operator==(addr);
    }
    bool isMulticastAddress() const;

private:
    AddressType m_type;
    unsigned char m_rawAddr[16];//IPv4 takes 4 bytes while IPv6 takes 16 bytes
    int m_index;//interface index, default to 0
};

extern const Address AnyIPv4;
extern const Address AnyIPv6;

enum SocketType
{
    TCP = 1,
    UDP = 2
};

//this class is targeting IPv4 and IPv6 TCP/UDP socket
class Socket
{
public:
    Socket():
        m_localAddr(AnyIPv4),
        m_remoteAddr(AnyIPv4),
        m_sockfd(-1)
    {}
    Socket(const Address& local, const Address& remote):
        m_localAddr(local),
        m_remoteAddr(remote)
    {}

    Socket(const Address& local):
        m_localAddr(local),
        m_remoteAddr(Address(local.Type(), nullptr))
    {}
    virtual int connect() = 0;
    virtual int disconnect();
    int connect(const Address& remoteAddr)
    {
        setRemoteAddr(remoteAddr);
        return connect();
    }
    virtual int send(const char *data, size_t len);
    virtual int receive(char* buf, size_t len);

    void setLocalAddr(const Address& addr)
    {
        m_localAddr = addr;
    }
    Address getLocalAddr() const
    {
        return m_localAddr;
    }
    void setRemoteAddr(const Address& addr)
    {
        m_remoteAddr = addr;
    }
    Address getRemoteAddr() const
    {
        return m_remoteAddr;
    }

protected:
    Address m_localAddr;
    Address m_remoteAddr;
    int m_sockfd;
};

#endif
