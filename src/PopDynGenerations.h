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

class PopDynGenerations : public PopulationDynamics
{
public:
	static unsigned int CURRENT_SPECIES_ID;

	PopDynGenerations();
	virtual ~PopDynGenerations();
	virtual void init();
	virtual void onCycle();
	virtual void onOrganismDeath(SimulationObject* org);

	void addStaticSpecies(SimulationObject* org, long population);
	void addEvolvingSpecies(SimulationObject* org, long population);
	void setGenerationTime(unsigned int time){mGenerationTime = time;}

	static const char mClassName[];
        static Orbit<PopDynGenerations>::MethodType mMethods[];
	static Orbit<PopDynGenerations>::NumberGlobalType mNumberGlobals[];

        PopDynGenerations(lua_State* luaState);
        int addStaticSpecies(lua_State* luaState);
        int addEvolvingSpecies(lua_State* luaState);
	int setGenerationTime(lua_State* luaState);

protected:

	std::list<SimulationObject*> mStaticSpecies;
	std::list<SimulationObject*> mEvolvingSpecies;
	std::list<std::list<SimulationObject*>*> mStaticSpeciesOrganismLists;
	std::list<std::list<SimulationObject*>*> mEvolvingSpeciesOrganismLists;
	std::list<long> mStaticSpeciesPopulations;
	std::list<long> mEvolvingSpeciesPopulations;

	unsigned int mGenerationTime;
	unsigned long mGeneration;
};
#endif

