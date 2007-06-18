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

#include "PopulationDynamics.h"
#include "Lab.h"

PopulationDynamics::PopulationDynamics()
{
	mHuman = NULL;
}

PopulationDynamics::~PopulationDynamics()
{	
}

void PopulationDynamics::init()
{
#ifdef __LABLOVE_WITH_GRAPHICS
	if (mHuman != NULL)
	{
		mHuman->createGraphics();
	}
#endif
	Lab::getSingleton().getSimulation()->addObject(mHuman);
}

void PopulationDynamics::setHuman(SimulationObject* human)
{
	mHuman = human;
	mHuman->mHuman = true;
}

int PopulationDynamics::setHuman(lua_State* luaState)
{
	SimulationObject* obj = (SimulationObject*)Orbit<Lab>::pointer(luaState, 1);
        setHuman(obj);
        return 0;
}

