#pragma once

#include <SFML/System/Vector2.hpp>

/*
* @brief 实体类，游戏中的所有游戏元素的共同基类
*/
class Entity {
public:
    void SetVelocity(sf::Vector2f velocity);
    void SetVelocity(float vx, float vy);
    sf::Vector2f GetVelocity() const;

private:
    sf::Vector2f velocity_;
};