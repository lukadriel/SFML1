#include "Tile.h"

Tile::Tile(Category::Type category):SceneNode(category)
{
}

sf::FloatRect Tile::getBoundingRect() const
{
	return getWorldTransform().transformRect(sprite.getGlobalBounds());
}


void Tile::setTile(const sf::Texture & texture, sf::IntRect rect, sf::Vector2f position)
{
	sprite.setTexture(texture);
	sprite.setTextureRect(rect);
	sprite.setPosition(position);
}

void Tile::drawCurrent(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(sprite, states);
}
