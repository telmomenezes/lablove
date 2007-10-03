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
#include "Random.h"
#include <math.h>
#include <stdlib.h>

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

SymbolRGB::SymbolRGB(SymbolRGB* mol)
{
    mRed = mol->mRed;
    mGreen = mol->mGreen;
    mBlue = mol->mBlue;
}

SymbolRGB::~SymbolRGB()
{   
}

Symbol* SymbolRGB::clone()
{
    return new SymbolRGB(this);
}

float SymbolRGB::bind(Symbol* sym)
{
    // TODO: check type
    SymbolRGB* symRGB = (SymbolRGB*)sym;

    double distance = 0.0f;
    distance += fabsf(((float)(mRed - symRGB->mRed)) / 255.0f);
    distance += fabsf(((float)(mGreen - symRGB->mGreen)) / 255.0f);
    distance += fabsf(((float)(mBlue - symRGB->mBlue)) / 255.0f);

    distance = distance / 3.0f;
    return distance;
}

void SymbolRGB::initRandom()
{
    mRed = random() % 256;
    mGreen = random() % 256;
    mBlue = random() % 256;
}

void SymbolRGB::mutate()
{
    int delta = random() % 255;
    unsigned int selector = random() % 3;

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

const char SymbolRGB::mClassName[] = "SymbolRGB";

Orbit<SymbolRGB>::MethodType SymbolRGB::mMethods[] = {{0,0}};

Orbit<SymbolRGB>::NumberGlobalType SymbolRGB::mNumberGlobals[] = {{0,0}};

