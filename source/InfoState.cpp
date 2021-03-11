#include "InfoState.h"
#include "GameObjectManager.h"
#include "SpriteRenderer.h"
#include "GameObject.h"
#include "InputManager.h"
#include "TimerManager.h"
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

void InfoState::init(StateManager* manager, std::shared_ptr<sf::RenderWindow> window)
{
    State::init(manager, window);

    auto backgroundInfo = std::make_unique<GameObject>("infoscreen");
    auto renderer = std::make_unique<SpriteRenderer>("../assets/info.png");
    backgroundInfo->addComponent(std::move(renderer));
    GameObjectManager::getInstance().add(std::move(backgroundInfo));

    TimerManager::getInstance().create(sf::seconds(2), [this]() {mInfoBuffer = true; });
}

void InfoState::exit()
{
    mInfoBuffer = false;
    GameObjectManager::getInstance().clear();
}

void InfoState::update(sf::Time delta)
{
    if (mInfoBuffer)
    {
        if (InputManager::getInstance().isKeyDown(sf::Keyboard::Space))
        {
            mManager->setState("menu");
        }

        if ((sf::Joystick::isConnected(0) && sf::Joystick::isButtonPressed(0, 0))
            || (sf::Joystick::isConnected(0) && sf::Joystick::isButtonPressed(0, 1))
            || (sf::Joystick::isConnected(1) && sf::Joystick::isButtonPressed(1, 0))
            || (sf::Joystick::isConnected(1) && sf::Joystick::isButtonPressed(1, 1)))
        {
            mManager->setState("menu");
        }
    }
}
