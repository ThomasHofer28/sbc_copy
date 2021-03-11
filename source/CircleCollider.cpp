#include "CircleCollider.h"
#include "BoxCollider.h"
#include "GameObject.h"
#include "QuickMaths.h"
#include "PhysicsManager.h"
#include "PolygonCollider.h"
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

bool CircleCollider::collidesWith(Collider* collider, MTV& mtv)
{
	
	BoxCollider* box = dynamic_cast<BoxCollider*>(collider);

	if (box)
	{
		bool collides = box->vsCircle(*this, mtv);

		if (collides)
		{
			if (QuickMaths::dot(mGo->getPosition() - box->getCenter(), mtv.normal) >= 0)
				mtv.normal = QuickMaths::negate(mtv.normal);

			return true;
		}
	}

	CircleCollider* circle = dynamic_cast<CircleCollider*>(collider);

	if (circle)
	{
		return vsCircle(*circle, mtv);
	}

	PolygonCollider* poly = dynamic_cast<PolygonCollider*>(collider);
	
	if (poly)
	{
		bool collides = poly->vsCircle(*this, mtv);

		if (collides)
		{
			if (QuickMaths::dot(poly->getCenter() - mGo->getPosition(), mtv.normal) >= 0)
				mtv.normal = QuickMaths::negate(mtv.normal);

			return true;
		}

	}
}

void CircleCollider::draw(sf::RenderWindow& window)
{
	if (getDebugDraw()) {
		window.draw(*this);
	}
}

void CircleCollider::update(sf::Time)
{
	setPosition(mGo->getPosition() + getOffset());
}

void CircleCollider::onStart()
{
	Collider::onStart();

	setOutlineColor(sf::Color(0, 255, 0, 255));
	setOutlineThickness(2);
	setFillColor(sf::Color(0, 255, 0, 0));

	if (mIsParsed)
	{
		setOffset(sf::Vector2f(getPosition().x - mGo->getPosition().x, getPosition().y - mGo->getPosition().y));
	}

	setDebugDraw(false);
}

CircleCollider::CircleCollider(NLTmxMapObject* obj, NLTmxMapObjectGroup* group)
{
	mIsParsed = true;

	std::map<string, string>::iterator it;
	if ((it = obj->properties.find("hasPhysics")) != obj->properties.end())
	{
		bool hasPhysics;
		std::string val = it->second;
		std::istringstream(val) >> std::boolalpha >> hasPhysics;
		setPhysics(hasPhysics);
	}

	setRadius(obj->height * 0.5f * Map::Scale);
	setPosition({ (float)obj->x * Map::Scale, (float)obj->y * Map::Scale });
	setLayer(RigidBody::Layer::e_Colliders);
}

bool CircleCollider::vsCircle(sf::CircleShape& b, MTV& mtv) 
{
	sf::Vector2f difference = sf::Vector2f(getPosition().x + getRadius(),getPosition().y + getRadius()) 
		- sf::Vector2f(b.getPosition().x + b.getRadius(), b.getPosition().y + b.getRadius());
	float length = QuickMaths::length(difference);
	float radiusSum = getRadius() + b.getRadius();

	if (length == 0)
	{
		mtv.normal = sf::Vector2f(-1 , -1 );
		mtv.penetration = -1;
		return true;
	}

	if (length < radiusSum)
	{
		mtv.normal = sf::Vector2f(-1 / length * difference.x, -1 / length * difference.y);
		mtv.penetration = radiusSum - length;
		return true;
	}

	return false;
}
