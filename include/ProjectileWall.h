#pragma once
#include "Component.h"
#include "NLTmxMap.h"
// (c) Coding: Femb�ck Samuel, Hofer Thomas
// FH - Salzburg

class ProjectileWall : public Component
{
public:
	ProjectileWall(NLTmxMapObject* obj, NLTmxMapObjectGroup* group);
	void onStart() override;
};

