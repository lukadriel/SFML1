#pragma once
#include "SceneNode.h"
#include "ResourceIdentifier.h"
#include "ResourceHolder.h"
class TextNode :
	public SceneNode
{
public:
	explicit TextNode(const FontHolder& fonts, const std::string& text);
	void setString(const std::string& text);
	
private:
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	sf::Text mText;
};

