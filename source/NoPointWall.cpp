#include "NoPointWall.h"
#include "GameObject.h"
#include "RigidBody.h"
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

NoPointWall::NoPointWall(NLTmxMapObject* obj, NLTmxMapObjectGroup* group)
{
}

void NoPointWall::onStart()
{
	auto rb = mGo->getComponent<RigidBody>();
	rb->setIgnore(RigidBody::Layer::e_PlayerWall, true);
	rb->setKinematic(true);
}