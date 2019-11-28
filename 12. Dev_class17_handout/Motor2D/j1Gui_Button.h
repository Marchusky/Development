#ifndef __j1GUI_BUTTON_H__
#define __j1GUI_BUTTON_H__

#include "j1Gui_Elements.h"

class Gui_Button :public Gui_Elements
{
public:
	Gui_Button(Element_type type, iPoint position, SDL_Rect rect, SDL_Texture* tex);

	~Gui_Button();

	bool Awake();

	bool Start();

	bool PreUpdate();

	bool PostUpdate();

	bool CleanUp();

public:

	SDL_Rect hovering_rect, clicking_rect;

};


#endif //__j1GUI_BUTTON_H__