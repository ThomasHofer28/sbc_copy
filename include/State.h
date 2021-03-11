#pragma once
#include <SFML\Graphics\RenderWindow.hpp>
#include "StateManager.h"
#include <TGUI/TGUI.hpp>
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

class GameStateManager;
class Game;

class State
{
public:
	virtual void init(StateManager* manager, std::shared_ptr<sf::RenderWindow> window) = 0;
	virtual void exit() = 0;
	virtual void update(sf::Time delta) = 0;
	virtual void draw(sf::RenderWindow& window);

	void onEvent(sf::Event& event);

protected:
	StateManager* mManager;
	tgui::Gui mGui;
};