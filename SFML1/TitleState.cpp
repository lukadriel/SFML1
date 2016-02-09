#include "TitleState.h"
#include "ResourceHolder.h"
#include "Utility.h"

TitleState::TitleState(StateStack& stack, Context context)
	: State(stack, context)
	, mText()
	, mShowText(true)
	, mCreditShown{ false,true }
	, mTextEffectTime(sf::Time::Zero)
	,mCreditShowTime(sf::Time::Zero)
{
	mBackgroundSprite.setTexture(context.textures->get(Textures::TitleScreen));
	mSfmlCredit.setTexture(context.textures->get(Textures::CreditSFML));
	mTiledCredit.setTexture(context.textures->get(Textures::CreditTILED));
	mText.setFont(context.fonts->get(Fonts::Main));
	mText.setString("Press any key to start");
	centerOrigin(mText);
	mText.setPosition(sf::Vector2f(context.window->getSize() / 2u));
	
}

void TitleState::draw()
{
	sf::RenderWindow& window = *getContext().window;
	if (!mCreditShown[0])
	{
		window.draw(mSfmlCredit);
	}
	if (!mCreditShown[1])
	{
		window.draw(mTiledCredit);
	}
	if (mCreditShown[0] && mCreditShown[1])
	{
		window.draw(mBackgroundSprite);
		if (mShowText)
			window.draw(mText);
	}
	
}

bool TitleState::update(sf::Time dt)
{
	mTextEffectTime += dt;
	mCreditShowTime += dt;
	if (mTextEffectTime >= sf::seconds(0.5f))
	{
		mShowText = !mShowText;
		mTextEffectTime = sf::Time::Zero;
	}
	if (mCreditShowTime >= sf::seconds(5.f))
	{
		mCreditShown[0] = true;
		mCreditShown[1] = false;
	}
	if (mCreditShowTime >= sf::seconds(10.f))
	{
		mCreditShown[1] = true;
	}
	return true;
}

bool TitleState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::KeyReleased)
	{
		requestStackPop();
		requestStackPush(States::Menu);
	}

	return true;
}


