#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Gui.h"

#include "j1Window.h"

j1Gui::j1Gui() : j1Module()
{
	name.create("gui");
}

// Destructor
j1Gui::~j1Gui()
{}

// Called before render is available
bool j1Gui::Awake(pugi::xml_node& conf)
{
	LOG("Loading GUI atlas");
	bool ret = true;

	atlas_file_name = conf.child("atlas").attribute("file").as_string("");
	buttonup_filename = conf.child("button_up").attribute("file").as_string("");
	buttondown_filename = conf.child("button_down").attribute("file").as_string("");

	return ret;
}

// Called before the first frame
bool j1Gui::Start()
{
	atlas = App->tex->Load(atlas_file_name.GetString());
	buttonup_texture = App->tex->Load(buttonup_filename.GetString());
	buttondown_texture = App->tex->Load(buttondown_filename.GetString());

	return true;
}

// Update all guis
bool j1Gui::PreUpdate()
{
	/*p2List_item<GUIelement*>* item = elements.start; // list test
	while (item)
	{
		item->data->PreUpdate();
		item = item->next;
	}*/

	for (int i = 0; i < elements.Count(); ++i)
	{
		GUIelement* e = *elements.At(i);

		// check and updates mouse state -----------------------
		int x, y = 0;
		App->input->GetMousePosition(x, y);
		if (e->CheckBoundariesXY(x, y))
		{
			e->SetMouseState(GUIelement::MouseEvent::ENTER);
		}
		else
		{
			if(e->guiState != GUIelement::MouseState::EXIT)
				e->SetMouseState(GUIelement::MouseEvent::EXIT);
		}
		// ------------------------------------------------------

		// call elements preupdate ------------------------------
		e->PreUpdate();
	}
	
	return true;
}

void GUIelement::SetMouseState(MouseEvent event)
{
	switch (event)
	{
	case MouseEvent::ENTER:
		if (guiState == MouseState::ENTER)
		{
		LOG("Mouse is hovering");
		guiState = MouseState::HOVER;
		}
		else if (guiState == MouseState::DONTCARE || guiState == MouseState::EXIT)
		{
			LOG("Mouse entered on boundaries");
			guiState = MouseState::ENTER;
		}
		break;
	case MouseEvent::EXIT:
		LOG("Mouse Exit");
		guiState = MouseState::EXIT;
		break;
	case MouseEvent::FAILED:
		break;
	default:
		LOG("unknown mouse event");
		break;
	}
}

// Called after all Updates
bool j1Gui::PostUpdate()
{
	for (int i = 0; i < elements.Count(); ++i)
	{
		GUIelement* e = *elements.At(i);
		e->PostUpdate();
	}

	return true;
}

// Called before quitting
bool j1Gui::CleanUp()
{
	LOG("Freeing GUI");

	elements.Clear();

	return true;
}

// const getter for atlas
const SDL_Texture* j1Gui::GetAtlas() const
{
	return atlas;
}

bool GUIelement::CheckBoundariesXY(int x, int y)
{
	return (x > boundaries.x && x < (boundaries.x + boundaries.w) && y > boundaries.y && y < (boundaries.y + boundaries.h));
}

// UIelements constructions
GUIBanner* j1Gui::AddGUIBanner(SDL_Texture* texture, const SDL_Rect& rect, const iPoint& position, const char* text, TextPos targetTextPos)
{
	GUIBanner* ret = nullptr;
	ret = new GUIBanner(texture, rect, position, text, targetTextPos);
	//elements.add(ret); // list method
	elements.PushBack(ret);
	// adds index
	ret->index = elements.Count();

	return ret;
}

GUIText* j1Gui::AddGUIText(const iPoint& position, const char* text, SDL_Color color)
{
	GUIText* ret = nullptr;
	ret = new GUIText(position, text, color);
	elements.PushBack(ret);
	// adds index
	ret->index = elements.Count();
	
	return ret;
}

GUIButton* j1Gui::AddGUIButton(SDL_Texture* clickTexture, SDL_Texture* unclickTexture, const SDL_Rect& rect, const iPoint& position, const char* text, TextPos targetTextPos)
{
	GUIButton* ret = nullptr;
	ret = new GUIButton(clickTexture,unclickTexture, rect, position, text, targetTextPos);
	elements.PushBack(ret);
	ret->index = elements.Count();
	
	return ret;
}


// class Gui ---------------------------------------------------

GUIelement::GUIelement(const iPoint& position) : position(position){}
GUIelement::GUIelement(){}

bool GUIelement::PreUpdate()
{
	return true;
}
bool GUIelement::PostUpdate()
{
	return true;
}
bool GUIelement::CleanUp()
{
	return true;
}

// 

GUIBanner::GUIBanner(SDL_Texture* texture, const SDL_Rect& rect, const iPoint& position, const char* text, TextPos targetTextPos) : image_texture(texture), section(rect), GUIelement(position)
{
	if (texture == nullptr) image_texture = (SDL_Texture*)App->gui->GetAtlas();

	if (text != nullptr)
	{
		SDL_Color testColor;
		testColor.r = 255;
		testColor.g = 255;
		testColor.b = 255;
		testColor.a = SDL_ALPHA_OPAQUE;

		text_texture = App->font->Print(text, testColor, App->font->default);

		// defines text position respect image position
		if(text_texture != NULL)
			textPosition = ReturnTextPosition(targetTextPos);

	}

	// updates object boundaries
	boundaries.x = position.x;
	boundaries.y = position.y;
	boundaries.w = rect.w;
	boundaries.h = rect.h;
}

bool GUIBanner::PreUpdate()
{
	return true;
}

bool GUIBanner::PostUpdate()
{
	App->render->Blit(image_texture, position.x, position.y, &section, 0.0F);
	if (text_texture != nullptr)
		App->render->Blit(text_texture, textPosition.x, textPosition.y, NULL, 0.0F);

	return true;
}

iPoint GUIBanner::ReturnTextPosition(TextPos targetPos)
{
	iPoint ret;
	// gets text image size
	uint text_width, text_height = 0;
	App->tex->GetSize(text_texture, text_width, text_height);

	switch (targetPos) // TODO: maybe adds an offset separation too?
	{
	case TextPos::NO:
		break;
	case TextPos::CENTERED:
		LOG("text centered");
		ret.x = position.x + ((section.w / 2) - (text_width / 2));
		ret.y = position.y + ((section.h / 2) - (text_height / 2));
		break;
	case TextPos::UP:
		LOG("text above the image");
		ret.x = position.x + ((section.w / 2) - (text_width / 2));
		ret.y = position.y - (text_height);
		break;
	case TextPos::BOTTOM:
		ret.x = position.x + ((section.w / 2) - (text_width / 2));
		ret.y = (position.y + section.h);
		break;
	case TextPos::LEFT:
		ret.x = position.x - (text_width);
		ret.y = position.y + ((section.h / 2) - (text_height / 2));
		break;
	case TextPos::RIGHT:
		ret.x = position.x + section.w;
		ret.y = position.y + ((section.h / 2) - (text_height / 2));
		break;
	default:
		LOG("unknown text position type");
		break;
	}

	return ret;
}

GUIText::GUIText(const iPoint& position, const char* text, SDL_Color color) : GUIelement(position)
{
	if (text != nullptr)
	{
		texture = App->font->Print(text, color, NULL);
	}
}

bool GUIText::PostUpdate()
{
	if (texture != nullptr)
		App->render->Blit(texture, position.x, position.y, NULL, 0.0F);

	return true;
}

GUIButton::GUIButton(SDL_Texture* click_texture,SDL_Texture* unclick_texture, const SDL_Rect& rect, const iPoint& position, const char* text, TextPos targetPos)
	: clicked_texture(click_texture), unclicked_texture(unclick_texture), GUIBanner(unclick_texture, rect, position, text, targetPos)
{
	//this->position = position;
}

bool GUIButton::PreUpdate()
{
	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN && guiState == MouseState::HOVER)
	{
		image_texture = clicked_texture;
		LOG("button clicked");
	}
	else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP) // always unclick
	{
		image_texture = unclicked_texture;
		LOG("button unclicked");
	}

	return true;
}
