#pragma once
#include "RenderComponent.h"
#include <SFML\Graphics\Sprite.hpp>
#include <vector>
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

class LayerRenderer : public RenderComponent
{
public:
	void add(std::unique_ptr<sf::Sprite> sprite);
	void draw(sf::RenderWindow& window) override;
	void onStart() override;
	void setScale(float scale);
	float getScale();

private:
	float mScale;
	std::vector<std::unique_ptr<sf::Sprite>> mSprites;
	sf::Transformable mTransformable;
};

