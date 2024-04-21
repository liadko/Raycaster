#include "headers.hpp"
#include "tools.hpp"
#include "player.hpp"
#include "map.hpp"
#include "object.hpp"
#include "client.hpp"

#include <chrono>

void loginPage(sf::RenderWindow& window, Player& player);
void mainLoop(sf::RenderWindow& window, Player& player);



int main()
{

    //cout << "Start.\n";

    //sf::UdpSocket udp1, udp2;
    //if (udp1.bind(sf::Socket::AnyPort) != sf::Socket::Done)
    //    cout << "Problem Binding 1\n";

    //if (udp2.bind(sf::Socket::AnyPort) != sf::Socket::Done)
    //    cout << "Problem Binding 2\n";


    //cout << udp2.getLocalPort() << "\n";

    //string message = "Penis";

    //udp1.send(message.c_str(), message.size(), "87.71.155.68", 21568);

    //void* buffer = malloc(128);
    //size_t buffer_size = 128;
    //size_t received;
    //sf::IpAddress address("87.71.155.68");
    //unsigned short port = 21568;
    //udp2.receive(buffer, buffer_size, received, address, port);
   
    //cout << "End.\n";
    //std::cin.get();

    //return 0;

    //Window
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Program", sf::Style::Close, sf::ContextSettings(24, 8, 8));

    window.setFramerateLimit(60);


    Player player(40, 21, window);


    loginPage(window, player);


    // Game loop
    mainLoop(window, player);

    return 0;
}


void loginPage(sf::RenderWindow& window, Player& player)
{
    // background image
    sf::Texture bg_tex;
    bg_tex.loadFromFile("sprites/BG.jpg");
    sf::Sprite bg_sprite(bg_tex);
    //bg_sprite.setTexture(bg_tex.copyToImage());
    //bg_tex.copyToImage();
    //bg_tex.~Texture();

    // font
    sf::Font input_font;
    if (!input_font.loadFromFile("Fonts/Roboto-Regular.ttf"))
    {
        std::cerr << "Error Loading File.\n";
        return;
    }


    // text box and text

    TextBox username(v2f(194, 271), v2f(461, 70), "liadkoren123", input_font);
    TextBox password(v2f(194, 377), v2f(461, 70), "password432", input_font);

    password.hidden = true;

    TextBox* text_boxes[3] = { nullptr, &username, &password };


    int box_focused = 1;



    v2f button_position(194, 492), button_size(460, 60);

    while (window.isOpen())
    {

        string typed_text = "";
        int backspace_counter = 0;


        sf::Event event;
        while (window.pollEvent(event))
            if (event.type == sf::Event::Closed)
                window.close();
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                window.close();
            else if (event.type == sf::Event::TextEntered) {
                //cout << event.text.unicode << "\n";
                // actual typing
                if (event.text.unicode > 32 && event.text.unicode < 127) {
                    typed_text += event.text.unicode;
                }

                // backspaces
                if (event.text.unicode == '\b')
                    backspace_counter++;
                if (event.text.unicode == 127) // ctrl backspace
                    backspace_counter = -100;

                // tab
                if (event.text.unicode == '\t' && box_focused)
                {
                    box_focused = (box_focused + 1) % 3;
                    if (box_focused == 0)
                        box_focused = 1;

                    text_boxes[box_focused]->turnOnCursor();
                }

                //enter
                if (event.text.unicode == '\r')
                {
                    string error;
                    if (player.client.tryLogIn(username.getString(), password.getString(), error))
                        return;
                    cout << error << '\n';
                }

            }
            else if (event.type == sf::Event::MouseButtonPressed) {
                // Check if mouse click is within the text box
                v2i mousePos = sf::Mouse::getPosition(window);

                box_focused = 0;
                for (int i = 1; i < 3; i++)
                {
                    if (text_boxes[i]->inBox(mousePos))
                    {
                        box_focused = i;
                        text_boxes[i]->turnOnCursor();
                    }

                }


                if (inBounds(button_position, button_size, mousePos))
                {
                    string error;
                    if (player.client.tryLogIn(username.getString(), password.getString(), error))
                        return;
                    cout << error << '\n';
                }

            }





        window.clear(sf::Color::Red);

        window.draw(bg_sprite);



        for (int i = 1; i < 3; i++)
            text_boxes[i]->draw(window, i == box_focused);


        //box highlight
        if (box_focused)
        {
            if (typed_text.size())
                text_boxes[box_focused]->addText(typed_text);
            if (backspace_counter)
                text_boxes[box_focused]->backspace(backspace_counter);
        }

        window.display();

    }
}

void mainLoop(sf::RenderWindow& window, Player& player)
{
    v2i screen_center(WIDTH / 2, HEIGHT / 2);


    int frame_count = 0;
    sf::Clock clock;



    Player::HitInfo* hits = new Player::HitInfo[WIDTH];

    std::thread udpThread(&Player::listenToServer, &player);


    player.setFocus(true);

    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event))
            if (event.type == sf::Event::Closed)
                player.quitGame();
            else if (player.window_focused && event.type == sf::Event::MouseButtonPressed)
                player.shootGun(event.mouseButton.button == sf::Mouse::Left);
            else if (event.type == sf::Event::LostFocus)
                player.setFocus(false);
            else if (event.type == sf::Event::GainedFocus)
                player.setFocus(true);
            else if (player.window_focused && event.type == sf::Event::MouseMoved)
            {
                v2i current_pos = sf::Mouse::getPosition(window);

                player.rotateHead(current_pos.x - screen_center.x,
                    current_pos.y - screen_center.y, dt);

                sf::Mouse::setPosition(screen_center, window);
            }
            else if (event.type == sf::Event::KeyReleased)
            {
                if (event.key.code == sf::Keyboard::Space)
                    player.debug();
            }


        if(frame_count % 100 == 0)
            cout << (1/dt) << "\n";


        cout << "Frame: " << frame_count << '\n';

        player.updateServer();


        player.handleKeys(dt);


        // Graphics
        window.clear(sf::Color::Red);

        player.map.drawSky(); // Sky

        player.map.drawGround();


        player.shootRays(hits); // populate hits[]

        // World

        {
            auto start = std::chrono::high_resolution_clock::now();
            std::lock_guard<std::mutex> lock(player.mtx);
            auto end = std::chrono::high_resolution_clock::now();

            player.drawWorld(hits, dt);
            
            std::chrono::duration<double> elapsed = end - start;

            //std::cout << "Elapsed time: " << elapsed.count() * 1000 << " ms" << std::endl;
        }



        //player.debug();

        player.drawGun(dt); // Gun

        player.drawCrosshair(); // Crosshair


        window.display(); // Render to screen

        frame_count++;
    }



    delete[] hits;
}
