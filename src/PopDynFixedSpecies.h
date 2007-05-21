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

#if !defined(__INCLUDE_POPDYN_FIXED_SPECIES_H)
#define __INCLUDE_POPDYN_FIXED_SPECIES_H

#include "PopulationDynamics.h"

#include <list>
#include <vector>
using namespace std;

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}
#include "luna.h"

class PopDynFixedSpecies : public PopulationDynamics
{
public:
	static unsigned int CURRENT_SPECIES_ID;

	PopDynFixedSpecies();
	virtual ~PopDynFixedSpecies();
	virtual void init();
	virtual void on_cycle();
	virtual void on_organism_death(Object* org);

	void add_static_species(Object* org, long population);
	void add_evolving_species(Object* org, long population);

	static const char className[];
        static Luna<PopDynFixedSpecies>::RegType methods[];

        PopDynFixedSpecies(lua_State* L);
        int add_static_species(lua_State* L);
        int add_evolving_species(lua_State* L);

protected:
	list<Object*> _static_species;
	list<Object*> _evolving_species;
	list<vector<Object*>*> _static_species_organism_vecs;
	list<vector<Object*>*> _evolving_species_organism_vecs;
	list<long> _static_species_populations;
	list<long> _evolving_species_populations;
	vector<unsigned int> _evolving_species_death_count;
	vector<float> _evolving_species_fitness;
	vector<float> _evolving_species_best_fitness;
};
#endif

