#include "headers.hpp"
#include "graphics.hpp"
#include "player.hpp"
#include "map.hpp"

int main()
{
    //Window
    const int width = 1280, height = 720;
    sf::RenderWindow window(sf::VideoMode(width, height), "Program", sf::Style::Close, sf::ContextSettings(24, 8, 8));

    sf::Clock clock;


    Map map(50, height - 600 - 50, 10, 10, window);
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


        window.clear();
        
        
        map.drawMap();
        player.draw();
        handleKeys(player, dt);
        

        window.display();
    }

    return 0;
}

