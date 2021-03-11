#pragma once
#include "State.h"
// (c) Coding: Femb�ck Samuel, Hofer Thomas
// FH - Salzburg

class CreditsState : public State
{
public:
	CreditsState() = default;
	void init(StateManager* manager, std::shared_ptr<sf::RenderWindow> window) override;
	void exit() override;
	void update(sf::Time delta) override;
private:
	bool mJustOnce = true;
};