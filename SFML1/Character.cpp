#include "Character.h"
#include "DataTables.h"
#include "Utility.h"
#include "CommandQueue.h"
#include "Pickup.h"
#include "ResourceHolder.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <cmath>


namespace
{
	const std::vector<CharacterData> Table = initializeCharacterData();
}

Character::Character(Type type, const TextureHolder& textures, const FontHolder& fonts)
	: Entity(Table[type].hitpoints)
	, mType(type)
	, mSprite(textures.get(Table[type].texture))
	, mFireCommand()
	, mMissileCommand()
	, mFireCountdown(sf::Time::Zero)
	, mIsFiring(false)
	, mIsLaunchingMissile(false)
	, mIsMarkedForRemoval(false)
	, mFireRateLevel(1)
	, mSpreadLevel(1)
	, mMissileAmmo(1000)
	, mDropPickupCommand()
	, mTravelledDistance(0.f)
	, mDirectionIndex(0)
	, mHealthDisplay(nullptr)
	, mMissileDisplay(nullptr)
	,mSens(Sens::Up)
{
	if (mType == Character::Enemy)
	{
		mSprite.setTextureRect(sf::IntRect(0, 0, 32, 48));
		setSens(Sens::Down);
	}		
	else
		mSprite.setTextureRect(sf::IntRect(0, 144, 32, 48));
	centerOrigin(mSprite);

	mFireCommand.category = Category::Scene;
	mFireCommand.action = [this, &textures](SceneNode& node, sf::Time)
	{
		createBullets(node, textures);
	};

	/*mMissileCommand.category = Category::Scene;
	mMissileCommand.action = [this, &textures](SceneNode& node, sf::Time)
	{
		createProjectile(node, Projectile::Missile, 0.f, 0.5f, textures);
	};*/

	mDropPickupCommand.category = Category::Scene;
	mDropPickupCommand.action = [this, &textures](SceneNode& node, sf::Time)
	{
		createPickup(node, textures);
	};

	std::unique_ptr<TextNode> healthDisplay(new TextNode(fonts, ""));
	mHealthDisplay = healthDisplay.get();
	attachChild(std::move(healthDisplay));

	if (getCategory() == Category::PlayerCharacter)
	{
		std::unique_ptr<TextNode> missileDisplay(new TextNode(fonts, ""));
		missileDisplay->setPosition(0, 70);
		mMissileDisplay = missileDisplay.get();
		attachChild(std::move(missileDisplay));
	}

	updateTexts();
	initAnimation();
}

void Character::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	
	target.draw(mSprite, states);
}

void Character::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	// Entity has been destroyed: Possibly drop pickup, mark for removal
	if (isDestroyed())
	{
		checkPickUpDrop(commands);

		mIsMarkedForRemoval = true;
		return;
	}

	// Check if bullets or missiles are fired
	checkProjectileLaunch(dt, commands);

	// Update enemy movement pattern; apply velocity
	updateMovementPattern(dt);
	Entity::updateCurrent(dt, commands);
	animator.update(dt);
	// Update texts
	updateTexts();
}

unsigned int Character::getCategory() const
{
	if (isAllied())
		return Category::PlayerCharacter;
	else
		return Category::EnemyCharacter;
}

sf::FloatRect Character::getBoundingRect() const
{
	return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

bool Character::isMarkedForRemoval() const
{
	return mIsMarkedForRemoval;
}

bool Character::isAllied() const
{
	return mType == Main;
}

float Character::getMaxSpeed() const
{
	return Table[mType].speed;
}

void Character::setSens(Sens sens)
{
	mSens = sens;
}

Sens Character::getSens()
{
	return mSens;
}

void Character::increaseFireRate()
{
	if (mFireRateLevel < 10)
		++mFireRateLevel;
}

void Character::increaseSpread()
{
	if (mSpreadLevel < 3)
		++mSpreadLevel;
}

void Character::collectMissiles(unsigned int count)
{
	mMissileAmmo += count;
}

void Character::fire()
 {
	// Only ships with fire interval != 0 are able to fire
	if (Table[mType].fireInterval != sf::Time::Zero)
		mIsFiring = true;
}

void Character::launchMissile()
{
	if (mMissileAmmo > 0)
	{
		mIsLaunchingMissile = true;
		--mMissileAmmo;
	}
}



void Character::animate(Sens sens)
{
	animator.playAnimation(sens, false);
	animator.animate(mSprite);
}

void Character::updateMovementPattern(sf::Time dt)
{
	// Enemy airplane: Movement pattern
	const std::vector<Direction>& directions = Table[mType].directions;
	if (!directions.empty())
	{
		// Moved long enough in current direction: Change direction
		if (mTravelledDistance > directions[mDirectionIndex].distance)
		{
			mDirectionIndex = (mDirectionIndex + 1) % directions.size();
			mTravelledDistance = 0.f;
		}

		// Compute velocity from direction
		float radians = toRadian(directions[mDirectionIndex].angle + 90.f);
		float vx = getMaxSpeed() * std::cos(radians);
		float vy = getMaxSpeed() * std::sin(radians);

		setVelocity(vx, vy);

		mTravelledDistance += getMaxSpeed() * dt.asSeconds();
	}
	animate(mSens);
}

void Character::checkPickUpDrop(CommandQueue& commands)
{
	if (!isAllied() && randomInt(3) == 0)
		commands.push(mDropPickupCommand);
}

void Character::checkProjectileLaunch(sf::Time dt, CommandQueue& commands)
{
	// Enemies try to fire all the time
	if (!isAllied())
		fire();

	// Check for automatic gunfire, allow only in intervals
	if (mIsFiring && mFireCountdown <= sf::Time::Zero)
	{
		// Interval expired: We can fire a new bullet
		commands.push(mFireCommand);
		mFireCountdown += Table[mType].fireInterval / (mFireRateLevel + 1.f);
		mIsFiring = false;
	}
	else if (mFireCountdown > sf::Time::Zero)
	{
		// Interval not expired: Decrease it further
		mFireCountdown -= dt;
		mIsFiring = false;
	}

	// Check for missile launch
	if (mIsLaunchingMissile)
	{
		commands.push(mMissileCommand);
		mIsLaunchingMissile = false;
	}
}

void Character::createBullets(SceneNode& node, const TextureHolder& textures) const
{
	Projectile::Type type = isAllied() ? Projectile::AlliedBullet : Projectile::EnemyBullet;

	switch (mSpreadLevel)
	{
	case 1:
		switch (mSens)
		{
		case Sens::Default:
			break;
		case Sens::Up:
			createProjectile(node, type, 0.0f, 0.5f, textures);
			break;
		case Sens::Down:
			createProjectile(node, type, 0.0f, -0.5f, textures);
			break;
		case Sens::Left:
			createProjectile(node, type, 0.0f, 0.0f, textures);
			break;
		case Sens::Right:
			createProjectile(node, type, 0.0f, 0.0f, textures);
			break;
		default:
			break;
		}
		//createProjectile(node, type, 0.0f, 0.5f, textures);
		break;

	case 2:
		switch (mSens)
		{
		case Sens::Default:
			break;
		case Sens::Up:
			createProjectile(node, type, -0.33f, 0.33f, textures);
			createProjectile(node, type, +0.33f, 0.33f, textures);
			break;
		case Sens::Down:
			createProjectile(node, type, -0.33f, 0.33f, textures);
			createProjectile(node, type, +0.33f, 0.33f, textures);
			break;
		case Sens::Left:
			createProjectile(node, type, 0.f, -0.33f, textures);
			createProjectile(node, type, 0.f, 0.33f, textures);
			break;
		case Sens::Right:
			createProjectile(node, type, 0.f, -0.33f, textures);
			createProjectile(node, type, 0.f, 0.33f, textures);
			break;
		default:
			break;
		}
		break;

	case 3:
		switch (mSens)
		{
		case Sens::Default:
			break;
		case Sens::Up:
			createProjectile(node, type, -0.5f, 0.33f, textures);
			createProjectile(node, type, 0.0f, 0.5f, textures);
			createProjectile(node, type, +0.5f, 0.33f, textures);
			break;
		case Sens::Down:
			createProjectile(node, type, -0.5f, 0.33f, textures);
			createProjectile(node, type, 0.0f, 0.5f, textures);
			createProjectile(node, type, +0.5f, 0.33f, textures);
			break;
		case Sens::Left:
			createProjectile(node, type, 0.33f,-0.5f , textures);
			createProjectile(node, type, 0.5f, 0.0f, textures);
			createProjectile(node, type, 0.33f, +0.5f, textures);
			break;
		case Sens::Right:
			createProjectile(node, type, 0.33f, -0.5f, textures);
			createProjectile(node, type, 0.5f, 0.0f, textures);
			createProjectile(node, type, 0.33f, +0.5f, textures);
			break;
		default:
			break;
		}
		
		break;
	}
}

void Character::createProjectile(SceneNode& node, Projectile::Type type, float xOffset, float yOffset, const TextureHolder& textures) const
{//modify to allow the bullet to go in the direction the character is facing
	std::unique_ptr<Projectile> projectile(new Projectile(type, textures));

	sf::Vector2f offset(xOffset * mSprite.getGlobalBounds().width, yOffset * mSprite.getGlobalBounds().height);
	sf::Vector2f velocity;
	switch (mSens)
	{
	case Sens::Default:
		break;
	case Sens::Up:
		velocity=sf::Vector2f(0, -projectile->getMaxSpeed());
		break;
	case Sens::Down:
		velocity = sf::Vector2f(0, projectile->getMaxSpeed());
		break;
	case Sens::Left:
		velocity = sf::Vector2f(-projectile->getMaxSpeed(), -1.f );
		break;
	case Sens::Right:
		velocity = sf::Vector2f(projectile->getMaxSpeed(), -1.f );		
		break;
	default:
		velocity = sf::Vector2f(0, -projectile->getMaxSpeed());
		break;
	}
	

	//float sign = isAllied() ? -1.f : +1.f;
	projectile->setPosition(getWorldPosition() + offset /** sign*/);
	projectile->setVelocity(velocity );
	node.attachChild(std::move(projectile));
}

void Character::createPickup(SceneNode& node, const TextureHolder& textures) const
{
	auto type = static_cast<Pickup::Type>(randomInt(Pickup::TypeCount));

	std::unique_ptr<Pickup> pickup(new Pickup(type, textures));
	pickup->setPosition(getWorldPosition());
	pickup->setVelocity(0.f, 1.f);
	node.attachChild(std::move(pickup));
}

void Character::updateTexts()
{
	mHealthDisplay->setString(std::to_string(getHitpoints()) + " HP");
	mHealthDisplay->setPosition(0.f, 50.f);
	mHealthDisplay->setRotation(-getRotation());

	/*if (mMissileDisplay)
	{
		if (mMissileAmmo == 0)
			mMissileDisplay->setString("");
		else
			mMissileDisplay->setString("M: " + std::to_string(mMissileAmmo));
	}*/
}

void Character::initAnimation()
{
	for (size_t i = 0; i < 4; i++)
	{
		thor::FrameAnimation frame;
		frame.addFrame(1.f, sf::IntRect(0, 48 * i, 32, 48));
		frame.addFrame(1.f, sf::IntRect(32, 48 * i, 32, 48));
		frame.addFrame(1.f, sf::IntRect(64, 48 * i, 32, 48));
		frame.addFrame(1.f, sf::IntRect(96, 48 * i, 32, 48));
		frameAnimation.push_back(frame);
	}
	animator.addAnimation(Sens::Down, frameAnimation[0], sf::seconds(0.1f));
	animator.addAnimation(Sens::Left, frameAnimation[1], sf::seconds(0.1f));
	animator.addAnimation(Sens::Right, frameAnimation[2], sf::seconds(0.1f));
	animator.addAnimation(Sens::Up, frameAnimation[3], sf::seconds(0.1f));
}



