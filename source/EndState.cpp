#include "EndState.h"
#include "GameObject.h"
#include "SpriteRenderer.h"
#include "GameObjectManager.h"
#include "InputManager.h"
#include "AnimationComponent.h"
#include <RigidBody.h>
#include <Map.h>
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

void EndState::init(StateManager* manager, std::shared_ptr<sf::RenderWindow> window)
{
    State::init(manager, window);

    // who won
    tgui::Label::Ptr winner = tgui::Label::create();
    winner->setText(mWinnner + " won, with " + std::to_string(mWinnerPoints) + " Points!");
    winner->setTextSize(60);
    winner->setPosition(sf::Vector2f(960,650) - (0.5f * winner->getFullSize()));
    winner->getRenderer()->setTextColor(tgui::Color(255, 255, 255, 255));
    mGui.add(winner, "Winner");
    
    if (mWinnner == "Player 1")
    {
        // idle animation player1
        auto player1idle = std::make_unique<GameObject>("player1idle");
        auto mPlayer1Idle = new AnimationComponent("Textures/blue.png", 16, 16);
        player1idle->addComponent(std::unique_ptr<AnimationComponent>(mPlayer1Idle));
        const int FPS = 8;
        auto idleSouth1 = new Animation("idle_south", { 0, 2 }, FPS, true);
        mPlayer1Idle->addAnimation(std::unique_ptr<Animation>(idleSouth1));
        mPlayer1Idle->play("idle_south");
        mPlayer1Idle->setLayer(RigidBody::Layer::e_UIPlayer);
        mPlayer1Idle->setScale(20.0f, 20.0f);
        player1idle->setPosition(960, 400);
        GameObjectManager::getInstance().add(std::move(player1idle));
    }
    else {
        // idle animation player2
        auto player2idle = std::make_unique<GameObject>("player2idle");
        auto mPlayer2Idle = new AnimationComponent("Textures/green.png", 16, 16);
        player2idle->addComponent(std::unique_ptr<AnimationComponent>(mPlayer2Idle));
        const int FPS = 8;
        auto idleSouth2 = new Animation("idle_south", { 0, 2 }, FPS, true);
        mPlayer2Idle->addAnimation(std::unique_ptr<Animation>(idleSouth2));
        mPlayer2Idle->play("idle_south");
        mPlayer2Idle->setLayer(RigidBody::Layer::e_UIPlayer);
        mPlayer2Idle->setScale(20.0f, 20.0f);
        player2idle->setPosition(960, 400);
        GameObjectManager::getInstance().add(std::move(player2idle));
    }
}

void EndState::exit()
{
    mWinnerPoints = 0;
    mGui.removeAllWidgets();
    InputManager::getInstance().clear();
    GameObjectManager::getInstance().clear();
}

void EndState::update(sf::Time delta)
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

void EndState::setWinner(std::string winner, int points)
{
    mWinnner = winner;
    mWinnerPoints = points;
}
