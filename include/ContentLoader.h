#pragma once
#include <unordered_map>
#include <SFML\System\Vector2.hpp>
#include <SFML\Graphics\Sprite.hpp>
#include <SFML\Graphics\Texture.hpp>
#include <memory>
#include <map>
#include "SpriteRenderer.h"
#include "NLTmxMap.h"
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

typedef std::shared_ptr<sf::Texture> TexturePtr;
typedef std::shared_ptr<SpriteRenderer> SpriteRendererPtr;
typedef void(*Creator)(NLTmxMapObject* obj, NLTmxMapObjectGroup* group, GameObject* go);

class ContentLoader
{
public:
	GameObject* loadMap(const std::string& filename, const sf::Vector2f& offset);
	static ContentLoader& getInstance();
	void clear();

private:
	ContentLoader();
	const std::string mAssetsPath = "../assets/";
	std::unordered_map <std::string, TexturePtr> mTilesetTextures;
	std::unordered_map <std::string, Creator> mCreators;
};
