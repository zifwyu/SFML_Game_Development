#pragma once

#include <SFML/System/Vector2.hpp>
#include "scene_node.h"

/*
* @brief 实体类，游戏中的所有游戏元素的共同基类
*/
class Entity : public SceneNode {
public:
    void SetVelocity(sf::Vector2f velocity);
    void SetVelocity(float vx, float vy);
    sf::Vector2f GetVelocity() const;

private:
    virtual void UpdateCurrent(sf::Time dt);

private:
    sf::Vector2f velocity_; //移动速度，单位像素/秒
};