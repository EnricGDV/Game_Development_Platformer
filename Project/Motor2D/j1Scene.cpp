#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Player.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "ModuleCollision.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{

	App->map->Load("map1.tmx");
	App->map->DrawObjects();
	//App->map->Load("iso.tmx");
	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
	{
		int map1 = App->player->Player.savedmap;
		App->LoadGame("save_game.xml");
		int map2 = App->player->Player.map;
		if (map1 != map2)
		{
			if (map1 == 1)
			{
				p2SString map = "map1.tmx";
				App->map->mapChange(&map);
				App->player->Player.position = App->player->Player.initPosition;
				App->scene->mapname = map;
				App->map->DrawObjects();
				App->player->Player.collider = App->collision->AddCollider(App->player->Player.colInit, COLLIDER_PLAYER, this);
				App->player->Player.map = 1;
			}
			else if (map1 == 2)
			{
				p2SString map = "map2.tmx";
				App->map->mapChange(&map);
				App->player->Player.position = App->player->Player.initPosition;
				App->scene->mapname = map;
				App->map->DrawObjects();
				App->player->Player.collider = App->collision->AddCollider(App->player->Player.colInit, COLLIDER_PLAYER, this);
				App->player->Player.map = 2;
			}
		}
	}
		

	if(App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		App->SaveGame("save_game.xml");



	App->map->Draw();
	

	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint map_coordinates = App->map->WorldToMap(x - App->render->camera.x, y - App->render->camera.y);
	p2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d Tile:%d,%d",
					App->map->data.width, App->map->data.height,
					App->map->data.tile_width, App->map->data.tile_height,
					App->map->data.tilesets.count(),
					map_coordinates.x, map_coordinates.y);

	App->win->SetTitle(title.GetString());
	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

bool j1Scene::ChangeScene()
{
	if (mapname == "map1.tmx")
	{
		mapname = "map2.tmx";
	}
	else if (mapname == "map2.tmx")
	{
		mapname = "map1.tmx";
	}
	App->map->mapChange(&mapname);
	App->map->DrawObjects();
	return true;
}