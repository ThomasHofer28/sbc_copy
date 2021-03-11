#pragma once
#include <map>
#include <string>
#include <SFML/Audio.hpp>
#include <list>
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

class SoundManager
{
public:
	static SoundManager& getInstance();
	void play(std::string name);
	void addSound(std::string filename, std::string name);
	void playMusic(std::string filename);
	void pauseMusic();
	void resumeMusic();

	void update();
	SoundManager();


private:
	sf::Music mMusic;
	std::list<std::unique_ptr<sf::Sound>> mSounds;
	std::map<std::string, std::unique_ptr<sf::SoundBuffer>> mSoundBuffers;
};