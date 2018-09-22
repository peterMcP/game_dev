#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1FileSystem.h"
#include "j1Audio.h"

// for manage keyboard input
#include "j1Input.h"

#include "SDL/include/SDL.h"
#include "SDL_mixer\include\SDL_mixer.h"
#pragma comment( lib, "SDL_mixer/libx86/SDL2_mixer.lib" )

j1Audio::j1Audio() : j1Module()
{
	music = NULL;
	name.create("audio");
}

// Destructor
j1Audio::~j1Audio()
{}

// Called before render is available
bool j1Audio::Awake(pugi::xml_node& config)
{
	LOG("Loading Audio Mixer");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
	{
		LOG("SDL_INIT_AUDIO could not initialize! SDL_Error: %s\n", SDL_GetError());
		active = false;
		ret = true;
	}

	// load support for the JPG and PNG image formats
	int flags = MIX_INIT_OGG;
	int init = Mix_Init(flags);

	if((init & flags) != flags)
	{
		LOG("Could not initialize Mixer lib. Mix_Init: %s", Mix_GetError());
		active = false;
		ret = true;
	}

	//Initialize SDL_mixer
	if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		LOG("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		active = false;
		ret = true;
	}

	// music and fx volume --------------
	// load stored volume value
	Mix_VolumeMusic(config.child("volume").attribute("music").as_int(128));
	// sets temp volume var to previosly loaded one
	volume = config.child("volume").attribute("music").as_int(128);
	//Mix_VolumeChunk(chunk,config.child("volume").attribute("fx").as_int(128));


	return ret;
}

// Called before quitting
bool j1Audio::CleanUp()
{
	if(!active)
		return true;

	LOG("Freeing sound FX, closing Mixer and Audio subsystem");

	if(music != NULL)
	{
		Mix_FreeMusic(music);
	}

	p2List_item<Mix_Chunk*>* item;
	for(item = fx.start; item != NULL; item = item->next)
		Mix_FreeChunk(item->data);

	fx.clear();

	Mix_CloseAudio();
	Mix_Quit();
	SDL_QuitSubSystem(SDL_INIT_AUDIO);

	return true;
}

// Play a music file
bool j1Audio::PlayMusic(const char* path, float fade_time)
{
	bool ret = true;

	if(!active)
		return false;

	if(music != NULL)
	{
		if(fade_time > 0.0f)
		{
			Mix_FadeOutMusic(int(fade_time * 1000.0f));
		}
		else
		{
			Mix_HaltMusic();
		}

		// this call blocks until fade out is done
		Mix_FreeMusic(music);
	}

	music = Mix_LoadMUS(path);

	if(music == NULL)
	{
		LOG("Cannot load music %s. Mix_GetError(): %s\n", path, Mix_GetError());
		ret = false;
	}
	else
	{
		if(fade_time > 0.0f)
		{
			if(Mix_FadeInMusic(music, -1, (int) (fade_time * 1000.0f)) < 0)
			{
				LOG("Cannot fade in music %s. Mix_GetError(): %s", path, Mix_GetError());
				ret = false;
			}
		}
		else
		{
			if(Mix_PlayMusic(music, -1) < 0)
			{
				LOG("Cannot play in music %s. Mix_GetError(): %s", path, Mix_GetError());
				ret = false;
			}
		}
	}

	LOG("Successfully playing %s", path);
	return ret;
}

// Load WAV
unsigned int j1Audio::LoadFx(const char* path)
{
	unsigned int ret = 0;

	if(!active)
		return 0;

	Mix_Chunk* chunk = Mix_LoadWAV(path);

	if(chunk == NULL)
	{
		LOG("Cannot load wav %s. Mix_GetError(): %s", path, Mix_GetError());
	}
	else
	{
		fx.add(chunk);
		ret = fx.count();
	}

	return ret;
}

// Play WAV
bool j1Audio::PlayFx(unsigned int id, int repeat)
{
	bool ret = false;

	if(!active)
		return false;

	if(id > 0 && id <= fx.count())
	{
		Mix_PlayChannel(-1, fx[id - 1], repeat);
	}

	return ret;
}

bool j1Audio::Load(pugi::xml_node& savegame)
{
	bool ret = true;

	// loads volume from savegame.xml
	volume = savegame.child("volume").attribute("music").as_int(128);

	// sets volume in sdl_mix
	Mix_VolumeMusic(volume);

	return ret;
}

bool j1Audio::Save(pugi::xml_node& savegame)
{
	bool ret = true;

	// stores to core config.xml ---------------------------------------------------
	pugi::xml_document config_file;
	pugi::xml_parse_result result = config_file.load_file("config.xml");

	pugi::xml_node coreNode = config_file.child("config").child("audio").child("volume");

	pugi::xml_attribute attr = coreNode.attribute("music");
	attr.set_value(volume);

	config_file.save_file("config.xml");
	// ------------------------------------------------------------------------------

	// stores to savegame.xml -------------------------------------------------------
	pugi::xml_node savegameNode = savegame.append_child("volume");

	savegameNode.append_attribute("music") = volume;
	// ------------------------------------------------------------------------------

	return ret;
}

void j1Audio::volumeControl(int vol)
{

	volume += vol;
	Mix_VolumeMusic(volume);

}

bool j1Audio::Update(float dt)
{
	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_KP_PLUS) == KEY_DOWN)
	{
		if (volume < MIX_MAX_VOLUME)
			volumeControl(1);
	}

	if (App->input->GetKey(SDL_SCANCODE_KP_MINUS) == KEY_DOWN)
	{
		if (volume > 0) volumeControl(-1);
	}



	return ret;
}