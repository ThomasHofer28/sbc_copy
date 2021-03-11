#include "BirdWall.h"
#include "GameObject.h"
#include "RigidBody.h"
// (c) Coding: Femb�ck Samuel, Hofer Thomas
// FH - Salzburg

BirdWall::BirdWall(NLTmxMapObject* obj, NLTmxMapObjectGroup* group)
{
}

void BirdWall::onStart()
{
	auto rb = mGo->getComponent<RigidBody>();
	rb->setLayer(RigidBody::Layer::e_BirdWall);
	rb->setIgnore(RigidBody::Layer::e_PlayerWall, true);
	rb->setKinematic(true);
}
