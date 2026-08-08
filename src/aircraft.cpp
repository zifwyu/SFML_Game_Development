#include "aircraft.h"
#include "resource_holder.h"

namespace {
    // 将飞机类内部的枚举类型，转换成资源持有器能看懂的类型，这样才能获取到资源
    Textures::ID ToTextureID(Aircraft::Type type) {
        switch (type) {
        case Aircraft::Type::kEagle:
            return Textures::ID::kEagle;
            break;
        case Aircraft::Type::kRaptor:
            return Textures::ID::kRaptor;
            break;
        default:
            break;
        }
    }
}

Aircraft::Aircraft(Type type, const TextureHolder& texture_holder)
    : type_(type)
    , sprite_(texture_holder.Get(ToTextureID(type))) {

    // 把精灵的原点位置，设置到其外接矩形的中心
    // 如果不设置，默认是左上角第一个像素
    sf::FloatRect bounds = sprite_.getLocalBounds();
    sprite_.setOrigin(bounds.getCenter());
}

void Aircraft::DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(sprite_, states);
}
