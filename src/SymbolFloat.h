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

#if !defined(__INCLUDE_SYMBOL_FLOAT_H)
#define __INCLUDE_SYMBOL_FLOAT_H

#include "Simulation.h"
#include "Symbol.h"
#include "Orbit.h"
#include "RandDistManager.h"

class SymbolFloat : public Symbol
{
public:
    SymbolFloat(lua_State* luaState);
    SymbolFloat(float value, float min=0.0f, float max=1.0f);
    SymbolFloat(SymbolFloat* sym);
    virtual ~SymbolFloat();
    virtual Symbol* clone();

    virtual float bind(Symbol* sym);
    virtual void initRandom();
    virtual void mutate();

    float getFloat(){return mFloat;}
    float getMin(){return mMin;}
    float getMax(){return mMax;}
    
    int initRandom(lua_State* luaState);

    static const char mClassName[];
    static Orbit<SymbolFloat>::MethodType mMethods[];
    static Orbit<SymbolFloat>::NumberGlobalType mNumberGlobals[];

protected:
    static mt_distribution* mDistFloat;

    float mFloat;
    float mMax;
    float mMin;
};
#endif

