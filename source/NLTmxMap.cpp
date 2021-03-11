#include <cassert>
#include <iostream>
#include "rapidxml/rapidxml.hpp"
#include "NLTmxMap.h"
#include <SFML/Graphics/Color.hpp>
#pragma warning(disable: 4996)
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

using namespace rapidxml;

NLTmxMapTileset* NLTmxMap::getTilesetForGid(int grid)
{
	NLTmxMapTileset* result = nullptr;

	for (auto tileset : tilesets)
	{
		if (tileset->firstGid > grid)
		{
			break;
		}

		result = tileset;
	}

	return result;
}

NLTmxMap* NLLoadTmxMap( char *xml )
{
    xml_document<> doc;
	doc.parse<0>(xml);
    
    xml_node<> *mapnode = doc.first_node("map");
    
    NLTmxMap* map = new NLTmxMap();
    
    map->width = atoi( mapnode->first_attribute( "width" )->value() );
    map->height = atoi( mapnode->first_attribute( "height" )->value() );
    map->tileWidth = atoi( mapnode->first_attribute( "tilewidth" )->value() );
    map->tileHeight = atoi( mapnode->first_attribute( "tileheight" )->value() );
    
    xml_node<> *tilesetnode = mapnode->first_node( "tileset" );
    
    while ( tilesetnode ) {
        NLTmxMapTileset* tileset = new NLTmxMapTileset();
        
        tileset->firstGid = atoi( tilesetnode->first_attribute( "firstgid" )->value() );
        tileset->name = tilesetnode->first_attribute( "name" )->value();
        tileset->tileWidth =  atoi( tilesetnode->first_attribute( "tilewidth" )->value() );
        tileset->tileHeight = atoi( tilesetnode->first_attribute( "tileheight" )->value() );
        tileset->filename = tilesetnode->first_node( "image" )->first_attribute( "source" )->value();
        
        //cout << "Tileset " << tileset->name << " filename " << tileset->filename << endl;
        
        map->tilesets.push_back( tileset );
        
        tilesetnode = tilesetnode->next_sibling( "tileset" );
    }
    
    const char *separators = " \t,\n\r";
    
    xml_node<> *layernode = mapnode->first_node( "layer" );
    
    while ( layernode ) {
        NLTmxMapLayer* layer = new NLTmxMapLayer();
        
        layer->name = layernode->first_attribute( "name" )->value();
        layer->width = atoi( layernode->first_attribute( "width" )->value() );
        layer->height = atoi( layernode->first_attribute( "height" )->value() );

        xml_node<> *propertiesNode = layernode->first_node( "properties" );
            
            if (propertiesNode) {
                xml_node<>* propertyNode = propertiesNode->first_node("property");
                while (propertyNode) {
                    NLTmxMapObjectProperty* property = new NLTmxMapObjectProperty();
                    property->name = propertyNode->first_attribute("name")->value();
                    property->value = propertyNode->first_attribute("value")->value();

                    layer->properties.push_back(property);

                    propertyNode = propertyNode->next_sibling("property");
                }
            }
        
        const char* data = layernode->first_node( "data" )->value();

		assert(strcmp(data, "csv"));
        
        layer->data = new int[ layer->width * layer->height ];
        
        char* copy = (char*) malloc( strlen( data ) + 1 );
        strcpy( copy, data );
        char* item = strtok( copy, separators );

        int index = 0;
        while ( item ) {
            layer->data[ index ] = atoi( item );
            index++;
            
            item = strtok( 0, separators );
        }

        free( copy );
        
        map->layers.push_back( layer );
        
        layernode = layernode->next_sibling( "layer" );
    }
    
    xml_node<> *objectgroupnode = mapnode->first_node( "objectgroup" );
    
    while ( objectgroupnode ) {
        NLTmxMapObjectGroup* group = new NLTmxMapObjectGroup();
        
        group->name = objectgroupnode->first_attribute( "name" )->value();
        auto color = objectgroupnode->first_attribute("color");
        if (color)
        {
            int r, g, b;
            sscanf(color->value() + 1, "%02x%02x%02x", &r, &g, &b);
            group->color = new sf::Color(r, g, b);
        }

        //group->width = atoi( objectgroupnode->first_attribute( "width" )->value() );
        //group->height = atoi( objectgroupnode->first_attribute( "height" )->value() );
        
        xml_attribute<> *visibleattr = objectgroupnode->first_attribute( "visible" );
        if ( visibleattr ) {
			group->visible = (atoi(visibleattr->value()) != 0);
        } else {
            group->visible = true;
        }
        
        //cout << "group " << group->name << endl;
        
        xml_node<> *objectnode = objectgroupnode->first_node( "object" );
        
        while ( objectnode ) {
            NLTmxMapObject* object = new NLTmxMapObject();
            
            auto nameattr = objectnode->first_attribute( "name" );
            if ( nameattr ) {
                object->name = nameattr->value();
            }
            auto gidattr = objectnode->first_attribute( "gid" );
            if ( gidattr ) {
                object->gid = atoi( gidattr->value() );
            }
			auto typeattr = objectnode->first_attribute("type");
			if (typeattr) {
				object->type = typeattr->value();
			}
            object->x = atoi( objectnode->first_attribute( "x" )->value() );
            object->y = atoi( objectnode->first_attribute( "y" )->value() );
            
            auto widthattr = objectnode->first_attribute( "width" );
            if ( widthattr ) {
                object->width = atoi( widthattr->value() );
            }
            
            auto heightattr = objectnode->first_attribute( "height" );
            if ( heightattr ) {
                object->height = atoi( heightattr->value() );
            }
            
            xml_node<> *propertiesnode = objectnode->first_node( "properties" );
            
            if ( propertiesnode ) {
                
                xml_node<> *propertynode = propertiesnode->first_node( "property" );
                
                while ( propertynode ) {
                    NLTmxMapObjectProperty* property = new NLTmxMapObjectProperty();
                    property->name = propertynode->first_attribute( "name" )->value();
                    property->value = propertynode->first_attribute( "value" )->value();
                    object->properties[property->name] = property->value;
                    propertynode = propertynode->next_sibling( "property" );
                }
            }

            xml_node<>* polygonnode = objectnode->first_node("polygon");

            if (polygonnode)
            {
                std::string pointsRaw = polygonnode->first_attribute("points")->value();

				size_t pos = 0;
				std::vector<std::string> points;
				while ((pos = pointsRaw.find(" ")) != std::string::npos) {
                    points.push_back(pointsRaw.substr(0, pos));
					pointsRaw.erase(0, pos + 1);
				}
                points.push_back(pointsRaw);

                std::vector<sf::Vector2f> points2f;
                for (std::string& point : points)
                {
                    sf::Vector2f ret;
                    pos = point.find(",");
                    ret.x = std::stof(point.substr(0, pos));
                    ret.y = std::stof(point.substr(pos + 1, point.size()));
                    points2f.push_back(ret);
                }

                object->polygon = points2f;
            }
            
            group->objects.push_back( object );
            
            objectnode = objectnode->next_sibling( "object" );
        }
        
        map->groups.push_back( group );
        
        objectgroupnode = objectgroupnode->next_sibling( "objectgroup" );
    }
    
    //free( (void*) xml );
    
    return map;
}

