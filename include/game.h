#pragma once

#include <SFML/Graphics.hpp>
#include "world.h"

class Game {
public:
    Game();
    void Run();

private:
    // 处理事件
    void ProcessEvents();
    /*
    * @brief 更新状态
    * @param[in] elapsed_time 从上一次逻辑更新状态，到现在，经过的时间
    * @note 改用固定时间步长后，elapsed_time就变成了固定值
    */
    void Update(sf::Time elapsed_time);
    // 绘制和渲染
    void Render();

    void HandlePlayerInput(sf::Keyboard::Scancode key, bool is_pressed);

    /*
    * @brief 更新统计数据
    * @param[in] elapsed_time 从上一次实际渲染一帧，到现在，经过的时间
    * @note 这里统计的是实际渲染帧的数据，而不是固定时间步长的逻辑更新数据
    */
    void UpdateStatistics(sf::Time elapsed_time);

private:
    sf::RenderWindow window_;
    World world_;

    sf::Font font_;
    sf::Text statistics_text_;
    sf::Time statistics_update_time_; //上一次渲染显示帧率数值，到现在，经过的时间
    std::size_t statistics_num_frames_ = 0; // 帧率统计


    // 每一帧分配的时间，这里用1/60，说明游戏是60帧/秒
    inline static const sf::Time kTimePerFrame{ sf::seconds(1.f / 60.f) };
};