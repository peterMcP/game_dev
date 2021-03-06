#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1PathFinding.h"
#include "j1Gui.h"
#include "j1Scene.h"

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
	if(App->map->Load("iso_walk.tmx") == true)
	{
		int w, h;
		uchar* data = NULL;
		if(App->map->CreateWalkabilityMap(w, h, &data))
			App->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);
	}

	debug_tex = App->tex->Load("maps/path2.png");

	// TODO 3: Create the banner (rect {485, 829, 328, 103}) and the text "Hello World"
	// method thats adds a banner and optional text embedded
	SDL_Color whiteColor = { 255,13,255,255 };
	App->gui->AddGUIBanner((SDL_Texture*)App->gui->GetAtlas(), { 485, 829, 328, 103 }, { 20, 60 }, "Banner with text on top", TextPos::UP);
	App->gui->AddGUIBanner((SDL_Texture*)App->gui->GetAtlas(), { 485, 829, 328, 103 }, { 360, 160 }, "Banner with text on left", TextPos::LEFT);
	App->gui->AddGUIBanner((SDL_Texture*)App->gui->GetAtlas(), { 485, 829, 328, 103 }, { 360, 60 }, "Banner with text centered", TextPos::CENTERED);
	// only banner
	App->gui->AddGUIBanner(nullptr, { 485, 829, 328, 103 }, { 300, 280 });
	// only text
	App->gui->AddGUIText({ 640,360 }, "<- Banner without text, this is a ONLY TEXT element", whiteColor);
	// buttons
	uint width, height = 0;
	App->win->GetWindowSize(width, height);
	App->gui->AddGUIButton(App->gui->buttondown_texture, App->gui->buttonup_texture, { 0, 0, 128, 23 }, 
						{ ((int)width * (int)App->win->GetScale()) / 2 - 64, 400 }, "BUTTON1", TextPos::CENTERED, App->gui->buttonhighlight_texture);
	App->gui->AddGUIButton(App->gui->buttondown_texture, App->gui->buttonup_texture, { 0, 0, 128, 23 }, 
						{ ((int)width * (int)App->win->GetScale()) / 2  - 64, 423 }, "BUTTON2", TextPos::CENTERED, App->gui->buttonhighlight_texture);
	App->gui->AddGUIButton(App->gui->buttondown_texture, App->gui->buttonup_texture, { 0, 0, 128, 23 }, 
						{ ((int)width * (int)App->win->GetScale()) / 2  - 64, 446 }, "BUTTON3", TextPos::CENTERED, App->gui->buttonhighlight_texture);

	// checkbox
	App->gui->AddGUICheckBox(App->gui->checkbox_down_texture, App->gui->checkbox_up_texture, { 0,0, 32,32 }, { 60,360 }, "this is a checkbox", TextPos::RIGHT, App->gui->checkbox_highlight_texture, App->gui->checkbox_check_texture);
	App->gui->AddGUICheckBox(App->gui->checkbox_down_texture, App->gui->checkbox_up_texture, { 0,0, 32,32 }, { 60,390 }, "this is another checkbox", TextPos::RIGHT, App->gui->checkbox_highlight_texture, App->gui->checkbox_check_texture);
	App->gui->AddGUICheckBox(App->gui->checkbox_down_texture, App->gui->checkbox_up_texture, { 0,0, 32,32 }, { 110,420 }, "another", TextPos::LEFT, App->gui->checkbox_highlight_texture, App->gui->checkbox_check_texture);


	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{

	// debug pathfing ------------------
	static iPoint origin;
	static bool origin_selected = false;

	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint p = App->render->ScreenToWorld(x, y);
	p = App->map->WorldToMap(p.x, p.y);

	if(App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		if(origin_selected == true)
		{
			App->pathfinding->CreatePath(origin, p);
			origin_selected = false;
		}
		else
		{
			origin = p;
			origin_selected = true;
		}
	}

	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	// Gui ---
	
	// -------
	if(App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		App->LoadGame("save_game.xml");

	if(App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		App->SaveGame("save_game.xml");

	if(App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera.y += floor(200.0f * dt);

	if(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y -= floor(200.0f * dt);

	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->camera.x += floor(200.0f * dt);

	if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x -= floor(200.0f * dt);

	App->map->Draw();

	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint map_coordinates = App->map->WorldToMap(x - App->render->camera.x, y - App->render->camera.y);
	p2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d Tile:%d,%d",
					App->map->data.width, App->map->data.height,
					App->map->data.tile_width, App->map->data.tile_height,
					App->map->data.tilesets.count(),
					map_coordinates.x, map_coordinates.y);

	//App->win->SetTitle(title.GetString());

	//// Debug pathfinding ------------------------------
	////int x, y;
	//App->input->GetMousePosition(x, y);
	//iPoint p = App->render->ScreenToWorld(x, y);
	//p = App->map->WorldToMap(p.x, p.y);
	//p = App->map->MapToWorld(p.x, p.y);

	//App->render->Blit(debug_tex, p.x, p.y);

	const p2DynArray<iPoint>* path = App->pathfinding->GetLastPath();

	for(uint i = 0; i < path->Count(); ++i)
	{
		iPoint pos = App->map->MapToWorld(path->At(i)->x, path->At(i)->y);
		App->render->Blit(debug_tex, pos.x, pos.y);
	}

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
