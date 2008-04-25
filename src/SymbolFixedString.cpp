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

#include "SymbolFixedString.h"
#include <math.h>
#include <stdlib.h>

mt_distribution* SymbolFixedString::mDistFixedString = gDistManager.getNewDistribution();

SymbolFixedString::SymbolFixedString(lua_State* luaState)
{
    mAlphabet = luaL_checkstring(luaState, 1);
    mString = luaL_checkstring(luaState, 2);
    mLength = mString.size();
    mAlphabetLength = mAlphabet.size();
}

SymbolFixedString::SymbolFixedString(string alphabet, string str)
{
    mString = str;
    mAlphabet = alphabet;
    mLength = mString.size();
    mAlphabetLength = mAlphabet.size();
}

SymbolFixedString::SymbolFixedString(SymbolFixedString* sym) : Symbol(sym)
{
    mString = sym->mString;
    mAlphabet = sym->mAlphabet;
    mLength = sym->mLength;
    mAlphabetLength = sym->mAlphabetLength;
}

SymbolFixedString::~SymbolFixedString()
{   
}

Symbol* SymbolFixedString::clone()
{
    return new SymbolFixedString(this);
}

float SymbolFixedString::getBinding(Symbol* sym)
{
    // TODO: check type
    SymbolFixedString* symStr = (SymbolFixedString*)sym;

    unsigned int match = 0;

    for (unsigned int i = 0; i < mLength; i++)
    {
        if (mString.substr(i, 1) == symStr->mString.substr(i, 1))
        {
            match++;
        }
    }

    float binding = ((float)match) / ((float)mLength);
    return 1.0f - binding;
}

void SymbolFixedString::initRandom()
{
    mString = "";

    for (unsigned int i = 0; i < mLength; i++)
    {
        unsigned int pos = mDistFixedString->iuniform(0, mAlphabetLength);
        mString += mAlphabet.substr(pos, 1);
    }
}

void SymbolFixedString::mutate()
{
    unsigned int replacePos = mDistFixedString->iuniform(0, mLength);
    unsigned int charIndex = mDistFixedString->iuniform(0, mAlphabetLength);
    replaceChar(replacePos, charIndex);
}

void SymbolFixedString::replaceChar(unsigned int replacePos, unsigned int charIndex)
{
    mString = mString.substr(0, replacePos)
                + mAlphabet.substr(charIndex, 1)
                + mString.substr(replacePos + 1);
}

const char SymbolFixedString::mClassName[] = "SymbolFixedString";

Orbit<SymbolFixedString>::MethodType SymbolFixedString::mMethods[] = {
    {"setAlwaysRandom", &Symbol::setAlwaysRandom},
    {"getID", &Symbol::getID},
    {"setID", &Symbol::setID},
    {0,0}
};

Orbit<SymbolFixedString>::NumberGlobalType SymbolFixedString::mNumberGlobals[] = {{0,0}};

