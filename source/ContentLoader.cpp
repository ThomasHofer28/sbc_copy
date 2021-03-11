#include "ContentLoader.h"
#include <SFML/System/FileInputStream.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "LayerRenderer.h"
#include "GameObjectManager.h"
#include "Map.h"
#include <functional>
#include "RigidBody.h"
#include "CircleCollider.h"
#include "BoxCollider.h"
#include "PolygonCollider.h"
#include "AnimationComponent.h"
#include "Player.h"
#include "Spawn.h"
#include "Wall.h"
#include "Pentagram.h"
#include "BirdWall.h"
#include "ProjectileWall.h"
#include "NoPointWall.h"
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

template<typename T>
void creator(NLTmxMapObject* obj, NLTmxMapObjectGroup* group, GameObject* go)
{
	auto comp = std::make_unique<T>(obj, group);
	go->addComponent(std::move(comp));
}

ContentLoader::ContentLoader()
{
	mCreators["c_spriteRenderer"] = creator<SpriteRenderer>;
	mCreators["c_rigidbody"] = creator<RigidBody>;
	mCreators["c_player"] = creator<Player>;
	mCreators["c_spawn"] = creator<Spawn>;
	mCreators["c_wall"] = creator<Wall>;
	mCreators["c_pentagram"] = creator<Pentagram>;
	mCreators["c_birdWall"] = creator<BirdWall>;
	mCreators["c_projectileWall"] = creator<ProjectileWall>;
	mCreators["c_nopointwall"] = creator<NoPointWall>;
}

GameObject* ContentLoader::loadMap(const string& filename, const sf::Vector2f& offset)
{
	sf::FileInputStream mapStream;
	if (!mapStream.open(filename))
	{
		sf::err() << "loadMap: could not open file " << filename << endl;
		return nullptr;
	}

	// convert FileInputStream to char* mapBuffer
	char* mapBuffer = new char[mapStream.getSize() + 1];
	mapStream.read(mapBuffer, mapStream.getSize());
	mapBuffer[mapStream.getSize()] = '\0';

	NLTmxMap* tilemap = NLLoadTmxMap(mapBuffer);
	delete[] mapBuffer;

	sf::err() << "Load tilemap with size: " << tilemap->width << ", "
		<< tilemap->height << " and tilesize: " << tilemap->tileWidth
		<< ", " << tilemap->tileHeight << std::endl;

	auto mapGo = std::make_unique<GameObject>("map");
	auto& map = mapGo->addComponent<Map>();
	map.parseFromXML(*tilemap);

	// load textures for every tileset
	for (auto tileset : tilemap->tilesets)
	{
		sf::err() << "Load tileset: " << tileset->name << " width filename: "
			<< tileset->filename << " and tilesize: " << tileset->tileWidth
			<< ", " << tileset->tileHeight << std::endl;

		auto texture = make_shared<sf::Texture>();
		if (!texture->loadFromFile(mAssetsPath + tileset->filename))
			sf::err() << "Could not load texture " << mAssetsPath + tileset->filename << endl;

		mTilesetTextures[tileset->name] = texture;
	}

	for (int layerIdx = 0; layerIdx < (int)tilemap->layers.size(); layerIdx++)
	{
		NLTmxMapLayer* layer = tilemap->layers[layerIdx];

		if (layer->name == "Don't include")
			continue;

		sf::err() << "Load layer: " << layer->name << " with width: "
			<< layer->width << " and height: " << layer->height << std::endl;

		int size = layer->width * layer->height;
		auto go = std::make_unique<GameObject>("layer_" + std::to_string(layerIdx));
		auto& layerRenderer = go->addComponent<LayerRenderer>();
		

		// iterate over all elements/tiles in the layer
		for (int i = 0; i < size; i++)
		{
			int gid = layer->data[i];

			if (gid == 0)
			{
				// 0 means there is no tile at this position.
				continue;
			}

			// get tileset for tile and allocate the corresponding tileset texture
			NLTmxMapTileset* tileset = tilemap->getTilesetForGid(gid);
			sf::Vector2i tileSize(tilemap->tileWidth, tilemap->tileHeight);
			sf::Texture& texture = *mTilesetTextures[tileset->name];

			// calculate position of tile
			sf::Vector2f position;
			position.x = (i % layer->width) * (float)tileSize.x;
			position.y = (i / layer->width) * (float)tileSize.y;
			position += offset;

			// number of tiles in tileset texture (horizontally)
			int tileCountX = texture.getSize().x / tileSize.x;

			// calculate 2d idx of tile in tileset texture
			int idx = gid - tileset->firstGid;
			int idxX = idx % tileCountX;
			int idxY = idx / tileCountX;

			// calculate source area of tile in tileset texture
			sf::IntRect source(idxX * tileSize.x, idxY * tileSize.y, tileSize.x, tileSize.y);  
			 
			// create tile (Sprite) and put it into the appropriate layer.
			auto sprite = make_unique<sf::Sprite>();
			sprite->setTexture(texture);
			sprite->setTextureRect(source);
			sprite->setPosition(position.x * Map::Scale, position.y * Map::Scale);
			sprite->setScale(Map::Scale, Map::Scale);

			layerRenderer.add(std::move(sprite));
		}

		GameObjectManager::getInstance().add(std::move(go));
	}

	GameObject* ret = mapGo.get();
	GameObjectManager::getInstance().add(std::move(mapGo));

	// go through all object layers
	for (auto group : tilemap->groups)
	{
		for (auto object : group->objects)
		{
			auto go = std::make_unique<GameObject>(object->name);

			sf::Vector2f position(object->x, object->y);
			position += offset;
			go->setPosition(position * Map::Scale);

			std::map<string, string>::iterator it;
			for (const auto& prop : object->properties)
			{
				auto creator = mCreators[prop.first];
				if (creator)
				{
					creator(object, group, go.get());
				}
			}

			GameObjectManager::getInstance().add(std::move(go));
		}
	}

	GameObjectManager::getInstance().updateLists();

	for (auto group : tilemap->groups)
	{
		if (group->name == "Colliders")
		{
			for (auto object : group->objects)
			{
				std::string boundTo = object->properties["boundTo"];
				auto go = GameObjectManager::getInstance().getObject(boundTo);

				if (object->type == "circle")
					creator<CircleCollider>(object, group, go);
				else if (object->type == "box")
					creator<BoxCollider>(object, group, go);
				else if (object->type == "polygon")
					creator<PolygonCollider>(object, group, go);
			}
		}
	}

	GameObjectManager::getInstance().update(sf::seconds(0));

	return ret;
}

ContentLoader& ContentLoader::getInstance()
{
	static ContentLoader instance;
	return instance;
}

void ContentLoader::clear()
{
	mTilesetTextures.clear();
	mCreators.clear();
}
