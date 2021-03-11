#include "RigidBody.h"
#include <iostream>
#include "GameObject.h"
#include "IRigidBodyObserver.h"
#include "PhysicsManager.h"
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

RigidBody::RigidBody(NLTmxMapObject* obj, NLTmxMapObjectGroup* group)
{
	std::map<string, string>::iterator it;
	if ((it = obj->properties.find("c_rigidbody_mass")) != obj->properties.end())
	{
		mInvMass = 1.0f / std::stoi(it->second);
	}
	else
	{
		std::cerr << "No mass found for rigidbody" << std::endl;
	}
}

RigidBody::~RigidBody()
{
}

void RigidBody::onAwake()
{
}

void RigidBody::onStart()
{
	mPos = mGo->getPosition();
	mObservers = mGo->getComponents<IRigidBodyObserver>();
}

void RigidBody::onEnable()
{
	PhysicsManager::getInstance().add(this);
}

void RigidBody::onDisable()
{
	PhysicsManager::getInstance().remove(this);
}

void RigidBody::correctPosition(sf::Time delta)
{
	mGo->move(mVelocity * delta.asSeconds() + mCorrection);
	mCorrection.x = mCorrection.y = 0;
}

void RigidBody::onCollision(Collider* other)
{
	for (auto observer : mObservers)
		observer->onCollision(other);
}

sf::Vector2f RigidBody::getPosition()
{
	return mPos;
}

void RigidBody::setPosition(sf::Vector2f pos)
{
	mPos = pos;
}

float RigidBody::getInvMass()
{
	return mIsKinematic ? 0 : mInvMass;
}

void RigidBody::setInvMass(float value)
{
	mInvMass = value;
}

sf::Vector2f RigidBody::getAcceleration()
{
	return mAcceleration;
} 

void RigidBody::setAcceleration(sf::Vector2f acc)
{
	mAcceleration = acc;
}

sf::Vector2f RigidBody::getVelocity()
{
	return mVelocity;
}

void RigidBody::setVelocity(sf::Vector2f vel)
{
	mVelocity = vel;
}
  
void RigidBody::addForce(sf::Vector2f force)
{
	mForces.push_back(force);
}

void RigidBody::addImpulse(sf::Vector2f impulse)
{
	mImpulses.push_back(impulse);
}

std::list<sf::Vector2f>& RigidBody::getForces()
{
	return mForces;
}

std::list<sf::Vector2f>& RigidBody::getImpulses()
{
	return mImpulses;
}

bool RigidBody::isKinematic()
{
	return mIsKinematic;
}

void RigidBody::setKinematic(bool isKinematic)
{
	mIsKinematic = isKinematic;
}

void RigidBody::setIgnore(Layer layer, bool ignore)
{
	if (ignore && !hasIgnore(layer))
		mIgnoredLayers.push_back(layer);
	else if (!ignore)
		mIgnoredLayers.erase(std::find(mIgnoredLayers.begin(), mIgnoredLayers.end(), layer));
}

void RigidBody::addIgnore(RigidBody* rb)
{
	mIgnoredRbs.push_back(rb);
}

bool RigidBody::hasIgnore(Layer layer)
{
	return std::find(mIgnoredLayers.begin(), mIgnoredLayers.end(), layer) != mIgnoredLayers.end();
}

bool RigidBody::hasIgnore(RigidBody* rb)
{
	return std::find(mIgnoredRbs.begin(), mIgnoredRbs.end(), rb) != mIgnoredRbs.end();
}

RigidBody::Layer RigidBody::getLayer()
{
	return mLayer;
}

void RigidBody::setLayer(Layer layer)
{
	mLayer = layer;
}
