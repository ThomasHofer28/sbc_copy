#pragma once
#include "Collider.h"
#include <SFML/Graphics/CircleShape.hpp>
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

class CircleCollider : public Collider, public sf::CircleShape
{
public:
	bool collidesWith(Collider* collider, MTV& mtv) override;
	bool vsCircle(sf::CircleShape& a, MTV& mtv);
	void draw(sf::RenderWindow& window) override;
	void update(sf::Time) override;
	void onStart() override;
	CircleCollider(NLTmxMapObject* obj, NLTmxMapObjectGroup* group);
	CircleCollider() = default;

private:
	bool mIsParsed = false;
};


