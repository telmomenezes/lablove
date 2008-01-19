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

#include "SymbolFloatVector.h"
#include "Simulation.h"

#include <math.h>
#include <stdlib.h>

mt_distribution* SymbolFloatVector::mDistFloatVector = gDistManager.getNewDistribution();

SymbolFloatVector::SymbolFloatVector(lua_State* luaState)
{
    mSize = luaL_checkint(luaState, 1);
    mMin = luaL_optnumber(luaState, 2, 0.0f);
    mMax = luaL_optnumber(luaState, 3, 1.0f);
    mVector = (float*)malloc(mSize * sizeof(float));
    calcMaxBinding();
}

SymbolFloatVector::SymbolFloatVector(unsigned int size, float min, float max)
{
    mSize = size;
    mMin = min;
    mMax = max;
    mVector = (float*)malloc(mSize * sizeof(float));
    calcMaxBinding();
}

SymbolFloatVector::SymbolFloatVector(SymbolFloatVector* sym) : Symbol(sym)
{
    mSize = sym->mSize;
    mMin = sym->mMin;
    mMax = sym->mMax;
    mVector = (float*)malloc(mSize * sizeof(float));
    memcpy(mVector, sym->mVector, mSize * sizeof(float));
    mMaxBinding = sym->mMaxBinding;
}

SymbolFloatVector::~SymbolFloatVector()
{
    if (mVector != NULL)
    {
        free(mVector);
        mVector = NULL;
    }
}

Symbol* SymbolFloatVector::clone()
{
    return new SymbolFloatVector(this);
}

void SymbolFloatVector::calcMaxBinding()
{
    float distance = 0.0f;
    float span = mMax - mMin;
    float span2 = span * span;
    distance += span2 * mSize;
    distance = sqrtf(distance);
    mMaxBinding = distance;
}

float SymbolFloatVector::getBinding(Symbol* sym)
{
    // TODO: check type
    SymbolFloatVector* symVec = (SymbolFloatVector*)sym;

    float distance = 0.0f;

    for (unsigned int i = 0; i < mSize; i++)
    {
        float diff = symVec->mVector[i] - mVector[i];
        distance += (diff * diff);
    }

    distance = sqrtf(distance);

    float bind = distance / mMaxBinding;

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

void SymbolFloatVector::initRandom()
{
    for (unsigned int i = 0; i < mSize; i++)
    {
        mVector[i] = mDistFloatVector->uniform(mMin, mMax);
    }
}

void SymbolFloatVector::mutate()
{
    // TODO: this mutation strategy is temporary
    float span = fabsf(mMax - mMin);
    float delta = mDistFloatVector->uniform(0.0f, span);
    unsigned int index = (unsigned int)(mDistFloatVector->uniform(0.0f, mSize));
    delta -= span / 2.0f;
    mVector[index] += delta;

    if (mVector[index] > mMax)
    {
        mVector[index] = mMax;
    }
    else if (mVector[index] < mMin)
    {
        mVector[index] = mMin;
    }
}

const char SymbolFloatVector::mClassName[] = "SymbolFloatVector";

Orbit<SymbolFloatVector>::MethodType SymbolFloatVector::mMethods[] =
{
    {"initRandom", &SymbolFloatVector::initRandom},
    {"setAlwaysRandom", &Symbol::setAlwaysRandom},
    {0,0}
};

int SymbolFloatVector::initRandom(lua_State* luaState)
{
    initRandom();
    return 0;
}

Orbit<SymbolFloatVector>::NumberGlobalType SymbolFloatVector::mNumberGlobals[] = {{0,0}};

