#pragma once
#include "Component.h"
#include <string>
#include <SFML\Graphics\Sprite.hpp>
#include <SFML\Graphics\Texture.hpp>

class Sprite : public Component, sf::Sprite
{
public:
	Sprite() = default;
	Sprite(std::string filename);
	void loadTexture(std::string filename, sf::IntRect rect = sf::IntRect());
	void draw(sf::RenderWindow& window) override;
	void setOriginMid();
	sf::Texture& getTexture();

protected:
	sf::Texture mTexture;
};