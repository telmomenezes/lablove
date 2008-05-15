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

#include "SimObj2D.h"

SimObj2D::SimObj2D(lua_State* luaState) : SimObj(luaState)
{
    mX = 0;
    mY = 0;
    mRot = 0;
    mSize = 0;
    mSizeSquared = 0;
    mSpeedX = 0;
    mSpeedY = 0;
    mSpeedRot = 0;
    mDrag = 0;
    mRotDrag = 0;
    mImpulseX = 0;
    mImpulseY = 0;
    mImpulseRot = 0;
    mInitialEnergy = 0;
    mEnergy = 0;
    mMaxAge = 0;
    mAgeLimit = 0;

    mCollisionDetectionIteration = 0;
    mLastSpeakTime = 0;
    mLastFireTime = 0;

    mChannelObjects = -1;
    mChannelSounds = -1;
    mChannelSelf = -1;
    mChannelBeta = -1;
}

SimObj2D::SimObj2D(SimObj2D* obj) : SimObj(obj)
{
    for (list<Graphic2D*>::iterator iterGraph = obj->mGraphics.begin();
            iterGraph != obj->mGraphics.end();
            iterGraph++)
    {
        Graphic2D* graph = (*iterGraph)->clone();
        mGraphics.push_back(graph);
    }
    
    mX = 0;
    mY = 0;
    mRot = 0;
    mSize = obj->mSize;
    mSizeSquared = obj->mSizeSquared;
    mSpeedX = 0;
    mSpeedY = 0;
    mSpeedRot = 0;
    mDrag = obj->mDrag;
    mRotDrag = obj->mRotDrag;
    mImpulseX = 0;
    mImpulseY = 0;
    mImpulseRot = 0;
    mInitialEnergy = obj->mInitialEnergy;
    mEnergy = 0;
    mMaxAge = obj->mMaxAge;
    mAgeLimit = 0;

    mCollisionDetectionIteration = 0;
    mLastSpeakTime = 0;
    mLastFireTime = 0;

    mChannelObjects = obj->mChannelObjects;
    mChannelSounds = obj->mChannelSounds;
    mChannelSelf = obj->mChannelSelf;
    mChannelBeta = obj->mChannelBeta;
}

SimObj2D::~SimObj2D()
{
    for (list<Graphic2D*>::iterator iterGraph = mGraphics.begin();
            iterGraph != mGraphics.end();
            iterGraph++)
    {
        delete (*iterGraph);
    }
    mGraphics.clear();
}

SimObj* SimObj2D::clone()
{
    return new SimObj2D(this);
}

void SimObj2D::draw()
{
    for (list<Graphic2D*>::iterator iterGraph = mGraphics.begin();
            iterGraph != mGraphics.end();
            iterGraph++)
    {
        (*iterGraph)->draw();
    }
}

void SimObj2D::addGraphic(Graphic2D* graph)
{
    mGraphics.push_back(graph);
}

void SimObj2D::setSize(float size)
{
    mSize = size;
    mSizeSquared = size * size;
}

void SimObj2D::deltaEnergy(double delta)
{
    mEnergy += delta;
}

const char SimObj2D::mClassName[] = "SimObj2D";

Orbit<SimObj2D>::MethodType SimObj2D::mMethods[] = {
    {"addSymbolTable", &SimObj::addSymbolTable},
	{"setSymbolName", &SimObj::setSymbolName},
	{"setBirthRadius", &SimObj::setBirthRadius},
	{"setFitnessMeasure", &SimObj::setFitnessMeasure},
    {"setBrain", &SimObj::setBrain},
	{"addGraphic", &SimObj2D::addGraphic},
	{"setSize", &SimObj2D::setSize},
	{"setDrag", &SimObj2D::setDrag},
	{"setRotDrag", &SimObj2D::setRotDrag},
	{"setInitialEnergy", &SimObj2D::setInitialEnergy},
	{"setMaxAge", &SimObj2D::setMaxAge},
    {0,0}
};

Orbit<SimObj2D>::NumberGlobalType SimObj2D::mNumberGlobals[] = {{0,0}};

int SimObj2D::addGraphic(lua_State* luaState)
{
    Graphic2D* graph = (Graphic2D*)Orbit<SimObj2D>::pointer(luaState, 1);
    addGraphic(graph);
    return 0;
}

int SimObj2D::setSize(lua_State* luaState)
{
    float size = luaL_checknumber(luaState, 1);
    setSize(size);
    return 0;
}

int SimObj2D::setDrag(lua_State* luaState)
{
    float drag = luaL_checknumber(luaState, 1);
    setDrag(drag);
    return 0;
}

int SimObj2D::setRotDrag(lua_State* luaState)
{
    float rotDrag = luaL_checknumber(luaState, 1);
    setRotDrag(rotDrag);
    return 0;
}

int SimObj2D::setInitialEnergy(lua_State* luaState)
{
    float energy = luaL_checknumber(luaState, 1);
    setInitialEnergy(energy);
    return 0;
}

int SimObj2D::setMaxAge(lua_State* luaState)
{
    llULINT maxAge = luaL_checkint(luaState, 1);
    setMaxAge(maxAge);
    return 0;
}

