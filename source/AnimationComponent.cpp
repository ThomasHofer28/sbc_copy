#include "AnimationComponent.h"
#include <iostream>
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

Animation::Animation(std::string name, std::vector<int> indices, float fps, bool isLoopable)
    : Indices(indices)
    , FrameIdx(0)
    , FrameTime(sf::seconds(1 / fps))
    , IsLoopable(isLoopable)
    , name(name)
{
    Indices = indices;
}

sf::IntRect& Animation::getActiveFrame()
{
    return Frames[FrameIdx];
}

bool Animation::goToNextIndex()
{
    if (FrameIdx == Indices.size() - 1)
    {
        if (IsLoopable)
			FrameIdx = 0;

        return false;
    }

    ++FrameIdx;
    return true;
}

AnimationComponent::AnimationComponent(std::string filename, int frameWidth, int frameHeight) 
    : SpriteRenderer(filename)
    , mFrameWidth(frameWidth)
    , mFrameHeight(frameHeight)
{
    setOrigin(frameWidth/2.f, frameHeight/2.f);
}

void AnimationComponent::onStart()
{
	sf::Vector2u size = mTexture.getSize();
	int columnFrames = size.x / mFrameWidth;

    for (auto& pair : mAnimations)
    {
        auto& animation = pair.second;

		for (const int i : animation->Indices)
		{
			sf::Rect<int> rect((mFrameWidth * i) % size.x, mFrameHeight * (int)(i / columnFrames), mFrameWidth, mFrameHeight);
			animation->Frames.push_back(rect);
		}
    }

    setTextureRect(mActiveAnimation->getActiveFrame());
}

void AnimationComponent::update(sf::Time delta)
{
    if (!mIsPaused)
    {
        mElapsedTime += delta;

        if (mElapsedTime.asSeconds() >= mActiveAnimation->FrameTime.asSeconds())
        {
            mElapsedTime = sf::microseconds(mElapsedTime.asMicroseconds() % mActiveAnimation->FrameTime.asMicroseconds());

            if (!mActiveAnimation->goToNextIndex() && !mActiveAnimation->IsLoopable)
            {
				mIsPaused = true;
            }

            setTextureRect(mActiveAnimation->getActiveFrame());
        }
    }
}

void AnimationComponent::addAnimation(std::unique_ptr<Animation> animation)
{
	mAnimations[animation->name] = std::move(animation);
}

void AnimationComponent::play(std::string name)
{
    mActiveAnimation = mAnimations[name].get();
    mIsPaused = false;
}

void AnimationComponent::setActive(bool isActive)
{
    mIsPaused = !isActive;
}

std::string AnimationComponent::getActiveAnimation()
{
    return mActiveAnimation->name;
}
