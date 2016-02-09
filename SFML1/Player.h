#pragma once
#include <SFML/Window/Event.hpp>
#include <map>
#include "Command.h"
#include "Character.h"

class CommandQueue;

class Player
{
public:
	enum Action
	{
		MoveLeft,
		MoveRight,
		MoveUp,
		MoveDown,
		Fire,
		/*LaunchMissile,*/
		ActionCount
	};
	enum StageStatus
	{
		Running,
		Success,
		Failure
	};
public:
	Player();
	void handleEvent(const sf::Event& event,CommandQueue& commands);
	void handleRealTimeInput(CommandQueue& commands);
	void assignKey(Action action, sf::Keyboard::Key key);
	sf::Keyboard::Key getAssignedKey(Action action) const;
	void setStageStatus(StageStatus status);
	StageStatus getStageStatus() const;


private:
	void initializeActions();
	static bool isRealTimeAction(Action action);

private:
	std::map<sf::Keyboard::Key, Action> mKeyBinding;
	std::map<Action, Command> mActionBinding;
	StageStatus mCurrentStageStatus;
};

