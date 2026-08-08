#pragma once

#include "resource_holder.h"
#include "resource_identifiers.h"
#include "aircraft.h"
#include "scene_node.h"
#include "sprite_node.h"

#include <SFML/Graphics.hpp>

/*
* @brief 世界类，游戏的"世界容器"，把窗口、摄像机、资源、场景图全部拢到一起
* @note
*   作为游戏规则的中央控制器，拥有所有游戏对象（实体），并在每一帧调用它们的 update() 与 draw()。
*
*   它承担三大职责：
*   1 持有资源：纹理统一加载、统一管理（TextureHolder）
*   2 持有场景：所有可见对象以场景图的形式挂在 scene_graph_ 这棵树上
*   3 持有摄像机：sf::View 决定"玩家透过哪扇窗户看世界"，并实现滚动
* 
*   精髓在于 游戏主循环不需要知道世界里有什么，只管每帧 world.update(dt); world.draw();
*   所有复杂性都被封装在 World 内部。这就是"高内聚、低耦合"的典型实践。
* 
*   这个类的设计精华:
*   1 场景图的分层 + 树形挂载:
*       World 用 scene_graph_ 作为根，scene_layers_ 作为层入口，再把具体对象挂到合适的层或父节点下。
*       背景层、空中层分离保证绘制顺序；护航飞机挂玩家节点下自动获得跟随效果。这是整个类最值得借鉴的设计。
*   2 摄像机与世界的分离:
*       world_view_ 独立于窗口存在。
*       世界坐标、屏幕坐标、视图变换三者解耦——这也是 SFML 视图系统的核心思想：通过改变视图中心来实现滚屏。
*       后续你想做小地图、HUD、视差背景，都是再创建一个 sf::View 切来切去。
*   3 滚动的"假象"手法:
*       游戏没有真的让飞机飞过 2000px 的世界，而是让视图以 -50px/s 上移，飞机自己在世界坐标里基本静止。
*       这种"摄像机动、对象不动"的手法是横版/纵版射击游戏的标配。
*   4 update / draw 分离:
*       逻辑更新和渲染严格分开，且都接受明确的时间参数 dt。这是固定时间步长游戏循环的基础，保证不同帧率下游戏速度一致。
*   5 资源管理集中化:
*       TextureHolder 在 World 构造时一次性加载所有纹理，全局共享。避免散落各处的 sf::Texture 和重复加载。
*   6 不可拷贝语义:
*       World 是唯一的。这避免了"不小心传值拷贝了一个 World"导致的灾难（窗口引用悬空、场景图被浅拷贝等）。
*/
class World {
public:
    explicit World(sf::RenderWindow& window);

    // 更新逻辑
    // dt，上一次更新逻辑，到现在的时间差
    void Update(sf::Time dt);

    // 渲染一帧
    void Draw();

private:
    // 加载所有纹理
    void LoadTextures();

    // 搭建场景图
    void BuildScene();

public:
    World(const World&) = delete;
    World& operator=(const World&) = delete;
    World(World&&) = delete;
    World& operator=(World&&) = delete;

private:
    /*
    * @brief 场景层枚举
    * @note
    *   游戏内，所有实体都有对应层。
    *   在渲染时，逐层渲染，下层所有实体渲染后，再渲染上层
    *   解决的是"哪一类物体应该压在哪一类物体上面"这个大尺度的遮挡问题
    */
    enum class Layer {
        kBackground, //背景层，沙漠背景
        kAir, //空中层，玩家飞机
        kLayerCount, //总层数
    };
private:

    // 对 SFML 渲染窗口的引用。World 自己不创建窗口，是外面（Game 类）把窗口传进来，World 借用它来绘制。
    sf::RenderWindow& window_;

    // 游戏摄像机。它定义了"从世界的哪个区域、以多大尺寸往窗口映射到"。
    // 所有绘制都以这个视图为坐标系。
    // 摄像机视图可以随便移动，虽然世界很大，但屏幕上只能看到视图内的东西
    sf::View world_view_;

    // 资源管理器（即 ResourceHolder<sf::Texture, Textures::ID>）。所有纹理在这里加载一次、处处复用。
    TextureHolder textures_;

    // 场景图的根节点。整棵场景树都挂在这个根下。
    // scene_graph_.update(dt) 会递归更新所有子孙节点，window_.draw(scene_graph_) 会递归绘制整棵树。
    SceneNode scene_graph_;

    // 层指针数组，是场景图的"快捷入口"。
    // buildScene() 里先创建 Background 和 Air 两个 SceneNode 作为 mSceneGraph 的直接子节点，再把它们的指针存进数组。
    // 后续往某一层加东西时，直接 mSceneLayers[Background]->attachChild(...)，不用每次都从根遍历。
    // 这里体现了分层架构（Layered Architecture）的思想：不同层独立管理、独立更新、独立绘制顺序。
    std::array<SceneNode*, static_cast<size_t>(Layer::kLayerCount)> scene_layers_;

    // 世界边界​。用来限制玩家飞机的水平活动范围，也作为背景精灵的平铺区域。
    sf::FloatRect world_bounds_;

    // 玩家出生点坐标
    sf::Vector2f spawn_position_;

    // 滚动速度​。负值表示视图向上移动（世界向下滚动），制造前进感。
    float scroll_speed_ = -50.f;

    // 指向玩家飞机的裸指针。因为 buildScene() 里 std::move(leader) 把所有权交给了场景图，
    // 但这里还需要一个快捷引用，方便 update() 里直接操作玩家（读位置、改速度）。
    // 用裸指针是安全的——Aircraft 的生命周期由场景图管理，World 整个生命周期内它都不会消失。
    Aircraft* player_aircraft_ = nullptr;
};