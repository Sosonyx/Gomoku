#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow win(sf::VideoMode(400, 300), "Gomoku");
    sf::CircleShape c(40);
    c.setFillColor(sf::Color::Black);
    c.setPosition(160, 110);
    for (int i = 0; i < 3 && win.isOpen(); ++i) {
        sf::Event e;
        while (win.pollEvent(e))
            if (e.type == sf::Event::Closed) win.close();
        win.clear(sf::Color(222, 184, 120));
        win.draw(c);
        win.display();
    }
    return 0;
}
