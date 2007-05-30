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

#include "SimSimple2D.h"
#include "LoveLab.h"
#include "ObjectSimple2D.h"
#include <math.h>
#include "functions.h"
#include "PopulationDynamics.h"
#include "defines.h"

#ifdef __LOVE_GRAPHICS
#include "GL/gl.h"
#endif

SimSimple2D::SimSimple2D()
{
	_view_x = 0;
	_view_y = 0;
	_show_grid = false;
	_show_view_range = false;
	_cell_grid = NULL;
	_last_mouse_x = 0;
	_last_mouse_y = 0;
	_mouse_pressed = false;
}

SimSimple2D::SimSimple2D(lua_State* L)
{
	_view_x = 0;
	_view_y = 0;
	_show_grid = false;
	_show_view_range = false;
	_cell_grid = NULL;
	_last_mouse_x = 0;
	_last_mouse_y = 0;
	_mouse_pressed = false;
}

SimSimple2D::~SimSimple2D()
{
	if (_cell_grid != NULL)
	{
		free(_cell_grid);
		_cell_grid = NULL;
	}
}

void SimSimple2D::set_world_dimensions(float world_width,
			float world_length,
			float cell_side)
{
	_world_width = world_width;
	_world_length = world_length;
	_cell_side = cell_side;
	_world_cell_width = (unsigned int)(ceilf(_world_width / _cell_side));
	_world_cell_length = (unsigned int)(ceilf(_world_length / _cell_side));

	unsigned int grid_size = _world_cell_width * _world_cell_length;
	_cell_grid = (ObjectSimple2D**)malloc(sizeof(ObjectSimple2D*) * grid_size);

	for (unsigned int i = 0; i < grid_size; i++)
	{
		_cell_grid[i] = NULL;
	}

	
}

void SimSimple2D::remove_object(SimulationObject* object)
{
	ObjectSimple2D* obj = (ObjectSimple2D*)object;

	if (obj->_next_cell_list != NULL)
	{
		obj->_next_cell_list->_prev_cell_list = obj->_prev_cell_list;
	}

	if (obj->_prev_cell_list == NULL)
	{
		int cell_pos = obj->get_cell_pos();

		if(cell_pos >= 0)
		{
			_cell_grid[cell_pos] = obj->_next_cell_list;
		}
	}
	else
	{
		obj->_prev_cell_list->_next_cell_list = obj->_next_cell_list;
	}

	Simulation::remove_object(object);
}

void SimSimple2D::move_view(int delta_x, int delta_y)
{
	_view_x -= delta_x;
	_view_y -= delta_y;

	if (_view_x < 0)
	{
		_view_x = 0;
	}
	
	if (_view_y < 0)
	{
		_view_y = 0;
	}

	if ((_view_x + LoveLab::get_instance().get_screen_width()) >= (unsigned int)_world_width)
	{
		_view_x = ((unsigned int)_world_width) - LoveLab::get_instance().get_screen_width();
	}

	if ((_view_y + LoveLab::get_instance().get_screen_height()) >= (unsigned int)_world_length)
	{
		_view_y = ((unsigned int)_world_length) - LoveLab::get_instance().get_screen_height();
	}
}

SimulationObject* SimSimple2D::get_object_by_screen_pos(int x, int y)
{
	list<SimulationObject*>::iterator iter_obj;
	for (iter_obj = _objects.begin(); iter_obj != _objects.end(); ++iter_obj)
	{
		ObjectSimple2D* obj = (ObjectSimple2D*)(*iter_obj);

		float dx = obj->_x - (float)(_view_x + x);
		float dy = obj->_y - (float)(_view_y + y);
		dx *= dx;
		dy *= dy;

		if ((dx + dy) <= obj->_size_squared)
		{
			return obj;
		}
	}

	return NULL;
}

#ifdef __LOVE_GRAPHICS
void SimSimple2D::draw_before_objects()
{
	if (_show_grid)
	{
		unsigned int cell_side = (unsigned int)_cell_side;

		glColor3ub(DEF_S2D_GRID_COLOR_R,
			DEF_S2D_GRID_COLOR_G,
			DEF_S2D_GRID_COLOR_B);
		glBegin(GL_LINES);

		unsigned int division = cell_side - (_view_x % cell_side);
		while (division < LoveLab::get_instance().get_screen_width())
		{
			glVertex2f(division, 0);
			glVertex2f(division, LoveLab::get_instance().get_screen_height());

			division += cell_side;
		}

		division = cell_side - (_view_y % cell_side);
		while (division < LoveLab::get_instance().get_screen_height())
		{
			glVertex2f(0, division);
			glVertex2f(LoveLab::get_instance().get_screen_width(), division);

			division += cell_side;
		}

		glEnd();
	}

}

bool SimSimple2D::on_key_down(int key)
{
	switch (key)
	{
	/*case SDLK_LEFT:
		if (m_human)
		{
			m_human->setTurnLeft(true);
		}
		return true;
	case SDLK_RIGHT:
		if (m_human)
		{
			m_human->setTurnRight(true);
		}
		return true;
	case SDLK_UP:
		if (m_human)
		{
			m_human->setGo(true);
		}
		return true;
	case SDLK_e:
		if (m_human)
		{
			m_human->setEat(true);
		}
		return true;*/
	default:
		return false;
	}
}

bool SimSimple2D::on_key_up(int key)
{
	switch (key)
	{
	/*case SDLK_LEFT:
		if (m_human)
		{
			m_human->setTurnLeft(false);
		}
		return true;
	case SDLK_RIGHT:
		if (m_human)
		{
			m_human->setTurnRight(false);
		}
		return true;
	case SDLK_UP:
		if (m_human)
		{
			m_human->setGo(false);
		}
		return true;
	case SDLK_e:
		if (m_human)
		{
			m_human->setEat(false);
		}
		return true;*/
	case SDLK_g:
		set_show_grid(!get_show_grid());
		return true;
	case SDLK_v:
		set_show_view_range(!get_show_view_range());
		return true;
	default:
		return false;
	}
}

bool SimSimple2D::on_mouse_button_down(int button, int x, int y)
{
	if (button == 1)
	{
		_last_mouse_x = x;
		_last_mouse_y = y;
		_mouse_pressed = true;

		SimulationObject* object = get_object_by_screen_pos(x, y);

		if (object != NULL)
		{
			set_selected_object(object);
			return true;
		}
	}

	return false;
}

bool SimSimple2D::on_mouse_button_up(int button, int x, int y)
{
	if (button == 1)
	{
		_mouse_pressed = false;
		return true;
	}

	return false;
}

bool SimSimple2D::on_mouse_move(int x, int y)
{
	if (_mouse_pressed)
	{
		int new_x = x;
		int new_y = y;

		move_view(new_x - _last_mouse_x, new_y - _last_mouse_y);

		_last_mouse_x = new_x;
		_last_mouse_y = new_y;
	}
}
#endif

const char SimSimple2D::className[] = "SimSimple2D";

Luna<SimSimple2D>::RegType SimSimple2D::methods[] = {
	{"set_population_dynamics", &Simulation::set_population_dynamics},
        {"set_world_dimensions", &SimSimple2D::set_world_dimensions},
        {0,0}
};

int SimSimple2D::set_world_dimensions(lua_State* L)
{
        int width = luaL_checkint(L, 1);
        int height = luaL_checkint(L, 2);
        int cell_side = luaL_checkint(L, 3);
        set_world_dimensions(width, height, cell_side);
        return 0;
}

