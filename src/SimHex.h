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

#if !defined(__INCLUDE_SIM_HEX_H)
#define __INCLUDE_SIM_HEX_H

#include "Sim2D.h"
#include "HexCell.h"
#include "SimObjHex.h"

class SimHex : public Sim2D
{
public:
    SimHex(lua_State* luaState=NULL);
    virtual ~SimHex();

    virtual void setWorldDimensions(float worldWidth,
                                     float worldLength,
                                     unsigned int cellSide,
                                     float hexSide);

    virtual void addObject(SimObj* object, bool init=false);
    virtual void updatePos(SimObj2D* obj, float origX, float origY, float targX, float targY);

    virtual bool onKeyDown(Art_KeyCode keycode);
    virtual bool onKeyUp(Art_KeyCode keycode);

    static const char mClassName[];
    static Orbit<SimHex>::MethodType mMethods[];
    static Orbit<SimHex>::NumberGlobalType mNumberGlobals[];

    int setWorldDimensions(lua_State* luaState);

protected:
    virtual void drawTerrain();

    void getHexCell(float x, float y, int& hX, int& hY);

    float mHexSide;
    float mHexA;
    float mHexB;
    int mHexCellWidth;
    int mHexCellLength;

    HexCell* mHexMap;
};
#endif

