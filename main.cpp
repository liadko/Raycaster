#include "headers.hpp"
#include "tools.hpp"
#include "player.hpp"
#include "map.hpp"
#include "object.hpp"
#include "client.hpp"


void loginPage(sf::RenderWindow& window, Map& map, Player& player);
void mainLoop(sf::RenderWindow& window, Map& map, Player& player);




int main()
{
    //Window
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Program", sf::Style::Close, sf::ContextSettings(24, 8, 8));
  
    Map map(20, window);
    
    Player player(40, 21, map, window);

    
    //loginPage(window, map, player);

    window.setMouseCursorVisible(false);
    
    // Game loop
    mainLoop(window, map, player);

    return 0;
}


void loginPage(sf::RenderWindow& window, Map& map, Player& player)
{
    // background image
    sf::Texture bg_tex;
    bg_tex.loadFromFile("Ps/BG.jpg");
    sf::Sprite bg_sprite(bg_tex);

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
                cout << event.text.unicode << "\n";
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
                    if(tryLogIn(username.getString(), password.getString()))
                        return;
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
                    if (tryLogIn(username.getString(), password.getString()))
                        return;
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

void mainLoop(sf::RenderWindow& window, Map& map, Player& player)
{
    v2i screen_center(WIDTH / 2, HEIGHT / 2);


    int frame_count = 0;
    sf::Clock clock;

    Player::HitInfo* hits = new Player::HitInfo[WIDTH];


    sf::Texture* textures = new sf::Texture[3];
    textures[0].loadFromFile("sprites/cop.png");
    textures[1].loadFromFile("sprites/missing_texture.png");
    float scalers[3] = { 0.00135f, 0.002f, 0.5f };

    vector<Object> objects;
    objects.emplace_back(32.5f, 19.2f, textures[0], scalers[0]);
    objects.emplace_back(3, 3, textures[1], scalers[1]);
    objects.emplace_back(10, 4, textures[0], scalers[0]);



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
            else if (event.type == sf::Event::KeyReleased)
            {
                if (event.key.code == sf::Keyboard::Space)
                    player.debug();
            }


        //if(frame_count % 1000 == 0)
        //    cout << floor(1 / dt) << "\n";

        //cout << map.sky_sensitivity << '\n';

        player.handleKeys(dt);


        // Graphics
        window.clear(sf::Color::Red);

        map.drawSky(); // Sky

        map.drawGround();

        
        player.shootRays(hits); // populate hits[]

        // World
        player.drawWorld(hits, objects); 

        //player.debug();

        player.drawGun(dt); // Gun

        player.drawCrosshair(); // Crosshair


        window.display(); // Render to screen

        frame_count++;
    }

    delete[] hits;
}
