#pragma once
#include "Component.h"
#include <SFML\System\Vector2.hpp>
#include "NLTmxMap.h"
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

class Map : public Component
{
public:
	static float Scale;
	sf::Vector2f getSize();
	void parseFromXML(NLTmxMap& map);
	sf::Vector2f randomMapPosition();
	void onStart() override;

private:
	sf::Vector2f mSize;
};

