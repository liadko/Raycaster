#include "headers.hpp"
#include "tools.hpp"
#include "player.hpp"
#include "map.hpp"

int main()
{
    //Window
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Program", sf::Style::Close, sf::ContextSettings(24, 8, 8));
    v2i screen_center(WIDTH / 2, HEIGHT / 2);

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
                cout << sf::Mouse::getPosition(window).x << " " << sf::Mouse::getPosition(window).y << "\n";
            else if (event.type == sf::Event::MouseMoved)
            {
                v2i current_pos = sf::Mouse::getPosition(window);
                
                player.rotateHead(current_pos.x - screen_center.x,
                    current_pos.y - screen_center.y, dt);

                sf::Mouse::setPosition(screen_center, window);
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
                map.sky_offset += map.sky_width * map.sky_scale;
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
                map.sky_offset -= map.sky_width * map.sky_scale;
        

        
        // cout << floor(1 / dt) << "\n";
        
        player.handleKeys(dt);


        window.clear(sf::Color::Red);
        
        map.drawSky();

        player.shootRays();
        

        //map.drawMap();

        //player.draw();

        player.drawCrosshair();
        

        window.display();
    }

    return 0;
}

