#pragma once

namespace sf {
    class Texture;
}

namespace Textures {
    enum class ID {
        kEagle,
        kRaptor,
        kDesert,
    };
}

template <typename Resource, typename Identifier>
class ResourceHolder;

using TextureHolder = ResourceHolder<sf::Texture, Textures::ID>;