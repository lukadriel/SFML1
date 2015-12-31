#pragma once
#include "Character.h"
#include <array>
#include "SpriteNode.h"
#include "CommandQueue.h"
class World:private sf::NonCopyable
{
public:
	explicit World(sf::RenderWindow& window);
	void update(sf::Time dt);
	void draw();
	CommandQueue getCommandQueue();

private:
	void loadTextures();
	void buildScene();
	void adaptPlayerPosition();
private:
	enum Layer
	{
		Background,
		Object,
		LayerCount
	};
	
private:
	sf::RenderWindow& mWindow;
	sf::View mWorldView;
	TextureHolder mTextures;
	SceneNode mSceneGraph;
	std::array<SceneNode*, LayerCount> mSceneLayers;
	sf::FloatRect mWorldBounds;
	sf::Vector2f mSpawnPosition;
	float mScrollSpeed;
	Character* mPlayerCharacter;
	CommandQueue mCommandQueue;

};

