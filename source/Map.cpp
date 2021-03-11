#include "Map.h"
#include "RigidBody.h"
#include "GameObject.h"
#include <random>
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

float Map::Scale = 3.75f;

sf::Vector2f Map::getSize()
{
	return mSize;
}

void Map::parseFromXML(NLTmxMap& map)
{
	mSize = sf::Vector2f(map.width * map.tileWidth, map.height * map.tileHeight);
}

sf::Vector2f Map::randomMapPosition()
{
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist6(1, 10000);
	sf::Vector2f outputVector = sf::Vector2f(dist6(rng) % (int)mSize.x, (dist6(rng) % (int)mSize.y));
	return outputVector;
}

void Map::onStart()
{
	mGo->getComponent<RigidBody>()->setKinematic(true);
	mGo->getComponent<RigidBody>()->setLayer(RigidBody::Layer::e_Map); 
}
