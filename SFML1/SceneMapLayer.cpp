#include "SceneMapLayer.h"

SceneMapLayer::SceneMapLayer():tmx::MapLayer(tmx::MapLayerType::Layer),
mapLayer(tmx::MapLayerType::Layer)
{
	
}

void SceneMapLayer::setLayer(tmx::MapLayer & layer)
{
	mapLayer = layer;
}



void SceneMapLayer::drawCurrent(sf::RenderTarget & target, sf::RenderStates states) const
{
}
