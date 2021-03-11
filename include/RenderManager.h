#pragma once
#include "RenderComponent.h"
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

class RenderManager
{
public:
	static RenderManager& getInstance();
	void add(RenderComponent* renderComponent);
	void draw(sf::RenderWindow& window);
	void remove(RenderComponent* renderComponent);
	RenderManager();

private:
	std::vector<std::vector<RenderComponent*>> mRenderList;
};