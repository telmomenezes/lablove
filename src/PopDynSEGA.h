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

#if !defined(__INCLUDE_POPDYN_SEGA_H)
#define __INCLUDE_POPDYN_SEGA_H

#include "Simulation.h"
#include "Log.h"
#include "Species.h"
#include "PopulationDynamics.h"

class PopDynSEGA : public PopulationDynamics
{
public:
    PopDynSEGA(lua_State* luaState=NULL);
    virtual ~PopDynSEGA();

    virtual void init(Simulation* sim);
    virtual void onCycle(gbULINT time, double realTime);
    virtual void onOrganismDeath(SimObj* org);

    virtual unsigned int addSpecies(Species* species);

    void setLogTimeInterval(gbULINT interval){mLogTimeInterval = interval * 1000;}
    void setEvolutionStopTime(unsigned int time){mEvolutionStopTime = ((gbULINT)time) * 1000;}

    static const char mClassName[];
    static Orbit<PopDynSEGA>::MethodType mMethods[];
    static Orbit<PopDynSEGA>::NumberGlobalType mNumberGlobals[];

    int setLogTimeInterval(lua_State* luaState);
    int setEvolutionStopTime(lua_State* luaState);
    int addSpecies(lua_State* luaState);

protected:
    static unsigned int CURRENT_SPECIES_ID;

    map<unsigned int, Species*> mSpecies;
    gbULINT mLogTimeInterval;

    gbULINT mEvolutionStopTime;
    bool mEvolutionOn;
};
#endif

