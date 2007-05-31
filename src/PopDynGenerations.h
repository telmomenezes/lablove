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

#if !defined(__INCLUDE_POPDYN_GENERATIONS_H)
#define __INCLUDE_POPDYN_GENERATIONS_H

#include "PopulationDynamics.h"

#include <list>
using namespace std;

#include "Orbit.h"

class PopDynGenerations : public PopulationDynamics
{
public:
	static unsigned int CURRENT_SPECIES_ID;

	PopDynGenerations();
	virtual ~PopDynGenerations();
	virtual void init();
	virtual void on_cycle();
	virtual void on_organism_death(SimulationObject* org);

	void add_static_species(SimulationObject* org, long population);
	void add_evolving_species(SimulationObject* org, long population);
	void set_generation_time(unsigned int time){_generation_time = time;}

	static const char class_name[];
        static Orbit<PopDynGenerations>::MethodType methods[];
	static Orbit<PopDynGenerations>::NumberGlobalType number_globals[];

        PopDynGenerations(lua_State* L);
        int add_static_species(lua_State* L);
        int add_evolving_species(lua_State* L);
	int set_generation_time(lua_State* L);

protected:

	list<SimulationObject*> _static_species;
	list<SimulationObject*> _evolving_species;
	list<list<SimulationObject*>*> _static_species_organism_lists;
	list<list<SimulationObject*>*> _evolving_species_organism_lists;
	list<long> _static_species_populations;
	list<long> _evolving_species_populations;

	unsigned int _generation_time;
	unsigned long _generation;
};
#endif

