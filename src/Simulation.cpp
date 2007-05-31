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

#include "Simulation.h"
#include "LoveLab.h"
#include <math.h>
#include "functions.h"
#include "PopulationDynamics.h"

Simulation::Simulation()
{
	_simulation_time = 0;
	_selected_object = NULL;
	_population_dynamics = NULL;
}

Simulation::~Simulation()
{
	list<SimulationObject*>::iterator iter_obj;
	for (iter_obj = _objects.begin(); iter_obj != _objects.end(); ++iter_obj)
	{
		delete *iter_obj;
	}
        _objects.clear();
}

void Simulation::init()
{
	_population_dynamics->init();
	LoveLab::get_instance().add_keyboard_mouse_handler(this);
}

void Simulation::add_object(SimulationObject* object)
{
	_objects.push_back(object);
}

void Simulation::remove_object(SimulationObject* object)
{
	if (object->is_selected())
	{
		_selected_object = NULL;
	}

	bool stop = false;
	list<SimulationObject*>::iterator iter_obj;
	for (iter_obj = _objects.begin(); (iter_obj != _objects.end()) && !stop; ++iter_obj)
	{
		if((*iter_obj) == object)
		{
			_objects.erase(iter_obj);
			delete object;
			return;
		}
	}
}

void Simulation::cycle()
{
	list<SimulationObject*>::iterator iter_obj;

	for (iter_obj = _objects_to_kill.begin();
		iter_obj != _objects_to_kill.end();
		iter_obj++)
	{
		_population_dynamics->on_organism_death(*iter_obj);
	}
	_objects_to_kill.clear();

	_population_dynamics->on_cycle();

#ifdef __LOVE_GRAPHICS
	draw_before_objects();
#endif

	for (iter_obj = _objects.begin(); iter_obj != _objects.end(); ++iter_obj)
	{
		SimulationObject* obj = *iter_obj;
		obj->on_cycle();
#ifdef __LOVE_GRAPHICS
		obj->draw();
#endif
	}

	_simulation_time++;
}

void Simulation::set_selected_object(SimulationObject* object)
{
	if (_selected_object != NULL)
	{
		_selected_object->set_selected(false);
	}
	_selected_object = object;
	if (_selected_object != NULL)
	{
		_selected_object->set_selected(true);
	}
}

void Simulation::kill_organism(SimulationObject* org)
{
	if (org->_deleted)
	{
		return;
	}

	org->_deleted = true;

	_objects_to_kill.push_back(org);
}

int Simulation::set_population_dynamics(lua_State *L)
{
        PopulationDynamics* pop_dyn = (PopulationDynamics*)Orbit<LoveLab>::pointer(L, 1);
        set_population_dynamics(pop_dyn);
        return 0;
}

