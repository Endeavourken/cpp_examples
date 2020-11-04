#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <memory>
#include <cstring>
#include <iostream>
#include "errors.h"
#include "socket.h"

void convertAddress(const Address& addr, sockaddr_storage* storage)
{
    if (addr.Type() == Address::IPv4)
    {
        struct sockaddr_in sock_addr;
        memset(&sock_addr, 0, sizeof(sock_addr));
        sock_addr.sin_family = AF_INET;
        memcpy(&sock_addr.sin_addr, addr.raw(), sizeof(sock_addr.sin_addr));
        memcpy(storage, &sock_addr, sizeof(sock_addr));
    }
    else if (addr.Type() == Address::IPv6)
    {
        struct sockaddr_in6 sock_addr;
        memset(&sock_addr, 0, sizeof(sock_addr));
        sock_addr.sin6_family = AF_INET6;
        sock_addr.sin6_scope_id = addr.index();
        memcpy(&sock_addr.sin6_addr, addr.raw(), sizeof(sock_addr.sin6_addr));
        memcpy(storage, &sock_addr, sizeof(sock_addr));
    }
    else
    {
        throw std::runtime_error("Unsupported address type");
    }
}

void convertIPv4Addr(const Address& addr, short port, sockaddr_in* sock_addr)
{
    if (addr.Type() != Address::IPv4)
    {
        throw std::runtime_error("Address is not of family IPv4");
    }

    memset(sock_addr, 0, sizeof(sockaddr_in));
    sock_addr->sin_family = AF_INET;
    if (addr == AnyIPv4)
    {
        sock_addr->sin_addr.s_addr = INADDR_ANY;
    }
    else
    {
        memcpy(&sock_addr->sin_addr, addr.raw(), sizeof(sock_addr->sin_addr));
    }
    sock_addr->sin_port = htons(port);
}

void convertIPv6Addr(const Address& addr, short port, sockaddr_in6* sock_addr)
{
    if (addr.Type() != Address::IPv6)
    {
        throw std::runtime_error("Address is not of family IPv6");
    }
    memset(sock_addr, 0, sizeof(sockaddr_in6));
    sock_addr->sin6_family = AF_INET6;
    sock_addr->sin6_scope_id = addr.index();
    if (addr == AnyIPv6)
    {
        sock_addr->sin6_addr = in6addr_any;
    }
    else
    {
        memcpy(&sock_addr->sin6_addr, addr.raw(), sizeof(sock_addr->sin6_addr));
    }
    sock_addr->sin6_port = htons(port);
}

std::shared_ptr<group_source_req> convertGroupSrcReq(const Address& addr, int if_index, const Address& source)
{
    std::shared_ptr<group_source_req> src_group_addr;
    switch (addr.Type())
    {
    case Address::IPv4:
        src_group_addr.reset(new group_source_req);
        memset(&src_group_addr->gsr_source, 0, sizeof(src_group_addr->gsr_source));
        convertAddress(source, &src_group_addr->gsr_source);
        src_group_addr->gsr_interface = if_index;
        memset(&src_group_addr->gsr_group, 0, sizeof(src_group_addr->gsr_group));
        convertAddress(addr, &src_group_addr->gsr_group);
        break;
    case Address::IPv6:
        src_group_addr.reset(new group_source_req);
        memset(&src_group_addr->gsr_source, 0, sizeof(src_group_addr->gsr_source));
        convertAddress(source, &src_group_addr->gsr_source);
        src_group_addr->gsr_interface = if_index;
        memset(&src_group_addr->gsr_group, 0, sizeof(src_group_addr->gsr_group));
        convertAddress(addr, &src_group_addr->gsr_group);
        break;
    default:
        std::cerr << "Unsupported address type" << std::endl;
        return nullptr;
    }
    return src_group_addr;
}

std::shared_ptr<group_req> convertGroupReq(const Address& addr, int if_index)
{
    std::shared_ptr<group_req> group_addr;
    switch (addr.Type())
    {
    case Address::IPv4:
        group_addr.reset(new group_req);
        group_addr->gr_interface = if_index;
        memset(&group_addr->gr_group, 0, sizeof(group_addr->gr_group));
        convertAddress(addr, &group_addr->gr_group);
        break;
    case Address::IPv6:
        group_addr.reset(new group_req);
        group_addr->gr_interface = if_index;
        memset(&group_addr->gr_group, 0, sizeof(group_addr->gr_group));
        convertAddress(addr, &group_addr->gr_group);
        break;
    default:
        std::cerr << "Unsupported address type" << std::endl;
        return nullptr;
    }
    return group_addr;
}

int convertReq(const Address& addr, int if_index, const Address& source, std::shared_ptr<group_req>& group, std::shared_ptr<group_source_req>& group_src)
{
    switch (addr.Type())
    {
    case Address::IPv4:
        if (source != AnyIPv4)
        {
            group_src = convertGroupSrcReq(addr, if_index, source);
        }
        else
        {
            group = convertGroupReq(addr, if_index);
        }
        break;
    case Address::IPv6:
        if (source != AnyIPv6)
        {
            group_src = convertGroupSrcReq(addr, if_index, source);
        }
        else
        {
            group = convertGroupReq(addr, if_index);
        }
        break;
    default:
        std::cerr << "Unsupported address type" << std::endl;
        return ERR_UNSUPPORTED_ADDR_TYPE;
    }
    return 0;
}
