#pragma once
#include "RenderComponent.h"
#include <string>
#include <SFML\Graphics\Sprite.hpp>
#include <SFML\Graphics\Texture.hpp>
#include "NLTmxMap.h"
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

class SpriteRenderer : public RenderComponent, public sf::Sprite
{
public:
	SpriteRenderer() = default;
	SpriteRenderer(std::string filename);
	SpriteRenderer(NLTmxMapObject* obj, NLTmxMapObjectGroup* group);
	void loadTexture(std::string filename, sf::IntRect rect = sf::IntRect());
	void draw(sf::RenderWindow& window) override;
	void onAwake() override;
	void setOriginMid();
	sf::Texture& getTexture();

protected:
	sf::Texture mTexture;
	std::string mFilename;
};