#include "BoxCollider.h"
#include "CircleCollider.h"
#include "GameObject.h"
#include <memory>
#include <math.h>
#include <algorithm>
#include "PhysicsManager.h"
#include "PolygonCollider.h"
#include "QuickMaths.h"
#include <iostream>
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

bool BoxCollider::collidesWith(Collider* collider, MTV& mtv)
{
	bool collides = false;

	BoxCollider* box = dynamic_cast<BoxCollider*>(collider);

	if (box)
	{
		return vsAABB(*box, mtv);
	}
	
	CircleCollider* circle = dynamic_cast<CircleCollider*>(collider);

	if (circle)
	{
		bool collides = vsCircle(*circle, mtv);

		if (collides)
		{
			if (QuickMaths::dot(getCenter() - circle->getGameObject()->getPosition(), mtv.normal) >= 0)
				mtv.normal = QuickMaths::negate(mtv.normal);

			return true;
		}
	}
	

	PolygonCollider* polygon = dynamic_cast<PolygonCollider*>(collider);

	if (polygon)
	{
		bool collides = polygon->vsPoly(getWorldPoints(), { {1, 0}, {0, 1}, {-1, 0}, {0, -1} }, getPosition(), mtv);

		if (collides)
		{
			if (QuickMaths::dot(getCenter() - polygon->getCenter(), mtv.normal) >= 0)
				mtv.normal = QuickMaths::negate(mtv.normal);

			return true;
		}
	}

	return false;
}

void BoxCollider::draw(sf::RenderWindow& window) 
{
	if (getDebugDraw()) {
		window.draw(*this);
	}	
}

void BoxCollider::update(sf::Time)
{
	setPosition (mGo->getPosition() + getOffset());
}

void BoxCollider::onStart()
{
	Collider::onStart();
	setOutlineColor(sf::Color(255, 0, 0, 255));
	setOutlineThickness(2);
	setFillColor(sf::Color(0, 0, 0, 0));
	setOffset(sf::Vector2f(getPosition().x - mGo->getPosition().x, getPosition().y - mGo->getPosition().y));
	setDebugDraw(false);
}

std::vector<sf::Vector2f> BoxCollider::getWorldPoints()
{
	std::vector<sf::Vector2f> ret;

	for (int i = 0; i < getPointCount(); ++i)
	{
		ret.push_back(getPosition() + getPoint(i));
	}

	return ret;
}

BoxCollider::BoxCollider(NLTmxMapObject* obj, NLTmxMapObjectGroup* group)
{

	std::map<string, string>::iterator it;
	if ((it = obj->properties.find("hasPhysics")) != obj->properties.end())
	{
		bool hasPhysics;
		std::string val = it->second;
		std::istringstream(val) >> std::boolalpha >> hasPhysics;
		setPhysics(hasPhysics);
	}

	setPosition({ (float)obj->x * Map::Scale, (float)obj->y * Map::Scale});
	setSize({ (float)obj->width * Map::Scale, (float)obj->height * Map::Scale });
	setLayer(RigidBody::Layer::e_Colliders);
}

bool BoxCollider::vsAABB(sf::RectangleShape& b, MTV& mtv)
{
	sf::Vector2f aExtend = getSize() * 0.5f;
	sf::Vector2f bExtend = b.getSize() * 0.5f;
	auto centerA = getPosition() + aExtend;
	auto centerB = b.getPosition() + bExtend;
	auto distance = centerB - centerA;

	float xOverlap = aExtend.x + bExtend.x - abs(distance.x);
	if (xOverlap > 0)
	{
		float yOverlap = aExtend.y + bExtend.y - abs(distance.y);

		if (yOverlap > 0)
		{
			if (xOverlap < yOverlap)
			{
				mtv.normal = distance.x <= 0 ? sf::Vector2f(-1, 0) : sf::Vector2f(1, 0);
				mtv.penetration = xOverlap;
			}
			else
			{
				mtv.normal = distance.y <= 0 ? sf::Vector2f(0, -1) : sf::Vector2f(0, 1);
				mtv.penetration = yOverlap;
			}

			return true;
		}
	}

	return false;
}

bool BoxCollider::vsCircle(sf::CircleShape& a, MTV& mtv)
{
	sf::Vector2f circleCenter = sf::Vector2f(a.getPosition().x + a.getRadius(), a.getPosition().y + a.getRadius());

	float testX = circleCenter.x;
	float testY = circleCenter.y;

	if (circleCenter.x < getPosition().x)
	{
		testX = getPosition().x;
	}
	else if (circleCenter.x > getPosition().x + getSize().x)
	{
		testX = getPosition().x + getSize().x;
	}

	if (circleCenter.y < getPosition().y)
	{
		testY = getPosition().y;
	}
	else if (circleCenter.y > getPosition().y + getSize().y)
	{
		testY = getPosition().y + getSize().y;
	}

	float distX = circleCenter.x - testX;
	float distY = circleCenter.y - testY;
	float distance = sqrt((distX * distX) + (distY * distY));

	if (distance <= a.getRadius()) 
	{
		if (abs(distX) > abs(distY)) 
		{
			if (distX != 0) {
				mtv.normal = sf::Vector2f(distX / (sqrt(distX * distX)), 0);
				mtv.penetration = a.getRadius() - distance;
			}
		}
		else 
		{
			if (distY != 0)
			{
				mtv.normal = sf::Vector2f(0, distY / (sqrt(distY * distY)));
				mtv.penetration = a.getRadius() - distance;
			}

		}
		return true;
	}

	return false;
}

sf::Vector2f BoxCollider::getCenter()
{
	float x = 0, y = 0;

	for (const auto& point : getWorldPoints())
	{
		x += point.x;
		y += point.y;
	}

	return sf::Vector2f(x / getPointCount(), y / getPointCount());
}
