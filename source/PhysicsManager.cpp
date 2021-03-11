#include "PhysicsManager.h"
#include <iostream>
#include "GameObject.h"
#include "GameObjectManager.h"
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

PhysicsManager& PhysicsManager::getInstance()
{
	static PhysicsManager instance;
	return instance;
}

void PhysicsManager::add(Collider* collider)
{
	mColliders.push_back(collider);
}

void PhysicsManager::add(RigidBody* rb)
{
	mRigidBodies.push_back(rb);
}

void PhysicsManager::remove(Collider* collider)
{
	mColliders.erase(std::find(mColliders.begin(), mColliders.end(), collider));
}

void PhysicsManager::remove(RigidBody* rb)
{
	mRigidBodies.erase(std::find(mRigidBodies.begin(), mRigidBodies.end(), rb));
}

void PhysicsManager::update(sf::Time delta)
{
	static sf::Time accumulator = sf::seconds(0);
	accumulator += delta;

	while (accumulator > mStep)
	{
		fixedUpdate();
		accumulator -= mStep;
	}
}

void PhysicsManager::fixedUpdate()
{
	findCollisions();
	resolveCollisions();
	GameObjectManager::getInstance().physicsUpdate(mStep);
	updateRigidbodies();
}

void PhysicsManager::findCollisions()
{
	if (mColliders.size() < 2)
		return;

	for (int i = 0; i < mColliders.size() - 1; ++i)
	{
		for (int a = i + 1; a < mColliders.size(); ++a)
		{
			auto col1 = mColliders[i];
			auto col2 = mColliders[a];
			auto rb1 = col1->getRigidBody();
			auto rb2 = col2->getRigidBody();
			
			if (rb1 == rb2 || rb1->hasIgnore(rb2->getLayer()) || rb2->hasIgnore(rb1->getLayer()) || rb1->hasIgnore(rb2) || rb2->hasIgnore(rb1))
				continue;

			MTV mtv;

			if (col1->collidesWith(col2, mtv))
			{
				rb1->onCollision(col2);
				rb2->onCollision(col1);

				if (col1->hasPhysics() && col2->hasPhysics())
				{
					Manifold manifold;
					manifold.mtv = mtv;
					manifold.rb1 = rb1;
					manifold.rb2 = rb2;

					mManifolds.push_back(manifold);
				}
			}
		}
	}
}

void PhysicsManager::resolveCollisions()
{
	for (auto& man : mManifolds)
	{
		if (man.rb1->getInvMass() == 0 && man.rb2->getInvMass() == 0)
		{
			continue;
		}

		sf::Vector2f rv = man.rb2->getVelocity() - man.rb1->getVelocity();
		float velAlongNormal = (rv.x * man.mtv.normal.x + rv.y * man.mtv.normal.y);

		if (velAlongNormal > 0)
		{
			continue;
		}

		float e = 0.1f;

		float j = (1 + e) * velAlongNormal;
		j /= man.rb1->getInvMass() + man.rb2->getInvMass();
		sf::Vector2f impulse = j * man.mtv.normal;

		man.rb1->setVelocity(man.rb1->getVelocity() + sf::Vector2f(man.rb1->getInvMass() * impulse.x, man.rb1->getInvMass() * impulse.y));
		man.rb2->setVelocity(man.rb2->getVelocity() - sf::Vector2f(man.rb2->getInvMass() * impulse.x, man.rb2->getInvMass() * impulse.y));

		const float percent = 0.5f;		
		const float slop = 0.f;
		sf::Vector2f correction = std::max(man.mtv.penetration - slop, 0.0f) / (man.rb1->getInvMass() + man.rb2->getInvMass()) * percent * man.mtv.normal;
		man.rb1->mCorrection += -man.rb1->getInvMass() * correction;
		man.rb2->mCorrection += man.rb2->getInvMass() * correction;
	}

	mManifolds.clear();
}

void PhysicsManager::updateRigidbodies()
{
	for (auto& body : mRigidBodies)
	{
		sf::Vector2f forces;

		for (const auto& f : body->getForces())
			forces += f;

		for (const auto& i : body->getImpulses())
			forces += i;

		body->getImpulses().clear();
		body->setAcceleration(forces * body->getInvMass());
		body->setVelocity(body->getVelocity() + body->getAcceleration() * mStep.asSeconds());
		body->correctPosition(mStep);
	}
}
