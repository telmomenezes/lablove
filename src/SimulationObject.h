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

#if !defined(__INCLUDE_SIMULATION_OBJECT_H)
#define __INCLUDE_SIMULATION_OBJECT_H

#include "Orbit.h"

class SimulationObject
{
public:
	SimulationObject();
	SimulationObject(SimulationObject* obj);
	virtual ~SimulationObject();

	virtual SimulationObject* clone(bool full=true) = 0;
	virtual void init(){}

	virtual void draw(){};
	
	virtual void on_cycle(){}
	virtual bool is_food(){return false;}
	void set_selected(bool selected){_selected = selected;}
	bool is_selected(){return _selected;}
	virtual void draw_brain();

        unsigned int get_species_id(){return _species_id;}
	void set_species_id(unsigned int id){_species_id = id;}
	float get_energy(){return _energy;}
	float get_initial_energy(){return _initial_energy;}
	void delta_energy(double delta){_energy += delta;}
	void set_energy(float energy){_energy = energy;}
	void set_initial_energy(float energy){_initial_energy = energy;}

	virtual SimulationObject* crossover(SimulationObject* other_parent){return this;}
	virtual void mutate(){}

	virtual void place_random()=0;

	int set_initial_energy(lua_State* L);

	bool _deleted;

protected:
	bool _selected;
        unsigned int _species_id;
	float _energy;
	float _initial_energy;
	unsigned long _creation_time;
};
#endif

