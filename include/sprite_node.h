#pragma once

#include "scene_node.h"
#include <SFML/Graphics.hpp>

/*
* @brief 精灵节点类
* @note
*   意义是，让一个普通的 sf::Sprite 能"挂"进场景图（Scene Graph）这棵大树里。
*   它本身几乎不添加新逻辑，只是在 sf::Sprite 外面套了一层 SceneNode 的壳。
* 
*   核心思想是用一棵场景树统一管理整个世界，所以哪怕是最简单的背景图，也要包装成 SceneNode 的派生类才能"入伙"
*/
class SpriteNode : public SceneNode {
public:
    explicit SpriteNode(const sf::Texture& texture);
    explicit SpriteNode(const sf::Texture& texture, const sf::IntRect& texture_rect);

private:
    virtual void DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

private:
    sf::Sprite sprite_;
};