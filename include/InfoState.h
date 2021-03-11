#pragma once
#include "State.h"
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

class InfoState : public State
{
public:
	InfoState() = default;
	void init(StateManager* manager, std::shared_ptr<sf::RenderWindow> window) override;
	void exit() override;
	void update(sf::Time delta) override;

private:
	bool mInfoBuffer = false;

};