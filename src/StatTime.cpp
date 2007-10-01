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

#include "StatTime.h"

StatTime::StatTime(lua_State* luaState)
{
    mLastSimTime = 0;
    mLastRealTime = 0;
}

StatTime::~StatTime()
{
}

void StatTime::init()
{
    fprintf(mFile, "sim_time,real_time,cycles_per_second\n");
    fflush(mFile);
}

void StatTime::dump(unsigned long time, double realTime)
{
    double cps = 0.0f;

    if (time != 0)
    {
        unsigned long deltaSim = time - mLastSimTime;
        double deltaReal = realTime - mLastRealTime;
        cps = ((double)deltaSim) / deltaReal;
    }

    mLastSimTime = time;
    mLastRealTime = realTime;

    fprintf(mFile, "%d,%f,%f", time, realTime, cps);

    fprintf(mFile, "\n");
    fflush(mFile);
}

const char StatTime::mClassName[] = "StatTime";

Orbit<StatTime>::MethodType StatTime::mMethods[] = {
    {"addField", &Statistics::addField},
    {"setFile", &Statistics::setFile},
    {0,0}
};

Orbit<StatTime>::NumberGlobalType StatTime::mNumberGlobals[] = {{0,0}};

