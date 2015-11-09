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

#if !defined(__INCLUDE_SYMBOL_FLOAT_VECTOR_H)
#define __INCLUDE_SYMBOL_FLOAT_VECTOR_H

#include "Simulation.h"
#include "Symbol.h"
#include "Orbit.h"


class SymbolFloatVector : public Symbol
{
public:
    SymbolFloatVector(lua_State* luaState);
    SymbolFloatVector(unsigned int size, float min=0.0f, float max=1.0f);
    SymbolFloatVector(SymbolFloatVector* sym);
    virtual ~SymbolFloatVector();
    virtual Symbol* clone();

    virtual float proximity(Symbol* sym);
    virtual bool equals(Symbol* sym);

    virtual void initRandom();
    virtual void mutate();

    float getItem(unsigned int index){return mVector[index];}
    float getMin(){return mMin;}
    float getMax(){return mMax;}

    static const char mClassName[];
    static Orbit<SymbolFloatVector>::MethodType mMethods[];
    static Orbit<SymbolFloatVector>::NumberGlobalType mNumberGlobals[];

    int initRandom(lua_State* luaState);

protected:
    void calcMaxBinding();

    unsigned int mSize;
    float* mVector;
    float mMax;
    float mMin;
    float mMaxBinding;
};
#endif

