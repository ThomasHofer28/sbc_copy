#pragma once
#include "State.h"
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

class IntroState : public State
{
public:
	IntroState() = default;
	void init(StateManager* manager, std::shared_ptr<sf::RenderWindow> window) override;
	void exit() override;
	void update(sf::Time delta) override;
private:
	bool mJustOnce = true;
};