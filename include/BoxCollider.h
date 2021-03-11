#pragma once
#include "Collider.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

class BoxCollider : public Collider, public sf::RectangleShape
{
public:
	bool collidesWith(Collider* collider, MTV& mtv) override;
	bool vsAABB(sf::RectangleShape& b, MTV& mtv);
	bool vsCircle(sf::CircleShape& a, MTV& mtv);
	void draw(sf::RenderWindow& window) override;
	void update(sf::Time) override;
	void onStart() override;
	sf::Vector2f getCenter();
	std::vector<sf::Vector2f> getWorldPoints();
	BoxCollider(NLTmxMapObject* obj, NLTmxMapObjectGroup* group);
	BoxCollider() = default;
};