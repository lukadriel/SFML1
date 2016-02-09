#pragma once
#include "SceneNode.h"
#include <tmx/MapLoader.h>


class Tile : public SceneNode, public tmx::MapTile, public tmx::MapObject
{
public:
	Tile(Category::Type category );
	virtual sf::FloatRect getBoundingRect() const;
	void setTile(const sf::Texture& texture,sf::IntRect rect,sf::Vector2f position);

private:
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	
private:
	sf::RectangleShape boundingRect;
};

