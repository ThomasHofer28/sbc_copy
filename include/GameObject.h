#pragma once
#include <string>
#include "Component.h"
#include <vector>
#include <memory>
#include <SFML\Graphics\Transformable.hpp>
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

class sf::RenderWindow;

class GameObject : public sf::Transformable
{
private:
	static int instances;
	int mId;
	std::string mName;
	std::vector<std::unique_ptr<Component>> mComponents;
	std::vector<std::unique_ptr<Component>> mComponentsToAdd;
	std::vector<Component*> mComponentsToRemove;
	std::vector<std::unique_ptr<GameObject>> mChildren;
	GameObject* mParent;
	bool mIsEnabled = true;
	bool mHasRigidBody = false;
	bool mIsMarkedForDelete = false;

public:
	~GameObject();
	GameObject(std::string name);
	void addChild(std::unique_ptr<GameObject> go);
	void setParent(GameObject* go);
	void addComponent(std::unique_ptr<Component> component);
	void update(sf::Time delta);
	void physicsUpdate(sf::Time delta);
	std::string getName();
	void setName(std::string name);
	int getId();
	void setEnabled(bool enabled);
	bool hasRigidbody();
	void setRigidbody(bool setBody);
	void markForDelete();
	bool isMarkedForDelete();
	bool isEnabled();

	template <typename T>
	T& addComponent()
	{
		auto component = std::make_unique<T>();
		component->onAdd(this);
		auto ret = component.get();
		mComponentsToAdd.push_back(std::move(component));
		return *ret;
	}

	template <typename T>
	void removeComponent()
	{
		for (const auto& component : mComponents)
		{
			if (typeid(*component) == typeid(T))
			{
				mComponentsToRemove.push_back(component.get());
			}
		}
	}

	template <typename T>
	T* getComponent()
	{
		for (const auto& component : mComponents)
		{
			if (typeid(*component) == typeid(T))
			{
				return static_cast<T*>(component.get());
			}
		}

		return nullptr;
	}

	template <typename T>
	std::vector<T*> getComponents()
	{
		std::vector<T*> components;

		for (const auto& component : mComponents)
		{
			T* t = dynamic_cast<T*>(component.get());
			if (t)
			{
				components.push_back(t);
			}
		}

		return components;
	}
};