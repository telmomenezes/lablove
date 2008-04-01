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

#if !defined(__INCLUDE_SYMBOL_RGB_H)
#define __INCLUDE_SYMBOL_RGB_H

#include "Symbol.h"
#include "Orbit.h"
#include "RandDistManager.h"

class SymbolRGB : public Symbol
{
public:
    SymbolRGB(lua_State* luaState=NULL);
    SymbolRGB(int r, int g, int b);
    SymbolRGB(SymbolRGB* sym);
    virtual ~SymbolRGB();
    virtual Symbol* clone();

    virtual float getBinding(Symbol* sym);
    virtual void initRandom();
    virtual void mutate();

    virtual int getRed(){return mRed;}
    virtual int getGreen(){return mGreen;}
    virtual int getBlue(){return mBlue;}

    int initRandom(lua_State* luaState);

    virtual string toString();

    static const char mClassName[];
    static Orbit<SymbolRGB>::MethodType mMethods[];
    static Orbit<SymbolRGB>::NumberGlobalType mNumberGlobals[];

    int mRed;
    int mGreen;
    int mBlue;

protected:
    static mt_distribution* mDistRGB;
};
#endif

