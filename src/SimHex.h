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

#include "SimCont2D.h"
#include "HexCell.h"

class SimHex : public SimCont2D
{
public:
    enum IntData {INT_CHANNEL_OBJECTS,
                INT_CHANNEL_SOUNDS,
                INT_CHANNEL_SELF,
                INT_CHANNEL_TERRAIN,
                INT_CHANNEL_BETA,
                INT_HEX_X,
                INT_HEX_Y
                };

    SimHex(lua_State* luaState=NULL);
    virtual ~SimHex();

    virtual void setWorldDimensions(float worldWidth,
                                     float worldLength,
                                     unsigned int cellSide,
                                     float hexSide);

    virtual void initializeData(SimulationObject* obj);
    virtual void addObject(SimulationObject* object, bool init=false);
    virtual void setPos(SimulationObject* obj, float x, float y);

    virtual bool onKeyDown(Art_KeyCode keycode);
    virtual bool onKeyUp(Art_KeyCode keycode);

    static const char mClassName[];
    static Orbit<SimHex>::MethodType mMethods[];
    static Orbit<SimHex>::NumberGlobalType mNumberGlobals[];

    int setWorldDimensions(lua_State* luaState);

protected:
    virtual void drawTerrain();

    void getHexCell(float x, float y, int& hX, int& hY);

    virtual void act(Agent* agent);

    float mHexSide;
    float mHexA;
    float mHexB;
    int mHexCellWidth;
    int mHexCellLength;

    HexCell* mHexMap;

    bool mHumanPaint;
};
#endif

