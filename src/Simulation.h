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

#if !defined(__INCLUDE_SIMULATION_H)
#define __INCLUDE_SIMULATION_H

#include "SimulationObject.h"
#include "PopulationDynamics.h"
#include "KeyboardMouseHandler.h"

#include <list>
using namespace std;

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}
#include "Lunar.h"

class Simulation : public KeyboardMouseHandler
{
public:
	Simulation();
	virtual ~Simulation();

	void init();

	virtual void add_object(SimulationObject* object);
	virtual void remove_object(SimulationObject* object);
	void cycle();
	virtual SimulationObject* get_object_by_screen_pos(int x, int y)=0;
	void set_selected_object(SimulationObject* object);
	SimulationObject* get_selected_object(){return _selected_object;}
	void set_population_dynamics(PopulationDynamics* pop_dyn){_population_dynamics = pop_dyn;}
	PopulationDynamics* get_population_dynamics(){return _population_dynamics;}
	virtual void kill_organism(SimulationObject* org);
	unsigned long time(){return _simulation_time;}

	static const char className[];
        static Lunar<Simulation>::RegType methods[];

        Simulation(lua_State* L);
	int set_population_dynamics(lua_State* L);

protected:
	virtual void draw_before_objects(){}
	
	list<SimulationObject*> _objects;
	list<SimulationObject*> _objects_to_kill;

	unsigned long _simulation_time;
	SimulationObject* _selected_object;
	PopulationDynamics* _population_dynamics;
};
#endif

