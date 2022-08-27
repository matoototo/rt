#include <vector>
#include <string>
#include <iostream>
#include <functional>

#include "vec3.h"
#include "image.h"
#include "scene.h"
#include "config.h"

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

using compute_next_func = std::function<sf::Image ()>;

struct Window {
    Window(const int& w, const int& h, compute_next_func next, std::string title = "rt"):
        w(w), h(h), title(title), compute_next(next) {};

    void show();
    int update(sf::Image);

    int w, h;
    std::string title;
    sf::Texture tex;
    sf::Sprite sprite;
    compute_next_func compute_next;
};

inline void Window::show() {
    sf::RenderWindow window(sf::VideoMode(w, h), title);
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Enter)
                   update(compute_next());
            }
        }
        window.clear();
        window.draw(sprite);
        window.display();
    }
}

inline int Window::update(sf::Image img) {
    if (!tex.loadFromImage(img)) {
        return EXIT_FAILURE;
    }
    this->sprite.setTexture(tex);
    return 0;
}
