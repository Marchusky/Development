#include "j1Gui_Image.h"
#include "j1App.h"

Gui_Image::Gui_Image(Element_type type, iPoint position, SDL_Rect rect, SDL_Texture* tex) : Gui_Elements(type, position, rect, tex)
{
	pos.x = position.x;
	pos.y = position.y;
	texture = tex;
	Rect = rect;
}

Gui_Image::~Gui_Image() {}

bool Gui_Image::Awake() {

	return true;
}

bool Gui_Image::Start()
{


	return true;
}

bool Gui_Image::PreUpdate() {

	return true;
}

bool Gui_Image::PostUpdate() {

	App->render->Blit(texture, pos.x, pos.y, &Rect, 0);
	return true;
}

bool Gui_Image::CleanUp() {

	return true;
}