#pragma once
#include "Component.h"

class LayerComponent : public Component
{
public:
	LayerComponent() = delete;
	LayerComponent(sf::RenderWindow& window);
	void update() override;

private:
	sf::RenderWindow& mWindow;
};