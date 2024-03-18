#include "headers.hpp"
#include "client.hpp"

#include <SFML/Network.hpp>


void sendUDP()
{
    sf::UdpSocket socket;

    // Bind the socket to any available port
    if (socket.bind(sf::Socket::AnyPort) != sf::Socket::Done)
    {
        std::cerr << "Error binding UDP socket" << std::endl;
        return;
    }

    sf::IpAddress recipientIpAddress("127.0.0.1");

    std::string data = "Gunshot!";

    // Send the data to the recipient
    if (socket.send(data.c_str(), data.size(), recipientIpAddress, 4321) != sf::Socket::Done)
    {
        std::cerr << "Error sending UDP packet" << std::endl;
        return;
    }

    std::cout << "Sent UDP packet to " << "127.0.0.1" << ":" << 4321 << std::endl;

}


bool tryLogIn(const string& username, const string& password)
{
    sf::TcpSocket socket;
    
    // Connect to server
    if (socket.connect("127.0.0.1", 21567) != sf::Socket::Done)
    {
        std::cerr << "Error binding Tcp socket" << std::endl;
        return true;
    }
    


    string message = "Hello!";
    int amount_sent = socket.send(message.c_str(), message.size());
}

