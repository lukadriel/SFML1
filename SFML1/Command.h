#pragma once
#include <functional>
#include <SFML/System/Time.hpp>
#include <cassert>
#include "Category.h"

class SceneNode;

struct Command
{
	Command();
	typedef std::function<void(SceneNode&, sf::Time)> Action;
	unsigned int category;
	Action action;
	
};

template <typename GameObject,typename Function>
Command::Action derivedAction(Function fn)
{
	return [=](SceneNode& node, sf::Time dt)
	{
		assert(dynamic_cast<GameObject*>(&node) != nullptr);
		fn(static_cast<GameObject&>(node), dt);
	};
}