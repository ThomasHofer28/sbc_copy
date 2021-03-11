#include "LayerRenderer.h"
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

void LayerRenderer::add(std::unique_ptr<sf::Sprite> sprite)
{
	mSprites.push_back(std::move(sprite));
}

void LayerRenderer::draw(sf::RenderWindow& window)
{
	for (auto& sprite : mSprites)
		window.draw(*sprite, mTransformable.getTransform());
}

void LayerRenderer::onStart()
{

}

void LayerRenderer::setScale(float scale)
{
	mScale = scale;
}

float LayerRenderer::getScale()
{
	return mScale;
}
