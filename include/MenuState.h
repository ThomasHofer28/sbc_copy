#pragma once
#include "State.h"
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

class Gamepad;

class MenuState : public State
{
public:
	MenuState() = default;
	void init(StateManager* manager, std::shared_ptr<sf::RenderWindow> window) override;
	void exit() override;
	void update(sf::Time delta) override;

private:
	Gamepad* mGamepad1 = nullptr;
	Gamepad* mGamepad2 = nullptr;
	int mMenuControl = 1;
	sf::RenderWindow* mWindow = nullptr;
	bool mMenuSwitch = true;
	bool mMadeGamepad1 = false;
	bool mMadeGamepad2 = false;
	bool mMenuBuffer = false;
	float mMenuTimer = 0.0f;
	float mMenuBufferTimer = 2.0f;
};