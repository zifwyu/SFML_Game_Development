#include "game.h"
#include <iostream>

Game::Game()
    : window_(sf::VideoMode({ 800, 600 }), "SFML Application")
    , texture_()
    , player_(texture_) 
    , statistics_text_(font_) {

    if (!texture_.loadFromFile("Media/Textures/Eagle.png")) {
        std::cout << "load error\n";
    }

    player_.setTexture(texture_, true);
    player_.setPosition({ 100.f, 100.f });

    if (!font_.openFromFile("Media/HarmonyOS_Sans_SC_Regular.ttf")) {
        std::cout << "load error\n";
    }

    statistics_text_.setPosition({ 5.f,5.f });
    statistics_text_.setCharacterSize(10);
}

void Game::Run() {
    sf::Clock clock;
    sf::Time time_since_last_update = sf::Time::Zero;
    while (window_.isOpen()) {
        // 返回自上一次调用 restart()（或时钟创建时）以来经过的时间（sf::Time）
        // 将时钟内部计时器归零，重新开始计时。
        sf::Time elapsed_time = clock.restart();
        time_since_last_update += elapsed_time;

        while (time_since_last_update > kTimePerFrame) {
            time_since_last_update -= kTimePerFrame;

            ProcessEvents();
            Update(kTimePerFrame);
        }

        UpdateStatistics(elapsed_time);
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
    window_.draw(statistics_text_);
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

void Game::UpdateStatistics(sf::Time elapsed_time) {
    statistics_update_time_ += elapsed_time;
    statistics_num_frames_ += 1;

    if (statistics_update_time_ > sf::seconds(1.0f)) {
        statistics_text_.setString(
            "Frames / Second = " + std::to_string(statistics_num_frames_) + "\n" + 
            "Time / Update = " + std::to_string(statistics_update_time_.asMicroseconds() / statistics_num_frames_) + "us");

        statistics_update_time_ -= sf::seconds(1.0f);
        statistics_num_frames_ = 0;
    }
}
