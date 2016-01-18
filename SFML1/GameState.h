#pragma once
#include "World.h"
#include "Player.h"
#include "State.h"

#include <SFML/Graphics/Text.hpp>
#include<SFML/Graphics/Sprite.hpp>


class GameState:public State
{
public:
	GameState(StateStack& stack,Context context);
	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event& event);

private:
	World mWorld;
	Player& mPlayer;
};

