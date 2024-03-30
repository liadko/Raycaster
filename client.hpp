#pragma once

#include <SFML/Network.hpp>
#include <boost/multiprecision/cpp_int.hpp>

typedef boost::multiprecision::cpp_int bigint;

using boost::multiprecision::powm;

bool tryLogIn(const string& username, const string& password, string& error);

void sendTCP(sf::TcpSocket& socket, const string& message);
bool recvTCP(sf::TcpSocket& socket, void*& payload, int& payload_size);

void sendUDP();

void PrintBytes(const unsigned char* pBytes, const uint32_t nBytes);

//Encryption
string encryptAES(const std::string& plaintext, unsigned char* key);
string decryptAES(const string& ciphertext, unsigned char* key);

//string bigintToHexString(const bigint& number);
unsigned char* bigintToBytes(bigint key);