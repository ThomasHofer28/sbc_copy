#pragma once
#include <vector>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <map>
#include <string>
#include "SpriteRenderer.h"
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

struct Animation
{
    std::string name;
    sf::Time FrameTime;
    int FrameIdx;
    bool IsLoopable;
    std::vector<sf::IntRect> Frames;
    std::vector<int> Indices;

    Animation(std::string name,std::vector<int> indices, float fps, bool isLoopable);
    sf::IntRect& getActiveFrame();
    bool goToNextIndex();
};

class AnimationComponent : public SpriteRenderer
{
public:
    AnimationComponent(std::string filename, int frameWidth, int frameHeight);
    void onStart() override;
    void update(sf::Time delta) override;
    void addAnimation(std::unique_ptr<Animation> animation);
    void play(std::string name);
    void setActive(bool isActive);
    std::string getActiveAnimation();

private:
    sf::Time mElapsedTime;
    bool mIsPaused = true;
    Animation* mActiveAnimation = nullptr;
    std::map<std::string, std::unique_ptr<Animation>> mAnimations;
    int mFrameWidth = 0;
    int mFrameHeight = 0;
};