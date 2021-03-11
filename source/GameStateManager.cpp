// GAMEDEV - Engine
// Femböck Samuel; fhs44393
// Hofer Thomas; fhs44401

#pragma once
#include "GameStateManager.h"
#include "MenuState.h"
#include <iostream>

void GameStateManager::setState(std::string stateName) 
{ 
	State* state = mStates[stateName];
	
	if (state != mCurrentState) 
	{
		if (mCurrentState != nullptr) 
		{ 
			mCurrentState->exit(); 
		}

		mCurrentState = state; 
		mCurrentState->init(this); 
	} 
}

void GameStateManager::registerState(std::string key, State* gameState)
{
	mStates.insert(std::pair<std::string, State*>(key, gameState));
}

GameStateManager::GameStateManager() : mCurrentState(nullptr) 
{

}

void GameStateManager::update(float deltaTime)
{
	mCurrentState->update(deltaTime);
}

void GameStateManager::draw()
{
	mCurrentState->draw();
}
