#pragma once
#include <string>
#include <SFML\Graphics\RenderWindow.hpp>
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

class GameObject;

class Component
{
public:
	bool isEnabled();
	void setEnabled(bool enabled);
	bool isDestroyed();
	GameObject* getGameObject();

	virtual ~Component() = 0;
	virtual void onAdd(GameObject* go);
	virtual void onAwake();
	virtual void onStart();
	virtual void onEnable();
	virtual void onDisable();
	virtual void update(sf::Time delta);
	virtual void physicsUpdate(sf::Time delta);

protected:
	GameObject* mGo = nullptr;

private:
	int mId = 0;
	bool misEnabled = true;
	bool mIsDestroyed = false;
};
