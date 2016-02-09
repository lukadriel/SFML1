#pragma once
#include "Entity.h"
#include "ResourceIdentifier.h"
#include "Projectile.h"
#include "TextNode.h"
#include "Command.h"

#include <SFML/Graphics/Sprite.hpp>

class Character : public Entity
{
public:
	enum Type
	{
		Main,
		Enemy,
		Allied,
		TypeCount,
	};
	Character(Type type, const TextureHolder& textures, const FontHolder& fonts);
	virtual unsigned int getCategory() const;
	virtual sf::FloatRect getBoundingRect() const;
	virtual bool isMarkedForRemoval() const;
	bool isAllied() const;
	float getMaxSpeed() const;
	void setSens(Sens sens);
	Sens getSens();

	void increaseFireRate();
	void increaseSpread();
	void collectMissiles(unsigned int count);
	void fire();
	void launchMissile();
	
	
private:
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void updateCurrent(sf::Time dt, CommandQueue& commands);
	void updateMovementPattern(sf::Time dt);
	void checkPickUpDrop(CommandQueue& commands);
	void checkProjectileLaunch(sf::Time dt, CommandQueue& commands);
	void animate(Sens sens);

	void createBullets(SceneNode& node, const TextureHolder& textures) const;
	void createProjectile(SceneNode& node, Projectile::Type type, float xOffset, float yOffset, const TextureHolder& textures) const;// will have to change 
	void createPickup(SceneNode& node, const TextureHolder& textures) const;
	void updateTexts();

	void initAnimation();
private:
	Type mType;
	sf::Sprite mSprite;
	Command mFireCommand;
	Command mMissileCommand;
	sf::Time mFireCountdown;
	bool mIsFiring;
	bool mIsLaunchingMissile;
	bool mIsMarkedForRemoval;
	int mFireRateLevel;
	int mSpreadLevel;
	int mMissileAmmo;
	Command mDropPickupCommand;
	float mTravelledDistance;
	std::size_t mDirectionIndex;
	TextNode* mHealthDisplay;
	TextNode* mMissileDisplay;
	Sens mSens;
	thor::Animator<sf::Sprite, Sens> animator;
	std::vector<thor::FrameAnimation> frameAnimation;
};

