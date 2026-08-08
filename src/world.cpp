#include "world.h"

World::World(sf::RenderWindow& window)
    : window_(window)
    // 默认视图与渲染目标大小相同，中心在窗口中心
    // 因此它初始显示世界坐标 [0,0] 到 [viewWidth, viewHeight] 的区域，即世界顶部
    , world_view_(window.getDefaultView())
    , world_bounds_({ 0.f,0.f }, { world_view_.getSize().x, 2000.f })
    // 这里出生点，y方向设置在世界底部靠上半个屏幕的位置，x方向设置在左右的中心点
    // 世界总长2000，顶部y坐标是0，底部y坐标是2000
    , spawn_position_(world_view_.getSize().x / 2.f, world_bounds_.size.y - world_view_.getSize().y / 2.f) {

    LoadTextures();
    BuildScene();

    // 摄像机视图是一个矩形，默认坐标是从零开始的，与渲染目标大小相同。也就意味着，会处于世界顶部
    // 将出生点设置为摄像机视图的中心，即把摄像机对准世界底部上方约半个屏幕的位置（y=1700）
    // 这样摄像机视野刚好覆盖世界的最底部区域 [1400, 2000]，不会超出世界边界
    world_view_.setCenter(spawn_position_);
}

void World::Update(sf::Time dt) {
    // 向上移动摄像机视图，给人的感觉就是世界在往下走
    world_view_.move({ 0.f, scroll_speed_ * dt.asSeconds() });

    // 返回的是世界坐标系下的位置。因为 player_aircraft_ 是挂在场景图里的 Aircraft 节点，
    // 它的 getPosition() 返回的是相对于其父节点（这里是 Air 层节点）的局部坐标
    // 但由于 Air 层节点本身在世界原点 (0,0) 且没有偏移，所以这个局部坐标恰好等于世界坐标。
    sf::Vector2f position = player_aircraft_->getPosition();
    sf::Vector2f velocity = player_aircraft_->GetVelocity();

    // X方向，玩家飞机的"有效活动范围"是 [150, 世界宽度-150]
    // 到达边界后，水平速度被反转（velocity.x = -velocity.x），飞机开始向反方向运动
    if (position.x <= world_bounds_.position.x + 150.f
        || position.x >= world_bounds_.position.x + world_bounds_.size.x - 150.f) {
        velocity.x = -velocity.x;
        player_aircraft_->SetVelocity(velocity);
    }

    scene_graph_.Update(dt);
}

void World::Draw() {
    window_.setView(world_view_);

    // 会自动调用scene_graph_的draw函数，将scene_graph_渲染到windows_
    window_.draw(scene_graph_);
}

void World::LoadTextures() {
    textures_.Load(Textures::ID::kEagle, "Media/Textures/Eagle.png");
    textures_.Load(Textures::ID::kRaptor, "Media/Textures/Raptor.png");
    textures_.Load(Textures::ID::kDesert, "Media/Textures/Desert.png");
}

void World::BuildScene() {

    // 对于每个layer层，创建一个空的根节点
    for (size_t i = 0; i < static_cast<size_t>(Layer::kLayerCount); ++i) {
        std::unique_ptr<SceneNode> layer = std::make_unique<SceneNode>();
        scene_layers_[i] = layer.get();

        scene_graph_.AttachChild(std::move(layer));
    }

    sf::Texture& texture = textures_.Get(Textures::ID::kDesert);
    sf::IntRect texture_rect(world_bounds_);
    texture.setRepeated(true); //允许纹理平铺。让纹理在 texture_rect 指定的大区域上重复平铺，而不是拉伸变形。

    std::unique_ptr<SpriteNode> background_sprite = std::make_unique<SpriteNode>(texture, texture_rect);
    background_sprite->setPosition({ world_bounds_.position.x, world_bounds_.position.y });
    scene_layers_[static_cast<size_t>(Layer::kBackground)]->AttachChild(std::move(background_sprite));

    std::unique_ptr<Aircraft> leader = std::make_unique<Aircraft>(Aircraft::Type::kEagle, textures_);
    player_aircraft_ = leader.get();
    player_aircraft_->setPosition(spawn_position_);
    player_aircraft_->SetVelocity(40.f, scroll_speed_);
    scene_layers_[static_cast<size_t>(Layer::kAir)]->AttachChild(std::move(leader));

    std::unique_ptr<Aircraft> left_escort = std::make_unique<Aircraft>(Aircraft::Type::kRaptor, textures_);
    left_escort->setPosition({ -80.f, 50.f });
    player_aircraft_->AttachChild(std::move(left_escort));

    std::unique_ptr<Aircraft> right_escort = std::make_unique<Aircraft>(Aircraft::Type::kRaptor, textures_);
    right_escort->setPosition({ 80.f, 50.f });
    player_aircraft_->AttachChild(std::move(right_escort));
}
