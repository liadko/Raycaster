#include "headers.hpp"
#include "tools.hpp"
#include "player.hpp"
#include "map.hpp"
#include "client.hpp"


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
    v2f box_size(461, 70);
    v2f username_box_position(194, 271), password_box_position(194, 377);
    v2f text_offset(20, 16);

    TextBox username(v2f(194, 271), v2f(461, 70), "liadkoren123", input_font);
    TextBox password(v2f(194, 377), v2f(461, 70), "password432", input_font);
    //sf::Text username_text("liadko21567", input_font, 30);
    
    password.hidden = true;

    TextBox* text_boxes[3] = { nullptr, &username, &password };


    int box_focused = 1;


    string typed_text = "";
    int backspace_counter = 0;



    v2f button_position(194, 492), button_size(460, 60);

    while (window.isOpen())
    {
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
                
                //backspaces
                if (event.text.unicode == '\b')
                    backspace_counter++;
                if (event.text.unicode == 127) // ctrl backspace
                    backspace_counter = -100;
                
                //
                if (event.text.unicode == '\t' && box_focused)
                {
                    box_focused = (box_focused + 1) % 3;
                    if (box_focused == 0)
                        box_focused = 1;

                    text_boxes[box_focused]->turnOnCursor();
                }
                //cursor_visible = true;
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

                //cursor_visible = true;
                //cursor_timer.restart();

                bool success = false;
                if (inBounds(button_position, button_size, mousePos))
                    success = tryLogIn(username.getString(), password.getString());

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
            if(backspace_counter)
                text_boxes[box_focused]->backspace(backspace_counter);


            if (box_focused == 1)
            {

                /*cursor.setPosition(
                    username_text.getPosition() +
                    v2f(username_text.getGlobalBounds().getSize().x + 6, 4));*/
                    /*
                if (backspace_counter)
                    username_text.setString(username_text.getString()
                        .substring(0, username_text.getString().getSize() - backspace_counter));
                if (backspace_counter < 0)
                    username_text.setString("");*/
            }
            else
            {
                /*box_shadow.setPosition(username_box_position);
                cursor.setPosition(
                    password_text.getPosition() +
                    v2f(password_text.getGlobalBounds().getSize().x + 6, 4));
                if (typed_text.size())
                    password_text.setString(password_text.getString() + typed_text);
                if (backspace_counter)
                    password_text.setString(password_text.getString()
                        .substring(0, password_text.getString().getSize() - backspace_counter));
                if (backspace_counter < 0)
                    password_text.setString("");*/
            }
                    
            
            //if(cursor_visible) window.draw(cursor);

            ////cursor timer
            //if (cursor_timer.getElapsedTime().asMilliseconds() > 500)
            //{
            //    cursor_visible ^= true;
            //    cursor_timer.restart();
            //}

        }

        //window.draw(password_box);
        //window.draw(password_text);
        //window.draw(username_text);

        window.display();

        typed_text = "";
        backspace_counter = 0;
    }
}

void mainLoop(sf::RenderWindow& window, Map& map, Player& player)
{
    v2i screen_center(WIDTH / 2, HEIGHT / 2);


    int frame_count = 0;
    sf::Clock clock;

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


        player.shootRays(); // World

        //player.drawGun(dt); // Gun

        //player.drawCrosshair(); // Crosshair


        window.display(); // Render to screen

        frame_count++;
    }
}




int main()
{
    //Window
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Program", sf::Style::Close, sf::ContextSettings(24, 8, 8));
  
    Map map(20, window);
    
    Player player(40, 21, map, window);


    v2i prev_mouse_position = sf::Mouse::getPosition(window);
    
    loginPage(window, map, player);

    window.setMouseCursorVisible(false);
    
    // Game loop
    mainLoop(window, map, player);

    return 0;
}

