#include "headers.hpp"
#include "client.hpp"



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


bool tryLogIn(const string& username, const string& password, string& error)
{
    if (username.size() <= 5)
    {
        error = "Username Must Be Longer Than 5 Characters";
        return false;
    }
    if (password.size() <= 5)
    {
        error = "Password Must Be Longer Than 5 Characters";
        return false;
    }


    sf::TcpSocket socket;
    
    // Connect to server
    if (socket.connect("127.0.0.1", 21567) != sf::Socket::Done)
    {
        error = "Failed To Connect To Server";
        return false;
    }
    
    bigint p("170141183460469231731687303715884105757");
    bigint g("340282366920938463463374607431768211507");
    bigint secret("5234");

    bigint x1 = powm(g, secret, p);


    string message = "X1:" + x1.str() + ":X1";
    int amount_sent = socket.send(message.c_str(), message.size());



    //bigint key = powm(x2, secret, p);

    return true;
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