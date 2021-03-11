#pragma once
#include "Timer.h"
#include <memory>
#include <list>
#include <vector>
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

class TimerManager
{
public:
	static TimerManager& getInstance();
	void update();
	void create(sf::Time duration, std::function<void()> callback);

private:
	std::vector<std::unique_ptr<Timer>> mTimers;
};

