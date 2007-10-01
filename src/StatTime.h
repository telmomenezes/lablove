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

#if !defined(__INCLUDE_STAT_TIME_H)
#define __INCLUDE_STAT_TIME_H

#include "Statistics.h"

#include "Orbit.h"

class StatTime : public Statistics
{
public:
    StatTime(lua_State* luaState=NULL);
    virtual ~StatTime();

    virtual void init();

    virtual void process(SimulationObject* obj){}
    virtual void dump(unsigned long time, double realTime);

    static const char mClassName[];
    static Orbit<StatTime>::MethodType mMethods[];
    static Orbit<StatTime>::NumberGlobalType mNumberGlobals[];

protected:
    unsigned long mLastSimTime;
    double mLastRealTime;
};
#endif

