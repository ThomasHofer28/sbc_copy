#include "Bird.h"
#include "CircleCollider.h"
#include "QuickMaths.h"
#include "GameObjectManager.h"
#include "Debug.h"
#include "AnimationComponent.h"
#include "Wall.h"
#include "Projectile.h"
#include "BoxCollider.h"
#include "Spawn.h"
#include "Player.h"
#include <iostream>
#include "TimerManager.h"
#include "Pentagram.h"
#include "GameState.h"
#include "Projectile.h"
#include "NoPointWall.h"
#include "SoundManager.h"
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

int Bird::mInstances = 0;

Bird::Bird()
{
	mInstances++;
}

Bird::~Bird()
{
	mInstances--;
}

void Bird::onAdd(GameObject* go)
{
	Component::onAdd(go);

	// TODO add shot animation
	mAnimBird = new AnimationComponent("Textures/bat.png", 32, 32);
	go->addComponent(std::unique_ptr<AnimationComponent>(mAnimBird));
	const int FPS = 10;
	auto walkSouth = new Animation("walk_south", { 1, 3 }, FPS, true);
	mAnimBird->addAnimation(std::unique_ptr<Animation>(walkSouth));
	auto walkNorth = new Animation("walk_north", { 9, 11 }, FPS, true);
	mAnimBird->addAnimation(std::unique_ptr<Animation>(walkNorth));
	auto walkRight = new Animation("walk_right", { 5, 7 }, FPS, true);
	mAnimBird->addAnimation(std::unique_ptr<Animation>(walkRight));
	auto walkLeft = new Animation("walk_left", { 13, 15 }, FPS, true);
	mAnimBird->addAnimation(std::unique_ptr<Animation>(walkLeft));

	mAnimBird->play("walk_south");
	mAnimBird->setOrigin(9, 8);
	mAnimBird->setLayer(RigidBody::Layer::e_Bird);
	mAnimBird->setScale(2.5, 2.5);

	mAnimBloodSplatter = new AnimationComponent("Textures/blood-splatter.png", 32, 32);
	go->addComponent(std::unique_ptr<AnimationComponent>(mAnimBloodSplatter));
	auto splatter = new Animation("splatter", { 0, 1, 2, 3, 4 }, 10, false);
	mAnimBloodSplatter->addAnimation(std::unique_ptr<Animation>(splatter));
	mAnimBloodSplatter->setScale(2, 2);
	mAnimBloodSplatter->setOrigin(9, 8);
	mAnimBloodSplatter->setLayer(RigidBody::Layer::e_Blood);
	mAnimBloodSplatter->play("splatter");
	mAnimBloodSplatter->setVisible(false);
	mAnimBloodSplatter->setActive(false);

	// circle collider
	auto coll = std::make_unique<CircleCollider>();
	coll->setRadius(16);
	coll->setLayer(RigidBody::Layer::e_Colliders);
	mGo->addComponent(std::move(coll));

	// shadow
	auto shadowSprite = std::make_unique<SpriteRenderer>("Textures/shadowbird.png");
	shadowSprite->setScale(2, 2);
	shadowSprite->setLayer(RigidBody::Layer::e_ShadowBird);
	shadowSprite->setOrigin(8, -16);
	shadowSprite->setColor(sf::Color(shadowSprite->getColor().r, shadowSprite->getColor().g, shadowSprite->getColor().b, 70));
	go->addComponent(std::move(shadowSprite));
}

void Bird::onStart()
{
	mGo->setPosition(960,590);
	mRb = mGo->getComponent<RigidBody>();
	mRb->setInvMass(400);
	mRb->setLayer(RigidBody::Layer::e_Bird);
	mRb->setIgnore(RigidBody::Layer::e_PlayerWall, true);
	newDestination();
}

void Bird::update(sf::Time delta)
{
	mDirection = sf::Vector2f(mDestination - mGo->getPosition());

	// update animation
	float angle = QuickMaths::axisOrientation(mDirection);
	if (angle > 315 || angle <= 45)
		mAnimBird->play("walk_left");
	else if (angle > 45 && angle <= 135)
		mAnimBird->play("walk_north");
	else if (angle > 135 && angle <= 225)
		mAnimBird->play("walk_right");
	else
		mAnimBird->play("walk_south");

	// update destination

	if (mFollow) // bird follows somebody
	{
		mDestination = mFollowObject->getPosition();
	}
	else // does not follow somebody
	{
		mNewPosTimer += delta.asSeconds();
		if (mNewPosTimer >= 10.0f)
		{
			newDestination();
			mNewPosTimer = 0.0f;
		}

		// check distance to players
		auto player1 = GameObjectManager::getInstance().getObject("Player 1");
		auto player2 = GameObjectManager::getInstance().getObject("Player 2");
		auto posPlayer1 = player1->getPosition();
		auto posPlayer2 = player2->getPosition();
		auto posBird = mGo->getPosition();
		auto distanceP1 = QuickMaths::length(posPlayer1 - posBird);
		auto distanceP2 = QuickMaths::length(posPlayer2 - posBird);
		if (distanceP1 < distanceP2 && distanceP1 < 130)
		{
			mFollowObject = player1;
			mFollow = true;
		}
		else if (distanceP2 < 130)
		{
			mFollowObject = player2;
			mFollow = true;
		}
	}

	if (!mFollow)
	{
		if (abs(mGo->getPosition().x - mDestination.x) <= 5 && abs(mGo->getPosition().y - mDestination.y) <= 5)
		{
			newDestination();
		}
	}

	if (mWasShot && QuickMaths::length(mGo->getPosition() - mProjectileStart) > 550)
	{
		mGo->markForDelete();
	}
}

void Bird::physicsUpdate(sf::Time delta)
{
	if (!mWasShot)
	{
		if (mFollow)
		{
			if (QuickMaths::length(mGo->getPosition() - mDestination) > mMinPlayerDistance)
				mGo->getComponent<RigidBody>()->setVelocity(QuickMaths::normalize(mDirection) * 150.f);
			else
				mGo->getComponent<RigidBody>()->setVelocity({0, 0});
		}
		else
		{
			mGo->getComponent<RigidBody>()->setVelocity(QuickMaths::normalize(mDirection) * 150.f);
		}
	}
}

void Bird::setLastHitProjectile(Projectile* projectile, sf::Vector2f projStartPos)
{
	mLastHitProjectile = projectile;
	mProjectileStart = projStartPos;
}

void Bird::newDestination()
{
	mDestination = QuickMaths::newRandomPosition(sf::Vector2f(200,200),sf::Vector2f(1720,880));
}

void Bird::shoot(sf::Vector2f force)
{
	mWasShot = true;
	mRb->setVelocity(force);
}

void Bird::onCollision(Collider* other)
{

	auto spawn = other->getGameObject()->getComponent<Spawn>();

	if (spawn)
	{
		mGo->getComponent<CircleCollider>()->setPhysics(false);
	}
	else if (!mWasShot) {
		mGo->getComponent<CircleCollider>()->setPhysics(true);
	}

	auto map = other->getGameObject()->getName() == "Bird Wall";

	if (map && mWasShot && !mIsDeleted)
	{
		mIsDeleted = true;
		mGivePoints = true;
		mRb->setEnabled(false);
		mAnimBird->setVisible(false);
		mAnimBloodSplatter->setVisible(true);
		mAnimBloodSplatter->setActive(true);

		if (mIsInPentagram)
			mValue *= 3;

		mLastHitProjectile->addValue(mValue);

		std::string name = mGo->getName();
		TimerManager::getInstance().create(sf::seconds(1), [name]() 
		{
			if (!dynamic_cast<GameState*>(StateManager::getInstance().getCurrentState()))
				return;

			auto go = GameObjectManager::getInstance().getObject(name);
			if (go)
			{
				go->markForDelete();
			}
		});

		SoundManager::getInstance().play("blood");
	}

	auto nopointwall = other->getGameObject()->getName() == "No Point Wall";

	if (nopointwall && !mIsDeleted)
	{
		mGo->markForDelete();
	}

	mIsInPentagram = other->getGameObject()->getComponent<Pentagram>() != nullptr;
}

void Bird::changeValue(int value)
{
	mValue = value;
}

bool Bird::isShot()
{
	return mWasShot;
}
