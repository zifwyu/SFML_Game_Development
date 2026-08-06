#include "game.h"

Game::Game()
    : window_(sf::VideoMode({ 800, 600 }), "SFML Application")
    , player_() {
    player_.setRadius(40.f);
    player_.setPosition({ 100.f, 100.f });
    player_.setFillColor(sf::Color::Cyan);
}

void Game::Run() {
    while (window_.isOpen()) {
        ProcessEvents();
        Update();
        Render();
    }
}

void Game::ProcessEvents() {
    while (auto event = window_.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window_.close();
        }
    }
}

void Game::Update() {}

void Game::Render() {
    window_.clear();
    window_.draw(player_);
    window_.display();
}
