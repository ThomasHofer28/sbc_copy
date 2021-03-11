#pragma once
#include "SpriteRenderer.h"
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

class Background : public SpriteRenderer
{
public:
	Background() = delete;
	Background(sf::RenderWindow& window);
	void onStart() override;

private:
	sf::RenderWindow& mWindow;
};