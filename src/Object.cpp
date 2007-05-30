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

#include "Object.h"
#include "LoveLab.h"
#include <stdlib.h>
#include "functions.h"

Object::Object()
{
	_deleted = false;
	_selected = false;

        _species_id = 0;
	_energy = 0;
	_initial_energy = 0;
	_creation_time = LoveLab::get_instance().get_simulation()->time();
}

Object::Object(Object* obj)
{
	_deleted = false;
	_selected = false;

        _energy = obj->_energy;
	_initial_energy = obj->_initial_energy;
        _species_id = obj->_species_id;
	_creation_time = LoveLab::get_instance().get_simulation()->time();
}

Object::~Object()
{	
}

void Object::draw_brain()
{
	int x = (LoveLab::get_instance().get_screen_width() / 2) - 160;
	int y = (LoveLab::get_instance().get_screen_height() / 2) - 4;
	//stringColor(LoveLab::get_instance().get_screen(), x, y, "Can not display brain of selected object", COLOR_INFO);
}

int Object::set_initial_energy(lua_State* L)
{
        float energy = luaL_checknumber(L, 1);
        set_initial_energy(energy);
        return 0;
}

