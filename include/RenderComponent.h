#pragma once
#include "Component.h"
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

class RenderComponent : public Component
{
public:
	virtual void draw(sf::RenderWindow& window) = 0;
	void onDisable() override;
	void onEnable() override;
	int getLayer();
	void setLayer(int layer);
	void setVisible(bool isVisible);
	bool isVisible();
	float getDepth();

private:
	int mLayer = 0;
	bool mIsVisible = true;
};