#pragma once
#include "Character.h"
#include <array>
#include "SpriteNode.h"
#include "CommandQueue.h"
#include "Tile.h"
namespace sf
{
	class RenderWindow;
}

class World : private sf::NonCopyable
{
public:
	explicit							World(sf::RenderWindow& window, FontHolder& fonts);
	void								update(sf::Time dt);
	void								draw();

	CommandQueue&						getCommandQueue();

	bool 								hasAlivePlayer() const;
	bool 								hasPlayerReachedEnd() const;


private:
	void								loadTextures();
	void								adaptPlayerPosition();
	void								adaptPlayerVelocity();
	void								handleCollisions();

	void								buildScene();
	void								addEnemies();
	void								addEnemy(Character::Type type, float relX, float relY);
	void								spawnEnemies();
	void								destroyEntitiesOutsideView();
	/*void								guideMissiles();*/
	sf::FloatRect						getViewBounds() const;
	sf::FloatRect						getBattlefieldBounds() const;
	void handleWallCollision(bool collision);
	void handleTileMapCollision();

private:
	enum Layer
	{
		Background,
		Ground,
		LayerCount
	};

	struct SpawnPoint
	{
		SpawnPoint(Character::Type type, float x, float y)
			: type(type)
			, x(x)
			, y(y)
		{
		}

		Character::Type type;
		float x;
		float y;
	};


private:
	sf::RenderWindow& mWindow;
	sf::View mWorldView;
	TextureHolder mTextures;
	FontHolder& mFonts;
	

	SceneNode mSceneGraph;
	std::array<SceneNode*, LayerCount> mSceneLayers;
	CommandQueue mCommandQueue;

	sf::FloatRect mWorldBounds;
	sf::Vector2f mSpawnPosition;
	float mScrollSpeed;
	Character* mPlayerCharacter;
	tmx::MapLoader mMap;
	Tile* mTile;
	
	bool tileCollision;
	std::vector<SpawnPoint> mEnemySpawnPoints;
	std::vector<Character*> mActiveEnemies;
};

