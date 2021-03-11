// GAMEDEV - Engine
// Femböck Samuel; fhs44393
// Hofer Thomas; fhs44401

#pragma once
#include <string>
#include <map>
class State;

class GameStateManager 
{
public:
	GameStateManager();
	void update(float delta);
	void draw();
	void setState(std::string stateName);
	void registerState(std::string, State*);

private:
	std::map<std::string, State*> mStates;
	State* mCurrentState;
};