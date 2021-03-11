#pragma once
#include "Component.h"
#include "NLTmxMap.h"
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

class NoPointWall : public Component
{
public:
	NoPointWall(NLTmxMapObject* obj, NLTmxMapObjectGroup* group);
	void onStart() override;
};