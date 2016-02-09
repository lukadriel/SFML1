#include "Player.h"
#include "CommandQueue.h"
#include "Character.h"
#include <string>
#include <algorithm>
struct CharacterMover
{
	CharacterMover(float vx, float vy) :velocity(vx, vy)
	{

	}
	void operator() (Character& character, sf::Time dt)const
	{
		//Character& character = static_cast<Character&>(node);
		character.accelerate(velocity * character.getMaxSpeed());
		if (velocity.x < 0)
		{
			character.setSens(Sens::Left);
			//character.animate(Sens::Left);
		}			
		else if (velocity.x>0)
		{
			character.setSens(Sens::Right);
			//character.animate(Sens::Right);
		}			
		else if (velocity.y < 0)
		{
			character.setSens(Sens::Up);
			//character.animate(Sens::Up);
		}			
		else if (velocity.y > 0)
		{
			character.setSens(Sens::Down);
			//character.animate(Sens::Down);
		}
			
	}

	sf::Vector2f velocity;
};

Player::Player():mCurrentStageStatus(Running)
{
	mKeyBinding[sf::Keyboard::A] = MoveLeft;
	mKeyBinding[sf::Keyboard::D] = MoveRight;
	mKeyBinding[sf::Keyboard::W] = MoveUp;
	mKeyBinding[sf::Keyboard::S] = MoveDown;
	mKeyBinding[sf::Keyboard::Space] = Fire;
	/*mKeyBinding[sf::Keyboard::M] = LaunchMissile;*/

	initializeActions();
	for (auto& pair : mActionBinding)
	{
		pair.second.category = Category::PlayerCharacter;
	}
}

void Player::handleEvent(const sf::Event& event, CommandQueue& commands)
{
	if (event.type == sf::Event::KeyPressed)
	{
		auto found = mKeyBinding.find(event.key.code);
		if (found!=mKeyBinding.end() && isRealTimeAction(found->second))
		{
			commands.push(mActionBinding[found->second]);
		}
	}
}

void Player::handleRealTimeInput(CommandQueue& commands)
{
	for (auto pair:mKeyBinding)
	{
		if (sf::Keyboard::isKeyPressed(pair.first) && isRealTimeAction(pair.second))
		{
			commands.push(mActionBinding[pair.second]);
		}
	}
}

void Player::assignKey(Action action, sf::Keyboard::Key key)
{
	for (auto itr = mKeyBinding.begin(); itr != mKeyBinding.end();)
	{
		if (itr->second == action)
			mKeyBinding.erase(itr++);
		else
			++itr;
	}
	mKeyBinding[key] = action;
}

sf::Keyboard::Key Player::getAssignedKey(Action action) const
{
	for(auto pair: mKeyBinding)
	{
		if (pair.second == action)
			return pair.first;
	}

	return sf::Keyboard::Unknown;
}

void Player::setStageStatus(StageStatus status)
{
	mCurrentStageStatus = status;
}

Player::StageStatus Player::getStageStatus() const
{
	return mCurrentStageStatus;
}

void Player::initializeActions()
{
	const float playerSpeed = 1.f;

	mActionBinding[MoveLeft].action = derivedAction<Character>(CharacterMover(-playerSpeed, 0.f));
	mActionBinding[MoveRight].action = derivedAction<Character>(CharacterMover(+playerSpeed, 0.f));
	mActionBinding[MoveUp].action = derivedAction<Character>(CharacterMover( 0.f,-playerSpeed));
	mActionBinding[MoveDown].action = derivedAction<Character>(CharacterMover(0.f,+playerSpeed ));
	mActionBinding[Fire].action = derivedAction<Character>([](Character& c, sf::Time) {c.fire(); });
	/*mActionBinding[LaunchMissile].action = derivedAction<Character>([](Character& c, sf::Time) {c.launchMissile(); });*/
}

bool Player::isRealTimeAction(Action action)
{
	switch (action)
	{
	case Player::MoveLeft:
	case Player::MoveRight:
	case Player::MoveUp:
	case Player::MoveDown:
	case Player::Fire:
	/*case Player::LaunchMissile:*/
		return true;
	default: return false;
	}
}