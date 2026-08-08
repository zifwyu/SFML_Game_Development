#include "sprite_node.h"

SpriteNode::SpriteNode(const sf::Texture& texture)
    : sprite_(texture) {

}

SpriteNode::SpriteNode(const sf::Texture& texture, const sf::IntRect& texture_rect)
    : sprite_(texture, texture_rect) {

}

void SpriteNode::DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(sprite_, states);
}
