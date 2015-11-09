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

#if !defined(__INCLUDE_TARGET_2D_H)
#define __INCLUDE_TARGET_2D_H

#include "SimObj2D.h"
#include "Orbit.h"

class Target2D : public SimObj2D
{
public:
    Target2D(lua_State* luaState=NULL);
    Target2D(Target2D* obj);
    virtual ~Target2D();
    virtual SimObj* clone();

    virtual void init();

    void setSpeed(float speed){mSpeed = speed;}

    virtual void process();

    void setEnergyLimits(float min, float max);
    void setEnergySizeFactor(float factor){mEnergySizeFactor = factor;}

    float mMinEnergy;
    float mMaxEnergy;
    float mEnergySizeFactor;

    static const char mClassName[];
    static Orbit<Target2D>::MethodType mMethods[];
    static Orbit<Target2D>::NumberGlobalType mNumberGlobals[];

    int setEnergyLimits(lua_State* luaState);
    int setEnergySizeFactor(lua_State* luaState);
    int setSpeed(lua_State* luaState);

protected:
    void initSpeed();

    float mSpeedX;
    float mSpeedY;
    float mSpeed;
};
#endif

