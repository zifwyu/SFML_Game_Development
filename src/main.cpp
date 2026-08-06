#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "SFML Application");
    
    sf::CircleShape shape;
    shape.setRadius(40.f);
    shape.setPosition({ 100.f, 100.f });
    shape.setFillColor(sf::Color::Cyan);

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        window.clear();
        window.draw(shape);
        window.display();
    }
    return 0;
}