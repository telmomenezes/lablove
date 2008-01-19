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

#include "GraphicGradient.h"
#include "SimulationObject.h"
#include "SymbolFloat.h"
#include "Simulation.h"

#include <stdexcept>

GraphicGradient::GraphicGradient(lua_State* luaState)
{
    mObject = NULL;
    mSize = 0;
    mRed = 0;
    mGreen = 0;
    mBlue = 0;
    mSymbolName = "";
    mReferenceSymbol = NULL;
    mRed1 = 0;
    mGreen1 = 0;
    mBlue1 = 0;
    mRed2 = 0;
    mGreen2 = 0;
    mBlue2 = 0;
    mCenter = 0.5f;
}

GraphicGradient::~GraphicGradient()
{   
}

Graphic* GraphicGradient::clone()
{
    GraphicGradient* graph = new GraphicGradient();
    graph->mSymbolName = mSymbolName;
    graph->mReferenceSymbol = mReferenceSymbol;
    graph->mRed1 = mRed1;
    graph->mGreen1 = mGreen1;
    graph->mBlue1 = mBlue1;
    graph->mRed2 = mRed2;
    graph->mGreen2 = mGreen2;
    graph->mBlue2 = mBlue2;
    graph->mCenter = mCenter;
    return graph;
}

void GraphicGradient::init(SimulationObject* obj)
{
    mObject = obj;

    mXIndex = obj->getNamedFloatDataIndex("x");
    if (mXIndex == -1)
    {
        throw std::runtime_error("Failed to initialize GraphicGradient: object does not define 'x' named data item");
    }
    mYIndex = obj->getNamedFloatDataIndex("y");
    if (mYIndex == -1)
    {
        throw std::runtime_error("Failed to initialize GraphicGradient: object does not define 'y' named data item");
    }
    mRotIndex = obj->getNamedFloatDataIndex("rot");
    if (mRotIndex == -1)
    {
        throw std::runtime_error("Failed to initialize GraphicGradient: object does not define 'rot' named data item");
    }

    SymbolFloat* symSize = (SymbolFloat*)obj->getSymbolByName("size");

    if (symSize == NULL)
    {
        throw std::runtime_error("Failed to initialize GraphicGradient: object does not define 'size' named symbol");
    }
    else
    {
        mSize = symSize->getFloat();
    }

    mSize = sqrtf(2.0 * mSize * mSize);

    Symbol* sym = (Symbol*)(obj->getSymbolByName(mSymbolName));

    if (sym == NULL)
    {
        throw std::runtime_error("Failed to initialize GraphicGradient: object does not define '" + mSymbolName + "' named symbol");
    }

    float binding = mReferenceSymbol->getBinding(sym);

    if (binding < mCenter)
    {
        binding /= mCenter * 2.0f;
    }
    else
    {
        binding -= mCenter;
        binding /= (1.0f - mCenter) * 2.0f;
        binding += 0.5f;
    }

    float deltaRed = mRed2 - mRed1;
    float deltaGreen = mGreen2 - mGreen1;
    float deltaBlue = mBlue2 - mBlue1;

    deltaRed *= binding;
    deltaGreen *= binding;
    deltaBlue *= binding;

    mRed = mRed1 + deltaRed;
    mGreen = mGreen1 + deltaGreen;
    mBlue = mBlue1 + deltaBlue;
}

void GraphicGradient::draw()
{
    float rot = mObject->mFloatData[mRotIndex];
    float centerX = mObject->mFloatData[mXIndex];
    float centerY = mObject->mFloatData[mYIndex];

    art_setRotation(centerX, centerY, rot);
    art_setColor(mRed, mGreen, mBlue, 255);
    art_fillSquare(centerX, centerY, mSize);
    art_clearRotation();
}

const char GraphicGradient::mClassName[] = "GraphicGradient";

Orbit<GraphicGradient>::MethodType GraphicGradient::mMethods[] = {
    {"setSymbolName", &GraphicGradient::setSymbolName},
    {"setReferenceSymbol", &GraphicGradient::setReferenceSymbol},
    {"setColor1", &GraphicGradient::setColor1},
    {"setColor2", &GraphicGradient::setColor2},
    {"setCenter", &GraphicGradient::setCenter},
    {0,0}
};

Orbit<GraphicGradient>::NumberGlobalType GraphicGradient::mNumberGlobals[] = {{0,0}};

int GraphicGradient::setSymbolName(lua_State* luaState)
{
    string name = luaL_checkstring(luaState, 1);
    setSymbolName(name);

    return 0;
}

int GraphicGradient::setReferenceSymbol(lua_State* luaState)
{
    Symbol* refSym = (Symbol*)Orbit<GraphicGradient>::pointer(luaState, 1);
    setReferenceSymbol(refSym);

    return 0;
}

int GraphicGradient::setColor1(lua_State* luaState)
{
    float red = luaL_checkint(luaState, 1);
    float green = luaL_checkint(luaState, 2);
    float blue = luaL_checkint(luaState, 3);

    setColor1(red, green, blue);

    return 0;
}

int GraphicGradient::setColor2(lua_State* luaState)
{
    float red = luaL_checkint(luaState, 1);
    float green = luaL_checkint(luaState, 2);
    float blue = luaL_checkint(luaState, 3);

    setColor2(red, green, blue);

    return 0;
}

int GraphicGradient::setCenter(lua_State* luaState)
{
    float center = luaL_checknumber(luaState, 1);
    setCenter(center);

    return 0;
}

