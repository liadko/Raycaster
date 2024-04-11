#pragma once

#include <SFML/Network.hpp>
#include <boost/multiprecision/cpp_int.hpp>

typedef boost::multiprecision::cpp_int bigint;

using boost::multiprecision::powm;

//bool tryLogIn(const string& username, const string& password, string& error);

bool sendTCP(sf::TcpSocket& socket, const string& message, string& error);
bool recvTCP(sf::TcpSocket& socket, void*& buffer, int& buffer_size);

bool sendUDP(sf::UdpSocket& socket, const string& message, string& error);
bool recvUDP(sf::UdpSocket& socket, void*& buffer, int& buffer_size);

void printBytes(const unsigned char* pBytes, const uint32_t nBytes);

//Encryption
string encryptAES(const std::string& plaintext, unsigned char* key, string& error);
string decryptAES(const string& ciphertext, unsigned char* key, string& error);

//string bigintToHexString(const bigint& number);
void bigintToBytes(bigint key, unsigned char* buffer);

class Client
{
private:
    sf::TcpSocket tcp_socket;
    sf::UdpSocket udp_socket;
    sf::IpAddress udp_address;
    string ip;
    int port;

    bigint p, g, secret;
    unsigned char key_bytes[16];


public:
    int player_id;

    Client();


    bool connectToServer(const string& ip, int port, string& error);

    bool tryLogIn(const string& username, const string& password, string& error);
    
    bool sendEncryptedTCP(const string& msg, string& error);
    bool recvEncryptedTCP(void*& buffer, int& bufferSize, string& error);
    static bool recvTCP(sf::TcpSocket& socket, void*& buffer, int& buffer_size);
    static bool sendTCP(sf::TcpSocket& socket, const string& message, string& error);

    bool sendEncryptedUDP(void* buffer, int size, string& error);
    bool recvEncryptedUDP(void*& buffer, int& bufferSize, string& error);
    bool sendUDP(sf::UdpSocket& socket, const string& message, string& error);
    bool connected = false;
};
