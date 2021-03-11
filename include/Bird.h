#pragma once
#include "Component.h"
#include "AnimationComponent.h"
#include "IRigidBodyObserver.h"
#include "RigidBody.h"
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

class Player;
class Projectile;

class Bird : public Component, public IRigidBodyObserver
{
public:
	static int mInstances;
	Bird();
	~Bird();
	void onAdd(GameObject* go) override;
	void onStart() override;
	void update(sf::Time delta) override;
	void physicsUpdate(sf::Time delta) override;
	void setLastHitProjectile(Projectile* projectile, sf::Vector2f projStartPos);
	void newDestination();
	void shoot(sf::Vector2f force);
	void onCollision(Collider* other) override;
	void changeValue(int value);
	bool isShot();

private:
	bool mFollow = false;
	bool mWasShot = false;
	bool mGivePoints = false;
	bool mIsInPentagram = false;
	float mNewPosTimer = 0.0f;
	bool mIsDeleted = false;
	float mMinPlayerDistance = 30;
	sf::Vector2f mDestination;
	sf::Vector2f mDirection;
	GameObject* mFollowObject = nullptr;
	AnimationComponent* mAnimBird = nullptr;
	AnimationComponent* mAnimBloodSplatter = nullptr;
	RigidBody* mRb = nullptr;
	int mValue = 1;
	sf::Vector2f mProjectileStart;
	Projectile* mLastHitProjectile = nullptr;
};