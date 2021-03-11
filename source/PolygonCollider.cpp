#include "PolygonCollider.h"
#include "GameObject.h"
#include <iostream>
#include "QuickMaths.h"
#include "CircleCollider.h"
#include "PhysicsManager.h"
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

PolygonCollider::PolygonCollider(NLTmxMapObject* obj, NLTmxMapObjectGroup* group)
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

    setPointCount(obj->polygon.size());

    for (int i = 0; i < obj->polygon.size(); ++i)
    {
        setPoint(i, obj->polygon[i] * Map::Scale);
    }
    
    //setOutlineColor(sf::Color(0, 255, 0, 255));
	//setOutlineThickness(2);
	setFillColor(sf::Color(0, 255, 0, 0));
    setPosition(obj->x * Map::Scale, obj->y * Map::Scale);
    setLayer(RigidBody::Layer::e_Colliders);
}

PolygonCollider::PolygonCollider(std::vector<sf::Vector2f> points)
{
	setPointCount(points.size());

	for (int i = 0; i < points.size(); ++i)
	{
		setPoint(i, points[i]);
	}

	//setOutlineColor(sf::Color(0, 255, 0, 255));
	//setOutlineThickness(2);
	setFillColor(sf::Color(0, 255, 0, 0));
	setLayer(RigidBody::Layer::e_Colliders);
}

void PolygonCollider::update(sf::Time delta)
{
    setPosition(mGo->getPosition() + getOffset());
}

void PolygonCollider::onStart()
{
	Collider::onStart();

	if (mIsParsed)
		setOffset(sf::Vector2f(getPosition().x - mGo->getPosition().x, getPosition().y - mGo->getPosition().y));

	setDebugDraw(false);
}

void PolygonCollider::draw(sf::RenderWindow& window)
{
	if(getDebugDraw())
		window.draw(*this);
}

struct Projection
{
	float min, max;
};

std::vector<sf::Vector2f> PolygonCollider::getAxes()
{
	std::vector<sf::Vector2f> result;
	auto points = getWorldPoints();
	sf::Vector2f edge;

	for (int i = 0; i < points.size(); ++i)
	{
		edge.x = points[(i + 1) % points.size()].x - points[i].x;
		edge.y = points[(i + 1) % points.size()].y - points[i].y;
		result.push_back(QuickMaths::normalize(QuickMaths::perp(edge)));
	}

	return result;
}

Projection getProjection(std::vector<sf::Vector2f>& points, sf::Vector2f& axis)
{
	Projection result;
	result.min = QuickMaths::dot(points[0], axis);
	result.max = result.min;
	float projection;

	for (int i = 1; i < points.size(); ++i)
	{
		projection = QuickMaths::dot(points[i], axis);

		if (projection < result.min)
			result.min = projection;
		else if (projection > result.max)
			result.max = projection;
	}

	return result;
}

bool PolygonCollider::vsPoly(std::vector<sf::Vector2f> verticesOther, std::vector<sf::Vector2f> axesOther, sf::Vector2f positionOther, MTV& mtv)
{
	auto p1Points = getWorldPoints();
	auto& p2Points = verticesOther;
	std::vector<sf::Vector2f> p1Axes = getAxes();
	std::vector<sf::Vector2f>& p2Axes = axesOther;

	mtv.penetration = std::numeric_limits<float>().max();

	for (int i = 0; i < p1Axes.size(); ++i)
	{
		Projection proj1 = getProjection(p1Points, p1Axes[i]);
		Projection proj2 = getProjection(p2Points, p1Axes[i]);

		double overlap = std::max(0.f, std::min(proj1.max, proj2.max) - std::max(proj1.min, proj2.min));
		if (overlap <= 0)
			return false;

		if (overlap < mtv.penetration)
		{
			mtv.penetration = (float)overlap;
			mtv.normal = p1Axes[i];
		}
	}

	for (int i = 0; i < p2Axes.size(); ++i)
	{
		Projection proj1 = getProjection(p1Points, p2Axes[i]);
		Projection proj2 = getProjection(p2Points, p2Axes[i]);

		double overlap = std::max(0.f, std::min(proj1.max, proj2.max) - std::max(proj1.min, proj2.min));
		if (overlap <= 0)
			return false;

		if (overlap < mtv.penetration)
		{
			mtv.penetration = (float)overlap;
			mtv.normal = p2Axes[i];
		}
	}

	return true;
}

std::vector<sf::Vector2f> PolygonCollider::getWorldPoints() 
{
	std::vector<sf::Vector2f> ret(getPointCount());

	for (int i = 0; i < getPointCount(); ++i)
	{
		ret[i] = getPosition() + getPoint(i);
	}

	return ret;
}

bool PolygonCollider::vsCircle(CircleCollider& other, MTV& mtv)
{
	auto p1Points = getWorldPoints();
	std::vector<sf::Vector2f> axes = getAxes();

	auto circleCenter = other.getGameObject()->getPosition();
	auto p2Points = std::vector<sf::Vector2f>{ circleCenter };

	mtv.penetration = std::numeric_limits<float>().max();

	sf::Vector2f closestPoint;
	float closestDistance = std::numeric_limits<float>().max();
	for (const auto& point : p1Points)
	{
		float lengthSquared = pow(point.x - circleCenter.x, 2) + pow(point.y - circleCenter.y, 2);
		if (lengthSquared < closestDistance)
		{
			closestDistance = lengthSquared;
			closestPoint = point;
		}
	}

	sf::Vector2f circleAxis = QuickMaths::normalize(closestPoint - circleCenter);
	axes.push_back(circleAxis);

	for (int i = 0; i < axes.size(); ++i)
	{
		Projection proj1 = getProjection(p1Points, axes[i]);
		Projection proj2 = getProjection(p2Points, axes[i]);
		proj2.min -= other.getRadius();
		proj2.max += other.getRadius();

		double overlap = std::max(0.f, std::min(proj1.max, proj2.max) - std::max(proj1.min, proj2.min));
		if (overlap <= 0)
			return false;

		if (overlap < mtv.penetration)
		{
			mtv.penetration = (float)overlap;
			mtv.normal = axes[i];
		}
	}

	return true;
}

sf::Vector2f PolygonCollider::getCenter()
{
	float x = 0, y = 0;

	for (const auto& point : getWorldPoints())
	{
		x += point.x;
		y += point.y;
	}

	return sf::Vector2f(x / getPointCount(), y / getPointCount());
}

bool PolygonCollider::collidesWith(Collider* collider, MTV& mtv)
{
	PolygonCollider* poly = dynamic_cast<PolygonCollider*>(collider);

	if (poly)
	{
		return vsPoly(poly->getWorldPoints(), poly->getAxes(), poly->getPosition(), mtv);
	}

	CircleCollider* circle = dynamic_cast<CircleCollider*>(collider);

	if (circle)
	{
		bool collides = vsCircle(*circle, mtv);

		if (collides)
		{
			if (QuickMaths::dot(getCenter() - circle->getPosition(), mtv.normal) >= 0)
			{
				mtv.normal = QuickMaths::negate(mtv.normal);
			}

			return true;
		}
	}

	BoxCollider* box = dynamic_cast<BoxCollider*>(collider);

	if (box)
	{
		bool collides = vsPoly(box->getWorldPoints(), { {1, 0}, {0, 1}, {-1, 0}, {0, -1} }, box->getPosition(), mtv);
		
		if (collides)
		{
			if (QuickMaths::dot(getCenter() - box->getCenter(), mtv.normal) >= 0)
				mtv.normal = QuickMaths::negate(mtv.normal);

			return true;
		}
	}
	
    return false;
}
