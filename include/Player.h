#pragma once
#include "NLTmxMap.h"
#include "Component.h"
#include "IRigidBodyObserver.h"
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

class Gamepad;
class AnimationComponent;
class RigidBody;

class Player : public Component, public IRigidBodyObserver
{
public:
	~Player() override;
	Player() = default;
	Player(NLTmxMapObject* obj, NLTmxMapObjectGroup* group);
	void update(sf::Time delta) override;
	void physicsUpdate(sf::Time delta) override;
	void onAdd(GameObject* go) override;
	void onStart() override;
	void addPoints(int points);
	int getPoints();
	Gamepad* getGamepad();
	virtual void onCollision(Collider* other) override;
	sf::Time getProjectileCooldown();

private:
	float mSpeed = 1;
	Gamepad* mGamepad;
	AnimationComponent* mAnim;
	sf::Vector2f mShootingDirection;
	sf::Vector2f mMove;
	RigidBody* mRigidBody;
	int mPoints = 0;
	bool mDashPhase1 = false;
	bool mDashPhase2 = false;
	float mDashCoolDown = 0;
	sf::Time mProjectileElapsed = sf::seconds(0);
	sf::Time mProjectileCooldown = sf::seconds(0.5);
	sf::Vector2f mLastMove = sf::Vector2f(0, 1);
};

