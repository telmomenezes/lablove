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

#include "Graphic2DGradient.h"
#include "SymbolFloat.h"
#include "Simulation.h"

#include <stdexcept>

Graphic2DGradient::Graphic2DGradient(lua_State* luaState)
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

Graphic2DGradient::~Graphic2DGradient()
{   
}

Graphic2D* Graphic2DGradient::clone()
{
    Graphic2DGradient* graph = new Graphic2DGradient();
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

void Graphic2DGradient::init(SimObj2D* obj)
{
    mObject = obj;

    mSize = obj->mSize;
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

void Graphic2DGradient::draw()
{
    float rot = mObject->mRot;
    float centerX = mObject->mX;
    float centerY = mObject->mY;

    art_setRotation(centerX, centerY, rot);
    art_setColor(mRed, mGreen, mBlue, 255);
    art_fillSquare(centerX, centerY, mSize);
    art_clearRotation();
}

const char Graphic2DGradient::mClassName[] = "Graphic2DGradient";

Orbit<Graphic2DGradient>::MethodType Graphic2DGradient::mMethods[] = {
    {"setSymbolName", &Graphic2DGradient::setSymbolName},
    {"setReferenceSymbol", &Graphic2DGradient::setReferenceSymbol},
    {"setColor1", &Graphic2DGradient::setColor1},
    {"setColor2", &Graphic2DGradient::setColor2},
    {"setCenter", &Graphic2DGradient::setCenter},
    {0,0}
};

Orbit<Graphic2DGradient>::NumberGlobalType Graphic2DGradient::mNumberGlobals[] = {{0,0}};

int Graphic2DGradient::setSymbolName(lua_State* luaState)
{
    string name = luaL_checkstring(luaState, 1);
    setSymbolName(name);

    return 0;
}

int Graphic2DGradient::setReferenceSymbol(lua_State* luaState)
{
    Symbol* refSym = (Symbol*)Orbit<Graphic2DGradient>::pointer(luaState, 1);
    setReferenceSymbol(refSym);

    return 0;
}

int Graphic2DGradient::setColor1(lua_State* luaState)
{
    float red = luaL_checkint(luaState, 1);
    float green = luaL_checkint(luaState, 2);
    float blue = luaL_checkint(luaState, 3);

    setColor1(red, green, blue);

    return 0;
}

int Graphic2DGradient::setColor2(lua_State* luaState)
{
    float red = luaL_checkint(luaState, 1);
    float green = luaL_checkint(luaState, 2);
    float blue = luaL_checkint(luaState, 3);

    setColor2(red, green, blue);

    return 0;
}

int Graphic2DGradient::setCenter(lua_State* luaState)
{
    float center = luaL_checknumber(luaState, 1);
    setCenter(center);

    return 0;
}

