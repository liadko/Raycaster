#include "headers.hpp"
#include "graphics.hpp"
#include "player.hpp"
#include "map.hpp"

int main()
{
    //Window
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Program", sf::Style::Close, sf::ContextSettings(24, 8, 8));

    sf::Clock clock;


    Map map(50, HEIGHT - 240 - 50, 10, 10, window);
    Player player(2, 2, map, window);

    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event))
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::MouseButtonPressed)
                cout << sf::Mouse::getPosition(window).x << " " << sf::Mouse::getPosition(window).y << "\n";

        
        cout << floor(1 / dt) << "\n";

        handleKeys(player, dt);


        window.clear(sf::Color(70, 170, 255));
        
        
        map.drawGround();

        player.shootRays();
        

        map.drawMap();
        player.draw();

        
        player.drawCrosshair();

        window.display();
    }

    return 0;
}

