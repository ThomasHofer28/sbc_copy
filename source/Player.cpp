#include "Player.h"
#include "Gamepad.h"
#include "InputManager.h"
#include <iostream>
#include "AnimationComponent.h"
#include "GameObject.h"
#include "QuickMaths.h"
#include "RigidBody.h"
#include "Projectile.h"
#include "GameObjectManager.h"
#include "Map.h"
#include "Collider.h"
#include "BoxCollider.h"
#include "Pentagram.h"
#include "SoundManager.h"
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

Player::~Player()
{
}

Player::Player(NLTmxMapObject* obj, NLTmxMapObjectGroup* group)
{
	mGamepad = InputManager::getInstance().addGamepad();
}

void Player::update(sf::Time delta)
{	
	mProjectileElapsed += delta;

	// dash (not hacky)
	if (mGamepad->isButtonDown(Gamepad::Button::LTrigger) && mDashCoolDown < 1)
	{
		auto color = mAnim->getColor();
		mAnim->setColor(sf::Color(color.r, color.g, color.b, 120));
		mDashCoolDown = 5.0f;
		mDashPhase1 = true;
		SoundManager::getInstance().play("dash");
	}

	if (!mDashPhase1 && !mDashPhase2)
	{
		mMove = { mGamepad->getAxis(Gamepad::Axis::LStickHorizontal), mGamepad->getAxis(Gamepad::Axis::LStickVertical) };
	}

	// shooting direction
	sf::Vector2f shootDirection(mGamepad->getAxis(Gamepad::Axis::RStickHorizontal), mGamepad->getAxis(Gamepad::Axis::RStickVertical));
	
	// rotate and set aiming sprite visible
	auto spriteaim = mGo->getComponent<SpriteRenderer>();
	sf::Color aimColor = spriteaim->getColor();

	if (shootDirection.x != 0 || shootDirection.y != 0)
	{
		// angle
		float aimAngle = QuickMaths::axisOrientation(shootDirection);
		spriteaim->setRotation(aimAngle+180);
		if (mProjectileElapsed.asSeconds() > 0.5)
		{
			spriteaim->setColor(sf::Color(0,138,255, 100));
		}
		else {
			spriteaim->setColor(sf::Color(255, 255, 255, 30));
		}
	}
	else {
		spriteaim->setColor(sf::Color(0, 138, 255, 0));
	}
	
	// shoot trigger
	if (mGamepad->isButtonDown(Gamepad::Button::RTrigger) && mProjectileElapsed >= mProjectileCooldown)
	{
		mProjectileElapsed = sf::seconds(0);
		sf::Vector2f normalizeDirection = QuickMaths::normalize(shootDirection);
		if (normalizeDirection.x != 0 || normalizeDirection.y != 0)
		{
			auto goproj = std::make_unique<GameObject>("proj");
			auto proj = std::make_unique<Projectile>(normalizeDirection, mGo->getPosition());
			auto rb = new RigidBody();
			rb->addIgnore(mRigidBody);
			goproj->addComponent(std::unique_ptr<RigidBody>(rb));
			proj->setPlayer(this);
			goproj->addComponent(std::move(proj));
			GameObjectManager::getInstance().add(std::move(goproj));
			SoundManager::getInstance().play("knife");
		}
	}

	if (mMove.x == 0 && mMove.y == 0)
	{
		std::string currentAnim = mAnim->getActiveAnimation();
		if (currentAnim == "walk_left")
			mAnim->play("idle_left");
		else if (currentAnim == "walk_north")
			mAnim->play("idle_north");
		else if (currentAnim == "walk_right")
			mAnim->play("idle_right");
		else if (currentAnim == "walk_south")
			mAnim->play("idle_south");
	}
	else 
	{
		float angle = QuickMaths::axisOrientation(mMove);
		if (angle > 315 || angle <= 45)
			mAnim->play("walk_left");
		else if (angle > 45 && angle <= 135)
			mAnim->play("walk_north");
		else if (angle > 135 && angle <= 225)
			mAnim->play("walk_right");
		else
			mAnim->play("walk_south");
	}
}

void Player::physicsUpdate(sf::Time delta)
{
	if (mDashCoolDown > 1)
	{
		mDashCoolDown -= 0.04f;	
	}

	// dash
	if (mDashPhase1)
	{
		mSpeed += 0.15;
		mRigidBody->setIgnore(RigidBody::Layer::e_Bird, true);
		mRigidBody->setIgnore(RigidBody::Layer::e_Players, true);
	}
	if (mSpeed > 4.5)
	{
		mDashPhase1 = false;
		mDashPhase2 = true;
	}
	if (mDashPhase2)
	{
		mSpeed -= 0.20;
	}
	if (mSpeed < 1)
	{
		mRigidBody->setIgnore(RigidBody::Layer::e_Bird, false);
		mRigidBody->setIgnore(RigidBody::Layer::e_Players, false);
		mDashPhase2 = false;
		auto color = mAnim->getColor();
		mAnim->setColor(sf::Color(color.r, color.g, color.b, 255));
		mSpeed = 1;
	}

	if (mMove.x != 0 || mMove.y != 0)
	{
		mRigidBody->addImpulse(QuickMaths::normalize(mMove) * mSpeed * 2000.f);
		mLastMove = mMove;
	}
	else if ((mLastMove.x != 0 || mLastMove.y != 0) && mSpeed > 1.15)
	{
		mRigidBody->addImpulse(QuickMaths::normalize(mLastMove) * mSpeed * 2000.f);
	}

	mRigidBody->setVelocity(mRigidBody->getVelocity() * 0.91f);
}

void Player::onAdd(GameObject* go)
{
	Component::onAdd(go);

	if (mGamepad->getIdx() == 0)
	{
		auto aimSprite1 = std::make_unique<SpriteRenderer>("Textures/aimblue1.png");
		aimSprite1->setScale(Map::Scale, Map::Scale);
		aimSprite1->setLayer(RigidBody::Layer::e_ShootDirection);
		aimSprite1->setOrigin(8, 8);
		aimSprite1->setColor(sf::Color(aimSprite1->getColor().r, aimSprite1->getColor().g, aimSprite1->getColor().b, 0));
		go->addComponent(std::move(aimSprite1));

		mAnim = new AnimationComponent("Textures/blue.png", 16, 16);
	}
	else if (mGamepad->getIdx() == 1)
	{
		auto aimSprite1 = std::make_unique<SpriteRenderer>("Textures/aimgreen1.png");
		aimSprite1->setScale(Map::Scale, Map::Scale);
		aimSprite1->setLayer(RigidBody::Layer::e_ShootDirection);
		aimSprite1->setOrigin(8, 8);
		aimSprite1->setColor(sf::Color(aimSprite1->getColor().r, aimSprite1->getColor().g, aimSprite1->getColor().b, 0));
		go->addComponent(std::move(aimSprite1));

		mAnim = new AnimationComponent("Textures/green.png", 16, 16);
	}


	go->addComponent(std::unique_ptr<AnimationComponent>(mAnim));

	const int FPS = 8;

	auto idleSouth = new Animation("idle_south",{ 0, 2 }, FPS, true);
	mAnim->addAnimation(std::unique_ptr<Animation>(idleSouth));

	auto walkSouth = new Animation("walk_south", { 1, 3 }, FPS, true);
	mAnim->addAnimation(std::unique_ptr<Animation>(walkSouth));

	auto idleNorth = new Animation("idle_north", { 4, 6 }, FPS, true);
	mAnim->addAnimation(std::unique_ptr<Animation>(idleNorth));

	auto walkNorth = new Animation("walk_north", { 5, 7 }, FPS, true);
	mAnim->addAnimation(std::unique_ptr<Animation>(walkNorth));

	auto idleRight = new Animation("idle_right", { 9, 11 }, FPS, true);
	mAnim->addAnimation(std::unique_ptr<Animation>(idleRight));

	auto walkRight = new Animation("walk_right", { 8, 10 }, FPS, true);
	mAnim->addAnimation(std::unique_ptr<Animation>(walkRight));

	auto idleLeft = new Animation("idle_left", { 12, 14 }, FPS, true);
	mAnim->addAnimation(std::unique_ptr<Animation>(idleLeft));

	auto walkLeft = new Animation("walk_left", { 13, 15 }, FPS, true);
	mAnim->addAnimation(std::unique_ptr<Animation>(walkLeft));

	mAnim->play("idle_south");
	
	mAnim->setScale(Map::Scale*1.5f, Map::Scale*1.5f);

	mAnim->setLayer(RigidBody::Layer::e_Players);
}

void Player::onStart()
{
	mRigidBody = mGo->getComponent<RigidBody>();
	mRigidBody->setLayer(RigidBody::Layer::e_Players);


}

void Player::addPoints(int points)
{
	mPoints += points;
}

int Player::getPoints()
{
	return mPoints;
}

Gamepad* Player::getGamepad()
{
	return mGamepad;
}

void Player::onCollision(Collider* other)
{
}

sf::Time Player::getProjectileCooldown()
{
	return mProjectileCooldown;
}
