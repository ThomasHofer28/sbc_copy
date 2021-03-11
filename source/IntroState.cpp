#include "IntroState.h"
#include "Game.h"
#include "SpriteRenderer.h"
#include "GameObjectManager.h"
#include "InputManager.h"
#include <iostream>
#include <TGUI/TGUI.hpp>
#include "Player.h"
#include "TimerManager.h"
#include "SoundManager.h"
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

void IntroState::init(StateManager* manager, std::shared_ptr<sf::RenderWindow> window)
{
    State::init(manager, window);
    // free sound from https://kenney.nl/
    SoundManager::getInstance().addSound("../assets/Sound/drawKnife1.ogg", "knife");
    // free sound from https://opengameart.org/content/swish-bamboo-stick-weapon-swhoshes
    SoundManager::getInstance().addSound("../assets/Sound/dash1.flac","dash");
    // royalty free
    SoundManager::getInstance().playMusic("../assets/Sound/dark.wav");
    // free sound effects from https://www.fesliyanstudios.com
    SoundManager::getInstance().addSound("../assets/Sound/blood.wav","blood");

    auto background = std::make_unique<GameObject>("background");
    auto renderer = std::make_unique<SpriteRenderer>("../assets/start.png");
    background->addComponent(std::move(renderer));
    GameObjectManager::getInstance().add(std::move(background));
}

void IntroState::exit()
{
    GameObjectManager::getInstance().clear();
}

void IntroState::update(sf::Time delta)
{
    if (mJustOnce)
    {
        TimerManager::getInstance().create(sf::seconds(2), [this]() {mManager->setState("credits"); });
        mJustOnce = false;
    }
}