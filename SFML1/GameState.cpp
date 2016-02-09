#include "GameState.h"

GameState::GameState(StateStack & stack, Context context)
	:State(stack,context),mWorld(*context.window,*context.fonts)
	,mPlayer(*context.player)
{
	mPlayer.setStageStatus(Player::Running);
}

void GameState::draw()
{
	mWorld.draw();
}

bool GameState::update(sf::Time dt)
{
	mWorld.update(dt);
	if (!mWorld.hasAlivePlayer())
	{
		mPlayer.setStageStatus(Player::Failure);
		requestStackPush(States::GameOver);
	}
	else if (mWorld.hasPlayerReachedEnd())
	{
		mPlayer.setStageStatus(Player::Success);
		requestStackPush(States::GameOver);
	}
	CommandQueue& commands = mWorld.getCommandQueue();
	mPlayer.handleRealTimeInput(commands);
	return true;
}

bool GameState::handleEvent(const sf::Event& event)
{
	CommandQueue& commands = mWorld.getCommandQueue();
	mPlayer.handleEvent(event,commands);

	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
		requestStackPush(States::Pause);
	return true;
}
