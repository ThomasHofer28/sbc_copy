#include "RenderManager.h"
#include "SpriteRenderer.h"
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

bool depthSort(RenderComponent* lhs, RenderComponent* rhs) { 
	return (lhs->getDepth() < rhs->getDepth()); 
}

void RenderManager::draw(sf::RenderWindow& window)
{
	for (auto& vector : mRenderList)
	{
		std::sort(vector.begin(), vector.end(), depthSort);
	}

	for (const auto& layer : mRenderList) 
	{
		for (const auto& component : layer) 
		{
			if (component->isVisible())
				component->draw(window);
		}
	}
}

void RenderManager::add(RenderComponent* renderComponent)
{
	int layer = renderComponent->getLayer();
	mRenderList[layer].push_back(renderComponent);
}

void RenderManager::remove(RenderComponent* renderComponent)
{
	int layer = renderComponent->getLayer();

	if (std::find(mRenderList[layer].begin(), mRenderList[layer].end(), renderComponent) != mRenderList[layer].end())
	{
		mRenderList[layer].erase(std::find(mRenderList[layer].begin(), mRenderList[layer].end(), renderComponent));
	}
}

RenderManager& RenderManager::getInstance()
{
	static RenderManager instance;
	return instance;
}

RenderManager::RenderManager()
{
	mRenderList.resize(100);
}
