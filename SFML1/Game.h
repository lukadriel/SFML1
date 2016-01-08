#pragma once
#include <iostream>
#include <vector>
#include <SFML\Graphics.hpp>
#include <Thor\Animations.hpp>
#include <Thor\Input.hpp>
#include "World.h"
#include "Player.h"

#define DOWN 0
#define LEFT 1
#define RIGHT 2
#define UP 3

#define SPEED 100.f

class Game: private sf::NonCopyable
{
public:
	Game();
	void run();
private:
	void processInput();
	void update(sf::Time deltaTime);
	void render();
	//void handlePlayerInput(sf::Keyboard::Key key, bool isPressed);
	//void set_Animation(std::vector <thor::FrameAnimation> &animation, thor::Animator<sf::Sprite, std::string> &movement);
private:
	/*bool mIsMovingUp=false, mIsMovingDown=false, mIsMovingLeft=false, mIsMovingRight=false, mIsMoving=false;
	sf::Texture mTexture;
	sf::Sprite mSprite;
	std::vector<thor::FrameAnimation> animation;
	thor::Animator<sf::Sprite, std::string> animator;*/
	static const sf::Time TimePerFrame;
	sf::RenderWindow mWindow;
	World mWorld;
	Player mPlayer;
};

