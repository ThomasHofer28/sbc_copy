#include "SoundManager.h"
#include <iostream>
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

SoundManager& SoundManager::getInstance()
{
	static SoundManager instance;
	return instance;
}

void SoundManager::play(std::string name)
{
	if (mSounds.size() >= 256)
		return;


	std::unique_ptr<sf::Sound> sound = std::make_unique<sf::Sound>();
	sound->setBuffer(*mSoundBuffers[name]);
	sound->play();

	if (name == "dash")
	{
		sound->setVolume(20.0f);
	}
	if (name == "knife")
	{
		sound->setVolume(80.0f);
	}
	if (name == "blood")
	{
		sound->setVolume(80.0f);
	}

	mSounds.push_back(std::move(sound));

}

void SoundManager::addSound(std::string filename, std::string name)
{
	std::unique_ptr<sf::SoundBuffer> buffer = std::make_unique<sf::SoundBuffer>();
	buffer->loadFromFile(filename);
	mSoundBuffers.insert(std::make_pair(name,std::move(buffer)));
}

void SoundManager::playMusic(std::string filename)
{
	if (!mMusic.openFromFile(filename))
	{
		std::cout << "music cant be loaded" << std::endl;
	}
	mMusic.setVolume(75.0f);
	mMusic.play();
	mMusic.setLoop(true);
}

void SoundManager::pauseMusic()
{
	mMusic.pause();
}

void SoundManager::resumeMusic()
{
	mMusic.play();
}

void SoundManager::update()
{
	if (mSounds.size() > 0)
	{
		if (mSounds.front()->getStatus() == sf::Sound::Status::Stopped)
		{
			mSounds.pop_front();
		}
	}
}

SoundManager::SoundManager()
{
}
