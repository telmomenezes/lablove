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

#include "SimulationObject.h"
#include "LoveLab.h"
#include <stdlib.h>
#include "functions.h"

SimulationObject::SimulationObject()
{
	_deleted = false;
	_selected = false;

        _species_id = 0;
	_energy = 0;
	_initial_energy = 0;
	_creation_time = LoveLab::get_instance().get_simulation()->time();
}

SimulationObject::SimulationObject(SimulationObject* obj)
{
	_deleted = false;
	_selected = false;

        _energy = obj->_energy;
	_initial_energy = obj->_initial_energy;
        _species_id = obj->_species_id;
	_creation_time = LoveLab::get_instance().get_simulation()->time();
}

SimulationObject::~SimulationObject()
{
	map<string, Chemistry*>::iterator iter_chems;

	for (iter_chems = _chemistries_map.begin();
		iter_chems != _chemistries_map.end();
		iter_chems++)
	{
		delete (*iter_chems).second;
	}

	_chemistries_map.clear();
}

int SimulationObject::set_initial_energy(lua_State* L)
{
        float energy = luaL_checknumber(L, 1);
        set_initial_energy(energy);
        return 0;
}

