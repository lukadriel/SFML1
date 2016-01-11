#include "StateStack.h"







State::Ptr StateStack::createState(States::ID stateID)
{
	auto found = mFactories.find(stateID);
	assert(found != mFactories.end());
	return found->second();
}

void StateStack::applyPendingChanges()
{
	for (PendingChange change : mPendingList)
	{
		switch (change.action)
		{
		case Push:
			mStack.push_back(createState(change.stateID));
			break;
		case Pop:
			mStack.pop_back();
			break;
		case Clear:
			mStack.clear();
			break;
		}
	}
	mPendingList.clear();
}

void StateStack::handleEvent(const sf::Event & event)
{
	for (auto itr = mStack.rbegin(); itr != mStack.rend(); itr++)
	{
		if (!(*itr)->handleEvent(event))
			return;
	}
	applyPendingChanges();
}
