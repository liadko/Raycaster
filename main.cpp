#include "headers.hpp"
#include "tools.hpp"
#include "player.hpp"
#include "map.hpp"

int main()
{
    //Window
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Program", sf::Style::Close, sf::ContextSettings(24, 8, 8));
    v2i screen_center(WIDTH / 2, HEIGHT / 2);

    int frame_count = 0;
    sf::Clock clock;

    Map map(20, window);
    Player player(40, 21, map, window);

    v2i prev_mouse_position = sf::Mouse::getPosition(window);
    window.setMouseCursorVisible(false);

    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event))
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::MouseButtonPressed)
                player.shootGun();
            else if (event.type == sf::Event::MouseMoved)
            {
                v2i current_pos = sf::Mouse::getPosition(window);
                
                player.rotateHead(current_pos.x - screen_center.x,
                    current_pos.y - screen_center.y, dt);

                sf::Mouse::setPosition(screen_center, window);
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
                map.sky_sensitivity += 100;
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
                map.sky_sensitivity -= 100;
        

        //if(frame_count % 1000 == 0)
        //    cout << floor(1 / dt) << "\n";

        //cout << map.sky_sensitivity << '\n';

        player.handleKeys(dt);


        // Graphics
        window.clear(sf::Color::Red);
        
        map.drawSky();

        player.shootRays();
        

        //map.drawMap(); 

        player.drawGun(dt);

        player.drawCrosshair();
        

        window.display();

        frame_count++;
    }

    return 0;
}

