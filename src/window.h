#pragma once

#include <map>
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
using move_func = std::function<void (const vec3&)>;

struct Window {
    Window(const int& w, const int& h, compute_next_func next, select_func sel, move_func cam, move_func obj, std::string title = "rt"):
        w(w), h(h), title(title), compute_next(next), select(sel), move_cam(cam), move_obj(obj) {};

    void show();
    int update(sf::Image);
    void start_refresh();

    int w, h;
    std::string title;
    sf::Texture tex;
    sf::Sprite sprite;
    std::future<sf::Image> next_image;
    compute_next_func compute_next;
    select_func select;
    move_func move_cam;
    move_func move_obj;
    bool computing_next = false;
};

inline void Window::start_refresh() {
    next_image = std::async(std::launch::async, compute_next);
    computing_next = true;
}

inline void Window::show() {
    sf::RenderWindow window(sf::VideoMode(w, h), title);

    std::map<int, vec3> move_dirs = {
        {sf::Keyboard::W, vec3(0, 0, -1)},
        {sf::Keyboard::S, vec3(0, 0, 1)},
        {sf::Keyboard::A, vec3(-1, 0, 0)},
        {sf::Keyboard::D, vec3(1, 0, 0)},
        {sf::Keyboard::Q, vec3(0, -1, 0)},
        {sf::Keyboard::E, vec3(0, 1, 0)}
    };

    std::map<int, vec3> move_obj_dirs = {
        {sf::Keyboard::I, vec3(0, 0, -1)},
        {sf::Keyboard::K, vec3(0, 0, 1)},
        {sf::Keyboard::J, vec3(-1, 0, 0)},
        {sf::Keyboard::L, vec3(1, 0, 0)},
        {sf::Keyboard::U, vec3(0, -1, 0)},
        {sf::Keyboard::O, vec3(0, 1, 0)}
    };

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Enter && !computing_next) {
                    start_refresh();
                }
                else if (move_dirs.find(event.key.code) != move_dirs.end() && !computing_next) {
                    move_cam(move_dirs[event.key.code]);
                    start_refresh();
                }
                else if (move_obj_dirs.find(event.key.code) != move_obj_dirs.end() && !computing_next) {
                    move_obj(move_obj_dirs[event.key.code]);
                    start_refresh();
                }
            }
            else if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left && !computing_next) {
                    auto xy = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
                    if (select(xy.x, h - xy.y)) {
                        start_refresh();
                    }
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
