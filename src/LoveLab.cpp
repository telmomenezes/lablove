/*
 * LOVE Lab
 * Copyright (C) 2007 Telmo Menezes.
 * telmo@telmomenezes.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the version 2 of the GNU General Public License 
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "LoveLab.h"
#include "functions.h"
#include "random.h"

#ifdef __LOVE_GRAPHICS
#include "GL/gl.h"
#endif

LoveLab* LoveLab::_love = NULL;

LoveLab::LoveLab()
{
	_stop = false;
	_screen_width = 800;
	_screen_height = 600;
	_color_depth = 32;
	_show_console = false;
	_cycle_start_time = 0;
	_last_cycle_start_time = 0;
	_fps = 0;
	_fps_sum = 0.0f;
        _show_console = false;
	_draw_brain = false;

        sprintf(_fps_string_buffer, "");
}

LoveLab::LoveLab(lua_State* L)
{
	_stop = false;
	_screen_width = 800;
	_screen_height = 600;
	_color_depth = 32;
	_show_console = false;
	_cycle_start_time = 0;
	_last_cycle_start_time = 0;
	_fps = 0;
	_fps_sum = 0.0f;
        _show_console = false;
	_draw_brain = false;

        sprintf(_fps_string_buffer, "");
}

LoveLab::~LoveLab()
{
}

void LoveLab::create()
{
	_love = this;
	_love->add_keyboard_mouse_handler(this);
}

LoveLab& LoveLab::get_instance()
{	
	return *_love;
}

void LoveLab::set_screen_dimensions(unsigned int screen_width,
					unsigned int screen_height,
					unsigned int color_depth)
{
	_screen_width = screen_width;
	_screen_height = screen_height;
	_color_depth = color_depth;
}

void LoveLab::set_seed_index(unsigned int index)
{
	random_seed_index(index);
}

void LoveLab::run()
{
#ifdef __LOVE_GRAPHICS
	const SDL_VideoInfo *info;
	Uint8  video_bpp;
	Uint32 videoflags;
        
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		fprintf(stderr, "Couldn't initialize SDL: %s\n",SDL_GetError());
		exit(1);
	}
	atexit(SDL_Quit);

	videoflags = SDL_OPENGL;

	if ((SDL_SetVideoMode(_screen_width, _screen_height, _color_depth, videoflags)) == NULL )
	{
		fprintf(stderr, "Couldn't set %ix%i video mode: %s\n", _screen_width, _screen_height, SDL_GetError());
		exit(2);
	}

 	SDL_WM_SetCaption("Love Simulation", "Love Simulation");

	glViewport(0, 0, (GLint) _screen_width, (GLint) _screen_height);
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glOrtho(0, _screen_width, 0, _screen_height, -1.0, 1.0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	_font.init();
#endif

	_simulation->init();

	while (running())
	{
		cycle();
	}
}

void LoveLab::cycle()
{
#ifdef __LOVE_GRAPHICS
	_last_cycle_start_time = _cycle_start_time;
	_cycle_start_time = SDL_GetTicks();

	glClear(GL_COLOR_BUFFER_BIT);
#endif

	_simulation->cycle();

#ifdef __LOVE_GRAPHICS
	long cycle_ticks = _cycle_start_time - _last_cycle_start_time;
	double cycle_time = ((double)cycle_ticks) / 1000.0f;
	_fps_sum += 1.0f / cycle_time;

	if ((_simulation->time() % 100) == 0)
	{
		_fps = _fps_sum / 100.0f;
		_fps_sum = 0.0f;
		sprintf(_fps_string_buffer, "FPS: %f", _fps);
	}

	glColor3f(0.15, 0.15, 0.15);
	_font.print(10, _screen_height - 10, "%s", _fps_string_buffer);

	if (_draw_brain)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(0.0, 0.0, 0.0, 0.5);
		glBegin(GL_POLYGON);
		glVertex2f(0, 0);
		glVertex2f(_screen_width, 0);
		glVertex2f(_screen_width, _screen_height);
		glVertex2f(0, _screen_height);
		glEnd();

		if (_simulation->get_selected_object())
		{
			_simulation->get_selected_object()->draw_brain();
		}
		else
		{
			glColor3f(1.0, 1.0, 1.0);
			int x = (_screen_width / 2) - 72;
			int y = (_screen_height / 2) - 4;
			_font.print(x, y, "No object selected");
		}
	}

	glFlush();
	SDL_GL_SwapBuffers();

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		list<KeyboardMouseHandler*>::iterator iter_handler = _handlers_list.begin();
		bool handled = false;
		while ((!handled) && (iter_handler != _handlers_list.end()))
		{
			switch (event.type)
			{
				case SDL_KEYDOWN:
					(*iter_handler)->on_key_down(event.key.keysym.sym);
					break;
				case SDL_KEYUP:
					(*iter_handler)->on_key_up(event.key.keysym.sym);
					break;
				case SDL_MOUSEBUTTONDOWN:
					(*iter_handler)->on_mouse_button_down(event.button.button,
										event.button.x,
										_screen_height - event.button.y);
					break;
				case SDL_MOUSEBUTTONUP:
					(*iter_handler)->on_mouse_button_up(event.button.button,
										event.button.x,
										_screen_height - event.button.y);
					break;
				case SDL_MOUSEMOTION:
					(*iter_handler)->on_mouse_move(event.button.x,
									_screen_height - event.button.y);
					break;
				default:
					break;
			}

			iter_handler++;
		}
	}
#endif
}

void LoveLab::add_keyboard_mouse_handler(KeyboardMouseHandler* handler)
{
	_handlers_list.push_front(handler);
}

void LoveLab::remove_keyboard_mouse_handler()
{
	_handlers_list.pop_front();
}

#ifdef __LOVE_GRAPHICS
bool LoveLab::on_key_up(int key)
{
	switch (key)
	{
	case SDLK_ESCAPE:
		_stop = true;
		return true;
	case SDLK_b:
		set_draw_brain(!get_draw_brain());
	default:
		return false;
	}
}
#endif

const char LoveLab::class_name[] = "LoveLab";

Orbit<LoveLab>::MethodType LoveLab::methods[] = {
        {"create", &LoveLab::create},
	{"set_simulation", &LoveLab::set_simulation},
        {"set_seed_index", &LoveLab::set_seed_index},
        {0,0}
};

Orbit<LoveLab>::NumberGlobalType LoveLab::number_globals[] = {{0,0}};

int LoveLab::create(lua_State* L)
{
        create();
        return 0;
}



int LoveLab::set_simulation(lua_State *L)
{
        Simulation* sim = (Simulation*)Orbit<LoveLab>::pointer(L, 1);
        set_simulation(sim);
        return 0;
}

int LoveLab::set_seed_index(lua_State* L)
{
        int index = luaL_checkint(L, 1);
        set_seed_index(index);
        return 0;
}

