#include "game.h"
#include <iostream>

Game::Game()
    : window_(sf::VideoMode({ 640, 480 }), "SFML Application", sf::Style::Close)
    , world_(window_)
    , statistics_text_(font_) {

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
    world_.Update(elapsed_time);
}

void Game::Render() {
    window_.clear();
    world_.Draw();

    // world_.Draw() 内部调用了 window_.setView(world_view_)
    // 此后窗口的活动视图 = world_view_（游戏摄像机）
    // 如果此时直接绘制 statistics_text_，会用 world_view_ 映射 {5,5}
    // 而 world_view_ 显示的是世界区域 (0,1400)~(800,2000)
    // {5,5} 不在摄像机视野内 → 文本不可见

    // 方案A（推荐）：切回默认视图
    // 默认视图与目标大小 1:1，世界坐标 = 屏幕像素坐标
    // 切回后，{5,5} 会映射到屏幕像素 (5,5)，即屏幕左上角
    // 且 setView 只改变"后续 draw 使用的视图"状态，
    // 不会影响 world_.Draw() 已经绘制到帧缓冲的游戏画面
    window_.setView(window_.getDefaultView());
    window_.draw(statistics_text_);
    window_.display();
}

void Game::HandlePlayerInput(sf::Keyboard::Scancode key, bool is_pressed) {

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
