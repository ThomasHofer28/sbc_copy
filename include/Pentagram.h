#pragma once
#include "Component.h"
#include "AnimationComponent.h"
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

class RigidBody;

class Pentagram : public Component
{
public:
	Pentagram(NLTmxMapObject* obj, NLTmxMapObjectGroup* group);
	void onStart() override;
	void onAdd(GameObject* go) override;
	void update(sf::Time delta) override;
	void traverse(const std::vector<int>& traversal, float speed);

private:
	std::vector<sf::Vector2f> mPoints;
	std::vector<int> mIndexTraversal;
	sf::Vector2f* mTargetPoint = nullptr;
	int mActiveIdx = 0;
	float mSpeed = 0.0f;

	RigidBody* mRb = nullptr;
};

