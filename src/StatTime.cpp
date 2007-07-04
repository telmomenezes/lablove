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
#include "Lab.h"

StatTime::StatTime(lua_State* luaState)
{
}

StatTime::~StatTime()
{
}

void StatTime::init()
{
	fprintf(mFile, "sim_time,real_time,cycles_per_second\n");
	fflush(mFile);
}

void StatTime::dump()
{
	fprintf(mFile, "%d,%f",
		Lab::getSingleton().getSimulation()->time(),
		Lab::getSingleton().realTime());

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

