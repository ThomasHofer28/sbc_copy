#pragma once
#include <vector>
#include <memory>
#include "GameObject.h"
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

class sf::RenderWindow;

class GameObjectManager
{
public:
	static GameObjectManager& getInstance();
	void update(sf::Time delta);
	void physicsUpdate(sf::Time delta);
	void updateLists();
	void add(std::unique_ptr<GameObject> go);
	void clear();
	GameObject* getObject(std::string name);
	std::vector<GameObject*> getObjects(std::string name);
	std::vector<std::unique_ptr<GameObject>>& getObjects();

private:
	GameObjectManager() = default;
	std::vector<std::unique_ptr<GameObject>> mObjects;
	std::vector<std::unique_ptr<GameObject>> mToAdd;
};