#include "GameObjectManager.h"
#include <iostream>
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

GameObjectManager& GameObjectManager::getInstance()
{
	static GameObjectManager instance;
	return instance;
}

void GameObjectManager::update(sf::Time delta)
{
	updateLists();

	auto it = mObjects.begin();
	while (it != mObjects.end())
	{
		if ((*it)->isMarkedForDelete())
		{
			it = mObjects.erase(it);
		}
		else
		{
			if ((*it)->isEnabled())
				(*it)->update(delta);

			++it;
		}
	}
}

void GameObjectManager::physicsUpdate(sf::Time delta)
{
	for (const auto& obj : mObjects)
	{
		obj->physicsUpdate(delta);
	}
}

void GameObjectManager::updateLists()
{
	for (auto& gameObject : mToAdd)
	{
		mObjects.push_back(std::move(gameObject));
	}

	mToAdd.clear();
}

void GameObjectManager::add(std::unique_ptr<GameObject> object)
{
	mToAdd.push_back(std::move(object));
}

void GameObjectManager::clear()
{
	mObjects.clear();
}

GameObject* GameObjectManager::getObject(std::string name)
{
	for (auto& object : mObjects)
	{
		if (object->getName() == name)
		{
			return object.get();
		}
	}

	return nullptr;
}

std::vector<GameObject*> GameObjectManager::getObjects(std::string name)
{
	std::vector<GameObject*> ret;

	for (const auto& obj : mObjects)
	{
		if (obj->getName() == name)
			ret.push_back(obj.get());
	}

	return ret;
}

std::vector<std::unique_ptr<GameObject>>& GameObjectManager::getObjects()
{
	return mObjects;
}
