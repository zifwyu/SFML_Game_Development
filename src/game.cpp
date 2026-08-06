#include "game.h"

Game::Game()
    : window_(sf::VideoMode({ 800, 600 }), "SFML Application")
    , player_() {
    player_.setRadius(40.f);
    player_.setPosition({ 100.f, 100.f });
    player_.setFillColor(sf::Color::Cyan);
}

void Game::Run() {
    sf::Clock clock;
    sf::Time time_since_last_update = sf::Time::Zero;
    while (window_.isOpen()) {
        // 返回自上一次调用 restart()（或时钟创建时）以来经过的时间（sf::Time）
        // 将时钟内部计时器归零，重新开始计时。
        time_since_last_update += clock.restart();

        while (time_since_last_update > kTimePerFrame) {
            time_since_last_update -= kTimePerFrame;

            ProcessEvents();
            Update(kTimePerFrame);
        }

        Render();
    }
}

void Game::ProcessEvents() {
    const auto on_close = [this](const sf::Event::Closed&) {
        window_.close();
        };

    const auto on_key_pressed = [this](const sf::Event::KeyPressed& key) {
        HandlePlayerInput(key.scancode, true);
        };

    const auto on_key_released = [this](const sf::Event::KeyReleased& key) {
        HandlePlayerInput(key.scancode, false);
        };
    
    window_.handleEvents(on_close, on_key_pressed, on_key_released);
}

void Game::Update(sf::Time elapsed_time) {
    sf::Vector2f movement(0.f, 0.f);
    if (is_move_up_) {
        movement.y -= kPlayerSpeed;
    }
    if (is_move_down_) {
        movement.y += kPlayerSpeed;
    }
    if (is_move_left_) {
        movement.x -= kPlayerSpeed;
    }
    if (is_move_right_) {
        movement.x += kPlayerSpeed;
    }

    player_.move(movement * elapsed_time.asSeconds());
}

void Game::Render() {
    window_.clear();
    window_.draw(player_);
    window_.display();
}

void Game::HandlePlayerInput(sf::Keyboard::Scancode key, bool is_pressed) {
    using enum sf::Keyboard::Scancode;
    switch (key) {
    case W: 
        is_move_up_ = is_pressed;
        break;
    case S:
        is_move_down_ = is_pressed;
        break;
    case A:
        is_move_left_ = is_pressed;
        break;
    case D:
        is_move_right_ = is_pressed;
        break;
    default:
        break;
    }
}
