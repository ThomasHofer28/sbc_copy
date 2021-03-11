#pragma once
#include "Collider.h"
#include <SFML/Graphics/ConvexShape.hpp>
#include "BoxCollider.h"
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

class CircleCollider;

class PolygonCollider : public Collider, public sf::ConvexShape
{
public:
	PolygonCollider(NLTmxMapObject* obj, NLTmxMapObjectGroup* group);
	PolygonCollider(std::vector<sf::Vector2f> points);
	PolygonCollider() = default;
	void update(sf::Time delta) override;
	void onStart() override;
	void draw(sf::RenderWindow& window) override;
	bool collidesWith(Collider* collider, MTV& mtv) override;
	bool vsPoly(std::vector<sf::Vector2f> verticesOther, std::vector<sf::Vector2f> axesOther, sf::Vector2f positionOther, MTV& mtv);
	bool vsCircle(CircleCollider& other, MTV& mtv);
	sf::Vector2f getCenter();
	std::vector<sf::Vector2f> getWorldPoints();
	std::vector<sf::Vector2f> getAxes();
	sf::ConvexShape mDebugShape;
	bool mIsParsed = false;
};

