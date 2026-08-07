#include "scene_node.h"

#include <algorithm>
#include <cassert>

SceneNode::SceneNode() {}

void SceneNode::AttachChild(SceneNodePtr child) {
    child->parent_ = this;
    children_.push_back(std::move(child));
}

SceneNode::SceneNodePtr SceneNode::DetachChild(const SceneNode& node) {
    auto found = std::find_if(children_.begin(), children_.end(), [&](SceneNodePtr& p) {return p.get() == &node; });
    assert(found != children_.end());

    SceneNodePtr result = std::move(*found);
    result->parent_ = nullptr;
    children_.erase(found);
    return result;
}

void SceneNode::Update(sf::Time dt) {
    UpdateCurrent(dt);
    UpdateChildren(dt);
}

sf::Vector2f SceneNode::GetWorldPosition() const {
    // 返回当前坐标系的原点，在世界坐标下的位置。通过将原点 (0,0) 与世界变换相乘得到。
    return GetWorldTransform() * sf::Vector2f();
}

sf::Transform SceneNode::GetWorldTransform() const {
    sf::Transform transform = sf::Transform::Identity; //初始化为单位矩阵

    // 计算从当前节点到根节点的累积变换矩阵。
    // 从当前节点开始，沿着 parent_ 链向上遍历直到 nullptr（根节点的父指针为 nullptr）。
    // 每次将当前节点的局部变换（node->getTransform()，继承自 sf::Transformable）左乘到累积变换上。
    for (const SceneNode* node = this; node != nullptr; node = node->parent_) {

        // 注意这里是从右到左应用变换：先应用当前节点的变换，再应用父级变换。
        //
        // 为什么，计算时，getTransform()要在左边，而不是右边？？？
        // 在线性代数中，矩阵乘法就像指令序列：当使用列向量约定时，左边的矩阵代表后执行的指令，右边的矩阵代表先执行的指令。
        // 当使用行向量约定时，就是反过来，左边的矩阵先执行。
        // 而SFML库，正好使用的是列向量约定
        // 
        // 根据已有的两个条件，需要先从当前坐标系转换到父级，再从父级一级一级转换到世界，就得到了当前坐标系到世界的转换关系
        transform = node->getTransform() * transform;
    }
    return transform;
}

void SceneNode::UpdateCurrent(sf::Time dt) {
    // 虚函数，默认什么都不做。
    // 子类应重写此函数来实现自己的更新逻辑（如移动、动画、碰撞检测等）。
}

void SceneNode::UpdateChildren(sf::Time dt) {
    for (auto& child : children_) {
        child->Update(dt);
    }
}

void SceneNode::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    // states 已经是父级坐标系到世界坐标系的转换关系了
    // getTransform() 是sf::Transformable 的接口，返回一个点从当前坐标系转换到父级坐标系的转换矩阵
    // 应用当前节点的局部变换：states.transform *= getTransform(); 
    // 将当前节点的变换与传入的状态变换相乘，结果作为新的变换传递给子节点。
    // 乘积结果，就是当前坐标系到世界坐标系的转换关系
    //
    // 为什么，计算时，getTransform()要在右边，而不是左边？？？
    // 在线性代数中，矩阵乘法就像指令序列：当使用列向量约定时，左边的矩阵代表后执行的指令，右边的矩阵代表先执行的指令。
    // 当使用行向量约定时，就是反过来，左边的矩阵先执行。
    // 而SFML库，正好使用的是列向量约定
    // 
    // 根据已有的两个条件，需要先从当前坐标系转换到父级，再从父级转换到世界，就得到了当前坐标系到世界的转换关系
    states.transform *= getTransform();

    DrawCurrent(target, states);
    DrawChildren(target, states);
}

void SceneNode::DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {
    // 虚函数，默认什么都不做。
    // 子类应重写此函数来实现自己的绘制逻辑（如绘制精灵、形状等）。
}

void SceneNode::DrawChildren(sf::RenderTarget& target, sf::RenderStates states) const {
    for (auto& child : children_) {
        child->draw(target, states);
    }
}
