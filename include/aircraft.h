#pragma once

#include "entity.h"
#include "resource_identifiers.h"

class Aircraft : public Entity {
public:
    enum class Type {
        kEagle,
        kRaptor,
    };

public:
    explicit Aircraft(Type type, const TextureHolder& texture_holder);

private:
    virtual void DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

private:
    Type type_;
    sf::Sprite sprite_;
};