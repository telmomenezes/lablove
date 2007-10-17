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

#if !defined(__INCLUDE_STATMEDAVGMINMAX_H)
#define __INCLUDE_STATMEDAVGMINMAX_H

#include "Statistics.h"

#include "Orbit.h"

#include <list>

using std::list;

class StatMedAvgMinMax : public Statistics
{
public:
    StatMedAvgMinMax(lua_State* luaState=NULL);
    virtual ~StatMedAvgMinMax();

    virtual void init();

    virtual void process(SimulationObject* obj, PopulationManager* popManager);
    virtual void dump(unsigned long time, double realTime);

    static const char mClassName[];
    static Orbit<StatMedAvgMinMax>::MethodType mMethods[];
    static Orbit<StatMedAvgMinMax>::NumberGlobalType mNumberGlobals[];

protected:
    list<list<float> > mValueLists;
};
#endif

