#pragma once

#include <SFML/Graphics.hpp>

class Game {
public:
    Game();
    void Run();

private:
    // 处理事件
    void ProcessEvents();
    // 更新状态
    void Update();
    // 绘制和渲染
    void Render();

    void HandlePlayerInput(sf::Keyboard::Scancode key, bool is_pressed);

private:
    sf::RenderWindow window_;
    sf::CircleShape player_;

    bool is_move_up_ = false;
    bool is_move_down_ = false;
    bool is_move_left_ = false;
    bool is_move_right_ = false;
};