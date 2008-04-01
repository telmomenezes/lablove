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

#include "SymbolRGB.h"
#include <math.h>
#include <stdlib.h>

mt_distribution* SymbolRGB::mDistRGB = gDistManager.getNewDistribution();

SymbolRGB::SymbolRGB(lua_State* luaState)
{
    if (luaState != NULL)
    {
        mRed = luaL_checkint(luaState, 1);
        mGreen = luaL_checkint(luaState, 2);
        mBlue = luaL_checkint(luaState, 3);
    }
    else
    {
        mRed = 0;
        mGreen = 0;
        mBlue = 0;
    }
}

SymbolRGB::SymbolRGB(int r, int g, int b)
{
    mRed = r;
    mGreen = g;
    mBlue = b;
}

SymbolRGB::SymbolRGB(SymbolRGB* sym) : Symbol(sym)
{
    mRed = sym->mRed;
    mGreen = sym->mGreen;
    mBlue = sym->mBlue;
}

SymbolRGB::~SymbolRGB()
{   
}

Symbol* SymbolRGB::clone()
{
    return new SymbolRGB(this);
}

float SymbolRGB::getBinding(Symbol* sym)
{
    // TODO: check type
    SymbolRGB* symRGB = (SymbolRGB*)sym;

    float distanceR = (float)(mRed - symRGB->mRed);
    float distanceG = (float)(mGreen - symRGB->mGreen);
    float distanceB = (float)(mBlue - symRGB->mBlue);

    float distance = (distanceR * distanceR);
    distance += (distanceG * distanceG);
    distance += (distanceB * distanceB);
    distance = sqrtf(distance);

    // 441.68f is the max possible distance (0, 0, 0) -> (255, 255, 255)
    distance = distance / 441.68;

    return 1.0f - distance;
}

void SymbolRGB::initRandom()
{
    mRed = mDistRGB->iuniform(0, 256);
    mGreen = mDistRGB->iuniform(0, 256);
    mBlue = mDistRGB->iuniform(0, 256);
}

void SymbolRGB::mutate()
{
    int delta = mDistRGB->iuniform(0, 256);
    int signal = mDistRGB->iuniform(0, 2);
    if (signal == 1)
    {
        delta = -delta;
    }
    unsigned int selector = mDistRGB->iuniform(0, 3);

    switch (selector)
    {
    case 0:
        mRed += delta;
        if (mRed > 255)
        {
            mRed = 255;
        }
        else if (mRed < 0)
        {
            mRed = 0;
        }
        break;
    case 1:
        mGreen += delta;
        if (mGreen > 255)
        {
            mGreen = 255;
        }
        else if (mGreen < 0)
        {
            mGreen = 0;
        }
        break;
    case 2:
        mBlue += delta;
        if (mBlue > 255)
        {
            mBlue = 255;
        }
        else if (mBlue < 0)
        {
            mBlue = 0;
        }
        break;
    }
}

int SymbolRGB::initRandom(lua_State* luaState)
{
    initRandom();
    return 0;
}

string SymbolRGB::toString()
{
    char str[255];
    sprintf(str, "(%d, %d, %d)", mRed, mGreen, mBlue);
    return str;
}

const char SymbolRGB::mClassName[] = "SymbolRGB";

Orbit<SymbolRGB>::MethodType SymbolRGB::mMethods[] =
{
    {"initRandom", &SymbolRGB::initRandom},
    {"setAlwaysRandom", &Symbol::setAlwaysRandom},
    {"getID", &Symbol::getID},
    {0,0}
};

Orbit<SymbolRGB>::NumberGlobalType SymbolRGB::mNumberGlobals[] = {{0,0}};

