#ifndef __j1GUI_LABEL_H__
#define __j1GUI_LABEL_H__

#include "j1Gui_Elements.h"

class Gui_Label :public Gui_Elements
{
public:
	Gui_Label(Element_type type, iPoint position, SDL_Rect rect, SDL_Texture* tex);

	~Gui_Label();

	bool Awake();

	bool Start();

	bool PreUpdate();

	bool PostUpdate();

	bool CleanUp();

public:

	SDL_Texture * hovering_tex;
	SDL_Texture * left_click;
	SDL_Texture * right_click;

};


#endif //__j1GUI_LABEL_H__