#pragma once

#include <SFML/System/Vector2.hpp>
#include "Component.h"
#include "IRigidBodyObserver.h"
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

class RigidBody;
class Player;
class AnimationComponent;
class GameObject;

class Projectile : public Component, public IRigidBodyObserver
{
public:
	~Projectile();
	Projectile();
	Projectile(sf::Vector2f& shootingDirection, sf::Vector2f startPos);
	void onStart() override;
	void onAdd(GameObject* go) override;
	void setPlayer(Player* player);
	Player* getPlayer();
	void onCollision(Collider* other) override;
	void setShootingDirection(sf::Vector2f& dir);
	void update(sf::Time delta) override;
	void addValue(int value);

private:
	bool mIsDeleted = false;
	sf::Vector2f mStartPos;
	sf::Vector2f mShootingDirection;
	Player* mPlayer = nullptr;
	RigidBody* mRb = nullptr;
	AnimationComponent* mAnimBird = nullptr;
	int mValue = 0;
	int mBirdCounter = 1;
	sf::Vector2f mCenter = sf::Vector2f(0,0);
};