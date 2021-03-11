#pragma once
#include "RenderComponent.h"
#include "RigidBody.h"
#include <Map.h>
#include <sstream>
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

struct MTV;

class Collider : public RenderComponent
{
private:
	sf::Vector2f mOffset;
	RigidBody* mRigidBody = nullptr;
	bool mDebugDraw = false;
	bool mHasPhysics = true;
	
public:
	virtual ~Collider() override;
	virtual void update(sf::Time delta) = 0;
	virtual void draw(sf::RenderWindow& window) = 0;
	virtual bool collidesWith(Collider* collider, MTV& mtv) = 0;
	virtual void onAdd(GameObject* go);
	virtual void onAwake() override;
	virtual void onStart() override;
	void setDebugDraw(bool change);
	bool getDebugDraw();
	void setOffset(sf::Vector2f offset);
	sf::Vector2f getOffset();
	RigidBody* getRigidBody();
	bool hasPhysics();
	void setPhysics(bool onoff);
};

