#pragma once
#include "Component.h"
#include <iostream>
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

bool Component::isEnabled()
{
	return misEnabled;
}

void Component::setEnabled(bool enabled)
{
	if (misEnabled != enabled)
	{
		misEnabled = enabled;

		if (enabled)
			onEnable();
		else
			onDisable();
	}
}

bool Component::isDestroyed()
{
	return mIsDestroyed;
}

GameObject* Component::getGameObject()
{
	return mGo;
}

Component::~Component()
{
}

void Component::onAwake()
{
}

void Component::onStart()
{
}

void Component::onEnable()
{
}

void Component::onDisable()
{
}

void Component::onAdd(GameObject* go)
{
	mGo = go;
	mId = (int)this;
}

void Component::update(sf::Time)
{
}

void Component::physicsUpdate(sf::Time delta)
{
}
