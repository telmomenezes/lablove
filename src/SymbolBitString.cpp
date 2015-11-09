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

#include "SymbolBitString.h"
#include "Random.h"
#include <math.h>
#include <stdlib.h>


SymbolBitString::SymbolBitString(lua_State* luaState)
{
    string str = luaL_checkstring(luaState, 1);
    mAllowWildcards = false;
    initFromString(str);
}

SymbolBitString::SymbolBitString(string str)
{
    mAllowWildcards = false;
    initFromString(str);
}

SymbolBitString::SymbolBitString(SymbolBitString* sym) : Symbol(sym)
{
    mLength = sym->mLength;
    mBits = (int*)malloc(mLength * sizeof(int));
    memcpy(mBits, sym->mBits, mLength * sizeof(int));
    mAllowWildcards = sym->mAllowWildcards;
}

SymbolBitString::~SymbolBitString()
{   
    if (mBits != NULL)
    {
        free(mBits);
        mBits = NULL;
    }
}

Symbol* SymbolBitString::clone()
{
    return new SymbolBitString(this);
}

void SymbolBitString::initFromString(string str)
{
    mLength = str.size();
    mBits = (int*)malloc(mLength * sizeof(int));

    for (unsigned int i = 0; i < mLength; i++)
    {
        switch (str[i])
        {
        case '0':
            mBits[i] = 0;
            break;
        case '1':
            mBits[i] = 1;
            break;
        case '*':
            mBits[i] = -1;
            mAllowWildcards = true;
            break;
        }
    }
}

float SymbolBitString::proximity(Symbol* sym)
{
    // TODO: check type
    SymbolBitString* symStr = (SymbolBitString*)sym;

    unsigned int match = 0;

    unsigned int minLength = mLength;
    unsigned int maxLength = symStr->mLength;

    if (mLength > symStr->mLength)
    {
        minLength = symStr->mLength;
        maxLength = mLength;
    }

    for (unsigned int i = 0; i < minLength; i++)
    {
        if ((mBits[i] == symStr->mBits[i])
            || (mBits[i] == -1)
            || (symStr->mBits[i] == -1))
        {
            match++;
        }
    }

    float binding = ((float)match) / ((float)maxLength);
    return binding;
}

bool SymbolBitString::equals(Symbol* sym)
{
    SymbolBitString* symStr = (SymbolBitString*)sym;

    if (mLength != symStr->mLength)
    {
        return false;
    }

    for (unsigned int i = 0; i < mLength; i++)
    {
        if (mBits[i] != symStr->mBits[i])
        {
            return false;
        }
    }

    return true;
}

int SymbolBitString::getRandomBit()
{
    int start = 0;
    if (mAllowWildcards)
    {
        start = -1;
    }

    return gRandom.iuniform(start, 2);
}

void SymbolBitString::initRandom()
{
    for (unsigned int i = 0; i < mLength; i++)
    {
        mBits[i] = getRandomBit();
    }
}

void SymbolBitString::mutate()
{
    unsigned int replacePos = gRandom.iuniform(0, mLength);
    mBits[replacePos] = getRandomBit();
}

string SymbolBitString::toString()
{
    string str = "";
    for (unsigned int i = 0; i < mLength; i++)
    {
        if (mBits[i] == 0)
        {
            str += "0";
        }
        else
        {
            str += "1";
        }
    }

    return str;
}

const char SymbolBitString::mClassName[] = "SymbolBitString";

Orbit<SymbolBitString>::MethodType SymbolBitString::mMethods[] = {
    {"setAlwaysRandom", &Symbol::setAlwaysRandom},
    {"getID", &Symbol::getID},
    {"setID", &Symbol::setID},
    {0,0}
};

Orbit<SymbolBitString>::NumberGlobalType SymbolBitString::mNumberGlobals[] = {{0,0}};

