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

private:
    sf::RenderWindow window_;
    sf::CircleShape player_;
};