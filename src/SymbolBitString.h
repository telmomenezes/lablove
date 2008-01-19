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

#if !defined(__INCLUDE_SYMBOL_BIT_STRING_H)
#define __INCLUDE_SYMBOL_BIT_STRING_H

#include "Symbol.h"
#include "Orbit.h"
#include "RandDistManager.h"

#include <string>

using std::string;

class SymbolBitString : public Symbol
{
public:
    SymbolBitString(lua_State* luaState=NULL);
    SymbolBitString(string str);
    SymbolBitString(SymbolBitString* sym);
    virtual ~SymbolBitString();
    virtual Symbol* clone();

    virtual float getBinding(Symbol* sym);
    virtual void initRandom();
    virtual void mutate();

    unsigned int getLength(){return mLength;}
    int getBit(unsigned int pos){return mBits[pos];}

    void setAllowWildcards(bool allow){mAllowWildcards = allow;}

    static const char mClassName[];
    static Orbit<SymbolBitString>::MethodType mMethods[];
    static Orbit<SymbolBitString>::NumberGlobalType mNumberGlobals[];

protected:
    void initFromString(string str);
    int getRandomBit();

    static mt_distribution* mDistBitString;

    int* mBits;
    unsigned int mLength;
    bool mAllowWildcards;
};
#endif

