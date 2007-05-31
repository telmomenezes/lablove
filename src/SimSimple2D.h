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

#if !defined(__INCLUDE_SIM_SIMPLE_2D_H)
#define __INCLUDE_SIM_SIMPLE_2D_H

#include "Simulation.h"
#include "ObjectSimple2D.h"
#include "PopulationDynamics.h"

#include <list>
using namespace std;

#include "Orbit.h"

class SimSimple2D : public Simulation
{
public:
	SimSimple2D();
	virtual ~SimSimple2D();

	void set_world_dimensions(float world_width,
			float world_length,
			float cell_side);
	virtual void remove_object(SimulationObject* object);
	float get_world_width(){return _world_width;}
	float get_world_length(){return _world_length;}
	int get_view_x(){return _view_x;}
	int get_view_y(){return _view_y;}
	float get_cell_side(){return _cell_side;}
	ObjectSimple2D** get_cell_grid(){return _cell_grid;}
	void set_show_grid(bool show){_show_grid = show;}
	void set_show_view_range(bool show){_show_view_range = show;}
	bool get_show_grid(){return _show_grid;}
	bool get_show_view_range(){return _show_view_range;}
	unsigned int get_world_cell_width(){return _world_cell_width;}
	unsigned int get_world_cell_length(){return _world_cell_length;}
	
	void move_view(int delta_x, int delta_y);
	SimulationObject* get_object_by_screen_pos(int x, int y);

#ifdef __LOVE_GRAPHICS
	virtual bool on_key_down(int keycode);
	virtual bool on_key_up(int keycode);
	virtual bool on_mouse_button_down(int button, int x, int y);
	virtual bool on_mouse_button_up(int button, int x, int y);
	virtual bool on_mouse_move(int x, int y);
#endif

	static const char class_name[];
        static Orbit<SimSimple2D>::MethodType methods[];
	static Orbit<SimSimple2D>::NumberGlobalType number_globals[];

        SimSimple2D(lua_State* L);
        int set_world_dimensions(lua_State* L);

private:
#ifdef __LOVE_GRAPHICS
	virtual void draw_before_objects();
#endif

	float _world_width;
	float _world_length;
	int _view_x;
	int _view_y;

	float _cell_side;
	unsigned int _world_cell_width;
	unsigned int _world_cell_length;
	ObjectSimple2D** _cell_grid;
	bool _show_grid;
	bool _show_view_range;
	int _last_mouse_x;
	int _last_mouse_y;
	bool _mouse_pressed;
};
#endif

