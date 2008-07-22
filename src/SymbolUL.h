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

#if !defined(__INCLUDE_SYMBOL_UL_H)
#define __INCLUDE_SYMBOL_UL_H

#include "Symbol.h"
#include "Orbit.h"
#include "RandDistManager.h"
#include "types.h"

class SymbolUL : public Symbol
{
public:
    SymbolUL(lua_State* luaState);
    SymbolUL(gbULINT value, gbULINT min=0, gbULINT max=1);
    SymbolUL(SymbolUL* sym);
    virtual ~SymbolUL();
    virtual Symbol* clone();

    virtual float proximity(Symbol* sym);
    virtual bool equals(Symbol* sym);

    virtual void initRandom();
    virtual void mutate();

    gbULINT getUL(){return mUL;}
    gbULINT getMin(){return mMin;}
    gbULINT getMax(){return mMax;}

    virtual string toString();

    static const char mClassName[];
    static Orbit<SymbolUL>::MethodType mMethods[];
    static Orbit<SymbolUL>::NumberGlobalType mNumberGlobals[];

protected:
    static mt_distribution* mDistUL;

    gbULINT mUL;
    gbULINT mMax;
    gbULINT mMin;
};
#endif

