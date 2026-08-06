#pragma once

#include <SFML/Graphics.hpp>

class Game {
public:
    Game();
    void Run();

private:
    // 处理事件
    void ProcessEvents();
    /*
    * @brief 更新状态
    * @param[in] elapsed_time 从上一次更新状态，到现在，经过的时间
    */
    void Update(sf::Time elapsed_time);
    // 绘制和渲染
    void Render();

    void HandlePlayerInput(sf::Keyboard::Scancode key, bool is_pressed);

private:
    sf::RenderWindow window_;
    sf::CircleShape player_;

    inline static const float kPlayerSpeed{ 100.f }; //每秒移动像素个数

    bool is_move_up_ = false;
    bool is_move_down_ = false;
    bool is_move_left_ = false;
    bool is_move_right_ = false;
};