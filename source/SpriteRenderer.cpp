#include "SpriteRenderer.h"
#include <iostream>
#include "GameObject.h"
#include <SFML/Graphics/RenderStates.hpp>
#include <unordered_map>
#include "RigidBody.h"
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

void SpriteRenderer::draw(sf::RenderWindow& window)
{
	window.draw(*this, mGo->getTransform());
}

void SpriteRenderer::onAwake()
{
	RenderComponent::onAwake();
	loadTexture(mFilename);
}

SpriteRenderer::SpriteRenderer(std::string filename)
	: mFilename(filename)
{
}

SpriteRenderer::SpriteRenderer(NLTmxMapObject* obj, NLTmxMapObjectGroup* group)
{
	std::map<string, string>::iterator it;
	if ((it = obj->properties.find("layer")) != obj->properties.end())
	{
		setLayer(mGo->getComponent<RigidBody>()->getLayer());
	}

	if ((it = obj->properties.find("filename")) != obj->properties.end())
	{
		mFilename = it->second;
	}
}

void SpriteRenderer::loadTexture(std::string filename, sf::IntRect rect)
{
	sf::Image image;
	std::string assetsPath = "../assets/";
	if (!image.loadFromFile(assetsPath + filename))
		std::cerr << "Image could not be loaded!";

	image.createMaskFromColor({ 255, 128, 255 }, 0);

	if (!mTexture.loadFromImage(image))
		std::cerr << "Texture could not be loaded!";

	setTexture(mTexture);
}

void SpriteRenderer::setOriginMid()
{
	sf::Vector2u size = mTexture.getSize();
	mGo->setOrigin(size.x / 2.0f, size.y / 2.0f);
}

sf::Texture& SpriteRenderer::getTexture()
{
	return mTexture;
}