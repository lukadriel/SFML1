#include "PauseState.h"
#include "ResourceHolder.h"
#include "Utility.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

PauseState::PauseState(StateStack & stack, Context context)
	:State(stack,context),mBackgroundSprite(),mPausedText(),mInstructionText()
{
	sf::Font& font = context.fonts->get(Fonts::Main);
	sf::Vector2f viewSize = context.window->getView().getSize();

	mPausedText.setFont(font);
	mPausedText.setString("Pause");
	centerOrigin(mPausedText);
	mPausedText.setPosition(0.5f * viewSize.x, 0.4f * viewSize.y);

	mInstructionText.setFont(font);
	mInstructionText.setString("Press backspace to return to main menu");
	centerOrigin(mInstructionText);
	mInstructionText.setPosition(0.7f * viewSize.x, 0.6f * viewSize.y);
	mInstructionText.setCharacterSize(20);

}

void PauseState::draw()
{
	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());

	sf::RectangleShape backgroundShape;
	backgroundShape.setFillColor(sf::Color(0, 0, 0, 150));
	backgroundShape.setSize(window.getView().getSize());

	window.draw(backgroundShape);
	window.draw(mPausedText);
	window.draw(mInstructionText);
}

bool PauseState::update(sf::Time dt)
{
	return false;
}

bool PauseState::handleEvent(const sf::Event & event)
{
	if (event.type != sf::Event::KeyPressed)
		return false;
	if (event.key.code==sf::Keyboard::Escape)
	{
		requestStackPop();
	}

	if (event.key.code==sf::Keyboard::BackSpace)
	{
		requestStateClear();
		requestStackPush(States::Menu);
	}
	return false;
}