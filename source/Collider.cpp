#include "Collider.h"
#include "PhysicsManager.h"
#include "GameObject.h"
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

Collider::~Collider()
{
	PhysicsManager::getInstance().remove(this);
}

void Collider::onAdd(GameObject* go)
{
	Component::onAdd(go);

	if (!mGo->hasRigidbody())
	{
		auto rb = std::make_unique<RigidBody>();
		mGo->addComponent(std::move(rb));
		mGo->setRigidbody(true);
	}
}
 
void Collider::onAwake()
{
	RenderComponent::onAwake();
	PhysicsManager::getInstance().add(this);
}

void Collider::onStart()
{
	mRigidBody = mGo->getComponent<RigidBody>();
}

void Collider::setDebugDraw(bool change)
{
	mDebugDraw = change;
}

bool Collider::getDebugDraw()
{
	return mDebugDraw;
}

void Collider::setOffset(sf::Vector2f offset)
{
	mOffset = offset;
}

sf::Vector2f Collider::getOffset()
{
	return mOffset;
}

RigidBody* Collider::getRigidBody()
{
	return mRigidBody;
}

bool Collider::hasPhysics()
{
	return mHasPhysics;
}

void Collider::setPhysics(bool onoff)
{
	mHasPhysics = onoff;
}
