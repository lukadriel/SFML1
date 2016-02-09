#pragma once
#include "State.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class TitleState : public State
{
public:
	TitleState(StateStack& stack, Context context);

	virtual void		draw();
	virtual bool		update(sf::Time dt);
	virtual bool		handleEvent(const sf::Event& event);
	


private:
	sf::Sprite			mBackgroundSprite;
	sf::Text			mText;
	sf::Sprite mSfmlCredit;
	sf::Sprite mTiledCredit;

	bool				mShowText;
	sf::Time			mTextEffectTime;

	bool mCreditShown[2];
	sf::Time mCreditShowTime;
};

