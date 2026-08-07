#pragma once

#include <SFML/Graphics.hpp>

#include <vector>
#include <memory>

/*
* @brief 游戏场景图的节点类
* @note
*   SceneNode 是构建游戏场景图的基础节点，它管理节点的层次结构、空间变换和递归更新/绘制。
*   具体来说，它做了三件事：
*   1 管理父子关系​：通过 AttachChild、DetachChild、children_ 和 parent_，构建一棵树形结构。对根节点的操作会自动影响所有后代。
*   2 提供空间变换​：继承 sf::Transformable，每个节点都有自己的位置、旋转、缩放。
        通过 GetWorldTransform() 和 GetWorldPosition() 可以计算节点在世界空间中的实际位置。
*   3 统一更新与绘制接口​：继承 sf::Drawable，并设计了 Update / draw 的递归流程。
        子类只需重写 UpdateCurrent 和 DrawCurrent 来实现具体行为，遍历子节点的逻辑由基类自动完成。
*   
*   核心意义是：将游戏对象组织成一棵树，实现“一次操作，全局生效”的效果。
*   1 简化代码：移动一个父节点，所有子节点跟着移动；绘制根节点，整棵树都被绘制。不需要手动管理每个对象的更新和渲染。
*   2 易于扩展：任何游戏对象（精灵、文本、粒子系统、摄像机等）都可以继承 SceneNode，只需实现自己的 UpdateCurrent 和 DrawCurrent，就能自动融入场景树。
*   3 统一生命周期：通过 std::unique_ptr 管理子节点，避免内存泄漏。DetachChild 可以安全地转移节点所有权。
*/
class SceneNode : public sf::Transformable, public sf::Drawable {
public:
    using SceneNodePtr = std::unique_ptr<SceneNode>;
public:
    SceneNode();
    SceneNode(const SceneNode&) = delete;
    SceneNode& operator=(const SceneNode&) = delete;

public:
    // 增加一个子节点
    void AttachChild(SceneNodePtr child);

    // 删除一个子节点
    SceneNodePtr DetachChild(const SceneNode& node);

    // 每帧调用一次，更新当前节点及其所有子节点
    // dt 两帧之间的时间间隔
    void Update(sf::Time dt);

    // 返回该节点坐标系的原点，在世界空间中的位置
    sf::Vector2f GetWorldPosition() const;

    // 返回该节点所在的坐标系，到世界坐标系的变换矩阵
    // 当前坐标系的一个点，经过这个矩阵，就能知道在世界坐标系的坐标
    sf::Transform GetWorldTransform() const;

private:
    // 更新当前节点，子类应重写的更新逻辑
    virtual void UpdateCurrent(sf::Time dt);

    // 遍历所有子节点，对每个子节点调用其 Update(dt)，实现递归更新
    // dt 两帧之间的时间间隔
    void UpdateChildren(sf::Time dt);

    // 继承自 sf::Drawable，是 SFML 渲染系统的入口。
    // 当调用 window.draw(node) 时，SFML 会调用此函数。
    // sf::RenderTarget& target：渲染目标（通常是窗口或纹理）。
    // sf::RenderStates states：渲染状态包，其中 states.transform 已包含从父级坐标系到世界坐标系的变换矩阵。
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    // 绘制自身节点，子类应重写的绘制逻辑
    virtual void DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

    // 遍历所有子节点，对每个子节点调用其 draw(target, states)，实现递归绘制
    void DrawChildren(sf::RenderTarget& target, sf::RenderStates states) const;

private:
    std::vector<SceneNodePtr> children_;
    SceneNode* parent_ = nullptr;
};