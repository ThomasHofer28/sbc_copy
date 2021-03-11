#include "Sprite.h"
#include <iostream>
#include "GameObject.h"

void Sprite::draw(sf::RenderWindow& window)
{
	window.draw(*this, mGo->getTransform());
}

Sprite::Sprite(std::string filename)
{
	loadTexture(filename);
}

void Sprite::loadTexture(std::string filename, sf::IntRect rect)
{
	sf::Image image;
	if (!image.loadFromFile(filename))
		std::cerr << "Image could not be loaded!";

	image.createMaskFromColor({ 255, 128, 255 }, 0);

	if (!mTexture.loadFromImage(image))
		std::cerr << "Texture could not be loaded!";

	mTexture.setSmooth(true);
	setTexture(mTexture);
}

void Sprite::setOriginMid()
{
	sf::Vector2u size = mTexture.getSize();
	mGo->setOrigin(size.x / 2.0f, size.y / 2.0f);
}

sf::Texture& Sprite::getTexture()
{
	return mTexture;
}