#pragma once
#include <SFML\Graphics\RenderWindow.hpp>
#include <SFML\Graphics\Sprite.hpp>
#include <vector>
#include <TGUI/TGUI.hpp>
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

typedef std::shared_ptr<sf::Sprite> SpritePtr;

class Game
{
public:
    void run();

private:
    bool init();
    void update();
    void draw();
    void shutdown();

    std::shared_ptr<sf::RenderWindow> mWindow;
};