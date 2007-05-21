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

#include "ObjectSimple2D.h"
#include "Love.h"
#include "SimSimple2D.h"
#include <stdlib.h>
#include "functions.h"
#include "random.h"

ObjectSimple2D::ObjectSimple2D()
{
	_x = -1.0f;
	_y = -1.0f;
	_size = 1.0f;
	_size_squared = 1.0f;
	_rot = 0.0f;

	_next_cell_list = NULL;
	_prev_cell_list = NULL;

	_cell_x = -1;
	_cell_y = -1;
	_cell_pos = -1;

	_max_age = 0;
	_low_age_limit = 0;
	_high_age_limit = 0;
	_metabolism = 0.0f;
}

ObjectSimple2D::ObjectSimple2D(ObjectSimple2D* obj) : Object(obj)
{
	_size = obj->_size;
	_size_squared = obj->_size_squared;
	_low_age_limit = obj->_low_age_limit;
	_high_age_limit = obj->_high_age_limit;
	
	if (_high_age_limit > 0)
	{
		_max_age = random_uniform_int(_low_age_limit, _high_age_limit);
	}
	else
	{
		_max_age = 0;
	}
	_metabolism = obj->_metabolism;

	_x = -1.0f;
	_y = -1.0f;
	_rot = 0.0f;

	_color = MoleculeRGB(obj->_color);
}

ObjectSimple2D::~ObjectSimple2D()
{	
}

void ObjectSimple2D::set_pos(float x, float y)
{
	SimSimple2D* sim = (SimSimple2D*)(Love::get_instance().get_simulation());

	if ((x < 0)
		|| (y < 0)
		|| (x >= sim->get_world_width())
		|| (y >= sim->get_world_length()))
	{
		return;
	}

	unsigned int cell_side = (unsigned int)(sim->get_cell_side());
	unsigned int target_cell_x = ((unsigned int)x) / cell_side;
	unsigned int target_cell_y = ((unsigned int)y) / cell_side;
	ObjectSimple2D** obj_grid = sim->get_cell_grid();
	unsigned int world_cell_width = sim->get_world_cell_width();
	unsigned int target_cell_pos = (target_cell_y * world_cell_width) + target_cell_x;
	ObjectSimple2D* target_cell = obj_grid[target_cell_pos];

	if (_x >= 0)
	{
		if ((_cell_x != target_cell_x) || (_cell_y != target_cell_y))
		{
			// Remove from origin cell
			if (_next_cell_list != NULL)
			{
				_next_cell_list->_prev_cell_list = _prev_cell_list;
			}
			if (_prev_cell_list == NULL)
			{
				obj_grid[_cell_pos] = _next_cell_list;
			}
			else
			{
				_prev_cell_list->_next_cell_list = _next_cell_list;
			}

			// Insert in new target cell
			_next_cell_list = target_cell;
			_prev_cell_list = NULL;
			if (target_cell != NULL)
			{
				target_cell->_prev_cell_list = this;
			}
			obj_grid[target_cell_pos] = this;
			_cell_x = target_cell_x;
			_cell_y = target_cell_y;
			_cell_pos = target_cell_pos;
		}
	}
	else
	{
		// Insert in target cell
		_next_cell_list = target_cell;
		_prev_cell_list = NULL;
		if (target_cell != NULL)
		{
			target_cell->_prev_cell_list = this;
		}
		obj_grid[target_cell_pos] = this;
		_cell_x = target_cell_x;
		_cell_y = target_cell_y;
		_cell_pos = target_cell_pos;
	}

	_x = x;
	_y = y;
}

void ObjectSimple2D::set_size(float size)
{
	_size = size;
	_size_squared = _size * _size;
}

void ObjectSimple2D::set_rot(float rot)
{
	_rot = normalize_angle(rot);
}



/*void ObjectSimple2D::after_draw()
{
	if (_selected)
	{
		SimSimple2D* sim = (SimSimple2D*)(Love::get_instance().get_simulation());

		int x1 = (int)_x - (int)sim->get_view_x();
		int y1 = (int)_y - (int)sim->get_view_y();
		int x2 = x1 + (int)_size;
		int y2 = y1 + (int)_size;
		x1 = x1 - (int)_size;
		y1 = y1 - (int)_size;

		glColor3f(0.2, 0.2, 0.9);
		glBegin(GL_LINE_LOOP);
		glVertex2f(x1, y1);
		glVertex2f(x2, y1);
		glVertex2f(x2, y2);
		glVertex2f(x1, y2);
		glEnd();
	}
}*/

void ObjectSimple2D::place_random()
{
	SimSimple2D* sim = (SimSimple2D*)Love::get_instance().get_simulation();

	unsigned int world_width = (unsigned int)sim->get_world_width();
	unsigned int world_length = (unsigned int)sim->get_world_length();

	set_pos(rand() % world_width, rand() % world_length);
	set_rot(rand() % 360);
}

void ObjectSimple2D::on_cycle()
{
	_energy -= _metabolism;

	if (_energy < 0)
	{
		Love::get_instance().get_simulation()->kill_organism(this);
	}

	if (_max_age > 0)
	{
		if (Love::get_instance().get_simulation()->time() - _creation_time >= _max_age)
		{
			Love::get_instance().get_simulation()->kill_organism(this);
		}
	}
}

void ObjectSimple2D::set_age_range(unsigned long low_age_limit, unsigned long high_age_limit)
{
	_low_age_limit = low_age_limit;
	_high_age_limit = high_age_limit;
}

int ObjectSimple2D::set_size(lua_State* L)
{
        int size = luaL_checkint(L, 1);
        set_size(size);
        return 0;
}

int ObjectSimple2D::set_age_range(lua_State* L)
{
        int low_age_limit = luaL_checkint(L, 1);
        int high_age_limit = luaL_checkint(L, 2);
        set_age_range(low_age_limit, high_age_limit);
        return 0;
}

int ObjectSimple2D::set_metabolism(lua_State* L)
{
        float metabolism = luaL_checknumber(L, 1);
        set_metabolism(metabolism);
        return 0;
}

int ObjectSimple2D::set_color(lua_State* L)
{
	MoleculeRGB* color = (MoleculeRGB*)Luna<Love>::pointer(L, 1);
        set_color(color);
        return 0;
}

