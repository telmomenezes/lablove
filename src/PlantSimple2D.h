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

#if !defined(__INCLUDE_PLANT_SIMPLE_2D_H)
#define __INCLUDE_PLANT_SIMPLE_2D_H

#include "ObjectSimple2D.h"

#include "Orbit.h"

class PlantSimple2D : public ObjectSimple2D
{
public:
	PlantSimple2D();
	PlantSimple2D(PlantSimple2D* plant) : ObjectSimple2D(plant){}
	virtual ~PlantSimple2D();
	virtual SimulationObject* clone(bool full=true);

	virtual bool isFood(){return true;}

#ifdef __LOVELAB_WITH_GRAPHICS
	virtual void draw();
#endif

	static const char mClassName[];
        static Orbit<PlantSimple2D>::MethodType mMethods[];
	static Orbit<PlantSimple2D>::NumberGlobalType mNumberGlobals[];

        PlantSimple2D(lua_State* luaState);
};
#endif

