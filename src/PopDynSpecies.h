/*
 * LabLOVE
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

#if !defined(__INCLUDE_POPDYN_SPECIES_H)
#define __INCLUDE_POPDYN_SPECIES_H

#include "Simulation.h"
#include "Log.h"
#include "Species.h"
#include "PopulationDynamics.h"

class PopDynSpecies : public PopulationDynamics
{
public:
    PopDynSpecies(lua_State* luaState=NULL);
    virtual ~PopDynSpecies();

    virtual void init(Simulation* sim);
    virtual void onCycle(llULINT time, double realTime);
    virtual void onOrganismDeath(SimObj* org);

    virtual unsigned int addSpecies(Species* species);

    void setLogTimeInterval(llULINT interval){mLogTimeInterval = interval * 1000;}
    void setEvolutionStopTime(unsigned int time){mEvolutionStopTime = ((llULINT)time) * 1000;}

    static const char mClassName[];
    static Orbit<PopDynSpecies>::MethodType mMethods[];
    static Orbit<PopDynSpecies>::NumberGlobalType mNumberGlobals[];

    int setLogTimeInterval(lua_State* luaState);
    int setEvolutionStopTime(lua_State* luaState);
    int addSpecies(lua_State* luaState);

protected:
    static unsigned int CURRENT_SPECIES_ID;

    map<unsigned int, Species*> mSpecies;
    llULINT mLogTimeInterval;

    llULINT mEvolutionStopTime;
    bool mEvolutionOn;
};
#endif

