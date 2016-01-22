#include "PauseState.h"
#include "ResourceHolder.h"
#include "Button.h"
#include "Utility.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

PauseState::PauseState(StateStack & stack, Context context)
	:State(stack, context), mBackgroundSprite(),
	mGUIContainer(),
	mPausedText()
{
	sf::Font& font = context.fonts->get(Fonts::Main);
	sf::Vector2f windowSize(context.window->getSize());

	mPausedText.setFont(font);
	mPausedText.setString("Game Paused");
	mPausedText.setCharacterSize(70);
	centerOrigin(mPausedText);
	mPausedText.setPosition(0.5f*windowSize.x, 0.4f*windowSize.y);

	auto returnButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
	returnButton->setPosition(0.5f*windowSize.x-100.f, 0.4f*windowSize.y+75.f);
	returnButton->setText("Return");
	returnButton->setCallback([this]()
	{
		requestStackPop();
	});

	auto menuButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
	menuButton->setPosition(0.5f*windowSize.x - 100, 0.4f*windowSize.y+ 125.f);
	menuButton->setText("Back To Menu");
	menuButton->setCallback([this]()
	{
		requestStateClear();
		requestStackPush(States::Menu);
	});

	mGUIContainer.pack(returnButton);
	mGUIContainer.pack(menuButton);

	

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
	window.draw(mGUIContainer);
	
}

bool PauseState::update(sf::Time dt)
{
	return false;
}

bool PauseState::handleEvent(const sf::Event & event)
{
	mGUIContainer.handleEvent(event);
	return false;
}
