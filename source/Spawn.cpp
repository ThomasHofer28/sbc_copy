#include "Spawn.h"
#include "GameObject.h"
#include "RigidBody.h"
#include "Bird.h"
#include "Collider.h"
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

Spawn::~Spawn()
{
}

Spawn::Spawn(NLTmxMapObject* obj, NLTmxMapObjectGroup* group)
{

}

void Spawn::update(sf::Time delta)
{
}

void Spawn::physicsUpdate(sf::Time delta)
{
}

void Spawn::onAdd(GameObject* go)
{
	Component::onAdd(go);
}

void Spawn::onStart()
{
	mGo->getComponent<RigidBody>()->setKinematic(true);
}