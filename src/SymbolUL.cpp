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

#include "SymbolUL.h"
#include "Simulation.h"

#include <math.h>
#include <stdlib.h>

mt_distribution* SymbolUL::mDistUL = Simulation::getNewDistribution();

SymbolUL::SymbolUL(lua_State* luaState)
{
    mUL = luaL_checkint(luaState, 1);
    mMin = luaL_optint(luaState, 2, 0);
    mMax = luaL_optint(luaState, 3, 100);
}

SymbolUL::SymbolUL(unsigned long value, unsigned long min, unsigned long max)
{
    mUL = value;
    mMin = min;
    mMax = max;
}

SymbolUL::SymbolUL(SymbolUL* sym)
{
    mUL = sym->mUL;
    mMin = sym->mMin;
    mMax = sym->mMax;
}

SymbolUL::~SymbolUL()
{   
}

Symbol* SymbolUL::clone()
{
    return new SymbolUL(this);
}

float SymbolUL::bind(Symbol* sym)
{
    // TODO: check type
    SymbolUL* symUL = (SymbolUL*)sym;

    float distance = fabsf((float)(symUL->mUL - mUL));
    float span = fabsf((float)(mMax - mMin));
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

void SymbolUL::initRandom()
{
    mUL = mDistUL->iuniform(mMin, mMax);
}

void SymbolUL::mutate()
{
    // TODO
}

const char SymbolUL::mClassName[] = "SymbolUL";

Orbit<SymbolUL>::MethodType SymbolUL::mMethods[] = {{0,0}};

Orbit<SymbolUL>::NumberGlobalType SymbolUL::mNumberGlobals[] = {{0,0}};

