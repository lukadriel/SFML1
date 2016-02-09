#pragma once
#include "SceneNode.h"
#include "tmx/MapLoader.h"
#include "SceneMapLayer.h"

#include <vector>

enum TiledLayer
{
	Ground,
	Wall,
	Object,
	LayerCount
};

class TiledMap : public SceneNode, public tmx::MapLoader
{
public:
	TiledMap(std::string path, std::string file);
private:


private:
	std::vector<SceneMapLayer> sceneMapLayer;
};

