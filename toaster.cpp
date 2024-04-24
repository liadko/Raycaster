#include "toaster.hpp"
#include "tools.hpp"


Toaster::Toaster() : first_toast_position(870, -10), goal_y(-10)
{ 
    toast_tex.loadFromFile("sprites/toast.png");
    toast_sprite.setTexture(toast_tex);
    
    font.loadFromFile("Fonts/Roboto-Medium.ttf");

    text = sf::Text("Missing Text", font, 18);
    text.setFillColor(sf::Color::Black);
}

void Toaster::drawToasts(sf::RenderWindow& window, float dt)
{
    
    for (int i = 0; i < toast_timers.size(); i++)
    {
        toast_timers[i] -= dt;
        if (toast_timers[i] < 0)
        {
            goal_y -= 67;
            toast_timers.erase(toast_timers.begin() + i);
            i--;
        }
    }


    first_toast_position.y = lerp(first_toast_position.y, goal_y, 0.05);

    int amount = 0;

    v2f toast_position = first_toast_position;
    for(int i = 0; i < toasts.size(); i++)
    {
        toast_slides[i] = lerp(toast_slides[i], 0, 0.3);
        v2f slide_offset(toast_slides[i], 0);
        
        
        toast_sprite.setPosition(toast_position + slide_offset);

        text.setString(toasts[i]);
        text.setPosition(toast_position + text_position + slide_offset);

        if (toast_position.y + toast_sprite.getLocalBounds().height - 24 > 0)
        {
            window.draw(toast_sprite);
            window.draw(text);

            amount++;
        }

        toast_position.y += 67;
    }

}

void Toaster::toast(const string& text)
{
    cout << "Toasting: " << text << "\n";
    toasts.push_back(text);
    toast_slides.push_back(500);
    toast_timers.push_back(lifetime);
}