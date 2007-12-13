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

#if !defined(__INCLUDE_POPDYN_SPECIES_BUFFERS_H)
#define __INCLUDE_POPDYN_SPECIES_BUFFERS_H

#include "PopDynSpecies.h"

class PopDynSpeciesBuffers : public PopDynSpecies
{
public:
    PopDynSpeciesBuffers(lua_State* luaState=NULL);
    virtual ~PopDynSpeciesBuffers();
    virtual void init(PopulationManager* popManager);
    virtual void onOrganismDeath(SimulationObject* org);

    unsigned int addSpecies(SimulationObject* org, unsigned int population, unsigned int bufferSize);

    void setCompCount(unsigned int count){mCompCount = count;}
    void setFitnessAging(float aging){mFitnessAging = aging;}

    static const char mClassName[];
    static Orbit<PopDynSpeciesBuffers>::MethodType mMethods[];
    static Orbit<PopDynSpeciesBuffers>::NumberGlobalType mNumberGlobals[];

    int addSpecies(lua_State* luaState);
    int setCompCount(lua_State* luaState);
    int setFitnessAging(lua_State* luaState);

protected:
    void mutateAndSend(unsigned int speciesID);

    unsigned int mCompCount;
    float mFitnessAging;
};
#endif
