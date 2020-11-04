#include <gtest/gtest.h>
#include <arpa/inet.h>
#include <cstring>
#include <string>
#include <net/if.h>
#include "udp_sock.h"

TEST(TestAddress, CreateIPv4Address) {
    std::string ip = "192.168.31.200";
    Address addr(Address::IPv4, ip);
    EXPECT_EQ(addr.Type(), Address::IPv4);
    auto raw_ip = inet_addr(ip.c_str());
    EXPECT_EQ(memcmp(addr.raw(), (char *)&raw_ip, 4), 0);
}

TEST(TestAddress, CreateIPv6Address) {
    std::string ip = "0::1";
    Address addr(Address::IPv6, ip);
    EXPECT_EQ(addr.Type(), Address::IPv6);
    char raw_ip[16] = {0};
    ASSERT_EQ(inet_pton(AF_INET6, ip.c_str(), raw_ip), 1);
    EXPECT_EQ(memcmp(addr.raw(), (char *)&raw_ip, 16), 0);
}

TEST(TestAddress, IPv6StringRepresentation) {
    std::string ip = "fe80::d185:2aba:5267:9b1";
    Address addr(Address::IPv6, ip, 2);
    EXPECT_EQ(addr.Type(), Address::IPv6);
    EXPECT_STREQ(addr.toString().c_str(), ip.c_str());
    EXPECT_EQ(addr.index(), 2);
}

TEST(TestAddress, MulticastAddress) {
    std::string ip("224.0.0.1");
    std::string ip2("ff80::d185:2aba:5267:9b1");
    Address addr(Address::IPv4, ip, 2);
    Address addr2(Address::IPv6, ip2, 2);
    EXPECT_EQ(addr.Type(), Address::IPv4);
    EXPECT_EQ(addr2.Type(), Address::IPv6);
    EXPECT_EQ(addr.isMulticastAddress(), true);
    EXPECT_EQ(addr2.isMulticastAddress(), true);
}

TEST(TestUDPSocket, ComByIPv4UDPSocket) {
    auto loop = Address(Address::IPv4, "127.0.0.1");
    UDPSocket socket1(loop, 10002);
    socket1.setRemoteAddr(loop, 10003);
    UDPSocket socket2(loop, 10003);
    socket2.setRemoteAddr(loop, 10002);
    
    std::string greeting("Hi there"), greeting_back("Hi");
    EXPECT_GT(socket1.Socket::send(greeting.c_str(), greeting.length()), 0);
    char buffer[128] = {0};
    EXPECT_GT(socket2.Socket::receive(buffer, sizeof(buffer)), 0);
    GTEST_LOG_(INFO) << "Socket 2 received greeting " << std::string(buffer) << std::endl;
    EXPECT_GT(socket2.send(greeting_back.c_str(), greeting_back.length()), 0);
    memset(buffer, 0, sizeof(buffer));
    EXPECT_GT(socket1.Socket::receive(buffer, sizeof(buffer)), 0);
    GTEST_LOG_(INFO) << "Socket 1 received greeting " << std::string(buffer) << std::endl;
    socket1.disconnect();
    socket2.disconnect();
}

TEST(TestUDPSocket, ComByIPv6UDPSocket) {
    auto loop = Address(Address::IPv6, "::1");
    UDPSocket socket1(AnyIPv6, 10002);
    UDPSocket socket2(loop, 10003);
    
    std::string greeting("Hi there"), greeting_back("Hi");
    EXPECT_GT(socket1.sendTo(greeting.c_str(), greeting.length(), loop, 10003), 0);

    char buffer[128] = {0};
    Address remoteAddr(AnyIPv6);
    short port = 0;
    EXPECT_GT(socket2.recvFrom(buffer, sizeof(buffer), remoteAddr, port), 0);
    socket1.disconnect();
    socket2.disconnect();
    GTEST_LOG_(INFO) << "Socket 2 received greeting " << std::string(buffer) << std::endl;
    GTEST_LOG_(INFO) << "Remote address is " << remoteAddr.toString() << ":" << port << std::endl;
}

TEST(TestMulticastGroup, JoinIPv4MulticastGroup) {
    UDPSocket rcvSock1;
    Address multicastAddr(Address::IPv4, std::string("224.1.0.1")); 
    EXPECT_EQ(rcvSock1.joinMultiCastGroup(multicastAddr, 2, 10002, AnyIPv4), 0);
    
    UDPSocket rcvSock2;
    EXPECT_EQ(rcvSock2.joinMultiCastGroup(multicastAddr, 2, 10002, AnyIPv4), 0);
    
    UDPSocket sndSock(Address(Address::IPv4, std::string("192.168.31.165")), 10003);
    std::string greeting("IPv4 Hi there"), greeting_back("IPv4 Hi");
    EXPECT_EQ(sndSock.setRemoteAddr(multicastAddr, 10002), 0);
    EXPECT_GT(sndSock.send(greeting.c_str(), greeting.length()), 0);
    
    char buffer[128] = {0};
    EXPECT_GT(rcvSock1.receive(buffer, sizeof(buffer)), 0);
    GTEST_LOG_(INFO) << "Rcv Socket 1 received greeting " << std::string(buffer) << std::endl;
    memset(buffer, 0, sizeof(buffer));
    EXPECT_GT(rcvSock2.receive(buffer, sizeof(buffer)), 0);
    GTEST_LOG_(INFO) << "Rcv Socket 2 also received greeting " << std::string(buffer) << std::endl;
    rcvSock1.disconnect();
    rcvSock2.disconnect();
    sndSock.disconnect();
}

TEST(TestMulticastGroup, JoinIPv6MulticastGroup) {
    UDPSocket rcvSock1;
    Address multicastAddr(Address::IPv6, std::string("ff01::1"), 2); 
    EXPECT_EQ(rcvSock1.joinMultiCastGroup(multicastAddr, 2, 10002, AnyIPv6), 0);
    
    Address source(Address::IPv6, std::string("fe80::d185:2aba:5267:9b1"));
    UDPSocket rcvSock2;
    EXPECT_EQ(rcvSock2.joinMultiCastGroup(multicastAddr, 2, 10002, source), 0);
    
    UDPSocket sndSock(Address(Address::IPv6, std::string("fe80::d185:2aba:5267:9b1"), 2), 10003);
    std::string greeting("IPv6 Hi there"), greeting_back("IPv6 Hi");
    EXPECT_EQ(sndSock.setRemoteAddr(multicastAddr, 10002), 0);
    EXPECT_GT(sndSock.send(greeting.c_str(), greeting.length()), 0);
    
    char buffer[128] = {0};
    EXPECT_GT(rcvSock1.receive(buffer, sizeof(buffer)), 0);
    GTEST_LOG_(INFO) << "Rcv Socket 1 received greeting " << std::string(buffer) << std::endl;
    memset(buffer, 0, sizeof(buffer));
    EXPECT_GT(rcvSock2.receive(buffer, sizeof(buffer)), 0);
    GTEST_LOG_(INFO) << "Rcv Socket 2 also received greeting " << std::string(buffer) << std::endl;
    EXPECT_EQ(rcvSock1.leaveMultiCastGroup(multicastAddr, 2, AnyIPv6), 0);
    EXPECT_EQ(rcvSock2.leaveMultiCastGroup(multicastAddr, 2, source), 0);
    
    rcvSock1.disconnect();
    rcvSock2.disconnect();
    sndSock.disconnect();
}