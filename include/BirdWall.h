#pragma once
#include "Component.h"
#include "NLTmxMap.h"
// (c) Coding: Femb�ck Samuel, Hofer Thomas
// FH - Salzburg

class RigidBody;

class BirdWall : public Component
{
public:
	BirdWall(NLTmxMapObject* obj, NLTmxMapObjectGroup* group);
	void onStart() override;
};

