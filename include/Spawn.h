#pragma once
#include "Component.h"
#include "NLTmxMap.h"
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

class RigidBody;

class Spawn : public Component
{
public:
	~Spawn() override;
	Spawn() = default;
	Spawn(NLTmxMapObject* obj, NLTmxMapObjectGroup* group);
	void update(sf::Time delta) override;
	void physicsUpdate(sf::Time delta) override;
	void onAdd(GameObject * go) override;
	void onStart() override;

private:
};