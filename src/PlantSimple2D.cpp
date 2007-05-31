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

#include "PlantSimple2D.h"
#include "LoveLab.h"
#include "math.h"
#include "defines.h"
#include "SimSimple2D.h"

#ifdef __LOVE_GRAPHICS
#include "GL/gl.h"
#endif

PlantSimple2D::PlantSimple2D()
{
	_next_cell_list = NULL;
	_prev_cell_list = NULL;

	_cell_x = -1;
	_cell_y = -1;
	_cell_pos = -1;
}

PlantSimple2D::PlantSimple2D(lua_State* L)
{
	_next_cell_list = NULL;
	_prev_cell_list = NULL;

	_cell_x = -1;
	_cell_y = -1;
	_cell_pos = -1;
}

PlantSimple2D::~PlantSimple2D()
{	
}

SimulationObject* PlantSimple2D::clone(bool full)
{
	return new PlantSimple2D(this);
}

#ifdef __LOVE_GRAPHICS
void PlantSimple2D::draw()
{
	SimSimple2D* sim = (SimSimple2D*)(LoveLab::get_instance().get_simulation());

	float x = (int)_x - (int)sim->get_view_x();
	float y = (int)_y - (int)sim->get_view_y();

	float p1x = x - _size;
	float p2x = x + _size;
	float p1y = y - _size;
	float p2y = y + _size;

	glColor3f(_color._red,
			_color._green,
			_color._blue);
	glBegin(GL_POLYGON);
	glVertex2f(p1x, p1y);
	glVertex2f(p2x, p1y);
	glVertex2f(p2x, p2y);
	glVertex2f(p1x, p2y);
	glEnd();
}
#endif

const char PlantSimple2D::class_name[] = "PlantSimple2D";

Orbit<PlantSimple2D>::MethodType PlantSimple2D::methods[] = {
	{"set_size", &ObjectSimple2D::set_size},
	{"set_color", &ObjectSimple2D::set_color},
	{"set_initial_energy", &SimulationObject::set_initial_energy},
	{"set_age_range", &ObjectSimple2D::set_age_range},
	{"set_metabolism", &ObjectSimple2D::set_metabolism},
        {0,0}
};

Orbit<PlantSimple2D>::NumberGlobalType PlantSimple2D::number_globals[] = {{0,0}};

