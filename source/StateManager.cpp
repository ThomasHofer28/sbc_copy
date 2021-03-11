#include "StateManager.h"
#include "MenuState.h"
#include "GameState.h"
#include "EndState.h"
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

StateManager& StateManager::getInstance()
{
	static StateManager instance;
	return instance;
}

void StateManager::setState(std::string stateName)
{
    State* state = mStates[stateName];

    if (state != mCurrentState)
    {
        if (mCurrentState != nullptr)
        {
            mCurrentState->exit();
        }

        mCurrentState = state;
        mCurrentState->init(this, mWindow);
    }
}

std::map<std::string, State*>& StateManager::getStates()
{
    return mStates;
}

State* StateManager::getCurrentState()
{
    return mCurrentState;
}

void StateManager::setWindow(std::shared_ptr<sf::RenderWindow> window)
{
    mWindow = window;
}

void StateManager::onEvent(sf::Event event)
{
    mCurrentState->onEvent(event);
}

void StateManager::registerState(std::string key, State* gameState)
{
    mStates.insert(std::pair<std::string, State*>(key, gameState));
}