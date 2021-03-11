#include "GameState.h"
#include "GameObjectManager.h"
#include <SFML\Graphics\RenderWindow.hpp>
#include "InputManager.h"
#include <iostream>
#include "ContentLoader.h"
#include "RenderManager.h"
#include "Map.h"
#include <random>
#include "BoxCollider.h"
#include "CircleCollider.h"
#include "AnimationComponent.h"
#include "Gamepad.h"
#include "SoundManager.h"
#include "Bird.h"
#include "Pentagram.h"
#include "Player.h"
#include "EndState.h"
#include "State.h"
#include "TimerManager.h"
#include "RigidBody.h"
// (c) Coding: Femb?ck Samuel, Hofer Thomas
// FH - Salzburg

void GameState::init(StateManager* manager, std::shared_ptr<sf::RenderWindow> window)
{
    State::init(manager, window);
    mWindow = window.get();
    mIsEndTime = false;
    mBirdCoutner = 0;
    // free assets used from https://0x72.itch.io/16x16-dungeon-tileset
    GameObject* mapGo = ContentLoader::getInstance().loadMap("../assets/Map.tmx", sf::Vector2f());
    mapGo->setScale(3, 3);
    auto mapRb = mapGo->getComponent<RigidBody>();
    mapRb->setLayer(RigidBody::Layer::e_PlayerWall);
    mapRb->setIgnore(RigidBody::Layer::e_BirdWall, true);

    createCandles();

    // remove gamestate tests when in production
    TimerManager::getInstance().create(sf::seconds(30), [] {
        if (!dynamic_cast<GameState*>(StateManager::getInstance().getCurrentState()))
            return;
        auto pentagram = GameObjectManager::getInstance().getObject("Pentagram")->getComponent<Pentagram>();
        pentagram->traverse({ 2,3,2,3 }, 150); 
        });

    TimerManager::getInstance().create(sf::seconds(60), [] {
        if (!dynamic_cast<GameState*>(StateManager::getInstance().getCurrentState()))
            return;

        auto pentagram = GameObjectManager::getInstance().getObject("Pentagram")->getComponent<Pentagram>();
        pentagram->traverse({ 1,2,1,2,1,2 }, 50); 
        });

    TimerManager::getInstance().create(sf::seconds(70), [] {
        if (!dynamic_cast<GameState*>(StateManager::getInstance().getCurrentState()))
            return;

        auto pentagram = GameObjectManager::getInstance().getObject("Pentagram")->getComponent<Pentagram>();
        pentagram->traverse({ 0,1,0,1 }, 150); 
        });

    TimerManager::getInstance().create(sf::seconds(10), [] {
        if (!dynamic_cast<GameState*>(StateManager::getInstance().getCurrentState()))
            return;

        auto pentagram = GameObjectManager::getInstance().getObject("Pentagram")->getComponent<Pentagram>();
        pentagram->traverse({ 3,4,3,4 }, 50); 
        });

    TimerManager::getInstance().create(sf::seconds(10), [] {
        if (!dynamic_cast<GameState*>(StateManager::getInstance().getCurrentState()))
            return;

        auto pentagram = GameObjectManager::getInstance().getObject("Pentagram")->getComponent<Pentagram>();
        pentagram->traverse({ 4,5,4,5 }, 50); 
        });

    // gui player 1 points
    tgui::Label::Ptr p1points = tgui::Label::create();
    p1points->setText(std::to_string(GameObjectManager::getInstance().getObject("Player 1")->getComponent<Player>()->getPoints()));
    p1points->setTextSize(42);
    p1points->setPosition(GameObjectManager::getInstance().getObject("Player 1")->getPosition());
    p1points->getRenderer()->setTextColor(tgui::Color(0, 183, 255, 100));
    mGui.add(p1points, "P1Points");

    // gui player 2 points
    tgui::Label::Ptr p2points = tgui::Label::create();
    p2points->setText(std::to_string(GameObjectManager::getInstance().getObject("Player 2")->getComponent<Player>()->getPoints()));
    p2points->setTextSize(42);
    p2points->setPosition(GameObjectManager::getInstance().getObject("Player 2")->getPosition());
    p2points->getRenderer()->setTextColor(tgui::Color(86, 182, 45, 100));
    mGui.add(p2points, "P2Points");



    // time
    tgui::Label::Ptr timeBox = tgui::Label::create();
    timeBox->setTextSize(40);
    timeBox->setPosition(890, -3);
    timeBox->getRenderer()->setTextColor(tgui::Color(255, 255, 255, 255));
    mGui.add(timeBox, "timer");

    std::vector<GameObject*> players = { GameObjectManager::getInstance().getObject("Player 1"), GameObjectManager::getInstance().getObject("Player 2") };

    for (auto player : players)
    {
        PlusPoint pp;
        pp.player = player->getComponent<Player>();
        pp.isVisible = false;
        pp.label = tgui::Label::create();
		pp.label->setAutoSize(true);
        if (pp.player->getGamepad()->getIdx() == 0)
            pp.label->getRenderer()->setTextColor({ 0, 183, 255 });
        else
            pp.label->getRenderer()->setTextColor({ 0, 255, 0 });
         
        mGui.add(pp.label);
        mPlusPoints[pp.player] = pp;
    }

}

void GameState::exit()
{
    // check winner and points
    int player1points = GameObjectManager::getInstance().getObject("Player 1")->getComponent<Player>()->getPoints();
    int player2points = GameObjectManager::getInstance().getObject("Player 2")->getComponent<Player>()->getPoints();
    std::string winner;
    int points = 0;
    if (player1points > player2points)
    {
        winner = "Player 1";
        points = player1points;
    }
    else if (player2points > player1points)
    {
        winner = "Player 2";
        points = player2points;
    }
    else {
        winner = "Both";
        points = player1points;
    }
    auto endState = dynamic_cast<EndState*>(StateManager::getInstance().getStates()["end"]);
    endState->setWinner(winner,points);

    mGamepad = nullptr;
    mSound = nullptr;
    mSoundBuffer = nullptr;
    mBirdSpawnTime = 0.0f;
    mPlayer1Idle = nullptr;
    mPlayer2Idle = nullptr;
    mEndTimer = 3.0f;
    mGameTimer = 120;
    mIsEndTime = false;
    mBirdSpawnCooldown = 3;
    
    mGui.removeAllWidgets();
    GameObjectManager::getInstance().clear();
    InputManager::getInstance().clear();
}

void GameState::update(sf::Time delta)
{
    for (auto& pair : mPlusPoints)
    {
        PlusPoint& pp = pair.second;
        if (pp.isVisible && pp.clock.getElapsedTime() > pp.showInterval)
        {
            pp.label->setVisible(false);
        }
    }

    mBirdSpawnTime += delta.asSeconds();
    if (mBirdSpawnCooldown > 0.5)
    {
        mBirdSpawnCooldown -= delta.asSeconds() / 40.f;
	}

	if (mBirdSpawnTime > mBirdSpawnCooldown)
	{
		if (Bird::mInstances < 25)
		{
			auto gobird = std::make_unique<GameObject>("bird" + std::to_string(mBirdCoutner));
			auto bird = std::make_unique<Bird>();
			gobird->addComponent(std::move(bird));
			GameObjectManager::getInstance().add(std::move(gobird));
            mBirdCoutner++;
		}

		mBirdSpawnTime = 0;
	}

	// UI punkte update
	tgui::Label::Ptr labelplayer1 = mGui.get<tgui::Label>("P1Points");
    auto player1 = GameObjectManager::getInstance().getObject("Player 1");
    labelplayer1->setText(std::to_string(player1->getComponent<Player>()->getPoints()));
    labelplayer1->setPosition(player1->getPosition() + sf::Vector2f(0, -80) + sf::Vector2f(-0.5 * labelplayer1->getFullSize()));
    tgui::Label::Ptr labelplayer2 = mGui.get<tgui::Label>("P2Points");
    auto player2 = GameObjectManager::getInstance().getObject("Player 2");
    labelplayer2->setText(std::to_string(player2->getComponent<Player>()->getPoints()));
    labelplayer2->setPosition(player2->getPosition() + sf::Vector2f(0, -80) + sf::Vector2f(-0.5 * labelplayer2->getFullSize()));

	// UI timer update
	if (mGameTimer >= 0)
	{
		mGameTimer -= delta.asSeconds();
		int minutes = (int)(mGameTimer / 60);
		int seconds = (int)mGameTimer % 60;
		tgui::Label::Ptr timeBox = mGui.get<tgui::Label>("timer");
		timeBox->setText(std::to_string(minutes) + " : " + std::to_string(seconds));
	}
    else
    {
		mManager->setState("end");
    }
}

void GameState::createCandles()
{
	auto animFlicker = new Animation("flicker", { 1, 2, 3, 4, 5, 6, 7, 8 }, 5, true);
	auto animOff = new Animation("off", { 0 }, 5, true);
	auto candles = GameObjectManager::getInstance().getObjects("Candle");

	for (const auto& candle : candles)
	{

		auto animFlickerCpy = new Animation(*animFlicker);
		auto animOffCpy = new Animation(*animOff);
		auto animCmp = new AnimationComponent("Textures/candle.png", 16, 23);
		animCmp->addAnimation(std::unique_ptr<Animation>(animFlickerCpy));
		animCmp->addAnimation(std::unique_ptr<Animation>(animOffCpy));
		animCmp->play("flicker");
		animCmp->setLayer(RigidBody::Layer::e_Candle);
		animCmp->setScale(Map::Scale, Map::Scale);
		candle->addComponent(std::unique_ptr<AnimationComponent>(animCmp));
	}
}

void GameState::displayPlusPoints(Player* player, int points, sf::Vector2f position)
{
	PlusPoint& pp = mPlusPoints[player];
	pp.clock.restart();
	pp.isVisible = true;
	pp.label->setText("+" + std::to_string(points));
	if (points > 10)
		pp.label->setText(pp.label->getText() + "!");
	pp.label->setTextSize(std::min(24 + points * 4, 80));
	pp.label->setVisible(true);
	pp.label->setPosition(position + sf::Vector2f(-0.5 * pp.label->getFullSize()));
}

bool GameState::checkEndState()
{
	return mIsEndTime;
}
