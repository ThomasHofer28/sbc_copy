#include "RenderComponent.h"
#include "RenderManager.h"
#include "GameObject.h"
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

void RenderComponent::onDisable()
{
	RenderManager::getInstance().remove(this);
}

void RenderComponent::onEnable()
{
	RenderManager::getInstance().add(this);
}

int RenderComponent::getLayer()
{
	return mLayer;
}

void RenderComponent::setLayer(int layer)
{
	mLayer = layer;
}

void RenderComponent::setVisible(bool isVisible)
{
	mIsVisible = isVisible;
}

bool RenderComponent::isVisible()
{
	return mIsVisible;
}

float RenderComponent::getDepth()
{
	return mGo->getPosition().y;
}
