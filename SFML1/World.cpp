#include "World.h"
#include "Pickup.h"
#include "Projectile.h"
#include "TextNode.h"

#include <SFML/Graphics/RenderWindow.hpp>

#include <algorithm>
#include <cmath>
#include <limits>

World::World(sf::RenderWindow& window, FontHolder& fonts)
	: mWindow(window)
	, mWorldView(window.getDefaultView())
	, mFonts(fonts)
	, mTextures()
	, mSceneGraph()
	, mSceneLayers()
	, mWorldBounds(0.f, 0.f, mWorldView.getSize().x, 2000.f)
	, mSpawnPosition(mWorldView.getSize().x / 2.f, mWorldBounds.height - mWorldView.getSize().y / 2.f)
	, mScrollSpeed(-30.f)
	, mPlayerCharacter(nullptr)
	, mEnemySpawnPoints()
	, mActiveEnemies()
	,tileCollision(false)
	,mMap("maps/")
{
	bool loaded = mMap.Load("Level1.tmx");
	loadTextures();
	buildScene();
	// Prepare the view
	mWorldView.setCenter(mSpawnPosition);
}

void World::update(sf::Time dt)
{
	// Scroll the world, reset player velocity
	mWorldView.move(0.f, mScrollSpeed * dt.asSeconds());
	mTile->move(0.f, mScrollSpeed * dt.asSeconds());
	mPlayerCharacter->setVelocity(0.f, 0.f);

	// Setup commands to destroy entities, and guide missiles
	destroyEntitiesOutsideView();
	//guideMissiles();

	// Forward commands to scene graph, adapt velocity (scrolling, diagonal correction)
	while (!mCommandQueue.isEmpty())
		mSceneGraph.onCommand(mCommandQueue.pop(), dt);
	adaptPlayerVelocity();

	// Collision detection and response (may destroy entities)
	handleCollisions();

	// Remove all destroyed entities, create new ones
	mSceneGraph.removeWrecks();
	spawnEnemies();

	// Regular update step, adapt position (correct if outside view)
	mSceneGraph.update(dt, mCommandQueue);
	adaptPlayerPosition();
	handleWallCollision(tileCollision);
	handleTileMapCollision();
}

void World::draw()
{
	mWindow.setView(mWorldView);
	mMap.Draw(mWindow, 0);
	mMap.Draw(mWindow, 1);
	mWindow.draw(mSceneGraph);	
	mMap.Draw(mWindow, 2);
	mMap.Draw(mWindow, 3);
	
}

CommandQueue& World::getCommandQueue()
{
	return mCommandQueue;
}

bool World::hasAlivePlayer() const
{
	return !mPlayerCharacter->isMarkedForRemoval();
}

bool World::hasPlayerReachedEnd() const
{
	return !mWorldBounds.contains(mPlayerCharacter->getPosition());
}

void World::loadTextures()
{
	mTextures.load(Textures::Main, "characters/vlad.png");
	mTextures.load(Textures::Allied, "characters/cloud.png");
	mTextures.load(Textures::Enemy, "characters/ghost.png");
	mTextures.load(Textures::Ground, "images/end.png");

	mTextures.load(Textures::Bullet, "images/Bullet.png");
	mTextures.load(Textures::Missile, "images/Missile.png");

	mTextures.load(Textures::HealthRefill, "images/HealthRefill.png");
	mTextures.load(Textures::MissileRefill, "images/MissileRefill.png");
	mTextures.load(Textures::FireSpread, "images/FireSpread.png");
	mTextures.load(Textures::FireRate, "images/FireRate.png");
	mTextures.load(Textures::Tileset, "maps/050-DarkSpace01.png");
	mTextures.load(Textures::Wall, "images/spikes.png");

	
}

void World::adaptPlayerPosition()
{
	// Keep player's position inside the screen bounds, at least borderDistance units from the border
	sf::FloatRect viewBounds = getViewBounds();
	const float borderDistance = 40.f;

	sf::Vector2f position = mPlayerCharacter->getPosition();
	position.x = std::max(position.x, viewBounds.left + borderDistance);
	position.x = std::min(position.x, viewBounds.left + viewBounds.width - borderDistance);
	position.y = std::max(position.y, viewBounds.top + borderDistance);
	position.y = std::min(position.y, viewBounds.top + viewBounds.height - borderDistance);
	mPlayerCharacter->setPosition(position);
}

void World::adaptPlayerVelocity()
{
	sf::Vector2f velocity = mPlayerCharacter->getVelocity();

	// If moving diagonally, reduce velocity (to have always same velocity)
	if (velocity.x != 0.f && velocity.y != 0.f)
		mPlayerCharacter->setVelocity(velocity / std::sqrt(2.f));

	// Add scrolling velocity
	mPlayerCharacter->accelerate(0.f, mScrollSpeed);
}

bool matchesCategories(SceneNode::Pair& colliders, Category::Type type1, Category::Type type2)
{
	unsigned int category1 = colliders.first->getCategory();
	unsigned int category2 = colliders.second->getCategory();

	// Make sure first pair entry has category type1 and second has type2
	if (type1 & category1 && type2 & category2)
	{
		return true;
	}
	else if (type1 & category2 && type2 & category1)
	{
		std::swap(colliders.first, colliders.second);
		return true;
	}
	else
	{
		return false;
	}
}

void World::handleCollisions()
{
	std::set<SceneNode::Pair> collisionPairs;
	mSceneGraph.checkSceneCollision(mSceneGraph, collisionPairs);

	for(SceneNode::Pair pair: collisionPairs)
	{
		if (matchesCategories(pair, Category::PlayerCharacter, Category::EnemyCharacter))
		{
			auto& player = static_cast<Character&>(*pair.first);
			auto& enemy = static_cast<Character&>(*pair.second);

			// Collision: Player damage = enemy's remaining HP
			player.damage(enemy.getHitpoints());
			enemy.destroy();
		}

		else if (matchesCategories(pair, Category::PlayerCharacter, Category::Pickup))
		{
			auto& player = static_cast<Character&>(*pair.first);
			auto& pickup = static_cast<Pickup&>(*pair.second);

			// Apply pickup effect to player, destroy projectile
			pickup.apply(player);
			pickup.destroy();
		}

		else if (matchesCategories(pair, Category::EnemyCharacter, Category::AlliedProjectile)
			|| matchesCategories(pair, Category::PlayerCharacter, Category::EnemyProjectile))
		{
			auto& character = static_cast<Character&>(*pair.first);
			auto& projectile = static_cast<Projectile&>(*pair.second);

			// Apply projectile damage to Character, destroy projectile
			character.damage(projectile.getDamage());
			projectile.destroy();
		}
		else if (matchesCategories(pair,Category::PlayerCharacter,Category::Wall))
		{
			tileCollision = true;
		}
		else if (matchesCategories(pair,Category::EnemyProjectile,Category::Wall)
			||matchesCategories(pair,Category::AlliedProjectile,Category::Wall))
		{
			auto& projectile = static_cast<Projectile&>(*pair.first);
			projectile.destroy();
		}
	}
}

void World::buildScene()
{
	// Initialize the different layers
	for (std::size_t i = 0; i < LayerCount; ++i)
	{
		Category::Type category = (i == Ground) ? Category::Scene : Category::None;

		SceneNode::Ptr layer(new SceneNode(category));
		mSceneLayers[i] = layer.get();

		mSceneGraph.attachChild(std::move(layer));
	}

	
	// Prepare the tiled background
	sf::Texture& texture = mTextures.get(Textures::Ground);
	sf::IntRect textureRect(0,0,1280,768);
	texture.setRepeated(false);

	// Add the background sprite to the scene
	std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(texture, textureRect));
	backgroundSprite->setPosition(mWorldBounds.left, mWorldBounds.top-768);
	mSceneLayers[Background]->attachChild(std::move(backgroundSprite));
	
	
	//Spikes
	std::unique_ptr<Tile> tile(new Tile(Category::Wall));
	mTile = tile.get();
	mTile->setTile(mTextures.get(Textures::Wall), sf::IntRect(0, 0, 1280, 100), /*mSpawnPosition -*/ sf::Vector2f(0.f, 1900.f));
	mSceneLayers[Ground]->attachChild(std::move(tile));

	//View coordinate
	
	
	
	// Add player's Character
	std::unique_ptr<Character> player(new Character(Character::Main, mTextures, mFonts));
	mPlayerCharacter = player.get();
	mPlayerCharacter->setPosition(mSpawnPosition);
	mSceneLayers[Ground]->attachChild(std::move(player));


	// Add enemy Character
	addEnemies();
}

void World::addEnemies()
{
	// Add enemies to the spawn point container
	addEnemy(Character::Enemy, 0.f, 550.f);
	addEnemy(Character::Enemy, -40.f, 590.f);
	addEnemy(Character::Enemy, 400.f, 550.f);
	addEnemy(Character::Enemy, -440.f, 590.f);
	addEnemy(Character::Enemy, +190.f, 1100.f);
	addEnemy(Character::Enemy, -190.f, 1100.f);
	addEnemy(Character::Enemy, +490.f, 1100.f);
	addEnemy(Character::Enemy, -490.f, 1100.f);
	addEnemy(Character::Enemy, -270.f, 1400.f);
	addEnemy(Character::Enemy, 270.f, 1600.f);
	addEnemy(Character::Enemy, 70.f, 1400.f);
	addEnemy(Character::Enemy, 140.f, 650.f);
	addEnemy(Character::Enemy, -140.f, 600.f);
	addEnemy(Character::Enemy, +100.f, 1100.f);
	addEnemy(Character::Enemy, -100.f, 1100.f);
	addEnemy(Character::Enemy, -70.f, 1400.f);
	addEnemy(Character::Enemy, 70.f, 1600.f);

	// Sort all enemies according to their y value, such that lower enemies are checked first for spawning
	std::sort(mEnemySpawnPoints.begin(), mEnemySpawnPoints.end(), [](SpawnPoint lhs, SpawnPoint rhs)
	{
		return lhs.y < rhs.y;
	});
}

void World::addEnemy(Character::Type type, float relX, float relY)
{
	SpawnPoint spawn(type, mSpawnPosition.x + relX, mSpawnPosition.y - relY);
	mEnemySpawnPoints.push_back(spawn);
}

void World::spawnEnemies()
{
	// Spawn all enemies entering the view area (including distance) this frame
	while (!mEnemySpawnPoints.empty()
		&& mEnemySpawnPoints.back().y > getBattlefieldBounds().top)
	{
		SpawnPoint spawn = mEnemySpawnPoints.back();

		std::unique_ptr<Character> enemy(new Character(spawn.type, mTextures, mFonts));
		enemy->setPosition(spawn.x, spawn.y);

		mSceneLayers[Ground]->attachChild(std::move(enemy));

		// Enemy is spawned, remove from the list to spawn
		mEnemySpawnPoints.pop_back();
	}
}

void World::destroyEntitiesOutsideView()
{
	Command command;
	command.category = Category::Projectile | Category::EnemyCharacter;
	command.action = derivedAction<Entity>([this](Entity& e, sf::Time)
	{
		if (!getBattlefieldBounds().intersects(e.getBoundingRect()))
			e.destroy();
	});

	mCommandQueue.push(command);
}

//void World::guideMissiles()
//{
//	// Setup command that stores all enemies in mActiveEnemies
//	Command enemyCollector;
//	enemyCollector.category = Category::EnemyCharacter;
//	enemyCollector.action = derivedAction<Character>([this](Character& enemy, sf::Time)
//	{
//		if (!enemy.isDestroyed())
//			mActiveEnemies.push_back(&enemy);
//	});
//
//	// Setup command that guides all missiles to the enemy which is currently closest to the player
//	Command missileGuider;
//	missileGuider.category = Category::AlliedProjectile;
//	missileGuider.action = derivedAction<Projectile>([this](Projectile& missile, sf::Time)
//	{
//		// Ignore unguided bullets
//		if (!missile.isGuided())
//			return;
//
//		float minDistance = std::numeric_limits<float>::max();
//		Character* closestEnemy = nullptr;
//
//		// Find closest enemy
//		for(Character* enemy: mActiveEnemies)
//		{
//			float enemyDistance = distance(missile, *enemy);
//
//			if (enemyDistance < minDistance)
//			{
//				closestEnemy = enemy;
//				minDistance = enemyDistance;
//			}
//		}
//
//		if (closestEnemy)
//			missile.guideTowards(closestEnemy->getWorldPosition());
//	});
//
//	// Push commands, reset active enemies
//	mCommandQueue.push(enemyCollector);
//	mCommandQueue.push(missileGuider);
//	mActiveEnemies.clear();
//}

sf::FloatRect World::getViewBounds() const
{
	return sf::FloatRect(mWorldView.getCenter() - mWorldView.getSize() / 2.f, mWorldView.getSize());
}

sf::FloatRect World::getBattlefieldBounds() const
{
	// Return view bounds + some area at top, where enemies spawn
	sf::FloatRect bounds = getViewBounds();
	bounds.top -= 100.f;
	bounds.height += 100.f;

	return bounds;
}



void World::handleWallCollision(bool collision)
{
	if (collision)
	{
		mPlayerCharacter->destroy();
		tileCollision = false;
	}
}

void World::handleTileMapCollision()
{
	for (auto layer = mMap.GetLayers().begin(); layer != mMap.GetLayers().end();++layer)
	{
		if (layer->name == "Collision")
		{
			for (auto object = layer->objects.begin(); object != layer->objects.end(); object++)
			{
				if (object->GetAABB().intersects(mPlayerCharacter->getBoundingRect()))
				{
					const float borderDistance = 5.f;

					sf::Vector2f position = mPlayerCharacter->getPosition();
					sf::Vector2f speed = mPlayerCharacter->getVelocity();
					//try to use the velocity of the character to determine the angle
					if (speed.x != 0)
						position.x = position.x - ((speed.x / std::abs(speed.x)) * borderDistance);
					if (speed.y != 0)
						position.y = position.y - ((speed.y / std::abs(speed.y)) * borderDistance);
					mPlayerCharacter->setPosition(position);
				}
			}
		}
	}
}

