#include "Component.h"
#include "NLTmxMap.h"
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

class Wall : public Component
{
public:
	Wall(NLTmxMapObject* obj, NLTmxMapObjectGroup* group);
};