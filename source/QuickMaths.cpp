#include "QuickMaths.h"
#include <cmath>
#include <random>
# define M_PI           3.14159265358979323846  /* pi */
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

sf::Vector2f QuickMaths::normalize(const sf::Vector2f& vec)
{
	sf::Vector2f ret;
	float length = sqrt(vec.x * vec.x + vec.y * vec.y);
	ret.x = vec.x / length;
	ret.y = vec.y / length;
	return ret;
}

float QuickMaths::dot(const sf::Vector2f& vec1, const sf::Vector2f& vec2)
{
    return vec1.x * vec2.x + vec1.y * vec2.y;
}

float QuickMaths::length(const sf::Vector2f& vec)
{
	return sqrt(vec.x * vec.x + vec.y * vec.y);
}

sf::Vector2f QuickMaths::perp(const sf::Vector2f& vec)
{
	return sf::Vector2f(-vec.y, vec.x);
}

sf::Vector2f QuickMaths::negate(const sf::Vector2f& vec)
{
	return sf::Vector2f(-1 * vec.x, -1 * vec.y);
}

float QuickMaths::axisOrientation(const sf::Vector2f& vec)
{
	return atan2(vec.y, vec.x)* (180.0f / M_PI) + 180;
}

sf::Vector2f QuickMaths::newRandomPosition(const sf::Vector2f startCorner,const sf::Vector2f endCorner)
{
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist6(1, 10000);
	sf::Vector2f outputVector = sf::Vector2f(dist6(rng) % ((int)endCorner.x-(int)startCorner.x), (dist6(rng) % ((int)endCorner.y - (int)startCorner.y)));
	outputVector += startCorner;
	return outputVector;
}
