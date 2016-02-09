#include "TiledMap.h"

TiledMap::TiledMap(std::string path, std::string file):tmx::MapLoader(path)
{
	Load(file);
	for (auto& layer : GetLayers())
	{
		sceneMapLayer.setLayer(layer);
	}
	std::unique_ptr<SceneMapLayer> scene(&sceneMapLayer);
	attachChild(std::move(scene));
}
