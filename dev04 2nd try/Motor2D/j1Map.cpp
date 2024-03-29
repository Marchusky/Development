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
	
	Layer* layer_var = data.layers.start->data;//variable to store each of the arrays before drawing
	TileSet* tileset_var = data.tilesets.start->data; //variable to store each of the tilesets before drawing

	for (uint y = 0; y < layer_var->height; y++)//variable fila
	{
		for (uint x = 0; x < layer_var->width; x++)//variable columna
		{
			//miramos la gid que corresponde a las coordenadas en las que nos encontramos mediante
			//transformar la x e y en un valor �nico con la funci�n Get(desde la capa que toca para
			//tener asignada la width que corresponde).
			uint gid_var = layer_var->gid_list[layer_var->Get(x, y)]; 
			
			//una vez encontrada la gid de las coordenadas en la layer, buscamos el tileRect que le 
			//toca a la id con la funci�n GetTileRect() creada en el todo 8.  
			SDL_Rect rect_var = tileset_var->GetTileRect(gid_var);

			//Ahora hacemos el blit de todo el asunto :)
			App->render->Blit(tileset_var->texture, x * (tileset_var->tile_width), y * (tileset_var->tile_height), &rect_var);

		}
	}
	
	//------- hasta aqu� para printear 1 capa pero �como printear el mapa con mas de una capa?-------// 

	// TODO 5: Prepare the loop to iterate all the tiles in a layer
	
	// TODO 9: Complete the draw function

}


// Called before quitting
bool j1Map::CleanUp()
{
	LOG("Unloading map");

	// Remove all tilesets
	p2List_item<TileSet*>* item;
	item = data.tilesets.start;

	while(item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	data.tilesets.clear();

	// TODO 2: clean up all layer data
	// Remove all layers

	p2List_item<Layer*>* layer;
	layer = data.layers.start;

	while (layer != NULL)
	{
		RELEASE(layer->data);
		layer = layer->next;
	}
	data.layers.clear();

	// Clean up the pugui tree
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

	// Load general info ----------------------------------------------
	if(ret == true)
	{
		ret = LoadMap();
	}

	// Load all tilesets info ----------------------------------------------
	pugi::xml_node tileset;
	for(tileset = map_file.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();

		if(ret == true)
		{
			ret = LoadTilesetDetails(tileset, set);
		}

		if(ret == true)
		{
			ret = LoadTilesetImage(tileset, set);
		}

		data.tilesets.add(set);
	}

	// TODO 4: Iterate all layers and load each of them
	// Load layer info ----------------------------------------------

	for (pugi::xml_node layer_node = map_file.child("map").child("layer"); layer_node; layer_node = layer_node.next_sibling("layer"))
	{
		Layer* layer = new Layer();

		if (ret == true)
		{			
			LoadLayer(layer_node, layer);
		}

		data.layers.add(layer);
	}



	if(ret == true)
	{
		LOG("Successfully parsed map XML file: %s", file_name);
		LOG("width: %d height: %d", data.width, data.height);
		LOG("tile_width: %d tile_height: %d", data.tile_width, data.tile_height);

		p2List_item<TileSet*>* item = data.tilesets.start;
		while(item != NULL)
		{
			TileSet* s = item->data;
			LOG("Tileset ----");
			LOG("name: %s firstgid: %d", s->name.GetString(), s->firstgid);
			LOG("tile width: %d tile height: %d", s->tile_width, s->tile_height);
			LOG("spacing: %d margin: %d", s->spacing, s->margin);
			item = item->next;
		}

		// TODO 4: Add info here about your loaded layers
		// Adapt this code with your own variables
		
		p2List_item<Layer*>* item_layer = data.layers.start;
		while(item_layer != NULL)
		{
			Layer* l = item_layer->data;
			LOG("Layer ----");
			LOG("name: %s", l->name.GetString());
			LOG("tile width: %d tile height: %d", l->width, l->height);
			item_layer = item_layer->next;
		}
	}

	map_loaded = ret;

	return ret;
}

// Load map general properties
bool j1Map::LoadMap()
{
	bool ret = true;
	pugi::xml_node map = map_file.child("map");

	if(map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		data.width = map.attribute("width").as_int();
		data.height = map.attribute("height").as_int();
		data.tile_width = map.attribute("tilewidth").as_int();
		data.tile_height = map.attribute("tileheight").as_int();
		p2SString bg_color(map.attribute("backgroundcolor").as_string());

		data.background_color.r = 0;
		data.background_color.g = 0;
		data.background_color.b = 0;
		data.background_color.a = 0;

		if(bg_color.Length() > 0)
		{
			p2SString red, green, blue;
			bg_color.SubString(1, 2, red);
			bg_color.SubString(3, 4, green);
			bg_color.SubString(5, 6, blue);

			int v = 0;

			sscanf_s(red.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.r = v;

			sscanf_s(green.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.g = v;

			sscanf_s(blue.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.b = v;
		}

		p2SString orientation(map.attribute("orientation").as_string());

		if(orientation == "orthogonal")
		{
			data.type = MAPTYPE_ORTHOGONAL;
		}
		else if(orientation == "isometric")
		{
			data.type = MAPTYPE_ISOMETRIC;
		}
		else if(orientation == "staggered")
		{
			data.type = MAPTYPE_STAGGERED;
		}
		else
		{
			data.type = MAPTYPE_UNKNOWN;
		}
	}

	return ret;
}

bool j1Map::LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	set->name.create(tileset_node.attribute("name").as_string());
	set->firstgid = tileset_node.attribute("firstgid").as_int();
	set->tile_width = tileset_node.attribute("tilewidth").as_int();
	set->tile_height = tileset_node.attribute("tileheight").as_int();
	set->margin = tileset_node.attribute("margin").as_int();
	set->spacing = tileset_node.attribute("spacing").as_int();
	pugi::xml_node offset = tileset_node.child("tileoffset");

	if(offset != NULL)
	{
		set->offset_x = offset.attribute("x").as_int();
		set->offset_y = offset.attribute("y").as_int();
	}
	else
	{
		set->offset_x = 0;
		set->offset_y = 0;
	}

	return ret;
}

bool j1Map::LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	pugi::xml_node image = tileset_node.child("image");

	if(image == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
		ret = false;
	}
	else
	{
		set->texture = App->tex->Load(PATH(folder.GetString(), image.attribute("source").as_string()));
		int w, h;
		SDL_QueryTexture(set->texture, NULL, NULL, &w, &h);
		set->tex_width = image.attribute("width").as_int();

		if(set->tex_width <= 0)
		{
			set->tex_width = w;
		}

		set->tex_height = image.attribute("height").as_int();

		if(set->tex_height <= 0)
		{
			set->tex_height = h;
		}

		set->num_tiles_width = set->tex_width / set->tile_width;
		set->num_tiles_height = set->tex_height / set->tile_height;
	}

	return ret;
}

 //TODO 3: Create the definition for a function that loads a single layer

bool j1Map::LoadLayer(pugi::xml_node& node, Layer* layer)
{
	bool ret = true;
//node --> "layer" child --> "data" 
//child --> "tile"(s) --> attribute(s) --> gid = ...

	if (node == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'layer' tag.");
		return false;
	}
	//load header data
	layer->name.create(node.attribute("name").as_string());
	layer->width = node.attribute("width").as_uint();
	layer->height = node.attribute("height").as_uint();

	//allocate the array 
	layer->gid_list = new uint[layer->width * layer->height];

	//fill the entire array with zeros using memset function
	memset(layer->gid_list, 0, sizeof(uint) * layer->width * layer->height);

	//iterata all tile gids and store them into the layer
	uint position_index = 0;

	//two versions for the different encodings ??
	const char* data_encoding = node.child("data").attribute("encoding").as_string();

	if (strcmp(data_encoding), "csv")
	{

	}
	else
	{
		for (pugi::xml_node tile = node.child("data").first_child(); tile; tile = tile.next_sibling())
		{
			layer->gid_list[position_index] = tile.attribute("gid").as_uint();
				position_index++;
		}
	}
	return ret;

}