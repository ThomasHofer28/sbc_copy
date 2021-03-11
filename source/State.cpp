#include "State.h"
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

void State::init(StateManager* manager, std::shared_ptr<sf::RenderWindow> window)
{
    mGui.setTarget(*window);
	mManager = manager;
}

void State::draw(sf::RenderWindow& window)
{
	mGui.draw();
}

void State::onEvent(sf::Event& event)
{
	mGui.handleEvent(event);
}
