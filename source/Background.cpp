#include "Background.h"
#include "GameObject.h"
#include <iostream>
#include "GameObjectManager.h"
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

Background::Background(sf::RenderWindow& window) : mWindow(window)
{
}

void Background::onStart()
{
	float sizeX = (mWindow.getSize().x * 1.f) / (mTexture.getSize().x * 1.f);
	float sizeY = (mWindow.getSize().y * 1.f) / (mTexture.getSize().y * 1.f);
	mGo->setScale(sizeX,sizeY);
	setOriginMid();
}