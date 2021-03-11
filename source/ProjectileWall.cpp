#include "ProjectileWall.h"
#include "RigidBody.h"
#include "GameObject.h"
// (c) Coding: Femb�ck Samuel, Hofer Thomas
// FH - Salzburg

ProjectileWall::ProjectileWall(NLTmxMapObject* obj, NLTmxMapObjectGroup* group)
{
}

void ProjectileWall::onStart()
{
	auto rb = mGo->getComponent<RigidBody>();
	rb->setKinematic(true);
	rb->setIgnore(RigidBody::Layer::e_BirdWall, true);
}
