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

#if !defined(__INCLUDE_LOVELAB_H)
#define __INCLUDE_LOVELAB_H

#include "Simulation.h"
#include "KeyboardMouseHandler.h"

#ifdef __LOVE_GRAPHICS
#include "SDL.h"
#include "FontVera8.h"
#endif

#include <list>
using namespace std;

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}
#include "luna.h"

class LoveLab : public KeyboardMouseHandler
{
public:
	LoveLab();
	virtual ~LoveLab();
	static LoveLab& get_instance();
	void create();
	void set_simulation(Simulation* simulation){_simulation = simulation;}
	Simulation* get_simulation(){return _simulation;}
	void set_screen_dimensions(unsigned int screen_width,
					unsigned int screen_height,
					unsigned int color_depth);
	void set_seed_index(unsigned int index);
	bool running(){return !_stop;}
	void run();
	void cycle();
	void set_draw_brain(bool draw){_draw_brain = draw;}
	bool get_draw_brain(){return _draw_brain;}
	unsigned int get_screen_width(){return _screen_width;}
	unsigned int get_screen_height(){return _screen_height;}
	void add_keyboard_mouse_handler(KeyboardMouseHandler* handler);
	void remove_keyboard_mouse_handler();

#ifdef __LOVE_GRAPHICS
	virtual bool on_key_up(int keycode);
#endif

	static const char className[];
        static Luna<LoveLab>::RegType methods[];

        LoveLab(lua_State* L);
        int create(lua_State* L);
	int set_simulation(lua_State* L);
	int set_seed_index(lua_State* L);

private:
	static LoveLab* _love;
	Simulation* _simulation;
	bool _stop;
	int _screen_width;
	int _screen_height;
	int _color_depth;
	bool _show_console;
	bool _draw_brain;
	unsigned long _cycle_start_time;
	unsigned long _last_cycle_start_time;
	double _fps;
	double _fps_sum;
	char _fps_string_buffer[255];
	
	list<KeyboardMouseHandler*> _handlers_list;

#ifdef __LOVE_GRAPHICS
	FontVera8 _font;
#endif
};
#endif

