#include "Pentagram.h"
#include "GameObjectManager.h"
#include "Map.h"
#include "Debug.h"
#include "GameObject.h"
#include "QuickMaths.h"
#include "RigidBody.h"
#include "Component.h"
#include "CircleCollider.h"
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

Pentagram::Pentagram(NLTmxMapObject* obj, NLTmxMapObjectGroup* group)
{
	std::map<string, string>::iterator it;
	if ((it = obj->properties.find("speed")) != obj->properties.end())
	{
		mSpeed = (float)(std::stoi(it->second));
	}
}

void Pentagram::onStart()
{
	mRb = mGo->getComponent<RigidBody>();

	auto points = GameObjectManager::getInstance().getObjects("Pentagram Point");
	for (const auto& p : points)
	{
		mPoints.push_back(p->getPosition()); 
	}

	mGo->setEnabled(false);
}

void Pentagram::onAdd(GameObject* go)
{
	Component::onAdd(go);

	auto animCmp = new AnimationComponent("Textures/pentagram-small.png", 54, 54);
	auto anim = new Animation("blink", { 0,1,2,3,4,5,6,7,8,9 }, 8, true);
	animCmp->addAnimation(std::unique_ptr<Animation>(anim));
	animCmp->play("blink");
	animCmp->setScale(Map::Scale * 0.75f, Map::Scale * 0.75f);
	animCmp->setLayer(RigidBody::Layer::e_Pentagram);
	go->addComponent(std::unique_ptr<AnimationComponent>(animCmp));

	auto coll = new CircleCollider;
	coll->setRadius(27.5 * Map::Scale * 0.74);
	coll->setLayer(RigidBody::Layer::e_Colliders);
	coll->setOffset({ -coll->getRadius(), -coll->getRadius() });
	go->addComponent(std::unique_ptr<CircleCollider>(coll));
	coll->setPhysics(false);
}

void Pentagram::update(sf::Time delta)
{
	auto distance = *mTargetPoint - mGo->getPosition();
	const float threshold = 3;

	if (QuickMaths::length(distance) - threshold <= 0)
	{
		if (mActiveIdx == mIndexTraversal.size() - 1) // done with traversal
		{
			mGo->setEnabled(false);
			return;
		}

		mGo->setPosition(*mTargetPoint);
		mTargetPoint = &mPoints[mIndexTraversal[++mActiveIdx]];
		mRb->setVelocity(QuickMaths::normalize(*mTargetPoint - mGo->getPosition()) * mSpeed);
	}
}

void Pentagram::traverse(const std::vector<int>& traversal, float speed)
{
	mSpeed = speed;
	mGo->setEnabled(true);
	mIndexTraversal = traversal;
	mActiveIdx = 0;
	mTargetPoint = &mPoints[mIndexTraversal[mActiveIdx]];
	mGo->setPosition(*mTargetPoint);
}
