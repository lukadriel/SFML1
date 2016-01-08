#include "Game.h"

const sf::Time Game::TimePerFrame = sf::seconds(1.f / 60.f);

Game::Game():mWindow(sf::VideoMode(640,480),"SFML"),mWorld(mWindow),
mPlayer()
{
	mWindow.setKeyRepeatEnabled(false);
}

void Game::run()
{
	sf::Clock clock;
	
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	while (mWindow.isOpen())
	{
		sf::Time elapsedTime = clock.restart();
		timeSinceLastUpdate += elapsedTime;
		while (timeSinceLastUpdate>TimePerFrame)
		{
			timeSinceLastUpdate -= TimePerFrame;
			processInput();
			update(TimePerFrame);
		}
		render();
	}
	
}




void Game::processInput()
{
	CommandQueue& commands = mWorld.getCommandQueue();
	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		mPlayer.handleEvent(event, commands);
		if (event.type == sf::Event::Closed)
		{
			mWindow.close();
		}
	}
	mPlayer.handleRealTimeInput(commands);

}

void Game::update(sf::Time deltaTime)
{

	/*sf::Vector2f movement(0.f, 0.f);
	if (mIsMovingUp)
	{
		movement.y -= SPEED;
	}		
	if (mIsMovingDown)
	{
		movement.y += SPEED;
	}		
	if (mIsMovingLeft)
	{
		movement.x -= SPEED;
	}
	if (mIsMovingRight)
	{
		movement.x += SPEED;
	}
	if (!mIsMoving)
	{
		animator.stopAnimation();
	}		
	else
	{
		animator.update(deltaTime);
		animator.animate(mSprite);
	}
	
	mSprite.move(movement*deltaTime.asSeconds());*/
	mWorld.update(deltaTime);
}

void Game::render()
{
	mWindow.clear();
	mWorld.draw();
	//mWindow.draw(mSprite);
	mWindow.setView(mWindow.getDefaultView());
	mWindow.display();
}

/*void Game::handlePlayerInput(sf::Keyboard::Key key, bool isPressed)
{
	if (key == sf::Keyboard::W)
	{
		mIsMovingUp = isPressed;
		if (animator.isPlayingAnimation())
			animator.stopAnimation();
		animator.playAnimation("up",true);
	}		
	else if (key == sf::Keyboard::S)
	{
		mIsMovingDown = isPressed;
		if (animator.isPlayingAnimation())
			animator.stopAnimation();
		animator.playAnimation("down", true);
	}		
	else if (key == sf::Keyboard::A)
	{
		mIsMovingLeft = isPressed;
		if (animator.isPlayingAnimation())
			animator.stopAnimation();
		animator.playAnimation("left", true);
	}		
	else if (key == sf::Keyboard::D)
	{
		mIsMovingRight = isPressed;
		if (animator.isPlayingAnimation())
			animator.stopAnimation();
		animator.playAnimation("right", true);
	}
	mIsMoving = mIsMovingDown || mIsMovingLeft || mIsMovingRight || mIsMovingUp;
		
}

void Game::set_Animation(std::vector<thor::FrameAnimation>& animation, thor::Animator<sf::Sprite, std::string>& movement)
{
	std::vector <thor::FrameAnimation> fAnimation(4);
	animation = fAnimation;
	for (int i = 0; i<4; i++)
	{
		animation[i].addFrame(1.f, sf::IntRect(0, 48 * i, 32, 48));
		animation[i].addFrame(1.f, sf::IntRect(32, 48 * i, 32, 48));
		animation[i].addFrame(1.f, sf::IntRect(64, 48 * i, 32, 48));
		animation[i].addFrame(1.f, sf::IntRect(96, 48 * i, 32, 48));
	}
	movement.addAnimation("down", animation[DOWN], sf::seconds(0.5f));
	movement.addAnimation("left", animation[LEFT], sf::seconds(0.5f));
	movement.addAnimation("right", animation[RIGHT], sf::seconds(0.5f));
	movement.addAnimation("up", animation[UP], sf::seconds(0.5f));
}*/
