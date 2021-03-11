#pragma once

// TMX map file parser from https://github.com/catnapgames/TestNLTmxMap
// modification: getTilesetForGid added (WL)

#include <string>
#include <vector>
#include <map>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

using namespace std;



class NLTmxMapTileset {
public:
    int firstGid;
    string name;
    int tileWidth;
    int tileHeight;
    string filename;
};

struct NLTmxMapObjectProperty {
    string name;
    string value;
};

class NLTmxMapLayer {
public:
    string name;
    int width;
    int height;
    int* data;
    vector<NLTmxMapObjectProperty*> properties;
    
    ~NLTmxMapLayer() {
        delete [] data;
    }
};

class NLTmxMapObject {
public:
    string name;
	string type;
    int gid;
    int x;
    int y;
    int width;
    int height;
    map<string, string> properties;
    std::vector<sf::Vector2f> polygon;
    
    ~NLTmxMapObject() {
    }
};


class NLTmxMapObjectGroup {
public:
    string name;
    int width;
    int height;
    bool visible;
    sf::Color* color = nullptr;
    
    vector<NLTmxMapObject*> objects;
    
    ~NLTmxMapObjectGroup() {
        for ( auto o : objects ) {
            delete o;
        }

        delete color;
    }
};


class NLTmxMap {
public:
    
    int width;
    int height;
    int tileWidth;
    int tileHeight;
    
    vector<NLTmxMapTileset*> tilesets;
    vector<NLTmxMapLayer*> layers;
    vector<NLTmxMapObjectGroup*> groups;
    
    ~NLTmxMap() {
        for ( auto g : groups ) {
            delete g;
        }
        
        for ( auto l : layers ) {
            delete l;
        }
        
        for ( auto ts : tilesets ) {
            delete ts;
        }
    }
    
	NLTmxMapTileset* getTilesetForGid(int grid);
};


NLTmxMap* NLLoadTmxMap( char *xml );

