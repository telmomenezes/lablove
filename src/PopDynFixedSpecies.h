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

class PopDynFixedSpecies : public PopulationDynamics
{
public:
	static unsigned int CURRENT_SPECIES_ID;

	PopDynFixedSpecies();
	virtual ~PopDynFixedSpecies();
	virtual void init();
	virtual void onCycle();
	virtual void onOrganismDeath(SimulationObject* org);

	void addStaticSpecies(SimulationObject* org, long population);
	void addEvolvingSpecies(SimulationObject* org, long population);

	static const char mClassName[];
        static Orbit<PopDynFixedSpecies>::MethodType mMethods[];
	static Orbit<PopDynFixedSpecies>::NumberGlobalType mNumberGlobals[];

        PopDynFixedSpecies(lua_State* luaState);
        int addStaticSpecies(lua_State* luaState);
        int addEvolvingSpecies(lua_State* luaState);

protected:
	list<SimulationObject*> mStaticSpecies;
	list<SimulationObject*> mEvolvingSpecies;
	list<vector<SimulationObject*>*> mStaticSpeciesOrganismVecs;
	list<vector<SimulationObject*>*> mEvolvingSpeciesOrganismVecs;
	list<long> mStaticSpeciesPopulations;
	list<long> mEvolvingSpeciesPopulations;
	vector<unsigned int> mEvolvingSpeciesDeathCount;
	vector<float> mEvolvingSpeciesFitness;
	vector<float> mEvolvingSpeciesBestFitness;
};
#endif

