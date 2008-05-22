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

#include "SimObjHex.h"

SimObjHex::SimObjHex(lua_State* luaState) : SimObj2D(luaState)
{
    mHumanPaint = false;
}

SimObjHex::SimObjHex(SimObjHex* obj) : SimObj2D(obj)
{
}

SimObjHex::~SimObjHex()
{
}

SimObj* SimObjHex::clone()
{
    return new SimObjHex(this);
}

const char SimObjHex::mClassName[] = "SimObjHex";

Orbit<SimObjHex>::MethodType SimObjHex::mMethods[] = {
    {"addSymbolTable", &SimObj::addSymbolTable},
	{"setSymbolName", &SimObj::setSymbolName},
	{"setBirthRadius", &SimObj::setBirthRadius},
	{"setFitnessMeasure", &SimObj::setFitnessMeasure},
    {"setBrain", &SimObj::setBrain},
    {0,0}
};

Orbit<SimObjHex>::NumberGlobalType SimObjHex::mNumberGlobals[] = {{0,0}};

