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

#include "SymbolFloat.h"
#include "Random.h"
#include <math.h>
#include <stdlib.h>

SymbolFloat::SymbolFloat(lua_State* luaState)
{
    mFloat = luaL_checknumber(luaState, 1);
    mMin = luaL_optnumber(luaState, 2, 0.0f);
    mMax = luaL_optnumber(luaState, 3, 1.0f);
}

SymbolFloat::SymbolFloat(float value, float min, float max)
{
    mFloat = value;
    mMin = min;
    mMax = max;;
}

SymbolFloat::SymbolFloat(SymbolFloat* sym)
{
    mFloat = sym->mFloat;
    mMin = sym->mMin;
    mMax = sym->mMax;
}

SymbolFloat::~SymbolFloat()
{   
}

Symbol* SymbolFloat::clone()
{
    return new SymbolFloat(this);
}

float SymbolFloat::bind(Symbol* sym)
{
    // TODO: check type
    SymbolFloat* symFloat = (SymbolFloat*)sym;

    float distance = fabsf(symFloat->mFloat - mFloat);
    float span = fabsf(mMax - mMin);
    float bind = distance / span;

    if (bind > 1.0f)
    {
        bind = 1.0f;
    }
    else if (bind < 0.0f)
    {
        bind = 0.0f;
    }

    return bind;
}

void SymbolFloat::initRandom()
{
    mFloat = Random::getUniformFloat(mMin, mMax);
}

void SymbolFloat::mutate()
{
    float span = fabsf(mMax - mMin);
    float delta = Random::getUniformFloat(0.0f, span);
    delta -= span / 2.0f;
    mFloat += delta;

    if (mFloat > mMax)
    {
        mFloat = mMax;
    }
    else if (mFloat < mMin)
    {
        mFloat = mMin;
    }
}

const char SymbolFloat::mClassName[] = "SymbolFloat";

Orbit<SymbolFloat>::MethodType SymbolFloat::mMethods[] = {{0,0}};

Orbit<SymbolFloat>::NumberGlobalType SymbolFloat::mNumberGlobals[] = {{0,0}};

