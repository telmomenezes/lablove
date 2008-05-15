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

#if !defined(__INCLUDE_SIM_OBJ_HEX_H)
#define __INCLUDE_SIM_OBJ_HEX_H

#include "SimObj2D.h"
#include "Orbit.h"

class SimObjHex : public SimObj2D
{
public:
    SimObjHex(lua_State* luaState=NULL);
    SimObjHex(SimObjHex* obj);
    virtual ~SimObjHex();
    virtual SimObj* clone();
    
    int mChannelTerrain;
    int mHexX;
    int mHexY;

    static const char mClassName[];
    static Orbit<SimObjHex>::MethodType mMethods[];
    static Orbit<SimObjHex>::NumberGlobalType mNumberGlobals[];
};
#endif

