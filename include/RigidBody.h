#pragma once
#include "Component.h"
#include <vector>
#include <list>
#include "NLTmxMap.h"
// (c) Coding: Femb?ck Samuel, Hofer Thomas
// FH - Salzburg

class Collider;
class IRigidBodyObserver;

class RigidBody : public Component
{
public:
	enum Layer
	{
		e_Map,
		e_PlayerWall,
		e_BirdWall,
		e_ShootDirection,
		e_Pentagram,
		e_Blood,
		e_Candle,
		e_ShadowBird,
		e_Players,
		e_Sword,
		e_Bird,
		e_Colliders,
		e_UIPlayer,
		e_None
	};

	RigidBody(NLTmxMapObject* obj, NLTmxMapObjectGroup* group);
	RigidBody() = default;
	~RigidBody() override;
	void onAwake() override;
	void onStart() override;
	void onEnable() override;
	void onDisable() override;
	void correctPosition(sf::Time delta);
	void onCollision(Collider* other);
	sf::Vector2f getPosition();
	void setPosition(sf::Vector2f pos);
	float getInvMass();
	void setInvMass(float value);
	sf::Vector2f getAcceleration();
	void setAcceleration(sf::Vector2f acc);
	sf::Vector2f getVelocity();
	void setVelocity(sf::Vector2f vel);
	void addForce(sf::Vector2f force);
	void addImpulse(sf::Vector2f impulse);
	std::list<sf::Vector2f>& getForces();
	std::list<sf::Vector2f>& getImpulses();
	bool isKinematic();
	void setKinematic(bool isKinematic);
	void setIgnore(Layer layer, bool ignore);
	void addIgnore(RigidBody* rb);
	bool hasIgnore(Layer layer);
	bool hasIgnore(RigidBody* rb);
	Layer getLayer();
	void setLayer(Layer layer);
	sf::Vector2f mCorrection;

private:
	sf::Vector2f mPos;
	float mInvMass = 1;
	sf::Vector2f mAcceleration;
	sf::Vector2f mVelocity;
	std::list<sf::Vector2f> mForces;
	std::list<sf::Vector2f> mImpulses;
	std::vector<IRigidBodyObserver*> mObservers;
	bool mIsKinematic = false;
	Layer mLayer = Layer::e_None;
	std::vector<Layer> mIgnoredLayers;
	std::vector<RigidBody*> mIgnoredRbs;
};