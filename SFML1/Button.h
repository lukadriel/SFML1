#pragma once
#include "Component.h"
#include "ResourceHolder.h"
#include "ResourceIdentifier.h"

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <vector>
#include <functional>

namespace GUI
{
	class Button :public Component
	{
	public:
		typedef std::shared_ptr<Button> Ptr;
		typedef std::function<void()> Callback;
	public:
		Button(const FontHolder& fonts, const TextureHolder& textures);
		void setCallback(Callback callback);
		void setText(const std::string& text);
		void setToggle(bool flag);
		virtual bool isSelectable() const;
		virtual void select();
		virtual void deselect();
		virtual void activate();
		virtual void deactivate();
		virtual void handleEvent(const sf::Event& event);

	private:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	private:
		Callback mCallback;
		sf::Sprite mSprite;
		sf::Texture mSelectedTexture, mNormalTexture, mPressedTexture;
		sf::Text mText;
		bool mIsToggle;
	};
}