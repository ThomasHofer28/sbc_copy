#include "GameObject.h"
#include <iostream>
#include "RenderComponent.h"
#include "RenderManager.h"
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

int GameObject::instances = 0;

GameObject::~GameObject()
{
	for (const auto& component : mComponents)
	{
		if (component->isEnabled())
			component->onDisable();
	}
}

GameObject::GameObject(std::string name)
{
	mName = name;
	mId = instances++;
}

bool GameObject::hasRigidbody()
{
	return mHasRigidBody;
}

std::string GameObject::getName()
{
	return mName;
}

void GameObject::setName(std::string name)
{
	mName = name;
}

int GameObject::getId()
{
	return mId;
}

void GameObject::setRigidbody(bool hasRigidBody)
{
	mHasRigidBody = hasRigidBody;
}

void GameObject::markForDelete()
{
	mIsMarkedForDelete = true;
}

bool GameObject::isMarkedForDelete()
{
	return mIsMarkedForDelete;
}

bool GameObject::isEnabled()
{
	return mIsEnabled;
}

void GameObject::addChild(std::unique_ptr<GameObject> go)
{
	go->setParent(this);
	mChildren.push_back(std::move(go));
}

void GameObject::setParent(GameObject* go)
{
	mParent = go;
}

void GameObject::addComponent(std::unique_ptr<Component> component)
{
	component->onAdd(this);
	mComponentsToAdd.push_back(std::move(component));
}

void GameObject::update(sf::Time delta)
{
	std::vector<Component*> componentsToAdd;

	for (auto& component : mComponentsToAdd)
	{
		componentsToAdd.push_back(component.get());
		mComponents.push_back(std::move(component));
	}

	for (const auto& component : componentsToAdd)
	{
		if (component->isEnabled())
			component->onEnable();
	}

	for (const auto& component : componentsToAdd)
	{
		component->onAwake();
	}

	for (const auto& component : componentsToAdd)
	{
		component->onStart();
	}

	mComponentsToAdd.clear();

	for (const auto& component : mComponents)
	{
		if (component->isEnabled())
			component->update(delta);
	}

	for (const auto& child : mChildren)
	{
		child->update(delta);
	}
}

void GameObject::physicsUpdate(sf::Time delta)
{
	for (const auto& cmp : mComponents)
	{
		cmp->physicsUpdate(delta);
	}
}

void GameObject::setEnabled(bool enabled)
{
	if (enabled != mIsEnabled)
	{
		mIsEnabled = enabled;

		for (const auto& component : mComponents)
		{
			component->setEnabled(enabled);
		}
	}
}
