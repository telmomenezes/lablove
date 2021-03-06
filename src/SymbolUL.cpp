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

#include "SymbolUL.h"
#include "Random.h"
#include <math.h>
#include <stdlib.h>


SymbolUL::SymbolUL(lua_State* luaState)
{
    mUL = luaL_checkinteger(luaState, 1);
    mMin = luaL_optinteger(luaState, 2, 0);
    mMax = luaL_optinteger(luaState, 3, 100);
}

SymbolUL::SymbolUL(gbULINT value, gbULINT min, gbULINT max)
{
    mUL = value;
    mMin = min;
    mMax = max;
}

SymbolUL::SymbolUL(SymbolUL* sym) : Symbol(sym)
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

float SymbolUL::proximity(Symbol* sym)
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

bool SymbolUL::equals(Symbol* sym)
{
    SymbolUL* symUL = (SymbolUL*)sym;

    return (symUL->mUL == mUL);
}

void SymbolUL::initRandom()
{
    mUL = gRandom.iuniform(mMin, mMax);
}

void SymbolUL::mutate()
{
    // TODO
}

string SymbolUL::toString()
{
    char str[255];
    sprintf(str, "%llu", mUL);
    return str;
}

const char SymbolUL::mClassName[] = "SymbolUL";

Orbit<SymbolUL>::MethodType SymbolUL::mMethods[] = {
    {"setAlwaysRandom", &Symbol::setAlwaysRandom},
    {"getID", &Symbol::getID},
    {"setID", &Symbol::setID},
    {0,0}
};

Orbit<SymbolUL>::NumberGlobalType SymbolUL::mNumberGlobals[] = {{0,0}};

