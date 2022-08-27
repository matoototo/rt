#pragma once

#include <vector>
#include <string>
#include <future>
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
using select_func = std::function<bool (int, int)>;

struct Window {
    Window(const int& w, const int& h, compute_next_func next, select_func sel, std::string title = "rt"):
        w(w), h(h), title(title), compute_next(next), select(sel) {};

    void show();
    int update(sf::Image);

    int w, h;
    std::string title;
    sf::Texture tex;
    sf::Sprite sprite;
    compute_next_func compute_next;
    select_func select;
    bool computing_next = false;
};

inline void Window::show() {
    sf::RenderWindow window(sf::VideoMode(w, h), title);
    std::future<sf::Image> next_image;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Enter && !computing_next) {
                    computing_next = true;
                    next_image = std::async(std::launch::async, compute_next);
                }
            }
            else if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left && !computing_next) {
                    select(event.mouseButton.x, h - event.mouseButton.y);
                    next_image = std::async(std::launch::async, compute_next);
                }
            }
        }
        if (next_image.valid() && next_image.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
            update(next_image.get());
            computing_next = false;
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
