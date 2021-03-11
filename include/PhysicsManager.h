#pragma once
#include <vector>
#include "Collider.h"
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

struct MTV
{
	sf::Vector2f normal;
	float penetration;
};

struct Manifold
{
	RigidBody* rb1;
	RigidBody* rb2;
	MTV mtv;
};

class PhysicsManager
{
public:
	static PhysicsManager& getInstance();
	void add(Collider* collider);
	void add(RigidBody* rb);
	void remove(Collider* collider);
	void remove(RigidBody* collider);
	void update(sf::Time delta);
	void fixedUpdate();
	void findCollisions();
	void resolveCollisions();
	void updateRigidbodies();

private:
	std::vector<Collider*> mColliders;
	std::vector<RigidBody*> mRigidBodies;
	std::vector<Manifold> mManifolds;
	sf::Time mStep = sf::seconds(1 / 120.f);
};
