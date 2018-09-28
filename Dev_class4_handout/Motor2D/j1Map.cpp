#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Map.h"
#include <math.h>

j1Map::j1Map() : j1Module(), map_loaded(false)
{
	name.create("map");
}

// Destructor
j1Map::~j1Map()
{}

// Called before render is available
bool j1Map::Awake(pugi::xml_node& config)
{
	LOG("Loading Map Parser");
	bool ret = true;

	folder.create(config.child("folder").child_value());

	return ret;
}

void j1Map::Draw()
{
	if(map_loaded == false)
		return;

	// TODO 6: Iterate all tilesets and draw all their 
	// images in 0,0 (you should have only one tileset for now)

	p2List_item<TileSetInfo*>* item = tilesets.start;
	bool ret = true;

	while (item != NULL && ret == true)
	{
		//if (item->data->image != nullptr)
		//{
			ret = App->render->Blit(item->data->image, 0, 0);
			item = item->next;
		//}
		
	}



}

// Called before quitting
bool j1Map::CleanUp()
{
	LOG("Unloading map");

	// TODO 2: Make sure you clean up any memory allocated
	// from tilesets / map

	tilesets.clear();
	delete tileset;

	map_file.reset();

	return true;
}

// Load new map
bool j1Map::Load(const char* file_name)
{
	bool ret = true;
	p2SString tmp("%s%s", folder.GetString(), file_name);

	pugi::xml_parse_result result = map_file.load_file(tmp.GetString());

	if(result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file_name, result.description());
		ret = false;
	}
	else
	{
		LOG("Successfully parsed map XML file: %s", file_name);
	}

	if(ret == true)
	{
		// TODO 3: Create and call a private function to load and fill
		// all your map data
		loadMapData(map_file.child("map"));
	}

	// TODO 4: Create and call a private function to load a tileset
	// remember to support more any number of tilesets!
	
	

	if(ret == true)
	{
		// TODO 5: LOG all the data loaded
		// iterate all tilesets and LOG everything
		if (!loadTilesetData(map_file.child("map")))//.child("tileset"));
		{
			LOG("Tileset data loading failed");
		}
	}

	// HOMEWORK
	/*	- Load all layers
		- As with tilesets you need to support any number of them
		- First create the structs based on the data from the XML
		- Then the private functions to fill the structs
		- Then LOG everything*/

	if (ret == true)
	{

		if (!loadMapLayers(map_file.child("map")))//.child("tileset"));
		{
			LOG("layer data loading failed");
		}
	}
	map_loaded = ret;

	return ret;
}

bool j1Map::loadMapLayers(pugi::xml_node& layerNode)
{
	bool ret = true;


	for (layerNode = layerNode.child("layer"); layerNode; layerNode = layerNode.next_sibling("layer"))
	{
		layer = new layerInfo();
		layerList.add(layer);

		layer->name = layerNode.attribute("name").as_string();
		layer->width = layerNode.attribute("width").as_uint(0);
		layer->height = layerNode.attribute("height").as_uint(0);

		LOG("Layer | ----");
		LOG("name: %s ", layer->name);
		LOG("width: %i height: %i", layer->width, layer->height);

		++layerCounter;
		delete layer;

	}

	return ret;

}

bool j1Map::loadMapData(pugi::xml_node& mapNode)
{
	bool ret = true;
	// orientation, doesnt works
	/*if (mapNode.child_value("orientation") == "orthogonal")
	{
		mapInfo.orientation = orientation::orthogonal;
	}*/

	if (mapNode.attribute("orientation").as_string() == "orthogonal")//(mapNode.attribute("orientation").as_string() == "orthogonal")
	{
		mapInfo.orientation = orientation::orthogonal;
	}
	if (mapNode.attribute("orientation").as_string() == "isometric")
	{
		mapInfo.orientation = orientation::isometric;
	}
	// render order, doesnt works

	if (mapNode.attribute("orientation").as_string() == "right-down") //default mode
	{
		mapInfo.renderOrder = renderOrder::right_down;
	}
	else if (mapNode.attribute("orientation").as_string() == "right-up")
	{
		mapInfo.renderOrder = renderOrder::right_up;
	}
	else if (mapNode.attribute("orientation").as_string() == "left-down") 
	{
		mapInfo.renderOrder = renderOrder::left_down;
	}
	else if (mapNode.attribute("orientation").as_string() == "left-up") 
	{
		mapInfo.renderOrder = renderOrder::left_up;
	}
	else {
		mapInfo.renderOrder = renderOrder::invalidRender;
	}

	mapInfo.version = mapNode.attribute("version").as_float(0.0f);
	mapInfo.width = mapNode.attribute("width").as_uint(0);
	mapInfo.height = mapNode.attribute("height").as_uint(0);
	mapInfo.tilewidth = mapNode.attribute("tilewidth").as_uint(0);
	mapInfo.tileheight = mapNode.attribute("tileheight").as_uint(0);

	LOG("width: %i height: %i", mapInfo.width, mapInfo.height);
	LOG("tile_width: %i tile_height: %i", mapInfo.tilewidth, mapInfo.tileheight);


	return ret;
}

bool j1Map::loadTilesetData(pugi::xml_node& tilesetNode)
{
	bool ret = true;

	for (tilesetNode = tilesetNode.child("tileset"); tilesetNode; tilesetNode = tilesetNode.next_sibling("tileset"))
	{
		tileset = new TileSetInfo();
		tilesets.add(tileset);

		tileset->firstgid = tilesetNode.attribute("firstgid").as_uint(0);
		tileset->name = tilesetNode.child_value("name");//attribute("name").as_string();
		tileset->tilewidth = tilesetNode.attribute("tilewidth").as_uint(0);
		tileset->tileheight = tilesetNode.attribute("tileheight").as_uint(0);
		tileset->spacing = tilesetNode.attribute("spacing").as_int(0);
		tileset->margin = tilesetNode.attribute("margin").as_int(0);
		tileset->tilecount = tilesetNode.attribute("tilecount").as_uint(0);
		tileset->columns = tilesetNode.attribute("columns").as_uint(0);

		p2SString tmp("%s%s", folder.GetString(), tilesetNode.child("image").attribute("source").as_string());
		tileset->image = App->tex->Load(tmp.GetString());//tilesetNode.child("image").attribute("source").as_string());
		
		LOG("Tileset | ----");
		LOG("name: %s firstgid: %i ", tileset->name, tileset->firstgid);
		LOG("tile width: %i tileheight: %i", tileset->tilewidth, tileset->tileheight);
		LOG("spacing: %i margin: %i", tileset->spacing, tileset->margin);

		++tilesetCounter;
		//delete tileset;
		
	}


	return ret;
}