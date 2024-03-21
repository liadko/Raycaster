#include "headers.hpp"
#include "client.hpp"

#include <SFML/Network.hpp>
#include <boost/multiprecision/cpp_int.hpp>


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
    
    boost::multiprecision::cpp_int p("170141183460469231731687303715884105757");
    boost::multiprecision::cpp_int g("340282366920938463463374607431768211507");




    string message = "Hello!";
    int amount_sent = socket.send(message.c_str(), message.size());
}

void c()
{

    /*previous_column = SHRT_MIN;

    steven_sprite.setColor(sf::Color(255, 255, 255, static_cast<unsigned char>(round(255 * std::min<float>(1, 2 * (1 - steven_distance / RENDER_DISTANCE))))));
    steven_sprite.setPosition(round(steven_screen_x - 0.5f * steven_size), round(floor_level - 0.5f * steven_size));
    steven_sprite.setScale(steven_size / static_cast<float>(CELL_SIZE), steven_size / static_cast<float>(CELL_SIZE));
    steven_sprite.setTextureRect(sf::IntRect(static_cast<unsigned short>(CELL_SIZE * floor(shifted_direction / frame_angle)), 0, CELL_SIZE, CELL_SIZE));

    i_window.draw(steven_sprite);*/
}