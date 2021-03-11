#include "Projectile.h"
#include "GameObject.h"
#include "PolygonCollider.h"
#include <cmath>
#include "Map.h"
#include "QuickMaths.h"
#include "RigidBody.h"
#include "Spawn.h"
#include "Bird.h"
#include "AnimationComponent.h"
#include "CircleCollider.h"
#include "Player.h"
#include "GameState.h"
#include "TimerManager.h"
#include "BirdWall.h"
#include "ProjectileWall.h"
#include "NoPointWall.h"
#include "SoundManager.h"
#include "StateManager.h"
#include <iostream>
# define M_PI           3.14159265358979323846  /* pi */
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

Projectile::~Projectile()
{
	if (mCenter == sf::Vector2f(0,0))
	{
		mCenter = mGo->getPosition();
	}

	mPlayer->addPoints(mValue);
	auto gameState = dynamic_cast<GameState*>(StateManager::getInstance().getCurrentState());
	if (gameState)
	{
		gameState->displayPlusPoints(mPlayer, mValue, mCenter);
	}
}

Projectile::Projectile()
{

}

Projectile::Projectile(sf::Vector2f& shootingDirection, sf::Vector2f startPos)
{
	mShootingDirection = shootingDirection;
	mStartPos = startPos;
}

void Projectile::onStart()
{
	mRb = mGo->getComponent<RigidBody>();
	mGo->setPosition(mStartPos);
	mRb->setVelocity(mShootingDirection * 700.0f);
	mRb->setIgnore(RigidBody::Layer::e_PlayerWall, true);
	sf::Vector2f velo = mRb->getVelocity();
	float angle = QuickMaths::axisOrientation(velo);
	mGo->getComponent<AnimationComponent>()->setRotation(angle-90);
}

void Projectile::onAdd(GameObject* go)
{
	Component::onAdd(go);

	// animation
	mAnimBird = new AnimationComponent("Textures/sword.png", 6, 13);
	go->addComponent(std::unique_ptr<AnimationComponent>(mAnimBird));
	const int FPS = 10;
	auto idleSouth = new Animation("dagger", { 0, 0 }, FPS, true);
	mAnimBird->addAnimation(std::unique_ptr<Animation>(idleSouth));
	mAnimBird->play("dagger");
	mAnimBird->setLayer(RigidBody::Layer::e_Sword);

	const float daggerScale = 0.75f * Map::Scale;
	mAnimBird->setScale(daggerScale, daggerScale);

	auto coll = new CircleCollider();
	coll->setRadius(2.5 * daggerScale);
	coll->setPhysics(false);
	coll->setLayer(RigidBody::Layer::e_Colliders);
	coll->setOrigin(1.5f * daggerScale, 2.0f * daggerScale);
	mGo->addComponent(std::unique_ptr<CircleCollider>(coll));
}

void Projectile::setPlayer(Player* player)
{
	mPlayer = player;

}

Player* Projectile::getPlayer()
{
	return mPlayer;
}

void Projectile::onCollision(Collider* other)
{
	auto spawn = other->getGameObject()->getComponent<Spawn>();

	if (spawn)
	{
		mGo->markForDelete();
		mCenter = mGo->getComponent<CircleCollider>()->getPosition();
	}

	auto wall = other->getGameObject()->getComponent<ProjectileWall>();

	if (wall && !mIsDeleted)
	{
		mIsDeleted = true;
		mCenter = mGo->getComponent<CircleCollider>()->getPosition();
		mRb->setEnabled(false);
		mGo->markForDelete();
	}

	auto nopointwall = other->getGameObject()->getComponent<NoPointWall>();

	if (nopointwall)
	{
		mGo->markForDelete();
		mCenter = mGo->getComponent<CircleCollider>()->getPosition();
	}

	auto bird = other->getGameObject()->getComponent<Bird>();
	
	if (bird && !bird->isShot())
	{
		bird->changeValue(mBirdCounter++);
	}

	if (bird)
	{
		bird->getGameObject()->getComponent<CircleCollider>()->setPhysics(false);
		bird->shoot(mRb->getVelocity());
		bird->setLastHitProjectile(this, mStartPos);
	}

	auto player = other->getGameObject()->getComponent<Player>();

	if (player)
	{
		player->getGameObject()->getComponent<RigidBody>()->addImpulse(mShootingDirection * 100000.f);
		mCenter = mGo->getComponent<CircleCollider>()->getPosition();
	}
}

void Projectile::setShootingDirection(sf::Vector2f& dir)
{
	mShootingDirection = dir;
}

void Projectile::update(sf::Time delta)
{
	if (QuickMaths::length(mGo->getPosition() - mStartPos) > 550)
	{
		mGo->markForDelete();
	}
}

void Projectile::addValue(int value)
{
	mValue += value;
}
