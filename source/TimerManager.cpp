#include "TimerManager.h"
#include <iostream>
#include "StateManager.h"
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

TimerManager& TimerManager::getInstance()
{
	static TimerManager instance;
	return instance;
}

void TimerManager::update()
{
	std::vector<std::unique_ptr<Timer>>::iterator it;
	for (it = mTimers.begin(); it != mTimers.end();)
	{
		if ((*it)->isFinished())
		{
			(*it)->getCallBack()();
			it = mTimers.erase(it);
		}
		else
		{
			it++;
		}
	}
}

void TimerManager::create(sf::Time duration, std::function<void()> callback)
{
	Timer* timer = new Timer(duration, callback);
	mTimers.push_back(std::unique_ptr<Timer>(timer));
}
