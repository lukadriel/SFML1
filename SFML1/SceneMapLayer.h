#pragma once
#include "SceneNode.h"
#include "Tile.h"
#include <tmx/MapLoader.h>


class SceneMapLayer : public SceneNode, public tmx::MapLayer
{
public:
	SceneMapLayer();
	void setLayer(tmx::MapLayer& layer);

private:
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
private:
	tmx::MapLayer& mapLayer;
	std::vector<Tile> tiles;
};

